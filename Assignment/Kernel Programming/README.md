# KERNEL PROGRAMMING

Write a device driver for a character device which implements a simple way of message passing. The kernel maintains a list of messages. To limit memory usage, we impose a limit of 4KB = 4*1024 bytes for each message and also impose a limit of the  total number of messages stored in the kernel, which is 1000.

### Your device driver should perform the following operations:

When the module is loaded, the device is created. An empty list of messages is created as well.

+ Removing the module deallocates all messages, removes the list of messages and removes the device.

+ Reading from the device returns one message, and removes this message from the kernel list. If the list of messages is empty, the reader returns **-EAGAIN**.

+ Writing to the device stores the message in kernel space and adds it to the list if the message is below the maximum size, and the limit of the number of all messages stored in the kernel  wouldn't be surpassed with this message. If the message is too big, -EINVAL is returned, and if the limit of the number of all messages was surpassed, **-EBUSY** is returned.

+ The kernel module which implements this driver must be called **charDeviceDriver.ko**.

You need to ensure that your code deals with multiple attempts at reading and writing at the same time.  Moreover, your code should handle several read and write attempts  concurrently.  Your critical sections should be as short as possible. The reader should obtain the messages in a **FIFO (first in first out)** manner.
