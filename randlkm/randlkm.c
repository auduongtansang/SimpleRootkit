#include <linux/random.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>

MODULE_AUTHOR("Au Duong Tan Sang 1712145");
MODULE_DESCRIPTION("Random number LKM");
MODULE_LICENSE("GPL");

char data[512] = "";

void NumToChar(unsigned int num, char cha[])
{
    int cnt = 0, i, j;
    do
    {
        cha[cnt++] = (char)(num % 10 + 48);
        num /= 10;
    } while (num != 0);
    
    i = 0;
    j = cnt - 1;
    while (i < j)
    {
        char temp = cha[i];
        cha[i] = cha[j];
        cha[j] = temp;
        i++; j--;
    }
    
    cha[cnt] = '\0';
}

ssize_t user_read(struct file *fp, char *buf, size_t cnt, loff_t *of)
{
    unsigned int number = 0;
    get_random_bytes(&number, sizeof(number));
    NumToChar(number, data);

    if (copy_to_user(buf, data, strlen(data)) != 0)
    {
        printk("copy_to_user(): fatal error.\n");
        return -1;
    }
    return strlen(data);
}

static struct file_operations randops =
{
    .read = user_read
};

static struct miscdevice randdev =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "randlkm",
    .fops = &randops
};

static int __init rand_init(void)
{
    misc_register(&randdev);
    printk("Loaded.\n");
    return 0;
}

static void __exit rand_exit(void)
{
    misc_deregister(&randdev);
    printk("Unloaded.\n");
    return;
}

module_init(rand_init);
module_exit(rand_exit);