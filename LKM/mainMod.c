
#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/uaccess.h>          // Required for the copy to user function
#include <linux/usb.h>

#define  DEVICE_NAME "lcdchar"    ///< The device will appear at /dev/ebbchar using this value
#define  CLASS_NAME  "lcd"        ///< The device class -- this is a character device driver
 
MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("algisb");    ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("Arduino LCD screen controller");  ///< The description -- see modinfo
MODULE_VERSION("0.1");            ///< A version number to inform users
 


static struct usb_device *device = NULL;
static struct usb_class_driver class;
//static unsigned char bulk_buf[MAX_PKT_SIZE];

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

static struct file_operations lcd_fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

static struct usb_driver lcd_driver = 
{    
    	.name = "LCD_screen",
	//.fops = &lcd_fops,
	.id_table = lcd_table,
	.probe = lcd_probe,
	.disconnect = lcd_disconnect,
};



static int __init lcdchar_init(void)
{
	printk(KERN_INFO "LCD_drv: Initializing the LCD_drv LKM\n");

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
 
static void __exit lcdchar_exit(void)
{
	printk(KERN_INFO "LCD_drv: Goodbye from the LKM!\n");

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

static ssize_t dev_write(struct file *f, const char __user *buf, size_t cnt, loff_t *off)
{

	return 0;
}

static int dev_release(struct inode *i, struct file *f)
{
	return 0;
}

static int lcd_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	printk(KERN_INFO "LCD_drv: PROBING! \n");

	if(device == NULL)
	{
		device = interface_to_usbdev(interface);

		class.name = "usb/lcd%d";
		class.fops = &lcd_fops;
		int retval = usb_register_dev(interface, &class);
		
		if (retval < 0)
		{	
			printk("Not able to get a minor for this device.");
		}
		else
		{
			printk("Minor obtained: %d\n", interface->minor);
		}
		return retval;
	}
	else
	{
		return 0;
	}

    /* called when a USB device is connected to the computer. */
}

static void lcd_disconnect(struct usb_interface *interface)
{
	/* called when unplugging a USB device. */
	usb_deregister_dev(interface, &class);
	device = NULL;
}


module_init(lcdchar_init);
module_exit(lcdchar_exit);
