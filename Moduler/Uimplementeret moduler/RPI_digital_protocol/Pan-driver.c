#include <linux/gpio.h> 
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/of_gpio.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/delay.h>

#define DONE_PIN 16
#define TRIGGER_PIN 20
#define PIN_HIGH 200

//Linux device driver prototypes
static int Pan_drv_init(void);
static void Pan_drv_exit(void);
ssize_t Pan_drv_read(struct file* filep, char* buf, size_t count, loff_t* f_pos);
ssize_t Pan_drv_write(struct file* filep, const char* buf, size_t count, loff_t* f_pos);

//Linux device driver globals
struct file_operations Pan_drv_fops = {
	.owner = THIS_MODULE,
	.read = Pan_drv_read,
	.write = Pan_drv_write,
};
static struct cdev Pan_drv_cdev;
static struct class* Pan_drv_class;
static dev_t devno;

//Driver globals 
static int pancakeRdy = 0;
static int IRQ_LINE;


/*************************************
 * Interrupt
 * **********************************/
static irqreturn_t pancakeDone_isr(int irq, void* dev_id) {
	/* Ensure that ISR is not triggered by glitch */
	if (gpio_get_value(DONE_PIN) == 1) {
		pancakeRdy++;
	}
	return IRQ_HANDLED;
}

/*************************************
 * Read
 * **********************************/
ssize_t Pan_drv_read(struct file* filep, char* buf, size_t count, loff_t* f_pos) {
	if(*f_pos >= 2) {
		return 0;
	}
	if (pancakeRdy > 0) {
		//copy to user 1, remove 1 from pancakeRdy
		copy_to_user(buf, "1\n" , 2);
		pancakeRdy--;
	} else {
		//copy to user 0
		copy_to_user(buf, "0\n", 2);
	}
	*f_pos += 2;
	return 2;
}

/*************************************
 * Write
 * **********************************/
ssize_t Pan_drv_write(struct file* filep, const char* buf, size_t count, loff_t* f_pos) {
	gpio_set_value(TRIGGER_PIN, 1);
	msleep(PIN_HIGH);
	gpio_set_value(TRIGGER_PIN, 0);
	return 1;
}

/************************************
 * Init
 *************************************/
static int Pan_drv_init(void) {
	int err = 0;

	err = alloc_chrdev_region(&devno, 0, 1, "Pan_drv");
	if (err < 0) {
		printk("Error in allocation of chrdrv\n");
		goto err_alloc_chrdev;
	}

	cdev_init(&Pan_drv_cdev, &Pan_drv_fops);
	err = cdev_add(&Pan_drv_cdev, devno, 1);
	if (err < 0) {
		printk("Error in cdev_add\n");
		goto err_cdev_add;
	}

	Pan_drv_class = class_create(THIS_MODULE, "Pan_class");
	if (IS_ERR(Pan_drv_class)) {
		printk("Error in class creation\n");
		goto err_class_create;
	}

	if (device_create(Pan_drv_class, NULL, MKDEV(MAJOR(devno), 0), &devno, "pan") == NULL) {
		printk("Error in done_pin device creation\n");
		goto err_device_create;
	}

	err = gpio_request(DONE_PIN, "Pan_drv_DONE_GPIO");
	if (err < 0) {
		printk("Error in pin request, done pin\n");
		goto err_done_pin_request;
	}

	err = gpio_direction_input(DONE_PIN);
	if (err < 0) {
		printk("Error in direction request, done pin\n");
		goto err_done_pin_direction;
	}

	IRQ_LINE = gpio_to_irq(DONE_PIN);
	
	err = request_irq(IRQ_LINE, pancakeDone_isr, IRQF_TRIGGER_RISING, "Pan_IRQ", NULL);
	if (err < 0) {
		printk("Error in irq request\n");
		goto err_request_irq;
	}

	err = gpio_request(TRIGGER_PIN, "Pan_drv_TRIGGER_GPIO");
	if (err < 0) {
		printk("Error in pin request, trigger pin\n");
		goto err_trigger_pin_request;
	}

	err = gpio_direction_output(TRIGGER_PIN, 0);
	if (err < 0) {
		printk("Error in pin request, trigger pin\n");
		goto err_trigger_pin_direction;
	}

	printk("Device major/minor created at %i\n", MAJOR(devno));

	return 0;
	
	err_trigger_pin_direction:
		gpio_free(TRIGGER_PIN);
	err_trigger_pin_request:
		free_irq(IRQ_LINE, NULL);
	err_request_irq:
	err_done_pin_direction:
		gpio_free(DONE_PIN);
	err_done_pin_request:
		device_destroy(Pan_drv_class, MKDEV(MAJOR(devno), 0));
	err_device_create:
		class_destroy(Pan_drv_class);
	err_class_create:
		cdev_del(&Pan_drv_cdev);
	err_cdev_add:
		unregister_chrdev_region(devno, 1);
	err_alloc_chrdev:
		return -1;
}

/************************************
 * Exit
 * **********************************/
static void Pan_drv_exit(void)
{
	printk("Removing Pan_driver\n");
	free_irq(IRQ_LINE, NULL);
	gpio_free(DONE_PIN);
	gpio_free(TRIGGER_PIN);
	device_destroy(Pan_drv_class, MKDEV(MAJOR(devno), 0));
	class_unregister(Pan_drv_class);
	cdev_del(&Pan_drv_cdev);
	unregister_chrdev_region(devno, 1);
}

module_init(Pan_drv_init);
module_exit(Pan_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pan-De-Mix");
MODULE_DESCRIPTION("PHUCC TRHIS");