#include <linux/module.h>  //...
#include <asm/unistd.h>  //__NR_open, __NR_write
#include <linux/init.h>  //module_init(), module_exit()
#include <linux/sched.h>  //current pointer
#include <linux/uaccess.h>  //copy_from_user()
#include <linux/kallsyms.h>  //kallsyms_lookup_name()

MODULE_AUTHOR("Au Duong Tan Sang 1712145");
MODULE_DESCRIPTION("Hooker LKM");
MODULE_LICENSE("GPL");

//sys_call_table address
void **sys_call_table_addr;

//Pointer to original syscalls
asmlinkage int (*original_open)(const char*, int);

//New syscall open()
asmlinkage int new_open(const char *pathname, int flags)
{
	//Copy file path argument from user-land
	char path[512];
	copy_from_user(path, pathname, strlen(pathname));
	path[strlen(pathname)] = '\0';
	
	//Logging and return original open()
    printk(KERN_INFO "[**] %s has called open() on %s.\n", current->comm, path);
    return original_open(pathname, flags);
}

//Make the page writable
void make_page_rw(unsigned long addr)
{
    unsigned int level;
    pte_t *pte = lookup_address(addr, &level);
    if (pte->pte & ~_PAGE_RW)
        pte->pte |= _PAGE_RW;
}

//Make the page read-only
void make_page_ro(unsigned long addr)
{
    unsigned int level;
    pte_t *pte = lookup_address(addr, &level);
    pte->pte &= ~_PAGE_RW;
}

//LKM loader
static int __init hooker_init(void)
{
    //Looking for address of sys_call_table
    sys_call_table_addr = (void*)kallsyms_lookup_name("sys_call_table");

    //Disable write-protection
    write_cr0(read_cr0() & (~0x10000));
    make_page_rw((unsigned long)sys_call_table_addr);

    //Store original syscalls and replace by the new ones
    original_open = sys_call_table_addr[__NR_open];
    sys_call_table_addr[__NR_open] = new_open;

    //Re-enable write-protection
    write_cr0(read_cr0() | 0x10000);
    make_page_ro((unsigned long)sys_call_table_addr);

    printk("[+] hooker loaded.\n");
    return 0;
}

//LKM unloader
static void __exit hooker_exit(void)
{
    //Disable write-protection
    write_cr0(read_cr0() & (~0x10000));
    make_page_rw((unsigned long)sys_call_table_addr);

    //Restore original syscalls
    sys_call_table_addr[__NR_open] = original_open;

    //Re-enable write-protection
    write_cr0(read_cr0() | 0x10000);
    make_page_ro((unsigned long)sys_call_table_addr);

    printk("[-] hooker unloaded.\n");
    return;
}

module_init(hooker_init);
module_exit(hooker_exit);
