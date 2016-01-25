#General Info

Minimum is a compiler that supports the Minimal-2D language with a few additional extensions

Corresponding binaries are in Debug/ and Release/, and are entirely stand-alone

Sources are located in Minimal-2D.
The only source file atm is 'Minimal-2D.cpp', the rest are standard vs.

Builds without warnings or errors with VS15, and shouldn't require any additional prerequsites.
Should also be entirely cross-platform with the STL, however this has not been tested.


#Installation

Debugging and release binaries are located in bin/ in their respective folders
The debugging will automatically dump the instruction pointer, heap, and stack (if enabled) on program exit

Example programs can be found in examples/ , but must be renamed to 'input.txt' before running


#How to Use

Build flags are located in `Minimal-2D.cpp`, at the top beneath the command dictionary.
The default build has all flags enabled.

The program takes an 'input.txt' in the same folder and runs it.
I/O takes place in the same console window.

After execution, the program will pause and give an instructions count.


#Language

##Execution
Execution always starts with the upper-left-most character, executing to the right
Operations can be found in the below section

##Memory
All memory is the same size and is unsigned. (default to 1B)
All memory wraps around. (BE CAREFUL, THE POINTER DOES WRAP)

###The Heap
The main memory in use is the 'heap' (usually referred to as the 'tape')
The entire heap starts filled with zeros
The program has access to a single pointer that starts pointing at address 0.
The heap is created in chunks, but has a limit of whatever the highest addressable address is
 * These chunks are managed by the interpreter, and don't exist from the program's standpoint
 * If you debug and get no memory dumps, no memory has been created or used yet
 
###The Stack
One of several language extensions, the stack is completely separate from the heap
It is of an infinite size (limited by the architecture of the machine)
Follows standard LIFO procedures for a stack


#Operations

##Minimum-2D
> | Increment the data pointer
< | Decrement the data pointer (does not allow overflow)
+ | Increment the value at the data pointer
- | Decrement the value at the data pointer
. | Output the value at the data pointer in ASCII
, | Inputs to the value at the data pointer in ASCII
/ | Skips the next instruction if the value at the pointer is 0
U | Tells the parser to process instructions up
D | Tells the parser to process instructions down
L | Tells the parser to process instructions left
R | Tells the parser to process instructions right
 
 
##Debugging
\# | Dumps the heap and pauses
\@ | Dumps the stack and pauses (only works if stack is also enabled)
 
##Stack
V | Push the value at the data pointer onto the stack
^ | Pops the top value from the stack and throws it away (does nothing if no stack)
! | Copy the top value from the stack into the value at the data pointer (does nothing if no stack)

##General Extensions
) | Sets the value at the data pointer to zero
; | Terminates the program
~ | Moves the value at the data pointer into the data pointer
& | Puts the current address into the data pointer (x at pointer, y at entry after) 
( | Jumps execution to the value at the data pointer (x at pointer, y at entry after)


#Useful Links
https://esolangs.org/wiki/Minimal-2D
