#include "stdafx.h"

/*
 * A compiler for the Minimal-2D language found on esolangs.org
 *
 * Interprets 'input.txt' from the working directory
 * Commands are interpreted left to right starting at the origin (0,0) ie: upper-left-most character
 * All commands are numbers, symbols or upper-case letters
 * Anything else is skipped, and does not count as an instruction
 * Traversing white or empty space does not count as an instruction
 * By default, a linear-addressed, zero-indexed heap is given for memory. It has a dynamic and unlimited size.
 *
 * Valid Commands:
 *
 *     > | Increment the data pointer
 *     < | Decrement the data pointer (does not allow overflow)
 *     + | Increment the value at the data pointer
 *     - | Decrement the value at the data pointer
 *     . | Output the value at the data pointer in ASCII
 *     , | Inputs to the value at the data pointer in ASCII
 *     / | Skips the next instruction if the value at the pointer is 0
 *     U | Tells the parser to process instructions up
 *     D | Tells the parser to process instructions down
 *     L | Tells the parser to process instructions left
 *     R | Tells the parser to process instructions right
 *       | Any unused characters count as NOP (NO oPeration) operations (does not factor into instruction count)
 *
 * _ENABLE_DEBUGGING
 *   Enables several debugging features
 *   Enables the following debugging commands
 *
 *     # | Dumps the heap and pauses
 *     @ | Dumps the stack and pauses (only works if stack is also enabled)
 *
 * _ENABLE_STACK
 *   Enables stack based features
 *   Enables the following stack commands
 *
 *     V | Pushes (copies) the value at the data pointer onto the stack, increments stack pointer
 *     ^ | Removes the top element from the stack, decrements stack pointer
 *     ! | Copies the value currently on the stack into the value at the data pointer
 *     [ | Pushes the data pointer onto the stack, and moves the value at the pointer into the pointer
 *     ] | Pops the value from the stack into the data pointer
 *     
 *
 * _ENABLE_EXTENSIONS
 *   Enables some generic language extensions
 *   Enables the following commands
 *
 *     ) | Sets the value at the data pointer to zero
 *     ; | Terminates the program
 *     & | Puts the current address into the data pointer (x at pointer, y at entry after) 
 *     ( | Jumps execution to the value at the data pointer (x at pointer, y at entry after)
 *
 */
#define _ENABLE_DEBUGGING 1
#define _ENABLE_STACK 1
#define _ENABLE_EXTENSIONS 1

/* Compiler Properties */
#define PTR_SIZE unsigned char
#define MEM_CHUNK 64

/* Project */
#include "Heap.h"
#include "iPtr.h"

#if _ENABLE_STACK
#include "Stack.h"
#endif

/* STL */
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdio.h>
#include <vector>


/* Instructions to Parse */
std::vector<std::vector<char> > instructions;
size_t iCount = 0;

/**
 * Entry Point
 */
int main()
{
  /* Read Raw file Contents */
  std::ifstream file("input.txt", std::ios::binary);
  std::vector<char> contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  /* Make 2D array */
  instructions.push_back(std::vector<char>());
  for(size_t indx = 0; indx < contents.size(); indx++)
  {
    switch(contents[indx])
    {
    case '\r':
      break;

    case '\n':
      instructions.push_back(std::vector<char>());
      break;

    default:
      instructions.back().push_back(contents[indx]);
    }
  }

  /* Process Loop */
  bool exit = false;
  while(!exit && (_ip.dir & IP_DIR::VERTICAL ? _ip.y < instructions.size() : _ip.x < instructions[_ip.y].size()))
  {
    /*
     * Process Character
     */
    if(_ip.x < instructions[_ip.y].size())
    {
      switch(instructions[_ip.y][_ip.x])
      {
        /* Standard Commands */
      case '<':
        _heap.ptr--;
        break;

      case '>':
        _heap.ptr++;
        break;

      case '+':
        _heap.value()++;
        break;

      case '-':
        _heap.value()--;
        break;

      case '.':
        std::cout << _heap.value();
        break;

      case ',':
        std::cin >> _heap.value();
        break;

      case '/':
        if(!_heap.value()) _ip.update();
        break;

      case 'U':
        _ip.dir = IP_DIR::UP;
        break;

      case 'D':
        _ip.dir = IP_DIR::DOWN;
        break;

      case 'L':
        _ip.dir = IP_DIR::LEFT;
        break;

      case 'R':
        _ip.dir = IP_DIR::RIGHT;
        break;

        /* Debug Commands */
#if _ENABLE_DEBUGGING
      case '#':
        _heap.dump();
        break;
#endif
        /* Debug and Stack Commands */
#if _ENABLE_DEBUGGING && _ENABLE_STACK
      case '@':
        _stack.dump();
        break;
#endif
        /* Stack Commands */
#if _ENABLE_STACK
      case 'V':
        _stack.push(_heap.value());
        break;

      case '^':
        _stack.pop();
        break;

      case '!':
        _heap.value() = _stack.peek();
        break;

      case '[':
        _stack.push(_heap.ptr);
        _heap.ptr = _heap.value();
        break;

      case ']':
        _heap.ptr = _stack.peek();
        _stack.pop();
        break;

#endif

        /* Extended Commands */
#if _ENABLE_EXTENSIONS
      case ')':
        _heap.value() = 0;
        break;

      case ';':
        exit = true;
        break;

      case '&':
        _heap.value() = _ip.x;
        _heap.ptr++;
        _heap.value() = _ip.y;
        _heap.ptr--;
        break;

      case '(':
        _ip.x = _heap.value();
        _heap.ptr++;
        _ip.y = _heap.value();
        _heap.ptr--;
        break;
#endif
      default:
        iCount--;
      }

      /* Update Instruction Count */
      iCount++;
    }

    /*
     * Update Instruction Pointer
     */
    _ip.update();
  }

#if defined _DEBUG
  _ip.dump();
  _heap.dump();
#endif

#if defined _DEBUG && _ENABLE_STACK
  _stack.dump();
#endif

  printf("\n\nProgram Execution Complete.\n%03u Instructions Processed.\n", iCount);
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  return 0;
}

