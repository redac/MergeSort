#!/bin/bash
  # Usage: first argument is the program mode, second one is the number of splits.
  # PLEASE CHANGE THE SORTALGO YOU WANT TO BE TESTED BEFORE RUNNING THIS SCRIPT
  for l in 1000 10000 100000 1000000 10000000 100000000 1000000000;
  do
    if [ $1 = "demoSort" ]; 
    then
    echo "Generating a file with $l lines..."; 
    ./bin/project -m generation -o ./tests/test_$l.txt -n $l
    echo "Sorting a file with $l lines using demoSort, $2 splits...";
    /usr/bin/time -v  ./bin/project -m demoSort -i ./tests/test_$l.txt -o ./tests/test_$l.innersort.txt -k $2 &>> ./tests/results_V$1.txt;
    echo "Cleaning up...";
    rm ./tests/test*;
    echo "Done.";
    elif [ $1 = "linuxSort" ]; 
    then
    echo "Generating a file with $l lines..."; 
    ./bin/project -m generation -o ./tests/test_$l.txt -n $l
    echo "Sorting a file with $l lines using the command "sort -g", $2 splits...";
    /usr/bin/time -v sort -g ./tests/test_$l.txt -o ./tests/test_$l.innersort.txt -k $2 &>> ./tests/results_V$1.txt;
    echo "Cleaning up...";
    rm ./tests/test*;
    echo "Done.";
    else
    echo "Please mention a program mode: 'demoSort' OR 'linuxSort' ";
    fi
  done