# PA5_BLACKJACK 

This file is intended as an instruction for how to compile the program. If you want a description of it, check the Project Report.
This program only compiles on Linux, and if you're using a tiling window manager (such as sway) with Linux, there is no guarantee that it will function.
I got the program working on WSL on my home PC which runs windows, anything beyond that is untested.

## Compiling the program.
There are several steps to compiling, first step is compiling the bootstrap script (initializes FreeType): <br>
All of these are done in a bash or bash-like terminal.
1. Compiling the bootstrap script: <br>
Type this into your terminal and run: <br>
Make sure you are in the top directory when running these.
```bash
gcc bootstrap_libs.c -o bootstrap
```
2. Running the bootstrap script:
```bash
./bootstrap
```
Next step is compiling the actual program: <br>
These are also done in the terminal 
1. Creating the build directory: <br>
Make sure you are in the top directory when running these.
```bash
cmake -B build
```
2. Compiling the program:
```bash
cmake --build build
```

