#include <linux/cdev.h>   // cdev_add, cdev_init
#include <linux/uaccess.h>  // copy_to_user
#include <linux/module.h> // module_init, GPL
#include <linux/spi/spi.h> // spi_sync,

#include "ADCVector.h"
#include "vectorRC.h"

#define MAXLEN 32
#define MODULE_DEBUG 0   // Enable/Disable Debug messages
#define BYTE_TO_BINARY(byte) \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') //Define for printing out bitwise for debugging


MODULE_LICENSE("GPL");

/* Char Driver Globals */
static struct spi_driver AnalogStick_spi_driver;
struct file_operations AnalogStick_fops;
static struct class *AnalogStick_class;
static dev_t devno;
static struct cdev AnalogStick_cdev;
static struct ADCVector AnalogStickVec;
/* Definition of SPI devices */

struct Myspi {
    struct spi_device *spi; // Pointer to SPI device
    int channel;            // channel, ex. adc ch 0
};

struct Myspi spi_dev;
/* Macro to handle Errors */
#define ERRGOTO(label, ...)                     \
{                                               \
    printk (__VA_ARGS__);                       \
    goto label;                                 \
} while(0)
/**********************************************************
 * CHARACTER DRIVER METHODS
 **********************************************************/

int readADCChannel(int channel); //Function for retrieving ADC value.

static char * AnalogStick_devnode(struct device * dev, umode_t * mode) {
    if(!mode) {
        return NULL;
    }
    if(dev->devt == MKDEV(MAJOR(devno), 0)) {
        *mode = 0666;
    }
    return NULL;
}

//Character Driver Module Init Method
static int __init AnalogStick_init(void) {
    int err=0;
    if (MODULE_DEBUG)
        printk("AnalogStick driver initializing\n");
    
    /* Allocate major number and register fops*/
    err = alloc_chrdev_region(&devno, 0, 255, "analog stick driver");
    if(MAJOR(devno) <= 0)
        ERRGOTO(err_no_cleanup, "Failed to register chardev\n");
    if (MODULE_DEBUG)
        printk(KERN_ALERT "Assigned major no: %i\n", MAJOR(devno));
    cdev_init(&AnalogStick_cdev, &AnalogStick_fops);
    err = cdev_add(&AnalogStick_cdev, devno, 255);
    if (err)
        ERRGOTO(err_cleanup_chrdev, "Failed to create class");

    /* Polulate sysfs entries */
    AnalogStick_class = class_create(THIS_MODULE, "analog stick class");
    if (IS_ERR(AnalogStick_class))
        ERRGOTO(err_cleanup_cdev, "Failed to create class");

    //change permissions
    AnalogStick_class->devnode = AnalogStick_devnode;

    /* Register SPI Driver */
    /* THIS WILL INVOKE PROBE, IF DEVICE IS PRESENT!!! */
    err = spi_register_driver(&AnalogStick_spi_driver);
    if(err)
        ERRGOTO(err_cleanup_class, "Failed SPI Registration\n");
    /* Success */
    return 0;

    /* Errors during Initialization */
    err_cleanup_class:
        class_destroy(AnalogStick_class);
    err_cleanup_cdev:
        cdev_del(&AnalogStick_cdev);
    err_cleanup_chrdev:
        unregister_chrdev_region(devno, 255);
    err_no_cleanup:
        return err;
}

//Character Driver Module Exit Method
static void __exit AnalogStick_exit(void) {
    spi_unregister_driver(&AnalogStick_spi_driver);
    class_destroy(AnalogStick_class);
    cdev_del(&AnalogStick_cdev);
    unregister_chrdev_region(devno, 255);
}

//Character Driver Read File Operations Method
ssize_t AnalogStick_read(struct file *filep, char __user *ubuf, size_t count, loff_t *f_pos)
{
    if (*f_pos >= 2)
        return 0;
    int len, ADC1, ADC0;
    char result, resultBuf[MAXLEN];
    ADC0 = readADCChannel(0);
    ADC1 = readADCChannel(1);
    ADCToVector(&AnalogStickVec, ADC0, ADC1);
    result = dataToSend(&AnalogStickVec);
    if (MODULE_DEBUG){
        printk("ADC0: %i, ADC1: %i. char = %d\n", ADC0, ADC1, result);
        printk("Result in bin: %c%c%c%c%c%c%c%c\n", BYTE_TO_BINARY(result));
    }
    // Convert integer to string limited to "count" size. Returns * length excluding NULL termination
    len = snprintf(resultBuf, count, "%d\n", result);
    // Copy data to user space
    if(copy_to_user(ubuf, resultBuf, len))
        return -EFAULT;
    // Move fileptr
    *f_pos += len;
    return len;
}

//Character Driver File Operations Structure
struct file_operations AnalogStick_fops = {
    .owner   = THIS_MODULE,
    .read    = AnalogStick_read,
};
/**********************************************************
 * LINUX DEVICE MODEL METHODS (spi)
 **********************************************************/

//AnalogStick Probe - Called when a device with the name "AnalogStick" is registered
static int AnalogStick_probe(struct spi_device *sdev) {
    int err = 0;
    struct device *AnalogStick_device;
    if (MODULE_DEBUG)
        printk(KERN_DEBUG "New SPI device: %s using chip select: %i\n", sdev->modalias, sdev->chip_select);
    /* Configure bits_per_word, always 8-bit for RPI!!! */
    sdev->bits_per_word = 8;
    spi_setup(sdev);
    /* Create devices, populate sysfs and active udev to create devices in /dev */
    /* We map spi_devs index to minor number here */
    AnalogStick_device = device_create(AnalogStick_class, NULL, MKDEV(MAJOR(devno), 0), NULL, "AnalogStick");
    if (IS_ERR(AnalogStick_device))
        printk(KERN_ALERT "FAILED TO CREATE DEVICE\n");
    else
        if (MODULE_DEBUG)
            printk(KERN_ALERT "Using spi_dev on major:%i, minor:0\n", MAJOR(devno));
    /* Update local array of SPI devices */
    spi_dev.spi = sdev;
    spi_dev.channel = 0x00; // channel address
    init(readADCChannel(1), readADCChannel(0));
    return err;
}

//AnalogStick Remove - Called when the device is removed - Can deallocate data if needed
static int AnalogStick_remove(struct spi_device *sdev) {
    int its_minor = 0;
    if (MODULE_DEBUG)
        printk(KERN_ALERT "Removing spi device\n");
    /* Destroy devices created in probe() */
    device_destroy(AnalogStick_class, MKDEV(MAJOR(devno), its_minor));
    return 0;
}

//spi Driver Struct - Holds function pointers to probe/release methods and the name under which it is registered
static const struct of_device_id of_AnalogStick_spi_device_match[] = {
  { .compatible = "DEM, AnalogStick", }, {}, };

static struct spi_driver AnalogStick_spi_driver = {
    .probe      = AnalogStick_probe,
    .remove     = AnalogStick_remove,
    .driver     = {
        .name   = "AnalogStick",
        .bus    = &spi_bus_type,
        .of_match_table = of_AnalogStick_spi_device_match,
        .owner  = THIS_MODULE,
    },
};

//Helping function for reading ADC
int readADCChannel(int channel) {
    int result;
    char resultBuf[2];
    struct spi_transfer t[3];
    struct spi_message m;
    memset(t, 0, sizeof(t));
    spi_message_init(&m);
    m.spi = spi_dev.spi;

    char startcmd = 0b00000001;
    char configcmd = (channel ? 0b11100000 : 0b10100000); 

    t[0].tx_buf = &startcmd;
    t[0].rx_buf = NULL;
    t[0].len = 1;
    spi_message_add_tail(&t[0], &m);

    t[1].tx_buf = &configcmd;
    t[1].rx_buf = resultBuf;
    t[1].len = 1;
    spi_message_add_tail(&t[1], &m);

    t[2].tx_buf = NULL;
    t[2].rx_buf = resultBuf + 1;
    t[2].len = 1;
    spi_message_add_tail(&t[2], &m);

    spi_sync(m.spi, &m);
    result = (resultBuf[0]<<8) + resultBuf[1];
    if (MODULE_DEBUG)
        printk("Read from ch %d: i1 = %d, i2 = %d. Returned %i\n", channel, resultBuf[0], resultBuf[1], result);
    return result;
}
/**********************************************************
 * GENERIC LINUX DEVICE DRIVER STUFF
 **********************************************************/
/*
 * Assignment of module init/exit methods
 */
module_init(AnalogStick_init);
module_exit(AnalogStick_exit);