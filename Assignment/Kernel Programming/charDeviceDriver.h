#define SUCCESS 0	//RETURN 0 IF OPERATION SUCCESS
#define DEVICE_NAME "chardev" // ASSIGN "chardev" TO THE DEVICE DRIVER


static int Major; //USED TO ASSIGN A MAJOR NUMBER FOR THE DEVICE DRIVER
static int max_len = 4096; // MAX BYTE FOR A MESSAGE 
static int total_msg = 4194304; //MAXIMUM NUMBER OF STRING INSIDE THE DEVICE DRIVER
static int device_Open = 0; // GET THE NUMBERS OF HOW MANY TIMES THE DEVICE BEEN OPENED


//STRUCTURE OF NODE INSIDE LIST
//=============================
struct Node {
	char *msg;
	struct Node *next;
};


//STRUCTURE OF LINKED LIST 
//========================
struct list {
	struct Node *head;
	size_t list_size;
};

//EXTRA POINTERS AND LIST MESSAGE TO BE USED IN LINKED LIST
//========================================================
static struct list msg_list;
static char *msg_ptr;

// LIST DEFINITION FOR FIFO IN DEVICE DRIVER
// =========================================
int list_write(struct list*, char*, size_t);
int list_read(struct list*, char**);
int list_total(struct list*);
int list_free(struct list*);

// DEFINITION OF OPERATION IN DEVICE DRIVER
// ========================================
int init_module(void);
void cleanup_module(void);

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);


static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release,
	
};