# Preemptable Ticket Spinlock (pmtlock)
Jiannan Ouyang
ouyang@cs.pitt.edu
06/2013

Preemptable Ticket Spinlock (pmtlock) is a lock algorithm designed for virtual
environment proposed by Jiannan Ouyang and John Lange in their VEE 2013 paper, 
"[Preemptable Ticket Spinlocks: Improving Consolidated Performance in the Cloud](http://www.cs.pitt.edu/~ouyang/files/publication/preemptable_lock-ouyang-vee13.pdf)"

This algorithm tackles the lock preemption problem in virtual environment, more
specifically, the lock waiter preemption problem.  The intuition is to
sacrifices fairness to ensure forward progress upon preemption, but also
preserve fairness whenever possible. It downgrades a fair lock to an unfair
lock when a lock waiter is preempted to prevent later waiters from waiting on
an available lock.

For more details, please refer to our [paper](http://www.cs.pitt.edu/~ouyang/files/publication/preemptable_lock-ouyang-vee13.pdf) 

*Updates*
01/10/2013
v1.0 was published in the VEE 2013 paper. Later I found it has problem
with wrapped-around ticket values. This problem is fixed in the v2.0 patch. 

v1.0 is also known to have problems with newer kernels. v2.0 patch is tested on
kernel-3.5.0. I'm working on porting it to newer kernels.

