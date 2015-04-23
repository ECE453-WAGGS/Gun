/*
 * Linux 2.6 and 3.0 'parrot' sample device driver
 *
 * Copyright (c) 2011, Pete Batard <pete@akeo.ie>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/kfifo.h>
#include <linux/platform_device.h>  /* Needed for Platform Driver Functions */ 
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include "zed_timer.h"

/* Module information */
MODULE_AUTHOR(AUTHOR);
MODULE_DESCRIPTION(DESCRIPTION);
MODULE_VERSION(VERSION);
MODULE_LICENSE("GPL");

unsigned long *base_addr;  	/* Vitual Base Address */
struct resource *res;  		/* Device Resource Structure */
unsigned long remap_size;  	/* Device Memory Size */

static struct kobject *ece453_obj;

/* Device variables */
static struct class* zed_class = NULL;
static struct device* zed_device = NULL;
static int zed_major;

static int irq;
static u32 irq_count = 0;

static struct fasync_struct *fasync_pl_queue ;

/* A mutex will ensure that only one process accesses our device */
static DEFINE_MUTEX(zed_device_mutex);

/* Module parameters that can be provided on insmod */
static bool debug = false;	/* print extra debug info */
module_param(debug, bool, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "enable debug info (default: false)");

/**
 * zed_irq_handler - 
 *
 * @irq: interrupt number
 * @dev_id: pointer to a platform device structure
 *
 * The handler is invoked when the PL timer expires
 * Returns IRQ_HANDLED
 */
static irqreturn_t zed_irq_handler(int irq, void *dev_id)
{
	u32 regVal;
	//struct platform_device *pdev = dev_id;

	regVal =ioread32(base_addr);
	regVal =ioread32(base_addr+1);
	
	regVal =ioread32(base_addr);
	regVal |= TCSR_INT_M ;
	iowrite32( regVal ,
		   base_addr);

	//dev_info(&pdev->dev, "Recieved IRQ");
	irq_count++;

  	kill_fasync(&fasync_pl_queue, SIGIO, POLL_IN);

	return IRQ_HANDLED;
}



static int zed_device_open(struct inode* inode, struct file* filp)
{
	dbg("");

	/* Ensure that only one process has access to our device at any one time
 	* For more info on concurrent accesses, see http://lwn.net/images/pdf/LDD3/ch05.pdf */
	if (!mutex_trylock(&zed_device_mutex)) {
		warn("another process is accessing the device\n");
		return -EBUSY;
	}

	return 0;
}

static int zed_device_close(struct inode* inode, struct file* filp)
{
	dbg("");
	mutex_unlock(&zed_device_mutex);
	return 0;
}

static ssize_t zed_device_write(struct file* filp, const char __user *buffer, size_t length, loff_t* offset)
{
	int i;
	
	length = length/4 ;  // Convert to the number of WORDs to read/write
	
	for ( i = 0; i < length; i++ )
	{
		iowrite32(*((u32 *)(buffer + (i*4))),base_addr + i);
	} 
	return length*4;
}

static ssize_t zed_device_read(struct file* filp, char __user *buffer, size_t length, loff_t* offset)
{
#if 0
	u32 myData;
	int i;

	length = length/4 ;  // Convert to the number of WORDs to read/write
	
	for ( i = 0; i < length; i++ )
	{
		myData = ioread32(base_addr + i);
		*((u32 *)buffer) = myData;
		 buffer += 4;
	} 
	return length*4;
#endif

	// return the IRQ count
	sprintf(buffer, "%d", irq_count);
	return sizeof(buffer);
}
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * function: pl_fasync
 *
 * This is invoked by the kernel when the user program opens this
 * input device and issues fcntl(F_SETFL) on the associated file
 * descriptor. fasync_helper() ensures that if the driver issues a
 * kill_fasync(), a SIGIO is dispatched to the owning application.
 */



static int pl_fasync (int fd, struct file *filp, int on)
{
    	printk(KERN_INFO "\npl_int: Inside pl_fasync \n");  // DEBUG
	return fasync_helper(fd, filp, on, &fasync_pl_queue);
}
 

/* The file_operation scructure tells the kernel which device operations are handled.
 * For a list of available file operations, see http://lwn.net/images/pdf/LDD3/ch03.pdf */
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = zed_device_read,
	.write = zed_device_write,
	.open = zed_device_open,
	.fasync	=	pl_fasync,
	.release = zed_device_close
};

/* Placing data into the read FIFO is done through sysfs */
static ssize_t sys_stop_timer(struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
	/* Turn on the timer */
	iowrite32( 0x00, 
		   base_addr);


	return count;
}

/* This sysfs entry resets the FIFO */
static ssize_t sys_start_timer(struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
	// Configure Interrupt Rate
	iowrite32( 5*SECOND_1,
		   base_addr + 1);

	/* Turn on the timer */
	iowrite32( TCSR_EINT_M | 
		   TCSR_ENT_M  | 
                   TCSR_ARHT_M |
		   TCSR_MODE_GEN_M | 
                   TCSR_UDT_DOWN_M,
		   base_addr);

	return count;
}


/* Declare the sysfs entries.*/

static struct kobj_attribute timer_start_attribute =
        __ATTR(start, 0222, NULL, sys_start_timer);
static struct kobj_attribute timer_stop_attribute =
        __ATTR(stop, 0222, NULL, sys_stop_timer);

/*
 * Create a group of attributes so that we can create and destory them all
 * at once.
 */
static struct attribute *attrs[] = {
        &timer_start_attribute.attr,
        &timer_stop_attribute.attr,
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


/* Shutdown the timer
 */
static void zed_shutdown(struct platform_device *pdev)
{
	iowrite32(0, base_addr);
}

/* Remove function for mytimer
 * When mytimer module is removed, turn off all the leds first,
 * release virtual address and the memory region requested.
 */
static int zed_remove(struct platform_device *pdev)
{

	zed_shutdown(pdev);

	//iounmap(base_addr);

	/* Release the region */
	release_mem_region(res->start, remap_size);

	kobject_put(ece453_obj);

	/* Release the IRQ */
	free_irq(irq, pdev);

	device_destroy(zed_class, MKDEV(zed_major, 0));
	class_unregister(zed_class);
	class_destroy(zed_class);
	unregister_chrdev(zed_major, DEVICE_NAME);

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
	int returnVal;

	int retval;
	dbg("");

	/* First, see if we can dynamically allocate a major for our device */
	zed_major = register_chrdev(0, DEVICE_NAME, &fops);
	if (zed_major < 0) {
		err("failed to register device: error %d\n", zed_major);
		retval = zed_major;
		goto failed_chrdevreg;
	}

	/* We can either tie our device to a bus (existing, or one that we create)
	 * or use a "virtual" device class. For this example, we choose the latter */
	zed_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(zed_class)) {
		err("failed to register device class '%s'\n", CLASS_NAME);
		retval = PTR_ERR(zed_class);
		goto failed_classreg;
	}

	/* With a class, the easiest way to instantiate a device is to call device_create() */
	zed_device = device_create(zed_class, NULL, MKDEV(zed_major, 0), NULL, CLASS_NAME "_" DEVICE_NAME);
	if (IS_ERR(zed_device)) {
		err("failed to create device '%s_%s'\n", CLASS_NAME, DEVICE_NAME);
		retval = PTR_ERR(zed_device);
		goto failed_devreg;
	}
	
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
		ret = -ENOMEM;
		goto err_release_region;
	}

	/* Register the interrupt */
	irq = platform_get_irq(pdev, 0);
	if (irq >= 0) {
		returnVal = request_irq(irq, zed_irq_handler, 0, pdev->name, pdev);
		if (returnVal != 0) {
			goto err_irq;
		}
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
        ece453_obj = kobject_create_and_add("ece453_timer", kernel_kobj);
        if (!ece453_obj)
                return -ENOMEM;

        /* Create the files associated with this kobject */
        ret = sysfs_create_group(ece453_obj, &attr_group);
        if (ret)
	{
                kobject_put(ece453_obj);
                return -ENOMEM;
	}

	
	mutex_init(&zed_device_mutex);
	return 0;
err_irq:
	free_irq(irq, pdev);
err_release_region:
	release_mem_region(res->start, remap_size);

failed_devreg:
	class_unregister(zed_class);
	class_destroy(zed_class);
failed_classreg:
	unregister_chrdev(zed_major, DEVICE_NAME);
failed_chrdevreg:
	return -1;

}

/* device match table to match with device node in device tree */
static const struct of_device_id zed_of_match[] __devinitconst = {
	{.compatible = "uw,ece453-timer-1.00.a"},
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
static int __init zed_module_init(void)
{
	return platform_driver_register(&zed_platform_driver);
}

/**
 * zed_module_exit -  Unregister the Device Configuration.
 */
static void __exit zed_module_exit(void)
{
	platform_driver_unregister(&zed_platform_driver);

}

module_init(zed_module_init);
module_exit(zed_module_exit);

