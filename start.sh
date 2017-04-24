#!/usr/bin/env bash

(echo ./MATO -i 1 -p in_c/main.pd -n ../neighbors1.txt -o 7001;
 echo ./MATO -i 2 -p in_c/main.pd -n ../neighbors2.txt -o 7002;
 echo ./MATO -i 3 -p in_c/main.pd -n ../neighbors3.txt -o 7003;
 echo ./MATO -i 4 -p in_c/main.pd -n ../neighbors4.txt -o 7004) | parallel