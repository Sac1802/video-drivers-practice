# 🧪 Practice2 – Basic Character Driver in Linux

This is a simple character driver developed for educational purposes while learning Linux kernel development. It allows reading and writing a "status" string to a virtual device. This module does **not** interact with real hardware, making it perfect for beginners.

---

## 📁 File Structure

- `practice.c`: the kernel module code
- `Makefile`: for building the module using the kernel build system

---

## ⚙️ What the driver does

- On **write** (`echo` to the device), it stores a custom string in kernel space.
- On **read** (`cat` from the device), it returns the last string written.
- The driver logs operations to `dmesg` using `printk()` for tracing.
- It handles up to 31 characters (32 including `\0`).

---

## 🧰 How to build and test

### 🧱 Compile the driver

```bash
make
````

Make sure `MODULE_LICENSE("GPL")` is declared in your `.c` file or you’ll get an error during `modpost`.

---

### 📦 Load the module

```bash
sudo insmod practice.ko
```

Check the message:

```bash
dmesg | tail
```

---

### 🛠 Create the device node

Use the major number shown by `dmesg`. Example:

```bash
sudo mknod /dev/practice c 511 0
sudo chmod 666 /dev/practice
```

---

## 🧪 Example usage

```bash
echo "ON" > /dev/practice
cat /dev/practice

# Output:
ON

echo "OFF" > /dev/practice
cat /dev/practice

# Output:
OFF

echo "DEVICE IS NOW ACTIVE" > /dev/practice
cat /dev/practice

# Output:
DEVICE IS NOW ACTIVE

echo "[ERROR] Voltage too low!" > /dev/practice
cat /dev/practice

# Output:
[ERROR] Voltage too low!
```

You can check the kernel logs to see internal operations:

```bash
dmesg | tail
```

### Sample `dmesg` output:

```
[ 3649.806701] Open file
[ 3649.806712] Status of device update to: DEVICE IS NOW ACTIVE
[ 3649.806717] Realease file
[ 3664.106113] Open file
[ 3664.106125] Status of device update to: [ERROR] Voltage too low!
[ 3664.106130] Realease file
```

---

## 🧼 Unload and clean

```bash
sudo rmmod practice
make clean
```

Remove the device node if desired:

```bash
sudo rm /dev/practice
```

---

## 👨‍💻 Author

**Said Acosta** (aka `Sac1802`) – Linux kernel enthusiast learning character device development step by step.

---


