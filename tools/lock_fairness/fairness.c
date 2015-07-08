/**
 * Authors 
 * Qihang Chen, Jianna Ouyang (ouyang@cs.pitt.edu)
 *
 * Methodology
 *
 *
 * Misc
 * set thread affinity in a linux kernel module 
 * The reason to use kthread_create_on_cpu is twofold:
 * 1) there's no way two kthreads on the same core are contending for a lock , its the scheduler's responsibility to schedule the kthreads on the same core
 * 2) if two kthreads are on the same core, there's no cache coherence issue for those two, which means the chances for the two threads to get a lock is the same
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

// number of conccurent kthread
#define THREAD_NUM 6

unsigned char naive_lock;
spinlock_t lock;

int LOCK_NUM = 100000 * THREAD_NUM;
int lock_cnts[THREAD_NUM];
int lock_aquired = 0;
struct task_struct* tasks[THREAD_NUM];

int flag[THREAD_NUM] = {0};

int finish = 0;
spinlock_t finish_lock;

struct timeval start;
struct timeval end;

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

	while (ACCESS_ONCE(thread_cnt) != THREAD_NUM);
}

void measure_naive_spinlock(unsigned long long thread_id) {

	join();

	while(lock_aquired < LOCK_NUM){
		naive_spin_lock(&naive_lock);
		lock_aquired++;
		naive_spin_unlock(&naive_lock);
		lock_cnts[thread_id]++;
	}
}

void measure_spinlock(unsigned long long thread_id) {
	unsigned long flags;

	join();

	while(lock_aquired < LOCK_NUM){
		spin_lock_irqsave(&lock,flags);
		lock_aquired++;
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
		for(i = 0; i < THREAD_NUM; i++)
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
	for(i = 0; i < THREAD_NUM; i++) {
		tasks[i] = kthread_create(measure_lock, (void*)i, "KTHREAD %lld", i);
		kthread_bind(tasks[i], i % THREAD_NUM);
	}

	do_gettimeofday(&start);
	for(i = 0; i < THREAD_NUM; i++)
		if (!IS_ERR(tasks[i])) wake_up_process(tasks[i]);
	return 0;
}

static void __exit fairness_exit(void)
{
	int i;
	for(i = 0; i < THREAD_NUM; i++){
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
