# Preemptable Ticket Spinlock (pmtlock)
Jiannan Ouyang
ouyang@cs.pitt.edu

# Introduction 
Current stable:  **pmtlock-v3.0** (Linux 3.12+), 07/2015

Preemptable ticket spinlock (pmtlock) is a novel spinlock algorithm designed
for virtual environments. It addresses the lock waiter preemption problem in
ticket spinlocks, where a lock waiter in a FIFO queue is preempted and later
waiters have to busy-wait even if the lock is already released.

The intuition of pmtlock is to sacrifice fairness to ensure forward progress
only when lock waiter preemption happens. This goal is achieved by allowing
waiters to acquire locks out of order when waiting longer than a threashold,
which indicates one or more earlier waiters have been preempted. The timeout
threshold of each waiter is propotional to the number of previous waiters. As a
result, fairness is largely preserved.

For more details, please refer to our paper "[Preemptable Ticket Spinlocks:
Improving Consolidated Performance in the
Cloud](http://dl.acm.org/citation.cfm?id=2451549)"
in proceedings of the 9th ACM SIGPLAN/SIGOPS International Conference on
Virtual Execution Environments (VEE '13).

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
