#include <linux/module.h>  //...
#include <linux/init.h>  //module_init(), module_exit()

MODULE_AUTHOR("Au Duong Tan Sang 1712145");
MODULE_DESCRIPTION("Simple hooker LKM");
MODULE_LICENSE("GPL");

//LKM loader
static int __init hooker_init(void)
{
    printk("[+] hooker loaded.\n");
    return 0;
}

//LKM unloader
static void __exit hooker_exit(void)
{
    printk("[-] hooker unloaded.\n");
    return;
}

module_init(hooker_init);
module_exit(hooker_exit);