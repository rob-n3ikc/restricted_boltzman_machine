#!/bin/bash

./split 0.2 101 allDataForFDT_lpv.txt lpv.tes lpv.tra
./train_test_anti_kdd3 150 lpv.tra lpv.tes > lpv.1.log 
./split 0.2 102 allDataForFDT_lpv.txt lpv.tes lpv.tra
./train_test_anti_kdd3 150 lpv.tra lpv.tes > lpv.2.log 
./split 0.2 103 allDataForFDT_lpv.txt lpv.tes lpv.tra
./train_test_anti_kdd3 150 lpv.tra lpv.tes > lpv.3.log 
./split 0.2 104 allDataForFDT_lpv.txt lpv.tes lpv.tra
./train_test_anti_kdd3 150 lpv.tra lpv.tes > lpv.4.log 
./split 0.2 105 allDataForFDT_lpv.txt lpv.tes lpv.tra
./train_test_anti_kdd3 150 lpv.tra lpv.tes > lpv.5.log 
