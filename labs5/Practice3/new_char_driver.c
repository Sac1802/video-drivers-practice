#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define DEVICE_NAME "driver_practice"

MODULE_AUTHOR("Sac1802");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This module is a practice of driver device");

// Structure to hold device information: device number and character device structure
static struct {
    dev_t devnum;       // Device number (major and minor)
    struct cdev cdev;   // Character device structure
} device_practice;

// Counter for the number of times the device file is read
static int access_count = 0;

// String representing the current device status
static char status_device[32] = "NOT STATUS\n";

// Buffer to store output data sent to userspace
static char ouput[64];

/**
 * file_read - Handles read operations on the device file.
 * @file: Pointer to the file structure.
 * @buf: Userspace buffer to copy data to.
 * @length: Number of bytes requested to read.
 * @offset: Current offset in the file.
 *
 * Returns the number of bytes read or zero if EOF is reached.
 * Updates the device status and read count on each read.
 */
static ssize_t file_read(struct file *file, char __user *buf, size_t length, loff_t *offset){
    int size;

    // Prevent multiple reads by returning 0 if offset is beyond start
    if (*offset > 0)
        return 0;

    // Update device status and prepare output string with current read count
    strncpy(status_device, "DEVICE ON\n", sizeof(status_device));
    snprintf(ouput, sizeof(ouput), "Status: %sReads: %d\n", status_device, access_count++);

    size = strlen(ouput);

    // Limit output size to requested length
    if (size > length)
        size = length;

    // Copy data from kernel space to user space
    if (copy_to_user(buf, ouput, size))
        return -EFAULT;

    // Update file offset to reflect bytes read
    *offset += size;
    return size;
}

/**
 * file_write - Handles write operations on the device file.
 * @file: Pointer to the file structure.
 * @buf: Userspace buffer containing data to write.
 * @length: Number of bytes to write.
 * @offset: Current offset in the file.
 *
 * Accepts commands from userspace, supports "RESET" command to reset read count and update status.
 * Returns the number of bytes written or a negative error code.
 */
static ssize_t file_write(struct file *file, const char __user *buf, size_t length, loff_t *offset){
    char kbuf[32] = {0};
    size_t len = (length < sizeof(kbuf) - 1) ? length : sizeof(kbuf) - 1;

    // Copy data from userspace to kernel buffer safely
    if (copy_from_user(kbuf, buf, len))
        return -EFAULT;

    kbuf[len] = '\0'; // Null-terminate the buffer

    // Check for "RESET" command to reset the device state
    if (strncmp(kbuf, "RESET", 5) == 0) {
        access_count = 0;
        strncpy(status_device, "DEVICE RESET\n", sizeof(status_device));
        snprintf(ouput, sizeof(ouput), "Status: %sReads: %d\n", status_device, access_count);
    }

    // Log received data for debugging purposes
    printk(KERN_INFO "Received: %s\n", kbuf);

    return length;
}

/**
 * file_open - Called when the device file is opened.
 * @inode: Pointer to the inode structure.
 * @file: Pointer to the file structure.
 *
 * Logs the open event.
 * Returns 0 on success.
 */
static int file_open(struct inode *inode, struct file *file){
    printk(KERN_INFO "Open file\n");
    return 0;
}

/**
 * file_release - Called when the device file is closed.
 * @inode: Pointer to the inode structure.
 * @file: Pointer to the file structure.
 *
 * Logs the release event.
 * Returns 0 on success.
 */
static int file_release(struct inode *inode, struct file *file){
    printk(KERN_INFO "Release file\n");
    return 0;
}

// File operations structure linking callbacks to device file operations
struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = file_read,
    .write = file_write,
    .open = file_open,
    .release = file_release,
};

/**
 * practice_module_init - Module initialization function.
 *
 * Allocates a character device region, initializes and adds the device to the system.
 * Logs success or failure.
 * Returns 0 on success or negative error code.
 */
static int practice_module_init(void){
    printk(KERN_INFO "Initialization Module");

    // Allocate major and minor numbers dynamically
    if (alloc_chrdev_region(&device_practice.devnum, 0, 1, DEVICE_NAME)){
        printk(KERN_WARNING "Failed to allocate device number\n");
        return -1;
    }

    // Initialize character device and set file operations
    cdev_init(&device_practice.cdev, &fops);
    device_practice.cdev.owner = THIS_MODULE;

    // Add device to the system
    if (cdev_add(&device_practice.cdev, device_practice.devnum, 1) < 0){
        unregister_chrdev_region(device_practice.devnum, 1);
        printk(KERN_ALERT "Failed to add cdev\n");
        return -1;
    }

    printk(KERN_INFO "Char driver registered: %d:%d\n", MAJOR(device_practice.devnum), MINOR(device_practice.devnum));
    return 0;
}

/**
 * practice_module_exit - Module cleanup function.
 *
 * Removes the character device from the system and unregisters the device number.
 * Logs cleanup actions.
 */
static void practice_module_exit(void){
    printk(KERN_INFO "Exit Module");
    cdev_del(&device_practice.cdev);
    unregister_chrdev_region(device_practice.devnum, 1);
    printk(KERN_INFO "Device unregistered\n");
}

// Specify initialization and cleanup functions
module_init(practice_module_init);
module_exit(practice_module_exit);
