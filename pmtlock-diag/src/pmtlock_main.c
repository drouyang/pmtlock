/*
 * pmtlock
 * (c) Jiannan Ouyang, 2013
 */

#include <linux/kernel.h>
#include <linux/module.h>

#include "pmtlock_debugfs.h"

#define AUTH "Jiannan Ouyang <ouyang@cs.pitt.edu>"
#define DESC "pmtlock admin and stat tool"

static int pmtlock_init(void) {
    //int ret = 0;

    printk(KERN_INFO "pmtlock: loaded\n");

    pmtlock_init_debugfs();

    return 0;
}

static void pmtlock_exit(void) {

    pmtlock_exit_debugfs();

    printk(KERN_INFO "pmtlock: exit\n");

    return;
}

module_init(pmtlock_init);
module_exit(pmtlock_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jiannan Ouyang <ouyang@cs.pitt.edu>");
MODULE_DESCRIPTION("pmtlock admin and stat tool");

