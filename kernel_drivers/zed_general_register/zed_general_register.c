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


unsigned long *base_addr;  	/* Vitual Base Address */
struct resource *res;  		/* Device Resource Structure */
unsigned long remap_size;  	/* Device Memory Size */

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
	int var;

	sscanf(buf, "%xu", &var);

	/* write the value out to the register */
    iowrite8(var, base_addr);

    return count;
}

static struct kobj_attribute general_register_attribute =
        __ATTR(write, 0222, NULL, ece453_write);

/*
 * Create a group of attributes so that we can create and destory them all
 * at once.
 */
static struct attribute *attrs[] = {
        &general_register_attribute.attr,
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

static struct kobject *ece453_obj;

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
	iounmap(base_addr);

	/* Release the region */
	release_mem_region(res->start, remap_size);

        kobject_put(ece453_obj);

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
	int ret = 0;

	int retval;

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
    ece453_obj = kobject_create_and_add("ece453_general_register", kernel_kobj);
    if (!ece453_obj)
            return -ENOMEM;

    /* Create the files associated with this kobject */
    ret = sysfs_create_group(ece453_obj, &attr_group);
    if (retval)
	{
                kobject_put(ece453_obj);
                return -ENOMEM;
	}

	return 0;


}

/* device match table to match with device node in device tree */
static const struct of_device_id zed_of_match[] __devinitconst = {
	{.compatible = "uw,ece453-leds-1.00.a"},
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
