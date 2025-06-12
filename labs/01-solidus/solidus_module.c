#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>          // file_operatiomns structure
#include <linux/cdev.h>        // character device structure
#include <linux/semaphore.h>   // semaphore structure
#include <linux/uaccess.h>     // user space access functions


struct fake_device{
    char data[100];
    struct semaphore sem;
} virtual_device;


static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int device_close(struct inode *inode, struct file *file) {
    printk(KERN_INFO "Device closed\n");
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t len, loff_t *offset) {
    printk(KERN_INFO "Read from device\n");
    return 0;
}

static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
    printk(KERN_INFO "Write to device\n");
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_close,
    .read = device_read,
    .write = device_write
};


struct cdev *my_cdev;
int major_number;
int ret;

dev_t dev_num;

#define DEVICE_NAME "solidus_device"

static int driver_entry(void){

    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0)
    {
        printk(KERN_ALERT "Failed to allocate a major number\n");
        return ret;
    }
    major_number = MAJOR(dev_num);
    printk(KERN_INFO "Major number is: %d\n", major_number);
    printk(KERN_INFO "\tuse \"mknod /dev/%s c %d 0\" to create a device file\n", DEVICE_NAME, major_number);


    my_cdev = cdev_alloc();
    my_cdev->ops = &fops;
    my_cdev->owner = THIS_MODULE;
    ret = cdev_add(my_cdev, dev_num, 1);
    if (ret < 0)
    {
        printk(KERN_ALERT "Unable to add cdev to kernel\n");
        return ret;
    }
    sema_init(&virtual_device.sem, 1);
    return 0;
}

static void driver_exit(void){
    cdev_del(my_cdev);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "Device unregistered and cleaned up\n");
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Said Acosta");
MODULE_DESCRIPTION("A simple character device driver example");
MODULE_VERSION("1.0");

module_init(driver_entry);
module_exit(driver_exit);
