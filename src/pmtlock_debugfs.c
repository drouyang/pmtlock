#include <linux/debugfs.h>

struct dentry *d_pmtlock_debugfs;

/*
static int pmtlock_open(struct inode *inode, struct file *file)
{
}

static ssize_t pmtlock_write(struct file *file, const char __user *buf,
        size_t count, loff_t *ppos)
{
}

static int pmtlock_release(struct inode *inode, struct file *file)
{
}

static const struct file_operations proc_pmtlock_operations = {
    .open		= pmtlock_open,
    .write		= pmtlock__write,
    .read		= seq_read,
    .llseek		= seq_lseek,
    .release	= pmtlock_release,
};
*/

int pmtlock_init_debugfs(void)
{
    d_pmtlock_debugfs = debugfs_create_dir("pmtlock", NULL);
    if (d_pmtlock_debugfs == NULL) {
        printk(KERN_WARNING "Could not create 'pmtlock' debugfs directory\n");
        return -1;
    }

    debugfs_create_u64("pmtlock_timeout_unit", 0644, d_pmtlock_debugfs, &pmtlock_timeout_unit);

    return 0;
}

int pmtlock_exit_debugfs(void)
{
    debugfs_remove_recursive(d_pmtlock_debugfs);

    return 0;
}
