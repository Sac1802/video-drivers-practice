#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define DEVICE_NAME "my_char_device"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sac1802");

static int file_open(struct inode *inode, struct file *file){
    printf(KERN_ALERT "Device opened\n");
    return 0;
}

static int file_release(struct inode *inode, struct  file *file){
    printk(KERN_INFO "Device closed\n");
    return 0;
};

static ssize_t file_read(struct file *file, char __user *buffer, size_t length,  loff_t *offset){
    printk(KERN_INFO "Reading from device\n");
    return 0;
}

static ssize_t file_write(struct file *file, const __user *buffer, size_t length, loff_t *offset){
    printk(KERN_INFO "Writing to device\n");
    return length;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = file_open,
    .read = file_read,
    .write = file_write,
    .release = file_release,
};


static int module_init_practice(void){
    printk(KERN_ALERT "Initializing the character device module\n");
    return 0;
}

static void module_exit_practice(void){
    printk(KERN_ALERT "Exiting the character device module\n");
}

module_init(module_init_practice);
module_exit(module_exit_practice);