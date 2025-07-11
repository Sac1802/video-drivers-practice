#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/threads.h>

#define MODULE_NAME "register_concurrency"
#define MAX_VAL    5
#define MAX_LEN   32

MODULE_AUTHOR("Sac1802");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This a practice driver character");

static struct {
    dev_t devnum;
    struct cdev cdev;
}device_practice;

static char status[32] = "NOT STATUS\n";
static char history_status[MAX_VAL][MAX_LEN] = {0};

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

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = file_read,
    .write = file_write,
    .open = file_open,
    .release = file_release,
};

static int practice_module_init(void){
    printk(KERN_INFO "Initialization of module\n");
    return 0;
}

static void practice_module_exit(void){
    printk(KERN_INFO "Exit of the module\n");
}

module_init(practice_module_init);
module_exit(practice_module_exit);