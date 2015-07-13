#!/bin/bash
for i in {1..5}
do
make install;sleep 20;make remove;
done
