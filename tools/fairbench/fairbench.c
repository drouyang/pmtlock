/**
 * Authors:
 * Qihang Chen
 * Jianna Ouyang (ouyang@cs.pitt.edu)
 *
 * Methodology
 * N kernel threads on N cores competing for the same lock, the distribution of
 * numbers of lock aquisitions indicates lock fairness
 */

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

#define THREAD_MAX 8192

unsigned char naive_lock;
spinlock_t lock;

int lock_num = 0;
int thread_num = 0;
int lock_cnts[THREAD_MAX];
int lock_aquired = 0;
struct task_struct* tasks[THREAD_MAX];

int flag[THREAD_MAX] = {0};

int finish = 0;
spinlock_t finish_lock;

struct timeval start;
struct timeval end;

unsigned long n_spin = 1000;

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


int thread_cnt = 0;
spinlock_t thread_cnt_lock;

// all kthreads should start at the same time
void join(void) {
	spin_lock(&thread_cnt_lock);
	thread_cnt++;
	spin_unlock(&thread_cnt_lock);

	while (ACCESS_ONCE(thread_cnt) != thread_num);
}

void measure_naive_spinlock(unsigned long long thread_id) {

	join();

	while(lock_aquired < lock_num){
		naive_spin_lock(&naive_lock);
		lock_aquired++;
		naive_spin_unlock(&naive_lock);
		lock_cnts[thread_id]++;
	}
}

void measure_spinlock(unsigned long long thread_id) {
	unsigned long flags;

	join();

	while(lock_aquired < lock_num){
		spin_lock_irqsave(&lock,flags);
		lock_aquired++;
		while (n_spin--);
		spin_unlock_irqrestore(&lock, flags);
		lock_cnts[thread_id]++;
	}
}

int measure_lock(void* thread_no){
	unsigned long long i;
	unsigned long long thread_id = (unsigned long long) thread_no;

	if (lock_selected == NAIVE_SPINLOCK) {
		measure_naive_spinlock(thread_id);
	} else {
		measure_spinlock(thread_id);
	}

	spin_lock(&finish_lock);
	if(!finish){
		finish = 1;
		spin_unlock(&finish_lock);
		do_gettimeofday(&end);
		for(i = 0; i < thread_num; i++)
			if(i != (unsigned long long )thread_no) 
				kthread_stop(tasks[i]);
		while(!kthread_should_stop()){
			usleep_range(1, 2);
		};
		//printk("thread %llu completed by cleanup_module\n", (unsigned long long)thread_no);
	} else {
		spin_unlock(&finish_lock);
		while(!kthread_should_stop()){
			usleep_range(1, 2);
		};
		//printk("thread %llu completed normally\n",(unsigned long long)thread_no);
	}

	flag[(unsigned long long)thread_no] = 1;
	return 0;
}

static int __init fairness_init(void)
{
	unsigned long long i;
	printk("init_module() called\n");
	spin_lock_init(&lock);
	spin_lock_init(&thread_cnt_lock);
	spin_lock_init(&finish_lock);
	naive_lock = 0;

	thread_num = num_online_cpus();
	lock_num = 100000 * thread_num;

	printk("[fairness] created of %d kthreads on %d cores\n", thread_num, thread_num);

	for(i = 0; i < thread_num; i++) {
		tasks[i] = kthread_create(measure_lock, (void*)i, "KTHREAD %lld", i);
		kthread_bind(tasks[i], i % thread_num);
	}

	do_gettimeofday(&start);
	for(i = 0; i < thread_num; i++)
		if (!IS_ERR(tasks[i])) wake_up_process(tasks[i]);
	return 0;
}

static void __exit fairness_exit(void)
{
	int i;
	for(i = 0; i < thread_num; i++){
		if(!flag[i]) 
			kthread_stop(tasks[i]);
		printk("[fairness] thread %d: %d\n", i, lock_cnts[i]);
	}

	printk("[fairness] Time elapsed (us): %ld\n", (end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));
	printk("exit_module() called\n");
}

module_init(fairness_init);
module_exit(fairness_exit);
MODULE_AUTHOR("Qihang Chen, Jiannan Ouyang");
MODULE_LICENSE("GPL");
