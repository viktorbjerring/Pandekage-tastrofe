#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define BYTE_TO_BINARY(byte) \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') //Used for debugging, for printing bitwise

#define DEV_MINOR 0
#define NBR_MINORS 1

struct gpio_dev {
  char pin;     /* Physical GPIO Pin */
  char dir;     /* 0 = In -- 1 = Out */
  char id[8];   /* Identifier */
};
static struct gpio_dev * gpio_devs;
static u8 gpio_len;
static u8 result_len;

static int devno;
static struct class *buttons_class;
static struct cdev my_cdev;

static char * buttons_devnode(struct device * dev, umode_t * mode) {
    if(!mode) {
        return NULL;
    }
    if(dev->devt == MKDEV(MAJOR(devno), 0)) {
        *mode = 0666;
    }
    return NULL;
}
/********************************************************
 *                   File Operations                    *
 ********************************************************/
static ssize_t buttons_read(struct file *filep, char __user *buf, size_t count, loff_t *f_pos)
{
  if(*f_pos >= 1)
    return 0;

  int err;
  char result[result_len];

  /* Pack GPIO values */
  memset(result, 0, result_len);
  for(int i = 0; i < gpio_len; i++)
  {
    int j = DIV_ROUND_DOWN_ULL(i, 8);
    result[j] |= (gpio_get_value(gpio_devs[i].pin)) << (j - (i % 8));
  }
  
  /* Convert result to string and copy string to user space */
  char my_buf[result_len + 1];
  int my_buf_len = 0;
  for(int i = 0; i < result_len; i++)
    my_buf_len += snprintf(&my_buf[i], 256, "%d", result[i]);
  my_buf_len += snprintf(&my_buf[result_len], 256, "\n");
  err = copy_to_user(buf, my_buf, my_buf_len);
  if (err < 0)
  {
    printk(KERN_ERR "Buttons Driver: Error copying to user.\n");
    return err;
  }

  /* Print result in binary for debugging */
  printk("Result in bin: %c%c%c%c%c%c%c%c\n", BYTE_TO_BINARY(result[0]));
  
  /* Increment read pointer */
  *f_pos += my_buf_len;

  return my_buf_len;
}

static struct file_operations my_fops = {
  .owner = THIS_MODULE,
  .read = buttons_read
};


/********************************************************
 *             Platform Driver Operations               *
 ********************************************************/
static int buttons_probe(struct platform_device *pdev)
{
  int err;
  struct device *gpio_device = &pdev->dev;
  printk("Buttons Driver: I've been probed!\n");

  /* Get amount of GPIO pins */
  err = of_gpio_count(pdev->dev.of_node);
  if(err < 0)
    goto err_exit;
  gpio_len = err;

  /* Find lowest amount of chars to hold GPIO values */
  result_len = DIV_ROUND_UP(gpio_len, 8);

  /* Allocate memory */
  gpio_devs = kmalloc_array(gpio_len, sizeof(struct gpio_dev), GFP_KERNEL);
  if(IS_ERR(gpio_devs))
  {
    err = PTR_ERR(gpio_devs);
    goto err_exit;
  }

  /* Fetch GPIO ports from overlay. Request and setup GPIOs */
  int count = 0;
  for (int i = 0; i < gpio_len; i++)
  {
    /* Fetch GPIO pin and direction. Set id of GPIO */
    gpio_devs[i].pin = of_get_gpio(pdev->dev.of_node, i);
    of_get_gpio_flags(pdev->dev.of_node, i, (enum of_gpio_flags*)&gpio_devs[i].dir);
    snprintf(gpio_devs[i].id, 16, "GPIO%d", gpio_devs[i].pin);

    /* Request GPIO pin */
    err = gpio_request(gpio_devs[i].pin, "Buttons");
    if (err < 0)
    {
      dev_err(&pdev->dev, "Failed to request GPIO%d", gpio_devs[i].pin);
      goto err_reg;
    }

    /* Set debounce time */
    err = gpio_set_debounce(gpio_devs[i].pin, 100);
    if (err == -ENOSYS)
      dev_err(&pdev->dev, "Debouncing not available on current device\n");

    /* Set direction of GPIO port */
    if (gpio_devs[i].dir == 0)
      gpio_direction_input(gpio_devs[i].pin);
    else
      gpio_direction_output(gpio_devs[i].pin, 0);

    printk("Buttons Driver: Requested %s\n", gpio_devs[i].id);

    count++;
  }

  /* Make device */
  gpio_device = device_create(buttons_class, NULL, MKDEV(MAJOR(devno), 0), NULL, "Buttons");
  if(IS_ERR(gpio_device))
    { err = PTR_ERR(gpio_device); goto err_reg; }
  
  printk("Buttons Driver: Device /dev/Buttons created\n");

  return 0;

  err_reg:
    /* Unregister all the currently registered GPIOs */
    for(int i = 0; i < count; i++)
      gpio_free(gpio_devs[i].pin);
    /* Free dynamic memory */
    kfree(gpio_devs);
  err_exit:
    printk("Buttons Driver: There has been an error! Error code %i\n", err);
    return err;
}

static int buttons_remove(struct platform_device *dev)
{
  printk("Buttons Driver: I've been removed!\n");

  for(int i = 0; i < gpio_len; i++)
  {
    gpio_free(gpio_devs[i].pin); /* Free all GPIO pins */
    device_destroy(buttons_class, MKDEV(MAJOR(devno), i));
    printk("Buttons Driver: Removed device %s!\n", gpio_devs[i].id);
  }

  /* Free memory */
  kfree(gpio_devs);

  return 0;
}

static const struct of_device_id buttons_device_match[] = {
  { .compatible = "DEM, buttons_drv",}, {},
};
static struct platform_driver buttons_platform_driver = {
  .probe = buttons_probe,
  .remove = buttons_remove,
  .driver = {
    .name = "buttons_driver",
    .of_match_table = buttons_device_match,
    .owner = THIS_MODULE,
  },
};


/********************************************************
 *                   INIT / EXIT                        *
 ********************************************************/
static int buttons_init(void)
{
  int err;

  /* Make device numbers */
  err = alloc_chrdev_region(&devno, 0, NBR_MINORS, "Buttons");
  if (err < 0)
    goto err_alloc;

  /* Create class */
  buttons_class = class_create(THIS_MODULE, "buttons_cls");
  if(IS_ERR(buttons_class))
    goto err_cls;


  /* Change permissions */
  buttons_class->devnode = buttons_devnode;


  /* Create character device */
  cdev_init(&my_cdev, &my_fops);
  err = cdev_add(&my_cdev, devno, NBR_MINORS);
  if (err < 0)
    goto err_cdev;

  /* Register platform driver */
  err = platform_driver_register(&buttons_platform_driver);
  if(err < 0)
    goto err_plat;

  printk("Buttons Driver: Succesfully loaded.\n");
  return 0;

  err_plat:
    cdev_del(&my_cdev);
  err_cdev:
    class_destroy(buttons_class);
  err_cls:
    unregister_chrdev_region(devno, 1);
  err_alloc:
    return err;
}

static void buttons_exit(void)
{
  platform_driver_unregister(&buttons_platform_driver); /* Unregister platform driver */
  cdev_del(&my_cdev); /* Delete character device */
  class_destroy(buttons_class); /* Delete class */
  unregister_chrdev_region(devno, NBR_MINORS); /* Remove device numbers */
  printk("Buttons Driver: Succesfully unloaded.\n");
}

module_init(buttons_init);
module_exit(buttons_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Oskari Tuormaa");
MODULE_DESCRIPTION("Simple GPIO driver");
