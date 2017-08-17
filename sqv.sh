#!/bin/bash

./split 0.2 101 allDataForFDT_sqv.txt sqv.tes sqv.tra
./train_test_anti_kdd3 150 sqv.tra sqv.tes > sqv.1.log 
./split 0.2 102 allDataForFDT_sqv.txt sqv.tes sqv.tra
./train_test_anti_kdd3 150 sqv.tra sqv.tes > sqv.2.log 
./split 0.2 103 allDataForFDT_sqv.txt sqv.tes sqv.tra
./train_test_anti_kdd3 150 sqv.tra sqv.tes > sqv.3.log 
./split 0.2 104 allDataForFDT_sqv.txt sqv.tes sqv.tra
./train_test_anti_kdd3 150 sqv.tra sqv.tes > sqv.4.log 
./split 0.2 105 allDataForFDT_sqv.txt sqv.tes sqv.tra
./train_test_anti_kdd3 150 sqv.tra sqv.tes > sqv.5.log 
