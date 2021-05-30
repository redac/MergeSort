#!/bin/bash
for v in 0 1 2 3 4;
do
    echo "VERSION $v"  >> ./output.csv;
    echo "size;1;2;5;10;15;20;25;30;40;50;" >> ./output.csv;
    for l in 10 100 #1000 10000 100000 1000000 10000000 100000000 1000000000;
    do
        ./bin/project -m generation -o ./tests/test_$l.txt -n $l
        echo -n "$size;" >> ./output.csv;
    for split in 50;
    do
        for n in {1..5};
        do
            /usr/bin/time -f "%S;%U" -a -o ./time ./bin/project -m projectV$version -i ./tests/test_$l.txt -o ./tests/testsort.txt -k $split;
            rm ./tmp/testsort.txt;
        done;
        total=0;
        for i in $( awk '{ print $0; }' ./time );
        do
            j=$( echo "$i" | awk -F ';' '{ print $1; }');
            k=$( echo "$i" | awk -F ';' '{ print $2; }');
            total=$(echo $total+$j+$k | bc );
        done;
        f=$(echo "scale=2; $total / 5" | bc);
        echo -n "$f;" >> ./output.csv;
        rm ./time;
        echo "$split of $size done";
    done;
    done;
       # echo "" >> ./output.csv;
        #rm ./tmp/test.txt
   # done;
done;