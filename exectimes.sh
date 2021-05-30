#!/bin/bash
  # Usage: first argument is the version of the project to be used, the second # argument is number of splits
  for l in 1000 10000 100000 1000000 10000000 100000000 1000000000;
  do 
    echo "Generating a file with $l lines"; 
    ./bin/project -m generation -o ./tests/test_$l.txt -n $l
    echo "Sorting a file with $l lines using V$1 of the project, $2 splits";
    /usr/bin/time -v  ./bin/project -m projectV$1 -i ./tests/test_$l.txt -o ./tests/test_$l.sort.txt -k $2 &>> ./tests/results_V$1.txt;
    echo "Cleaning up...";
    rm ./tests/test*;
  done

  #for l in 10 100 1000 10000 100000 1000000 10000000 100000000 1000000000;
  #do 
  #  echo "Sorting a file with $l lines using V$1 of the project, $2 splits" 
  #  /usr/bin/time -v  ./bin/project -m projectV$1 -i ./tests/test_$l.txt -o ./tests/test_$l.sort.txt -k $2 &> ./tests/results_V$1.txt;
  #done
  
  #add -a to append
  # echo "Is the file sorted? $(sort -c /tmp/test.sort.txt)"