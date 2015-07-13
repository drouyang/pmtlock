# Preemptable Ticket Spinlock (pmtlock)
Jiannan Ouyang
ouyang@cs.pitt.edu

# Introduction 
Current stable:  **pmtlock-v3.0.patch** (Linux 3.12+), 07/2015

Preemptable ticket spinlock (pmtlock) is a novel spinlock algorithm designed
for virtual environments. It addresses the lock waiter preemption problem in
ticket spinlocks, where a lock waiter in a FIFO queue is preempted and later
waiters have to busy-wait even if the lock is already released.

The intuition of pmtlock is to sacrifice fairness to ensure forward progress
only when lock waiter preemption happens. This goal is achieved by allowing
waiters to acquire locks out of order when waiting longer than a threshold,
which indicates one or more earlier waiters have been preempted. The timeout
threshold of each waiter is proportional to the number of previous waiters. As a
result, fairness is largely preserved.

For more details, please refer to our paper "[Preemptable Ticket Spinlocks:
Improving Consolidated Performance in the
Cloud](http://dl.acm.org/citation.cfm?id=2451549)"
in proceedings of the 9th ACM SIGPLAN/SIGOPS International Conference on
Virtual Execution Environments (VEE '13).

# Tools
* kernel-modules/fairbench: spinlock fairness benchmark. It creates N kthreads on N cores
  competing for the same lock, and reports the lock acquisition distribution
as well as the time consumed.
* kernel-modules/parameter: online tuning of the timeout parameter
* kernel-modules/overhead: measure average lock/unlock latency

## Updates

### 07/08/2015
Added pmtlock-v3.0, supports Linux 3.12+, tested on Linux 4.0.

v3.0 patches adopts a compact implementation (pmt-cpt), plus a statc timeout
update scheme (pmt-cpt-st).  Those improvements reduce the pmtlock size by
half, yields a simpler implementation, while ensuring the same level of
performance and fairness comparing to v2.0 implementation.

* pmt.patch: ported v2.0 to linux 3.12+ 
* pmt-cpt.patch: reduced lock size
* pmt-cpt-st.patch (pmtlock-v3.0.patch): reduce lock size, plus static timeout threshold update.

Added fairbench for spinlock fairness evaluation.

### 11/22/2013
Linux kernel 3.12 upstreamed the paravirt ticket spinlock patch, which changes
the code layout. Pmtlock need to be ported to newer kernels before using.

### 01/10/2013

v1.0 was published in the VEE 2013 paper. 

v2.0 patch fixes a wrapped-around ticket values bug. It has been tested on 8
core machine running kernel 3.5 and 32 core machines with kernel 3.11.

