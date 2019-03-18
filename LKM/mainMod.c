
#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>          // Required for the copy to user function
#include <linux/usb.h>

#define  DEVICE_NAME "ebbchar"    ///< The device will appear at /dev/ebbchar using this value
#define  CLASS_NAME  "ebb"        ///< The device class -- this is a character device driver
 
MODULE_LICENSE("MIT");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("algisb");    ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("Arduino LCD screen controller");  ///< The description -- see modinfo
MODULE_VERSION("0.1");            ///< A version number to inform users
 

static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
static int lcd_probe(struct usb_interface *interface, const struct usb_device_id *id);
static void lcd_disconnect(struct usb_interface *interface);


#define LCD_VENDOR_ID 0x2341
#define LCD_PRODUCT_ID 0x0043

static struct usb_device_id lcd_table [] = {
    { USB_DEVICE(LCD_VENDOR_ID, LCD_PRODUCT_ID) },
    { }
};

MODULE_DEVICE_TABLE (usb, lcd_table);

//static struct file_operations fops =
//{
//   .open = dev_open,
//   .read = dev_read,
//   .write = dev_write,
//   .release = dev_release,
//};

static struct usb_driver lcd_driver = 
{    
    	.name = "LCD_screen",
	//.fops = &fops,
	.id_table = lcd_table,
	.probe = lcd_probe,
	.disconnect = lcd_disconnect,
};



static int __init ebbchar_init(void)
{
	printk(KERN_INFO "EBBChar: Initializing the EBBChar LKM\n");

	int result;

        /* register this driver with the USB subsystem */
        result = usb_register(&lcd_driver);
        if (result < 0)
	{
                printk("usb_register failed for the "__FILE__ "driver. Error number %d", result);
                return -1;
        }

        return 0;
}
 
static void __exit ebbchar_exit(void)
{
	printk(KERN_INFO "EBBChar: Goodbye from the LKM!\n");

	usb_deregister(&lcd_driver);
}
 

static int dev_open(struct inode *inodep, struct file *filep)
{
      return 0;
}
 
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
      return 0;
}
 
static int lcd_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    
    printk(KERN_INFO "EBBChar: YAH YAH WEINER MEIN KAMPF\n");
    
    /* called when a USB device is connected to the computer. */
	return 0;
}

static void lcd_disconnect(struct usb_interface *interface)
{
    /* called when unplugging a USB device. */
}


module_init(ebbchar_init);
module_exit(ebbchar_exit);
