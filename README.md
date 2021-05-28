# Merge Sort - OS2 Course Project

## Build the documentation

1. In the root directory of the project, run the following command: `make doc`
2. Open the `doc.html` explaining the project

## Main commands

- compile the project: `make`
- display project help `./bin/project -h`
- generate a file with 5000 numbers (lines) `./bin/project -m generation -o /tmp/test.txt -n 5000`
- sort a file using a specific project version, k being the number of splits ` ./bin/project -m projectV0 -i /tmp/test.txt -o /tmp/test.sort.txt -k 5`
- get the execution time of a command: `/usr/bin/time -v [command]`