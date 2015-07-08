#!/bin/bash
for i in {1..5}
do
make install;sleep 10;make remove;
done
