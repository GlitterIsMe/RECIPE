#!/bin/bash
for INDEX in fastfair masstree art hot levelhash clht; do
    for WORKLOAD in a b c; do
        echo "numactl --cpubind=1 ./build/ycsb-1kw $INDEX $WORKLOAD randint uniform 1"
        #eval "numactl --cpubind=1 ./build/ycsb-1kw $INDEX $WORKLOAD randint uniform 1"
    done
done

for INDEX in fastfair masstree art hot; do
        echo "numactl --cpubind=1 ./build/ycsb $INDEX e randint uniform 1"
        #eval "numactl --cpubind=1 ./build/ycsb $INDEX e randint uniform 1"
done