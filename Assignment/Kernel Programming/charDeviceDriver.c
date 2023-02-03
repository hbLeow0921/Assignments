#include <linux/kernel.h> //contain types or functions in kernel
#include <linux/module.h> // core header to load LKM in the kernel
#include <linux/fs.h>	//use to define file structure
#include <linux/spinlock.h> //use for mutex function
#include <asm/uaccess.h> //use to access copy_to_user function
#include <linux/slab.h> 
#include <linux/sched.h> 
#include "charDeviceDriver.h"

MODULE_LICENSE("GPL");

DEFINE_MUTEX(mut_lock); //use for init / cleanup / open / release
DEFINE_MUTEX(rw_lock); //use for read / write operation 


//LINKED LIST FOR FIFO IMPLEMENTATION
//===================================
int list_write(struct list *list, char *buf, size_t len) //use to write into list
{
	struct Node *cur = list->head; //create a new pointer point to the first node of list
	if (!cur) { 
		list->head = kmalloc(sizeof(struct Node), GFP_KERNEL);
		if (!list->head)
			return 0;

		list->head->msg = kmalloc(sizeof(char) * len, GFP_KERNEL);
		strcpy(list->head->msg, buf); //copy msg into node
		list->head->next = NULL;
	} else {
		while (cur->next)
			cur = cur->next;

		cur->next = kmalloc(sizeof(struct Node), GFP_KERNEL);
		if (!cur->next)
			return 0;
		cur->next->msg = kmalloc(sizeof(char) * len, GFP_KERNEL);
		strcpy(cur->next->msg, buf); //copy the message into the list
		cur->next->next = NULL;
	}

	// Always update the total size
	list->list_size += len * sizeof(char);

	return 1;
}

int list_read(struct list *list, char **buf) //use to read the list
{
	struct Node *next_node = NULL;

	if (list->head == NULL)
		return 0;

	next_node = list->head->next;

	int len = strlen(list->head->msg) + 1;
	size_t message_size = len * sizeof(char);

	*buf = kmalloc(message_size, GFP_KERNEL);
	strcpy(*buf, list->head->msg); // copy to buf from list msg 

	// Update total size
	list->list_size -= message_size;

	kfree(list->head->msg); //free the msg 
	kfree(list->head); // free the node inside the list to avoid memory leak
	list->head = next_node;

	return 1;
}

int list_total(struct list *list)
{
	int count = 0;
	struct Node *cur = list->head;
	struct Node *tmp;
	while (cur != NULL)
	{
		count++;
		tmp = cur;
		cur = list -> head->next;
		kfree(tmp);
	}
	return count;


}

int list_free(struct list *list) //use to free the list
{
	struct Node *cur = list->head;
	struct Node* tmp;

	while (cur != NULL) {
		tmp = cur; // point to current node
		cur = cur->next; // cur point to next node
		kfree(tmp); // free current node
	}

	list->head = NULL;
	list->list_size = 0;

	return 1;
}

// Character device driver implementation
// --------------------------------------



int init_module(void)
{
	Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
		printk(KERN_ALERT "Registering char device failed with %d\n", Major);
		return Major;
	}

	msg_list.head = NULL;
	msg_list.list_size = 0;

	// FIND THE RIGHT MAJOR/MINOR NUMBERS
	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);

	return SUCCESS;
}

void cleanup_module(void)
{
	// Unregister the device 
	unregister_chrdev(Major, DEVICE_NAME);
	//free the list of message 
	list_free(&msg_list); // free the whole list
}




static int device_open(struct inode *inode, struct file *file)
{
	mutex_lock(&mut_lock);
	if (device_Open) {
		mutex_unlock(&mut_lock);
		return -EBUSY;
	}
	device_Open++;
	mutex_unlock(&mut_lock);

	try_module_get(THIS_MODULE);

	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	mutex_lock(&mut_lock);
	device_Open--;
	mutex_unlock(&mut_lock);
	module_put(THIS_MODULE);

	return SUCCESS;
}

static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset)
{
	int bytes_read = 0; // use to record the string of bytes

	mutex_lock(&rw_lock);
	if (!list_read(&msg_list, &msg_ptr)) { //if list is equal to NULL
		mutex_unlock(&rw_lock); //unlock the mutex continue new operation
		return -EAGAIN; // return EAGAIN
	}

	printk("Read Current String: %s\n", msg_ptr);
	printk("MAX message size (No exceed!): %d\n", max_len);
	printk("Current List message size: %d\n", msg_list.list_size);
	printk("MAX messages size: %d\n\n", total_msg);

	if (*msg_ptr == 0) {
		mutex_unlock(&rw_lock);
		return 0;
	}

	char *ptr = msg_ptr;

	while (length && *ptr) {
		put_user(*(ptr++), buffer++); //put into list

		length--;
		bytes_read++;
	}

	if (*ptr == '\0')
		put_user(*ptr, buffer);

	kfree(msg_ptr); // free the message after reading it
	msg_ptr = NULL;
	mutex_unlock(&rw_lock); //unlock after done operation

	return bytes_read;
}


static ssize_t device_write(struct file *filp, const char __user *buffer, size_t length, loff_t *offset)
{
	
	int length_buff = length + 1; // MAXIUMUM COUNT OF BYTES TO COPY
	size_t message_size = length_buff * sizeof(char);

	printk("Current Message size: %d\n", message_size);
	printk("MAX message size (No exceed!): %d\n", max_len);
	printk("Current List message size: %d\n", msg_list.list_size);
	printk("MAX messages size (No exceed!): %d\n\n", total_msg);
	
	//CHECK IF ONE MESSAGE IS MORE THAN 4KB
	if (message_size > max_len) {
		printk(KERN_ALERT "Error: the message size is bigger than 4K");
		return -EINVAL;
	}
	
	//CHECK TOTAL MESSAGE IN THE LIST. IF EXCEED RETURN EBUSY
	if (msg_list.list_size + message_size > total_msg) {
		printk(KERN_ALERT "Error: the total messages size is bigger than the maximum allowed");
		return -EBUSY;
	}

	//NOT EXCEED THEN START GETTING STRING FROM THE USER SPACE
	mutex_lock(&rw_lock);// LOCK AND STARTING TO DO OPERATION
	char *buf = kmalloc(sizeof(char) * length_buff, GFP_KERNEL); // SOURCE ADDRESS IN USERSPACE
	strncpy_from_user(buf, buffer, length_buff - 1);
	buf[length_buff - 1] = '\0';  // USE TO STOP WHEN END OF THE FILES

	if (!list_write(&msg_list, buf, length_buff))
		printk(KERN_ALERT "An error occurred when adding the message");

	kfree(buf); // FREE TO AVOID MEMORY LEAK
	mutex_unlock(&rw_lock); //UNLOCK THE MUTEX AFTER FINISH THE OPERATION

	return length;
}