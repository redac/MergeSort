#!/bin/bash
  # Usage: first argument is the version of the project to be used, the second # argument is number of splits
  for l in 1000 10000 100000 1000000 10000000; 
  do 
    echo "Generating a file with $l lines"; 
    ./bin/project -m generation -o ./tests/test_$l.txt -n $l
    echo "Sorting a file with $l lines using V$1 of the project, $2 splits" 
    ./bin/project -m projectV$1 -i ./tests/test_$l.txt -o ./tests/test_$l.sort.txt -k $2 | tee -a ./tests/times.txt;
  done
  #add -a to append
  # echo "Is the file sorted? $(sort -c /tmp/test.sort.txt)"