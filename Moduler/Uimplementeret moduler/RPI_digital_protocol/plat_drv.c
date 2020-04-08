 #include <linux/gpio.h> 
 #include <linux/fs.h>
 #include <linux/cdev.h>
 #include <linux/device.h>
 #include <linux/uaccess.h>
 #include <linux/module.h>
 #include <linux/platform_device.h>
 #include <linux/of_gpio.h>
 #include <linux/err.h>
 #include <linux/slab.h> // For dynamic kernel memory
 #include <linux/interrupt.h>
 #include <linux/wait.h>
 #include <linux/sched.h>

#define MAXLEN 32

// Char Driver Globals
 static struct platform_driver plat_drv_platform_driver;
 struct file_operations plat_drv_fops;
 static struct class *plat_drv_class;
 static struct cdev plat_drv_cdev;
 static dev_t devno;

// GPIO struct for information about GPIO-devices
 struct gpio_dev {
    int no;   // GPIO number
    int dir; // 0: in, 1: out
    /* GPIO attributes */
    char toggle_state;
    int IRQ_LINE;
    int int_flag;
 };

static int gpios_len;
static struct gpio_dev* gpio_devs;

//IRQ
static DECLARE_WAIT_QUEUE_HEAD(IRQ_QUEUE);
static irqreturn_t mygpio_isr(int irq, void *dev_id);

/************************************************************
* LINUX ATTRIBUTE METHODS (EXERCISE 8)
* Must be before attribute macros
*************************************************************/

/* Sysfs "show" method for gpio_state prototype */
static ssize_t gpio_state_show(struct device *dev, struct device_attribute *attr, char *buf){
    int* gpio_number = dev_get_drvdata(dev);
    
    // Find the current struct in use based of the gpio_number:
    struct gpio_dev* gpio_dev_in_use = container_of(gpio_number,struct gpio_dev, no);

    int len = sprintf(buf, "%d\n", gpio_dev_in_use->toggle_state);
    
    printk(KERN_ALERT "State for pancake %i\n", gpio_dev_in_use->toggle_state);
    return len;
}

/* Sysfs "store" method for gpio_state  prototype */
static ssize_t gpio_state_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t size) {
    int* gpio_number = dev_get_drvdata(dev);
    
    // Find the current struct in use based of the gpio_number:
    struct gpio_dev* gpio_dev_in_use = container_of(gpio_number,struct gpio_dev, no);

    int err = kstrtou8(buf, 0, &gpio_dev_in_use->toggle_state);

    if(err<0) {
        printk(KERN_ALERT "Unable to parse string in store\n");
        return err;
    }

    if(gpio_dev_in_use->toggle_state == 1) {
        printk(KERN_ALERT "Pancake done");
    }

    printk(KERN_ALERT "State is changed to %i\n", gpio_dev_in_use->toggle_state);
    // Return size even though we only read first char
    return size;
}

/************************************************************
 DEVICE ATTRIBUTES (Exercise 8)
************************************************************/


/* Make device_attributes:*/
// Make dev_attr_gpio_state in precompiler.
// Expects gpio_state_show() and gpio_state_store()
// Same for delay
DEVICE_ATTR_RW(gpio_state);

// Attribute array of all the led attributes we made on lines above
static struct attribute *gpio_attrs[] = {
    &dev_attr_gpio_state.attr,
    // More attributes,
    NULL, // Must end with null
};

/* Make groups */
// Make gpio_groups, expects gpio_attrs. Parameter is just gpio
ATTRIBUTE_GROUPS(gpio);


/************************************************************
* LINUX DEVICE MODEL METHODS (Platform)
*************************************************************/
const int minor_to_use = 0;

static int plat_drv_probe(struct platform_device *pdev){
    static int err = 0;
    struct device *gpio_device = &pdev->dev;
    enum of_gpio_flags flag;
    u32 gpio_num;

    printk(KERN_ALERT "Digital protocol driver device: %s\n", pdev->name);

    if((gpios_len = of_gpio_count(pdev->dev.of_node)) < 0) { // Set gpios_len and check for error
        dev_err(&pdev->dev, "of_gpio_count unreadable\n");
        return -EINVAL;
    }
    
    gpio_devs = kmalloc(gpios_len * sizeof(struct gpio_dev), GFP_ATOMIC);

    for (int i = 0; i < gpios_len; i++)
    {
        gpio_num = of_get_gpio(pdev->dev.of_node, i);
        if(gpio_num < 0) {
            if(gpio_num != -EPROBE_DEFER) {
                dev_err(&pdev->dev, "Failed to parse io %d\n", gpio_num);
            }
        }
        
        err = of_get_gpio_flags(pdev->dev.of_node, i, &flag);
        if(err == -ENOSYS) {
            printk(KERN_ALERT "IDK but a fail in flag reading\n");
            return -ENOSYS;
        }
        
        gpio_devs[i].no = gpio_num;
        gpio_devs[i].dir = flag;
        // Exercise 8: default values
        gpio_devs[i].toggle_state = 0;

        printk(KERN_ALERT "GPIO no: %i, I/O: %i\n", gpio_devs[i].no, gpio_devs[i].dir);
    }   
    
    for (size_t i = 0; i < gpios_len; i++)
    {
        err = gpio_request(gpio_devs[i].no, "GPIO");
        if(err < 0)
            gpio_free(gpio_devs[i].no);

        if (gpio_devs[i].dir == 1){
        err = gpio_direction_output(gpio_devs[i].no, 0);
        if(err < 0)
            gpio_free(gpio_devs[i].no);
        }
        else{
        err = gpio_direction_input(gpio_devs[i].no);
        if(err < 0)
            gpio_free(gpio_devs[i].no);            
        }
        
        gpio_devs[i].IRQ_LINE = gpio_to_irq(gpio_devs[i].no);
        printk("IRQ_LINE: %i\n", gpio_devs[i].IRQ_LINE);

        err = request_irq(gpio_devs[i].IRQ_LINE, mygpio_isr, IRQF_TRIGGER_RISING, "GPIO_IRQ", (void *)i);
        if(err < 0)
        {
            printk(KERN_ALERT "Failed to request irq\n");
            gpio_free(gpio_devs[i].no); 
        }

        // Since ex. 8 we added gpio_devs[i].no argument
        gpio_device = device_create(plat_drv_class, NULL, MKDEV(MAJOR(devno), i), &gpio_devs[i].no, "digital_protocol_dev%d", i);
       
    
        if(IS_ERR(gpio_device)){
            printk(KERN_ALERT "ERROR found\n");
            return -EFAULT;
        }
        else {
            printk(KERN_ALERT "Created /dev/digital_protocol_dev%i with major:%i, minor:%i\n",i,MAJOR(devno),i);
        }
    }

    return 0;
}

static int plat_drv_remove(struct platform_device *pdev){ 
    
    // Delete all our devices based on gpios_len
    for(int i = 0; i < gpios_len; ++i) {
        free_irq(gpio_devs[i].IRQ_LINE, NULL);
        device_destroy(plat_drv_class, MKDEV(MAJOR(devno), i));
    }

    kfree(gpio_devs); // Free the beast
    printk(KERN_ALERT "Devices has been destroyed");

    return 0;
}

/************************************************************
* LINUX CHARACTER DRIVER METHODS
************************************************************/

static int __init plat_drv_init(void) { // could add __init
    static int err = 0;

    // Make device numbers - dynamic
    err = alloc_chrdev_region(&devno,0,255,"plat_drv_driver");
    if(MAJOR(devno) <= 0){
        printk("Failed to register chardev\n");
        goto err_exit;
    }
    printk(KERN_ALERT "Major: %i\n", MAJOR(devno));

    // Cdev
    cdev_init(&plat_drv_cdev, &plat_drv_fops);
    err = cdev_add(&plat_drv_cdev, devno, 255);
    if(err) {
        printk("Cdev failed");
        goto err_cleanup_chrdev;
    }

    // sysfs create class
    plat_drv_class = class_create(THIS_MODULE, "platform_class");
    if (IS_ERR(plat_drv_class)){
        printk("Failed to create class");
        goto err_cleanup_cdev;
    }

    // ATTRIBUTE DEFAULT DEVICE SETTINGS - Must be after class_create and before driver_register
    plat_drv_class->dev_groups = gpio_groups;

    // Register platform driver
    err = platform_driver_register(&plat_drv_platform_driver);
    if(err) {
        printk("Platform registration failed");
        goto err_cleanup_class;
    }

    printk(KERN_ALERT "PLATFORM DRIVER INSTALLED\n");
    return 0; 

  err_cleanup_class:
    class_destroy(plat_drv_class);

  err_cleanup_cdev:
    cdev_del(&plat_drv_cdev);

  err_cleanup_chrdev:
    unregister_chrdev_region(MAJOR(devno), minor_to_use);

  err_exit:
    return err;
}

static void __exit plat_drv_exit(void) { // could add __exit
    platform_driver_unregister(&plat_drv_platform_driver);
    class_destroy(plat_drv_class);
    cdev_del(&plat_drv_cdev);
    unregister_chrdev_region(MAJOR(devno), minor_to_use);
    
    printk(KERN_ALERT "Platform has been deleted\n");
}

ssize_t plat_drv_read(struct file *filep, char *buf, size_t count, loff_t *f_pos) { // could add __user
    int err;
    char str[2];
    int minor;
    minor = iminor(filep->f_inode);
    printk("Kigger lige paa %d\n", minor);

    wait_event_interruptible(IRQ_QUEUE, gpio_devs[minor].int_flag);

    gpio_devs[minor].toggle_state = gpio_get_value(gpio_devs[minor].no);
    sprintf(str, "%d", gpio_devs[minor].toggle_state);
    
    int gpio_len = strlen(str) + 1;
    gpio_len = gpio_len > count ? count : gpio_len;
    err = copy_to_user(buf, str, gpio_len);
    if(err != 0) {
        printk("Error in copy_to_user");
        return -1;
    }
    return gpio_len;
}

ssize_t plat_drv_write(struct file *filep, const char *ubuf, size_t count, loff_t *f_pos) { // could add __user
    static int err = 0;
    int minor, len, value;
    char str[MAXLEN];

    minor = iminor(filep->f_inode);
    printk(KERN_ALERT "Skriver til plat_drv minor: %i \n", minor);

    len = count < MAXLEN ? count : MAXLEN;

    err = copy_from_user(str, ubuf, len);
    if(err){
        printk("Error in copy_from_user");
        return -EFAULT;
    }

    // Null termination to string
     str[len] = '\0';
    
    // Convert string to int
    sscanf(str, "%i", &value); 
    //gpio_value = 0;
    printk("Platform Driver: Writing %i \n", value);
    gpio_set_value(gpio_devs[minor].no, value);
    return count;
}

//ISR routine
static irqreturn_t mygpio_isr(int irq, void *dev_id)
{
    printk("Interrupt %d\n", irq);
    gpio_devs[(int)&dev_id].int_flag = 1;
    wake_up_interruptible(&IRQ_QUEUE);
    return IRQ_HANDLED;
}

// File operations plat_drv
 struct file_operations plat_drv_fops = {
 .owner = THIS_MODULE,
 .read = plat_drv_read,
 .write = plat_drv_write,
 };
//
// Platform driver struct
 static const struct of_device_id platform_device_match[] ={
    { .compatible = "TUcoding, plat_drv",}, {},
 };
 static struct platform_driver plat_drv_platform_driver = {
 .probe = plat_drv_probe,
 .remove = plat_drv_remove,
 .driver = {
    .name = "platform driver",
    .of_match_table = platform_device_match,
    .owner = THIS_MODULE,},
 };

//
/************************************************************
 * GENERIC LINUX DEVICE DRIVER STUFF
 * *********************************************************/

// Assignment of module init/exit methods
 module_init(plat_drv_init);
 module_exit(plat_drv_exit);
 
// Assignment of author and license
 MODULE_LICENSE("GPL");
 MODULE_AUTHOR("Pan-De-Mix");
 MODULE_DESCRIPTION("Digital protocol driver");