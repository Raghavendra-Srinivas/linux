/*
 * VT HeapLab HAWK Driver.
 *
 * This is a new driver 
 *
 * 2020 (c) Virginia Tech, Heap Lab.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <linux/init.h>
#include <linux/io.h>


#include <stdio.h>
#include <stdint.h>

#define DRIVER_NAME "VTHEAPLAB_HAWK"

#define PLIC_BASE    0xfff1100000ULL

#define IRQ_ENABLE_TARGET1 0x0002000 //Per IrQ 1 bit from lsb = 'b100 for hawk
#define IRQ_ENABLE_TARGET2 0x0002080 //

#define IRQ_PRIORITY_SRC0 0x0000000
#define IRQ_PRIORITY_SRC1 0x0000004
#define IRQ_PRIORITY_SRC2 0x0000008 //for hawk -set it to 3'b111
#define IRQ_PRIORITY_SRC3 0x000000C //for hawk -set it to 3'b111

/* Register offsets for the Hawk */

/* Hawk Address Register Bit Masks */

/* Hawk Write Data Register Bit Masks */

/* Hawk Read Data Register Bit Masks */

/* Hawk Control Register Bit Masks */

/* Global Interrupt Enable Register (GIER) Bit Masks */
#define XEL_GIER_GIE_MASK	0x80000000	/* Global Enable */

/* Hawk Status Register (TSR) Bit Masks */


//#ifdef __BIG_ENDIAN
//#define hawk_readl		ioread32be
//#define hawk_writel	iowrite32be
//#else
#define hawk_readl	hawk_readl
#define hawk_writel	iowrite32
//#endif

/*************************/
/* Hawk driver calls */
/*************************/

/**
 * hawk_enable_interrupts - Enable the interrupts for the EmacLite device
 * @drvdata:	Pointer to the Emaclite device private data
 *
 * This function Hawk interrupt for inflation along
 * with the Global Interrupt Enable.
 */




static void hawk_enable_interrupts()
{
  //u32 reg_data;

  u64 *addr;
  u32 val;

  printk(KERN_ALERT "Enabling HAWK Interrupt ..\n");
  //PLIC programming to enable HAWK interrupt 
  addr = (u64*)(PLIC_BASE+IRQ_ENABLE_TARGET1);
  val = hawk_readl(addr); //*addr;
  printk(KERN_ALERT "PLIC TARGET1 IRQ Enable before 0x%llx, data = 0x%x\n",addr,val);
  val = val | 0x8;
  hawk_writel(val,addr);//*addr=val; 
  printk(KERN_ALERT "PLIC TARGET1 IRQ Enable after 0x%llx, data = 0x%x\n",addr,*addr);

  addr = (u64*)(PLIC_BASE+IRQ_PRIORITY_SRC3);
  val = hawk_readl(addr); //*addr;
  printk(KERN_ALERT "PLIC PRIORITY SRC3 before 0x%llx, data = 0x%x\n",addr,val);
  val = val | 0x7;
  hawk_writel(val,addr);//*addr=val; 
  printk(KERN_ALERT "PLIC PRIORITY SRC3 after 0x%llx, data = 0x%x\n",addr,*addr);

   	/* Enable the Global Interrupt Enable */
	//hawk_writel(XEL_GIER_GIE_MASK, drvdata->base_addr + XEL_GIER_OFFSET);
}

/**
 * hawk_disable_interrupts - Disable the interrupts for the EmacLite device
 * @drvdata:	Pointer to the Emaclite device private data
 *
 * This function disables the Tx and Rx interrupts for the Emaclite device,
 * along with the Global Interrupt Enable.
 */
static void hawk_disable_interrupts()
{
	u32 reg_data;

	/* Disable the Global Interrupt Enable */
	//hawk_writel(XEL_GIER_GIE_MASK, drvdata->base_addr + XEL_GIER_OFFSET);

	/* Disable the Tx interrupts for the first buffer */
	//reg_data = hawk_readl(drvdata->base_addr + XEL_TSR_OFFSET);
	//hawk_writel(reg_data & (~XEL_TSR_XMIT_IE_MASK),
	//		 drvdata->base_addr + XEL_TSR_OFFSET);

}

/**********************/
/* Interrupt Handlers */
/**********************/

irqreturn_t isr(int irq, void *dev_id, struct pt_regs *regs) {
  printk(KERN_ALERT "HAWK INTERRUPT has got triggered : Printing from ISR");
  return IRQ_HANDLED;
}
static int __init irq_ex_init(void) {
  hawk_enable_interrupts();
  printk (KERN_INFO "Interrupt with Interrupt ID: 3 adding\n");
  return request_irq (3,(irq_handler_t)isr,IRQF_SHARED,"hawk_interrupt",(void*)(isr));
}
static void __exit irq_ex_exit(void) {
  printk (KERN_INFO "Interrupt with Interrupt ID: 3 removed\n");
  free_irq(3,(void*)(isr));
}
module_init(irq_ex_init);
module_exit(irq_ex_exit);

MODULE_AUTHOR("VT Heap Lab.");
MODULE_DESCRIPTION("VT Heap Lab Hawk driver");
MODULE_LICENSE("GPL");
