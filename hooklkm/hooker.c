#include <linux/module.h>  //...
#include <linux/init.h>  //module_init(), module_exit()
#include <linux/kallsyms.h>  //kallsyms_lookup_name()

MODULE_AUTHOR("Au Duong Tan Sang 1712145");
MODULE_DESCRIPTION("Simple hooker LKM");
MODULE_LICENSE("GPL");

//sys_call_table address
void **sys_call_table_addr;

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

    //Re-enable write-protection
    write_cr0(read_cr0() | 0x10000);
    make_page_ro((unsigned long)sys_call_table_addr);

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