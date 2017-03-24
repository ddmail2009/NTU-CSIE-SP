#!/bin/bash

./borrow 1 100
./borrow 2 100
./borrow 1 200
./query 1 > queryoutput
./return 1
./query 1 >> queryoutput
./borrow 1 200
./borrow 1 200
./return 1
./return 1

#invalid input
./borrow 999 -1
./borrow -1 -1
./borrow 1 -1
./borrow 999 1
./query 888 >> queryoutput
./return -1
