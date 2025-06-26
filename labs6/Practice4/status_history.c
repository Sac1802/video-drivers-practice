/**
 * @file status_history.c
 * @brief Simple Linux character driver that stores a history of status messages.
 * @author Sac1802
 * @license GPL
 * 
 * This module implements a character device that allows writing and reading status messages.
 * It keeps the last 5 messages in a circular buffer and returns them all upon read.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/sched.h>

#define DEVICE_NAME "status_history"
#define MAX_ITEM 5     ///< Max number of status entries stored
#define MAX_LEN  32    ///< Max length of each status message

MODULE_AUTHOR("Sac1802");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This module is a practice driver that stores a history of device statuses.");

/**
 * @brief Device representation
 */
static struct {
    dev_t devnum;         ///< Device number
    struct cdev cdev;     ///< Char device structure
} device_practice;

static char device_status[MAX_LEN] = "NOT STATUS\n";                ///< Last written status
static char history_status[MAX_ITEM][MAX_LEN] = {0};                ///< Circular buffer for status history
static int current_index = 0;                                       ///< Current index in the circular buffer

/**
 * @brief Handles read operations from the device
 * 
 * Concatenates all stored status messages and sends them to user space.
 * 
 * @param file Pointer to file structure
 * @param buf Buffer to copy data to in user space
 * @param length Maximum number of bytes to copy
 * @param offset Current offset in the file
 * @return Number of bytes read, or negative error code
 */
static ssize_t file_read(struct file *file, char __user *buf, size_t length, loff_t *offset){
    printk(KERN_INFO "Read file\n");
    char tmp_buffer[MAX_ITEM * MAX_LEN] = {0};
    int i, total = 0;

    if(*offset > 0)
        return 0;

    for(i = 0; i < MAX_ITEM; i++){
        if(strlen(history_status[i]) == 0)
            continue;
        strlcat(tmp_buffer, history_status[i], sizeof(tmp_buffer));
    }

    total = strlen(tmp_buffer);
    if(total > length)
        total = length;

    if(copy_to_user(buf, tmp_buffer, total))
        return -EFAULT;

    *offset += total;
    return total;
}

/**
 * @brief Handles write operations to the device
 * 
 * Stores a new status message into the circular buffer.
 * 
 * @param file Pointer to file structure
 * @param buf Buffer from user space containing the status string
 * @param length Number of bytes to write
 * @param offset Offset in the file
 * @return Number of bytes written, or negative error code
 */
static ssize_t file_write(struct file *file, const char __user *buf, size_t length, loff_t *offset){
    printk(KERN_INFO "Write file\n");
    char kbuf[MAX_LEN] = {0};

    int len = (length < MAX_LEN - 1) ? length : MAX_LEN - 1;
    if(copy_from_user(kbuf, buf, len))
        return -EFAULT;
    kbuf[len] = '\0';

    strncpy(device_status, kbuf, sizeof(device_status));
    strncpy(history_status[current_index], device_status, MAX_LEN);
    current_index = (current_index + 1) % MAX_ITEM;

    printk(KERN_INFO "Received: %s\n", kbuf);
    return length;
}

/**
 * @brief Called when device file is opened
 */
static int file_open(struct inode *inode, struct file *file){
    printk(KERN_INFO "Open file\n");
    return 0;
}

/**
 * @brief Called when device file is closed
 */
static int file_release(struct inode *inode, struct file *file){
    printk(KERN_INFO "Release file\n");
    return 0;
}

/**
 * @brief File operations supported by the device
 */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = file_read,
    .write = file_write,
    .open = file_open,
    .release = file_release,
};

/**
 * @brief Module initialization
 * 
 * Allocates device numbers, sets up char device, and logs process info.
 */
static int module_practice_init(void){
    printk(KERN_INFO "Initialization module\n");

    if(alloc_chrdev_region(&device_practice.devnum, 0, 1, DEVICE_NAME)){
        printk(KERN_WARNING "Failed to allocate device number\n");
        return -1;
    }

    cdev_init(&device_practice.cdev, &fops);
    device_practice.cdev.owner = THIS_MODULE;

    if(cdev_add(&device_practice.cdev, device_practice.devnum, 1) < 0){
        unregister_chrdev_region(device_practice.devnum, 1);
        printk(KERN_ALERT "Failed to add cdev\n");
        return -1;
    }

    printk(KERN_INFO "Char driver registered: %d:%d\n",
           MAJOR(device_practice.devnum), MINOR(device_practice.devnum));
    printk(KERN_INFO "The process id \"%s\" (pid %i)\n", current->comm, current->pid);
    return 0;
}

/**
 * @brief Module cleanup
 * 
 * Unregisters device and removes char device.
 */
static void module_practice_exit(void){
    printk(KERN_INFO "Exit of the module\n");
    cdev_del(&device_practice.cdev);
    unregister_chrdev_region(device_practice.devnum, 1);
    printk(KERN_INFO "Device unregistered\n");
}

module_init(module_practice_init);
module_exit(module_practice_exit);
