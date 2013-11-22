# Preemptable Ticket Spinlock (pmtlock)
Jiannan Ouyang
ouyang@cs.pitt.edu
06/2013

# Introduction 
Current stable:  **pmtlock-v2.0** 

Preemptable Ticket Spinlock (pmtlock) is a lock algorithm designed for virtual
environment proposed by Jiannan Ouyang and John Lange in their VEE 2013 paper, 
"[Preemptable Ticket Spinlocks: Improving Consolidated Performance in the Cloud](http://www.cs.pitt.edu/~ouyang/files/publication/preemptable_lock-ouyang-vee13.pdf)"

This algorithm tackles the lock preemption problem in virtual environment, more
specifically, the lock waiter preemption problem.  The intuition is to
sacrifice fairness to ensure forward progress upon preemption by allow waiters to
acquire locks out of order when an earlier waiter is preempted.  It thus prevents later
waiters from waiting on an available lock.

For more details, please refer to our [paper](http://www.cs.pitt.edu/~ouyang/files/publication/preemptable_lock-ouyang-vee13.pdf) 

# Tools
A kernel module is provided to tune timeout parameter online for v2.0 patch.

## Updates
### 01/10/2013

v1.0 was published in the VEE 2013 paper. 

v2.0 patch fixes a wrapped-around ticket values bug. It has been tested on 8
core machine running kernel 3.5 and 32 core machines with kernel 3.11.

### 11/22/2013
Linux kernel 3.12 upstreamed the pv_lock patch, which is not compatible with
current pmtlock implementation.  Porting work is still under going.


