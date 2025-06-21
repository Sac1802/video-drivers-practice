# Simple Linux Character Driver

This is a basic Linux character device driver written in C. It demonstrates how to create a device that interacts with user space using standard system calls like `read()` and `write()`. It doesn't interact with any physical hardware — it's a simulation that stores a simple status string inside the kernel module.

## 🧠 What this driver does

- Creates a character device named `my_char_device`.
- Accepts input from user space using `echo` and `write()`.
- Returns a corresponding status message using `cat` and `read()`.
- Simulates simple device behavior without any hardware.

## 📁 File Structure

- `my_char_driver.c` – Main source file with the character device logic.
- `Makefile` – Used to compile the kernel module.

---

## 🔧 How to Build and Load the Driver

### 1. Build the module

```bash
make
````

### 2. Insert the module into the kernel

```bash
sudo insmod my_char_driver.ko
```

### 3. Check the logs

```bash
dmesg | grep "Char driver registered"
```

Expected output:

```
Char driver registered: 240:0
```

Use the major and minor numbers from the output in the next step.

### 4. Create the device node

```bash
sudo mknod /dev/practice c 240 0
sudo chmod 666 /dev/practice
```

Replace `240` with the actual major number printed by `dmesg`.

---

## 🧪 How to Test It

You can interact with the device using `echo` and `cat`.

```bash
echo "1" > /dev/practice
cat /dev/practice
```

Expected output:

```
READ DEVICE
```

```bash
echo "0" > /dev/practice
cat /dev/practice
```

Expected output:

```
EXIT READ DEVICE
```

```bash
echo "x" > /dev/practice
cat /dev/practice
```

Expected output:

```
IVALID INPUT
```

Full example session:

```
root@sac1802-HP-Laptop-15-ef2xxx /h/s/D/M/K/V/l/Character_driver_Practice (main)# echo "1" > /dev/practice
root@sac1802-HP-Laptop-15-ef2xxx /h/s/D/M/K/V/l/Character_driver_Practice (main)# cat /dev/practice
READ DEVICE
root@sac1802-HP-Laptop-15-ef2xxx /h/s/D/M/K/V/l/Character_driver_Practice (main)# echo "0" > /dev/practice
root@sac1802-HP-Laptop-15-ef2xxx /h/s/D/M/K/V/l/Character_driver_Practice (main)# cat /dev/practice
EXIT READ DEVICE
root@sac1802-HP-Laptop-15-ef2xxx /h/s/D/M/K/V/l/Character_driver_Practice (main)# echo "x" > /dev/practice
root@sac1802-HP-Laptop-15-ef2xxx /h/s/D/M/K/V/l/Character_driver_Practice (main)# cat /dev/practice
IVALID INPUT
```

---

## 🧹 Unload the Module

```bash
sudo rmmod my_char_driver
```

---

## 📚 Concepts Demonstrated

* Character device registration (`alloc_chrdev_region`, `cdev_add`)
* File operations struct (`open`, `read`, `write`, `release`)
* User-kernel space communication (`copy_to_user`, `copy_from_user`)
* Device interaction via `/dev` file
* Kernel logging with `printk`

---

## ⚠️ Notes

* This driver is **for educational purposes only**.
* It doesn't perform any real I/O with hardware.
* Make sure to use the correct major number when creating the device file.

---

