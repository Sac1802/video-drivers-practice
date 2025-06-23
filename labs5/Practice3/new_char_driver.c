#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "driver_practice"

MODULE_AUTHOR("Sac1802");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This module is a practice of driver device");

static struct {
    dev_t devnum;
    struct cdev cdev;
}device_practice;

static ssize_t file_read(struct file *file, char __user *buf, size_t length, loff_t *offset){
    printk(KERN_INFO "Read file\n");
    return length;
}

static ssize_t file_write(struct file *file, const char __user *buf, size_t length, loff_t *offset){
    printk(KERN_INFO "Write file\n");
    return length;
}

static int file_open(struct inode *inode, struct file *file){
    printk(KERN_INFO "Open file\n");
    return 0;
}

static int file_release(struct inode *inode, struct file *file){
    printk(KERN_INFO "Release file\n");
    return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = file_read,
    .write = file_write,
    .open = file_open,
    .release = file_release,
};

static int practice_module_init(void){
    printk(KERN_INFO "Initialization Module");
    return 0;
}

static void practice_module_exit(void){
    printk(KERN_INFO "Exit Module");
}

module_init(practice_module_init);
module_exit(practice_module_exit);