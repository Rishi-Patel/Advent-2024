# Advent of Code 2024


## Building
Each Day is separated into a different .cpp file and compiled separately.\
Most (if not all) Days use the Util.h header for some frequently used file parsing.\

I've provided the .vscode configs I use to streamline building and debugging in VSCode. However, I use CMake so this should be easily built wherever/however you want. Gonna outline a basic CMake build here using Make:

> mkdir build\
> cd build\
> cmake .. \
> make all

You can also specify specific days:
> make Day01

## Running
For some reason I've set up Parts 1 and 2 as constant values in the code. So if you want to switch between Parts, change the enum value at the top of each .cpp file and re-build:

>enum Part {One,Two};\
>constexpr Part PART = Part::Two; // Change this to whatever part you want to run

To run, simply execute the binary and pass the input file as an argument:

> ./Day01 Day01.txt

The text file can be named whatever you want if you run the program directly like this. However to make it easy to automate in VSCode, my .vscode configs expect the textfile to have the same name as the binary and all my text inputs are named this way. 

But this can be change by editing the "args" setting in .vscode/launch.json