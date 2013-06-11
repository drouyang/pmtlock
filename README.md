# Preemptable Ticket Spinlock (pmtlock)
Jiannan Ouyang
ouyang@cs.pitt.edu
06/2013

## Introduction
Preemptable Ticket Spinlock (pmtlock) is a lock algorithm designed for virtual
environment proposed by Jiannan Ouyang in his VEE 2013 paper, 
"Preemptable Ticket Spinlocks: Improving Consolidated Performance in the Cloud"

The idea is to sacrifices fairness to ensure forward progress upon preemption, 
but also preserve fairness whenever possible. To this end, it allows a fair lock 
to downgrade to unfair lock to prevent the lock waiter preemption problem, in which 
a preempted waiter in the queue head can not acquire an available lock, meanwhile it 
prevents other waiters to get the available lock.

pmtlock is a guest side optimization, and thus can be used as a complementary 
technique to host side optimizations such as co-scheduling and Pause-Loop Exiting.

## Implementation
There are two stages in the lock function: preemptable loop and spinning loop, which are described briefly below. For more details, please refer to the paper 
"Preemptable Ticket Spinlocks: Improving Consolidated Performance in the Cloud"

The spinning loop is basically a unfair spinlock that ensure mutual exclusions between waiter. The preemptable loop, on the other hand, is more interesting and is the core algorithm.

timeout =  TIMEOUT_UNIT * (inc.tail - inc.head);
do {
	current_head = ACCESS_ONCE(lock->tickets.head);
	if (inc.head != current_head) {
		inc.head = current_head;
		timeout =  TIMEOUT_UNIT * (inc.tail - inc.head);
	}
	cpu_relax();
} while (timeout-- > 0);

In preeemptable loop, a timeout is maintained to detect long time waiting.
It is calculated by the position of a node in queue, that is (ticket - current_head),
multiplied by a TIMEOUT_UNIT, which is a tunable parameter. In this way, timeout
is proportional to a node's position in queue.

The loop is exited only when timeout (t) is non-positive, however it captures three conditions
* if waiter's position is now head of queue, t = 0
* if waiter timed out, t = 0
* if a waiter is preempted by later waiters, it's t < 0 (because current head is larger than its ticket)

Note that the ticket value is possible to overflow and wrap around. In this case, a waiter may have ticket 
that is smaller than current queue head. This is also captured by t < 0. This is a boundary case, because of
which we may temporarily lose some fairness, but it does not influence the correctness of mutual exclusion.

