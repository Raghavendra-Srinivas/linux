/*
 * VtHeapLab HAWK driver for the Xilinx Ethernet MAC Lite device.
 *
 * This is a new flat driver which is based on the original emac_lite
 * driver from John Williams <john.williams@xilinx.com>.
 *
 * 2007 - 2013 (c) Xilinx, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 */

#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/interrupt.h>

#define DRIVER_NAME "vtheaplab_hawk"

#define HAWK_LOW_WATERMARK 0x4 

#define hawk_readl      ioread32  	
#define hawk_writel	iowrite32

struct hawk_local {
	void __iomem *base_addr;
};

/**
 * hawk_of_probe - Unbind the driver from the Hawk device.
 * @of_dev:	Pointer to OF device structure
 *
 * This function is called if a device is physically removed from the system or
 * if the driver module is being unloaded. It frees any resources allocated to
 * the device.
 *
 * Return:	0, always.
 */
static int hawk_of_remove(struct platform_device *of_dev)
{
	return 0;
}

//
/**
 * hawk_of_probe - Probe method for the Emaclite device.
 * @ofdev:	Pointer to OF device structure
 *
 * This function probes for the Emaclite device in the device tree.
 * It initializes the driver data structure and the hardware, 
 * Return:	0, if the driver is bound to the Emaclite device, or
 *		a negative error if there is failure.
 */
static int hawk_of_probe(struct platform_device *ofdev)
{
	struct resource *res;
	struct hawk_local *lp = NULL;
	u32 reg_data;
	//struct device *dev = &ofdev->dev;

	int rc = 0;

	dev_info(dev, " Hawk Device Tree Probing\n");

	res = platform_get_resource(ofdev, IORESOURCE_MEM, 0);
	lp->base_addr = devm_ioremap_resource(&ofdev->dev, res);
	if (IS_ERR(lp->base_addr)) {
		rc = PTR_ERR(lp->base_addr);
		goto error;
	}

	//Configure watermark register
  	printk(KERN_ALERT "Writing the Hawk Low watermark register\n");
	hawk_writel(0x1234, lp->base_addr + HAWK_LOW_WATERMARK);
	//Read back
	reg_data = hawk_readl(lp->base_addr + HAWK_LOW_WATERMARK);
  	printk(KERN_ALERT "Reading back the Hawk Low watermark register =%x\n",reg_data);

	dev_info(dev,
		 "Hawk Base address=0x%p, Low Water Mark value=%x\n",lp->base_addr,reg_data);
	return 0;
error:
  	printk(KERN_ALERT "Error getting Base address of Hawk");
	return rc;
}

/* Match table for OF platform binding */
static const struct of_device_id hawk_of_match[] = {
	{ .compatible = "hawk", },
	{ /* end of list */ },
};
MODULE_DEVICE_TABLE(of, hawk_of_match);

static struct platform_driver hawk_of_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.of_match_table = hawk_of_match,
	},
	.probe		= hawk_of_probe,
	.remove		= hawk_of_remove,
};

module_platform_driver(hawk_of_driver);

MODULE_AUTHOR("VT Heap Lab.");
MODULE_DESCRIPTION("VT Heap Lab Hawk driver");
MODULE_LICENSE("GPL");

