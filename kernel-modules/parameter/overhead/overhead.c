/* Author: Qihang Chen, Jianna Ouyang (ouyang@cs.pitt.edu) */

#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#define NAIVE_SPINLOCK 0
#define SYSTEM_SPINLOCK 1

/* select target lock algorithm */
unsigned long long lock_selected = SYSTEM_SPINLOCK;
//unsigned long long lock_selected = NAIVE_SPINLOCK;

static inline unsigned long long my_rdtsc(void) {
        unsigned long long d;
	rdtscll(d);
        return d;
}


unsigned char naive_lock;
spinlock_t lock;

int LOCK_NUM = 100000;
int lock_aquired = 0;
struct task_struct* task;

void naive_spin_lock(unsigned char *lock) {
	for (;;) {
		if (xchg(lock, 1) == 0)
			return;
		cpu_relax();
	}
}

void naive_spin_unlock(unsigned char *lock) {
	xchg(lock, 0);
}

void measure_naive_spinlock(void) {
	unsigned long long start, end;
	unsigned long long elapsed_cycles;

	start = my_rdtsc();	
	while(lock_aquired < LOCK_NUM){
		naive_spin_lock(&naive_lock);
		lock_aquired++;
		naive_spin_unlock(&naive_lock);
	}
	end = my_rdtsc();

	printk("start = %llu \n", start);
	printk("end   = %llu \n", end);

	elapsed_cycles = end - start;

	printk("cycles = %llu \n", elapsed_cycles);
	printk("cycles per lock/unlock = %llu \n", elapsed_cycles / LOCK_NUM);
}

void measure_spinlock(void) {
	unsigned long flags;
	unsigned long long start, end;
	unsigned long long elapsed_cycles;

	start = my_rdtsc();	
	while(lock_aquired < LOCK_NUM){
		spin_lock_irqsave(&lock,flags);
		lock_aquired++;
		spin_unlock_irqrestore(&lock, flags);
	}
	end = my_rdtsc();

	printk("start = %llu \n", start);
	printk("end   = %llu \n", end);

	elapsed_cycles = (unsigned long long) (end - start);

	printk("cycles = %llu \n", elapsed_cycles);
	printk("cycles per lock/unlock = %llu \n", elapsed_cycles / LOCK_NUM);
}

int measure_lock(void){

	if (lock_selected == NAIVE_SPINLOCK) {
		measure_naive_spinlock();
	} else {
		measure_spinlock();
	}

	return 0;
}

static int __init fairness_init(void)
{
	printk("init_module() called\n");

	spin_lock_init(&lock);
	naive_lock = 0;

	measure_lock();

	return 0;
}

static void __exit fairness_exit(void)
{
	printk("exit_module() called\n");
}

module_init(fairness_init);
module_exit(fairness_exit);
MODULE_AUTHOR("Qihang Chen, Jiannan Ouyang");
MODULE_LICENSE("GPL");
