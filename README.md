# Merge Sort - OS2 Course Project

## Build the documentation

1. In the root directory of the project, run the following command: `make doc`
2. Open the `doc.html` explaining the project

## Main commands

- compile the project: `make`
- display project help: `./bin/project -h`
- run tests using a the exectimes bash script (argument 1 is the project version and argument 2 is the number of splits): `./exectimes.sh`

### Other helpful commands

- generate a file with 5000 numbers (lines) `./bin/project -m generation -o /tmp/test.txt -n 5000`
- sort a file using a specific project version, k being the number of splits ` ./bin/project -m projectV0 -i /tmp/test.txt -o /tmp/test.sort.txt -k 5`
- get the execution time of a command: `/usr/bin/time -v [command]`

## Tests

All tests are done under an Ubuntu WSL on a Windows 10 machine with an Intel(R) Core(TM) i5-8400 CPU @ 2.80GHz, 16.0 GB of RAM and a Nvidia GTX 1070 GPU.
Sources Files are compiled using version 9.3.0 of gcc.

