# Status History Character Driver – Linux Kernel Module

This kernel module is a character device driver created for practice purposes. It provides a virtual device named `status_history`, which stores a fixed number of status messages (up to 5) written by the user.

Each time a message is written to the device file (`/dev/status_history`), it is saved into a circular buffer. When reading from the device, the driver returns the full history of the most recent 5 status messages, preserving their order of insertion.

## 🔧 Functionality

* **Write operation**:
  When a string is written to the device (e.g., using `echo`), it is stored in a buffer. The buffer holds the 5 most recent messages. When the buffer is full, the oldest message is overwritten (circular behavior).

* **Read operation**:
  When the device is read (e.g., using `cat`), the full list of stored messages is printed in order (oldest to newest).

* **Output to kernel log**:
  Each read/write operation prints debug messages to the kernel log using `printk()`.

## 💻 Example usage

```bash
# Insert the module and create the device file (assuming major number 510 for example)
sudo insmod status_history.ko
sudo mknod /dev/status_history c 510 0
sudo chmod 666 /dev/status_history
```

### Writing status messages

```bash
echo "First status" > /dev/status_history
echo "Second status" > /dev/status_history
echo "Third status" > /dev/status_history
echo "Fourth status" > /dev/status_history
echo "Fifth status" > /dev/status_history
```

### Reading the device

```bash
cat /dev/status_history
```

**Output:**

```
First status
Second status
Third status
Fourth status
Fifth status
```

Now, write a new status, exceeding the buffer size:

```bash
echo "Sixth status" > /dev/status_history
cat /dev/status_history
```

**Output:**

```
Sixth status
Second status
Third status
Fourth status
Fifth status
```

As shown, the oldest message ("First status") has been overwritten by the newest one ("Sixth status").

## 📦 Summary

This module demonstrates:

* How to implement a circular buffer for string storage in the kernel
* How to manage read/write operations in a character driver
* Use of basic memory handling and user-kernel data transfer with `copy_from_user()` and `copy_to_user()`
* Logging kernel activity with `printk()`

