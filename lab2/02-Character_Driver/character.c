#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define DEVICE_NAME "simple_char_device"

MODULE_AUTHOR("Said Acosta");
MODULE_DESCRIPTION("Simple Character Device Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");

static int file_open(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "Inside the %s function\n", __func__);
    return 0;
};

static ssize_t file_read(struct file *file, char __user *buf, size_t len, loff_t *ppos)
{
    printk(KERN_ALERT "Inside the %s function\n", __func__);
    return 0;
}
static ssize_t file_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
    printk(KERN_ALERT "Inside the %s function\n", __func__);
    return 0;
}

static int file_release(struct inode *inode, struct file *file)
{
    printk(KERN_ALERT "Inside the %s function\n", __func__);
    return 0;
}

static struct file_operations file =
{
    .owner = THIS_MODULE,
    .open = file_open,
    .read = file_read,
    .write = file_write,
    .release = file_release,
};

/**
* The first parameter is the major number for the device
* The second parameter is the name of the device
* The third parameter is a pointer to the file operations structure
* that defines the operations that can be performed on the device.
* The major number 240 is used here, but it can be any number that is not already in use.
* The DEVICE_NAME is a string that identifies the device.
* The file structure contains pointers to functions that handle open, read, write, and release operations.
* The file operations structure is used by the kernel to call the appropriate functions
* when a user program interacts with the device.
* The file operations structure is defined in the <linux/fs.h> header file.
* The file operations structure is used to define the operations that can be performed on the device.
*/
static int simple_module_init(void){
    printk(KERN_ALERT "Inside the %s function\n", __func__);

    /*Register with the kernel and indicate that are registering a character device driver */
    register_chrdev(240, DEVICE_NAME, &file);
    return 0;
}

static void simple_module_exit(void){
    printk(KERN_ALERT "Inside the %s function\n", __func__);
    /* Unregister the character device driver */
    unregister_chrdev(240, DEVICE_NAME);
}

module_init(simple_module_init);
module_exit(simple_module_exit);