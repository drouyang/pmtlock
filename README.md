# Preemptable Ticket Spinlock (pmtlock)
Jiannan Ouyang
ouyang@cs.pitt.edu
06/2013

# Introduction 
Current stable:  **pmtlock-v2.0** (Linux 3.5 - 3.11)

Preemptable Ticket Spinlock (pmtlock) is a lock algorithm designed for virtual
environment proposed by Jiannan Ouyang and John Lange in their VEE 2013 paper, 
"[Preemptable Ticket Spinlocks: Improving Consolidated Performance in the Cloud](http://www.cs.pitt.edu/~ouyang/files/publication/preemptable_lock-ouyang-vee13.pdf)"

This algorithm tackles the lock preemption problem in virtual environment, more
specifically, the lock waiter preemption problem.  The intuition is to
sacrifice fairness to ensure forward progress upon preemption by allow waiters to
acquire locks out of order when an earlier waiter is preempted.  It thus prevents later
waiters from waiting on an available lock.

For more details, please refer to our [paper](http://www.cs.pitt.edu/~ouyang/files/publication/preemptable_lock-ouyang-vee13.pdf) 

# V3.0 Patch 
### pmtlock v3.0 (Linux 3.12+, tested on Linux 4.0)
* pmtlock.patch

### lightweight pmtlock (pmtlock-lt)
pmtlock-lt = pmtlock-compact + pmtlock-static

Patches:
* 1-pmtlock-compact.patch: reduced lock size
* 2-pmtlock-static.patch: static timeout threshold update

# Tools
* paramter: online tuning of the timeout parameter
* lock\_fairness: create N kthread on N cores, competing for the same lock and report lock aquisition distribution and time consumed.
* overhead: measure everage lock/unlock latency

## Updates

### 07/08/2014
Ported pmtlock to Linux 3.12+ (v3.0). Added new kernel modules for evaluation.

### 01/10/2013

v1.0 was published in the VEE 2013 paper. 

v2.0 patch fixes a wrapped-around ticket values bug. It has been tested on 8
core machine running kernel 3.5 and 32 core machines with kernel 3.11.

### 11/22/2013
Linux kernel 3.12 upstreamed the paravirt ticket spinlock patch, which changes
the code layout. Pmtlock need to be ported to newer kernels before using.
