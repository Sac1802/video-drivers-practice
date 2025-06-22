#include <linux/module.h>      // Core header for loading LKMs into the kernel
#include <linux/init.h>        // Macros for module init/exit
#include <linux/fs.h>          // Header for the Linux file system support
#include <linux/cdev.h>        // Character device structure
#include <linux/uaccess.h>     // Required for copy_to_user and copy_from_user

#define DEVICE_NAME "practice_1"  // Name of the device as it will appear in /dev

// Module metadata
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sac180");
MODULE_DESCRIPTION("Practice character driver");

// Structure to hold the device number and cdev object
static struct {
    dev_t devnum;         // Device number (major + minor)
    struct cdev cdev;     // Character device structure
} device;

// Buffer to store the device status; initialized with default message
static char device_status[32] = "NO STATUS\n";

// Function called when the device file is opened
static int file_open(struct inode *inode, struct file *file){
    printk(KERN_INFO "Open file\n");
    return 0;
}

// Function called when the device file is closed
static int file_release(struct inode *inode, struct file *file){
    printk(KERN_INFO "Release file\n");
    return 0;
}

// Function called when user reads from the device file
static ssize_t file_read(struct file *file, char __user *buf, size_t length, loff_t *offset){
    int size;

    // Only allow a single read (no repeat reads)
    if (*offset > 0)
        return 0;

    // Get the size of the stored message
    size = strlen(device_status);

    // Adjust size if user buffer is smaller
    if (size > length)
        size = length;

    // Copy data from kernel space to user space
    if (copy_to_user(buf, device_status, size))
        return -EFAULT;

    // Update offset so further reads return 0
    *offset += size;
    return size;
}

// Function called when user writes to the device file
static ssize_t file_write(struct file *file, const char __user *buf, size_t length, loff_t *offset){
    char kbuf[32] = {0};  // Temporary kernel buffer
    size_t len = (length < sizeof(kbuf) - 1) ? length : sizeof(kbuf) - 1;

    // Copy input data from user space into kernel buffer
    if (copy_from_user(&kbuf, buf, len))
        return -EFAULT;

    // Update the device_status with the new value
    strncpy(device_status, kbuf, sizeof(device_status));
    printk(KERN_INFO "Status of device updated to: %s", kbuf);
    return length;
}

// File operations structure mapping user operations to driver functions
struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = file_read,
    .write = file_write,
    .open = file_open,
    .release = file_release
};

// Module initialization function
static int practice_module_init(void){
    printk(KERN_INFO "Initialize Module\n");

    // Allocate a major and minor number dynamically
    if (alloc_chrdev_region(&device.devnum, 0, 1, DEVICE_NAME) < 0){
        printk(KERN_WARNING "Failed to allocate device number\n");
        return -1;
    }

    // Initialize the character device with the file operations
    cdev_init(&device.cdev, &fops);
    device.cdev.owner = THIS_MODULE;

    // Register the character device with the system
    if (cdev_add(&device.cdev, device.devnum, 1) < 0){
        unregister_chrdev_region(device.devnum, 1);
        printk(KERN_ALERT "Failed to add cdev\n");
        return -1;
    }

    printk(KERN_INFO "Char driver registered: %d:%d\n", MAJOR(device.devnum), MINOR(device.devnum));
    return 0;
}

// Module cleanup function
static void practice_module_exit(void){
    // Remove the character device and free the device number
    cdev_del(&device.cdev);
    unregister_chrdev_region(device.devnum, 1);
    printk(KERN_INFO "Device unregistered\n");
}

// Register module init and exit functions
module_init(practice_module_init);
module_exit(practice_module_exit);
