# Kernel Module: Spinlock Fairness Measurement

### Author
Qihang Chen, Jiannan Ouyang

### Usage
```sh
$ make
$ dmesg -c # clear dmesg
$ make install
$ make remove 
$ dmesg | grep fairness | grep thread | cut -d ":" -f 2 | python calc_index.py
$ dmesg | grep fairness | grep Time | cut -d ":" -f 2
```
