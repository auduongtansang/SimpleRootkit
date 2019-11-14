#include <linux/module.h>  //...
#include <linux/random.h>  //get_random_bytes()
#include <linux/fs.h>  //file_operations structure
#include <linux/init.h>  //module_init(), module_exit()
#include <linux/uaccess.h>  //copy_to_user()
#include <linux/miscdevice.h>  //miscdevice structure

MODULE_AUTHOR("Au Duong Tan Sang 1712145");
MODULE_DESCRIPTION("Random number LKM");
MODULE_LICENSE("GPL");

char data[512] = "";

//Numbers to characters
void NumToChar(unsigned int num, char cha[])
{
    //Put numbers to a character array
    int cnt = 0, i, j;
    do
    {
        cha[cnt++] = (char)(num % 10 + 48);
        num /= 10;
    } while (num != 0);
    
    //Reverse the array
    i = 0;
    j = cnt - 1;
    while (i < j)
    {
        char temp = cha[i];
        cha[i] = cha[j];
        cha[j] = temp;
        i++; j--;
    }
    
    //End of the array
    cha[cnt] = '\0';
}

//Occur when an user application call read()
ssize_t user_read(struct file *fp, char *buf, size_t cnt, loff_t *of)
{
    //Generate some random bytes and change them to characters
    unsigned int number = 0;
    get_random_bytes(&number, sizeof(number));
    NumToChar(number, data);

    //Send those characters to user application
    if (copy_to_user(buf, data, strlen(data)) != 0)
    {
        printk("copy_to_user(): fatal error.\n");
        return -1;
    }
    return strlen(data);
}

//We only use "read" operation
static struct file_operations randops =
{
    .read = user_read
};

//Identity of device file
static struct miscdevice randdev =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "randlkm",
    .fops = &randops
};

//LKM loader
static int __init rand_init(void)
{
    //Register device file
    misc_register(&randdev);
	
    printk("[+] randlkm loaded.\n");
    return 0;
}

//LKM unloader
static void __exit rand_exit(void)
{
    //Unregister device file
    misc_deregister(&randdev);
	
    printk("[-] randlkm unloaded.\n");
    return;
}

module_init(rand_init);
module_exit(rand_exit);
