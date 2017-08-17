#!/bin/bash

./split 0.2 101 allDataForFDT_idv.txt idv.tes idv.tra
./train_test_anti_kdd3 150 idv.tra idv.tes > idv.1.log 
./split 0.2 102 allDataForFDT_idv.txt idv.tes idv.tra
./train_test_anti_kdd3 150 idv.tra idv.tes > idv.2.log 
./split 0.2 103 allDataForFDT_idv.txt idv.tes idv.tra
./train_test_anti_kdd3 150 idv.tra idv.tes > idv.3.log 
./split 0.2 104 allDataForFDT_idv.txt idv.tes idv.tra
./train_test_anti_kdd3 150 idv.tra idv.tes > idv.4.log 
./split 0.2 105 allDataForFDT_idv.txt idv.tes idv.tra
./train_test_anti_kdd3 150 idv.tra idv.tes > idv.5.log 
