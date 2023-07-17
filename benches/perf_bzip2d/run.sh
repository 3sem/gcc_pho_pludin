#! /bin/bash

cd ./bench

for ((i=1;i<=10;i++))
do
	qemu-aarch64 -L /usr/aarch64-linux-gnu ../bench.elf -d -k -c small.bz2 > datasets/shuffled_unpacked
	qemu-aarch64 -L /usr/aarch64-linux-gnu ../bench.elf -z -k -c datasets/shuffled_unpacked > datasets/shuffled_packed
done
