#!/bin/bash
g++ train_test_kdd.cpp kdd2.cpp bmachina.cpp -o train_test_kdd2 -g
g++ train_test_kdd.cpp kdd3.cpp bmachina.cpp -o train_test_kdd3 -g
g++ train_test_kdd.cpp kdd3g.cpp bmachina.cpp -o train_test_kdd3g -g
g++ train_test_kdd.cpp kdd4.cpp bmachina.cpp -o train_test_kdd4 -g
g++ train_test_anti_kdd.cpp kdd2.cpp bmachina.cpp -o train_test_anti_kdd2 -g
g++ train_test_anti_kdd.cpp kdd3.cpp bmachina.cpp -o train_test_anti_kdd3 -g
g++ train_test_anti_kdd.cpp kdd3g.cpp bmachina.cpp -o train_test_anti_kdd3g -g
g++ train_test_anti_kdd.cpp kdd4.cpp bmachina.cpp -o train_test_anti_kdd4 -g
