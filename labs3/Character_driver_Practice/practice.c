#include <linux/module.h>     // For all module macros (e.g., module_init, module_exit)
#include <linux/kernel.h>     // For printk and kernel macros
#include <linux/fs.h>         // For file operations and device numbers
#include <linux/cdev.h>       // For character device structure
#include <linux/uaccess.h>    // For copy_to_user and copy_from_user
#include <linux/string.h>     // For string handling (strncpy, strlen)

#define DEVICE_NAME "my_char_device"

MODULE_LICENSE("GPL");             // Specifies the license of the module
MODULE_AUTHOR("Sac1802");          // Indicates the author

// Struct to hold device-specific information
static struct {
    dev_t devnum;                 // Device number (major and minor)
    struct cdev cdev;             // Character device structure
} device;

// Internal status buffer to simulate device state
static char device_status[32] = "NO STATUS\n";

// Called when the device file is opened
static int file_open(struct inode *inode, struct file *file) {
    printk(KERN_ALERT "Device opened\n");
    return 0;
}

// Called when the device file is closed
static int file_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device closed\n");
    return 0;
}

// Called when reading from the device file
static ssize_t file_read(struct file *file, char __user *buffer, size_t length, loff_t *offset) {
    int size;

    // Allow reading only once (mimics EOF behavior)
    if (*offset > 0)
        return 0;

    // Get the size of the message
    size = strlen(device_status);
    if (size > length)
        size = length;

    // Copy kernel buffer to user space
    if (copy_to_user(buffer, device_status, size))
        return -EFAULT;

    *offset += size;
    return size;
}

// Called when writing to the device file
static ssize_t file_write(struct file *file, const char __user *buf, size_t length, loff_t *offset) {
    char kbuf = 0;

    // Copy 1 byte from user to kernel space
    if (copy_from_user(&kbuf, buf, 1))
        return -EFAULT;

    // Update the internal device status based on input
    if (kbuf == '1') {
        strncpy(device_status, "READ DEVICE\n", sizeof(device_status));
        printk(KERN_INFO "READ DEVICE");
    } else if (kbuf == '0') {
        strncpy(device_status, "EXIT READ DEVICE\n", sizeof(device_status));
        printk(KERN_INFO "EXIT DEVICE READ");
    } else {
        strncpy(device_status, "INVALID INPUT\n", sizeof(device_status));
        printk(KERN_WARNING "Received invalid input: %c\n", kbuf);
    }

    return length;
}

// File operations structure: links our defined functions to kernel
struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = file_open,
    .read = file_read,
    .write = file_write,
    .release = file_release,
};

// Called when the module is loaded into the kernel
static int module_init_practice(void) {
    printk(KERN_ALERT "Initializing the character device module\n");

    // Allocate device number dynamically
    if (alloc_chrdev_region(&device.devnum, 0, 1, DEVICE_NAME) < 0) {
        printk(KERN_ALERT "Failed to allocate device number\n");
        return -1;
    }

    // Initialize and add the cdev structure
    cdev_init(&device.cdev, &fops);
    device.cdev.owner = THIS_MODULE;

    if (cdev_add(&device.cdev, device.devnum, 1) < 0) {
        unregister_chrdev_region(device.devnum, 1);
        printk(KERN_ALERT "Failed to add cdev\n");
        return -1;
    }

    printk(KERN_INFO "Char driver registered: %d:%d\n", MAJOR(device.devnum), MINOR(device.devnum));
    return 0;
}

// Called when the module is removed from the kernel
static void module_exit_practice(void) {
    printk(KERN_ALERT "Exiting the character device module\n");

    // Remove device from system
    cdev_del(&device.cdev);
    unregister_chrdev_region(device.devnum, 1);

    printk(KERN_INFO "Device unregistered\n");
}

// Register init and exit functions
module_init(module_init_practice);
module_exit(module_exit_practice);
