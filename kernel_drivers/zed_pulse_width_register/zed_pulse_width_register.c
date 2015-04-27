/*
 * Sample kobject implementation
 *
 * Copyright (C) 2004-2007 Greg Kroah-Hartman <greg@kroah.com>
 * Copyright (C) 2007 Novell Inc.
 *
 * Released under the GPL version 2 only.
 *
 */
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/kfifo.h>
#include <linux/platform_device.h>  /* Needed for Platform Driver Functions */
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include "ece453_zed.h"

#define ZED_NR_SERVOS 2
#define ZED_PAN_SERVO_ADDR 0x6f200000
#define ZED_TILT_SERVO_ADDR 0x6f220000

struct ece453_servo {
    struct platform_device* pdev;   /* Platform device */
    struct kobject* obj;            /* Kernel object */
    unsigned long* base_addr;       /* Virtual base address */
    struct resource* res;           /* Device resource structure */
    unsigned long remap_size;       /* Device memory size */
};

static struct ece453_servo servos[ZED_NR_SERVOS];

static ssize_t zed_get_servo_number(size_t start_addr)
{
    switch (start_addr) {
        case ZED_PAN_SERVO_ADDR: return 0;
        case ZED_TILT_SERVO_ADDR: return 1;
        default: return -1;
    }
}

#define zed_get_servo(field, value)                         \
({                                                          \
    int i;                                                  \
    struct ece453_servo* servo = NULL;                      \
    for (i = 0; i < ZED_NR_SERVOS; i++) {                   \
        if (servos[i].field == value) servo = &servos[i];   \
    }                                                       \
    servo;                                                  \
})

/*
 * This module shows how to create a simple subdirectory in sysfs called
 * /sys/kernel/kobject-example  In that directory, 3 files are created:
 * "foo", "read", and "write".  If an integer is written to these files, it can be
 * later read out of it.
 */

/* Module parameters that can be provided on insmod */
static bool debug = false;	/* print extra debug info */
module_param(debug, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "enable debug info (default: false)");

static ssize_t ece453_write(struct kobject *kobj, struct kobj_attribute *attr,
                       const char *buf, size_t count)
{
    struct ece453_servo* servo;
	int var;

    servo = zed_get_servo(obj, kobj);
    if (servo == NULL) {
        return -1;
    }

	sscanf(buf, "%xu", &var);
	iowrite32(var, servo->base_addr);

    return count;
}

static struct kobj_attribute pulse_width_register_attribute =
        __ATTR(write, 0222, NULL, ece453_write);

/*
 * Create a group of attributes so that we can create and destory them all
 * at once.
 */
static struct attribute *attrs[] = {
        &pulse_width_register_attribute.attr,
        NULL,   /* need to NULL terminate the list of attributes */
};

/*
 * An unnamed attribute group will put all of the attributes directly in
 * the kobject directory.  If we specify a name, a subdirectory will be
 * created for the attributes with the directory being the name of the
 * attribute group.
 */
static struct attribute_group attr_group = {
        .attrs = attrs,
};

/*
 *
 */
static void zed_shutdown(struct platform_device *pdev)
{
}

/* Remove function for mytimer
 * When mytimer module is removed, turn off all the leds first,
 * release virtual address and the memory region requested.
 */
static int zed_remove(struct platform_device *pdev)
{
    struct ece453_servo* servo;

    servo = zed_get_servo(pdev, pdev);
	iounmap(servo->base_addr);

	/* Release the region */
	release_mem_region(servo->res->start, servo->remap_size);

    kobject_put(servo->obj);

	return 0;
}


/**
 * zed_drv_probe -  Probe call for the device.
 *
 * @pdev:	handle to the platform device structure.
 *
 **/
static int __devinit zed_probe(struct platform_device *pdev)
{
    struct kobject* obj;
    unsigned long* base_addr;
    struct resource* res;
    unsigned long remap_size;
    char dev_name[100];
    size_t servo_number;

    int ret = 0;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "No memory resource\n");
		return -ENODEV;
	}

	remap_size = res->end - res->start + 1;
	dev_info(&pdev->dev, "remap_size = %d\n", (int)remap_size);
	if (!request_mem_region(res->start, remap_size, pdev->name)) {
		dev_err(&pdev->dev, "Cannot request IO\n");
		return -ENXIO;
	}

	base_addr = ioremap(res->start, remap_size);
	if (base_addr == NULL) {
		dev_err(&pdev->dev, "Couldn't ioremap memory at 0x%08lx\n",
				(unsigned long)res->start);

		release_mem_region(res->start, remap_size);

		return -ENOMEM;
	}

  	/*
     * Create a simple kobject with the name of "ece453_example",
     * located under /sys/kernel/
     *
     * As this is a simple directory, no uevent will be sent to
     * userspace.  That is why this function should not be used for
     * any type of dynamic kobjects, where the name and number are
     * not known ahead of time.
     */
    sprintf(dev_name, "ece453_pulse_width_register_%x", (int) res->start);
    obj = kobject_create_and_add(dev_name, kernel_kobj);
    if (!obj)
        return -ENOMEM;

    /* Create the files associated with this kobject */
    ret = sysfs_create_group(obj, &attr_group);
    if (ret) {
        kobject_put(obj);
        return -ENOMEM;
	}

    /* Everything succeeded, initialize the servo details */
    servo_number = zed_get_servo_number(res->start);
    if (servo_number == -1) {
        kobject_put(obj);
        return -ENODEV;
    }

    servos[servo_number].pdev = pdev;
    servos[servo_number].obj = obj;
    servos[servo_number].base_addr = base_addr;
    servos[servo_number].res = res;
    servos[servo_number].remap_size = remap_size;

	return 0;
}

/* device match table to match with device node in device tree */
static const struct of_device_id zed_of_match[] __devinitconst = {
	{.compatible = "uw,ece453-pulse-width-register-1.00.a"},
	{},
};

/* platform driver structure for mytimer driver */
static struct platform_driver zed_platform_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = zed_of_match},
	.probe = zed_probe,
	.remove = __devexit_p(zed_remove),
	.shutdown = __devexit_p(zed_shutdown)
};

/**
 * zed_module_init -  register the Device Configuration.
 *
 * Returns 0 on success, otherwise negative error.
 */
static int __init example_init(void)
{
	return platform_driver_register(&zed_platform_driver);
}

/**
 * zed_module_exit -  Unregister the Device Configuration.
 */
static void __exit example_exit(void)
{
	platform_driver_unregister(&zed_platform_driver);

}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(" <Joe Krachey, jkrachey@wisc.edu>");
