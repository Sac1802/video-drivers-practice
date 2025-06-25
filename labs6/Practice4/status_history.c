#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define DEVICE_NAME "status_history"

static struct {
    dev_t devnum;
    struct cdev cdev;
}device_practice;

static char device_status[32] = "NOT STATUS\n";
static char *history_status = NULL;

static ssize_t file_read(struct file *file, char __user *buf, int length, off_t *offset){
    printk(KERN_INFO "Read file\n");
    return length;
}

static ssize_t file_write(struct file *file, const char __user *buf, int length, off_t *offset){
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


static int module_practice_init(void){
    printk(KERN_INFO "Initialization module");
    return 0;
}

static void module_practice_exit(void){
    printk(KERN_INFO "Exit of the module");
}

module_init(module_practice_init);
module_exit(module_practice_exit);