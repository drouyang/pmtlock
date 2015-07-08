#!/bin/bash

for x in 0 2 4 6 8 10 12 14
do
	echo $x > /sys/kernel/debug/pmtlock/pmtlock_timeout_shift
	cat /sys/kernel/debug/pmtlock/pmtlock_timeout_shift
	./run.sh
done

echo "done"
