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
 *     V | Push the value at the data pointer onto the stack
 *     ^ | Pops the top value from the stack and throws it away (does nothing if no stack)
 *     ! | Copy the top value from the stack into the value at the data pointer (does nothing if no stack)
 *
 * _ENABLE_EXTENSIONS
 *   Enables some generic language extensions
 *   Enables the following commands
 *
 *     ) | Sets the value at the data pointer to zero
 *     ; | Terminates the program
 *     ~ | Moves the value at the data pointer into the data pointer
 *     & | Puts the current address into the data pointer (x at pointer, y at entry after) 
 *     ( | Jumps execution to the value at the data pointer (x at pointer, y at entry after)
 *
 */
#define _ENABLE_DEBUGGING 1
#define _ENABLE_STACK 1
#define _ENABLE_EXTENSIONS 1

/* STL */
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdio.h>
#include <vector>

/* Compiler Properties */
#define PTR_SIZE unsigned char
#define TAPE_CHUNK 100

/* Instructions to Parse */
std::vector<std::vector<char> > instructions;
size_t iCount = 0;

/* Instruction Coordinates */
struct IP
{
  IP(PTR_SIZE _x, PTR_SIZE _y)
    :x(_x),
     y(_y)
  {

  }

  PTR_SIZE x;
  PTR_SIZE y;
} _ip(0, 0);

/* Direction of New Instruction */
enum class IP_DIR : std::uint8_t
{
  RIGHT = 1,  // 001
  DOWN  = 2,  // 010
  LEFT  = 5,  // 101
  UP    = 6,  // 110

  HORIZONTAL = 1, // 001
  VERTICAL = 2    // 010
} _ip_dir = IP_DIR::RIGHT;
inline bool operator&(const IP_DIR& a, const IP_DIR& b)
{ return static_cast<bool>((static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0); }

/* Heap Memory */
std::vector<PTR_SIZE> heap;
PTR_SIZE ptr = 0;

/* Stack Memory */
#if _ENABLE_STACK
std::vector<PTR_SIZE> stack;
#endif

/*
 * Tape Access
 */
unsigned char& getHeapValue()
{
  /* Ensure Bounds */
  while(ptr >= heap.size())
  {
    heap.resize(heap.size() + TAPE_CHUNK);
    for(size_t i = heap.size() - TAPE_CHUNK; i < heap.size(); i++)
      heap[i] = 0;
  }

  /* Return Value */
  return heap[ptr];
}

/*
 * Updates the IP
 */
void updateIP()
{
  switch(_ip_dir)
  {
  case IP_DIR::RIGHT:
    _ip.x++;
    break;

  case IP_DIR::DOWN:
    _ip.y++;
    break;

  case IP_DIR::LEFT:
    _ip.x--;
    break;

  case IP_DIR::UP:
    _ip.y--;
  }
}

/*
 * Dumps the contents of the instruction pointer to the screen
 */
#if _ENABLE_DEBUGGING
void ipDump()
{
  printf("\n\nInstruction (%03u, %03u)\n\n", _ip.x, _ip.y);
}
#endif

/*
 * Dumps the contents of the heap to the screen
 */
#if _ENABLE_DEBUGGING
void heapDump()
{
  std::cout << "\n\nProgram Heap Dump (uint8_t)";
  for(size_t i = 0; i < heap.size(); i++)
  {
    if(i % 0x10 == 0) printf("\n0x%x\t| ", i);
    printf(" %03i", heap[i]);
  }
  printf("\nPtr: 0x%02x [%03i]", ptr, getHeapValue());
  std::cout << "\n\n";
  std::getc(stdin);

  std::cout << "\n\nProgram Heap Dump (char)";
  for(size_t i = 0; i < heap.size(); i++)
  {
    if(i % 0x10 == 0) printf("\n0x%x\t| ", i);
    printf("%c ", heap[i]);
  }
  printf("\nPtr: 0x%02x [%c]", ptr, getHeapValue());
  std::cout << "\n\n";
  std::getc(stdin);
}
#endif

/**
 * Dumps the contents of the stack to the screen
 */
#if _ENABLE_DEBUGGING && _ENABLE_STACK
void stackDump()
{
  std::cout << "\n\nProgram Stack Dump (uint8_t)|(char)\n::TOP::\n";
  for(size_t i = stack.size() - 1; i < stack.size(); i--)
    printf("%4i | %c\n", stack[i], stack[i]);
  std::cout << "::BOT::\n\n";
  std::getc(stdin);
}
#endif

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
  while(!exit && (_ip_dir & IP_DIR::VERTICAL ? _ip.y < instructions.size() : _ip.x < instructions[_ip.y].size()))
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
        if(ptr - 1 < ptr) ptr--;
        break;

      case '>':
        if(ptr + 1 > ptr) ptr++;
        break;

      case '+':
        getHeapValue()++;
        break;

      case '-':
        getHeapValue()--;
        break;

      case '.':
        std::cout << getHeapValue();
        break;

      case ',':
        std::cin >> getHeapValue();
        break;

      case '/':
        if(!getHeapValue()) updateIP();
        break;

      case 'U':
        _ip_dir = IP_DIR::UP;
        break;

      case 'D':
        _ip_dir = IP_DIR::DOWN;
        break;

      case 'L':
        _ip_dir = IP_DIR::LEFT;
        break;

      case 'R':
        _ip_dir = IP_DIR::RIGHT;
        break;

        /* Debug Commands */
#if _ENABLE_DEBUGGING
      case '#':
        heapDump();
        break;
#endif
        /* Debug and Stack Commands */
#if _ENABLE_DEBUGGING && _ENABLE_STACK
      case '@':
        stackDump();
        break;
#endif
        /* Stack Commands */
#if _ENABLE_STACK
      case 'V':
        stack.push_back(getHeapValue());
        break;

      case '^':
        if(stack.size())
          stack.pop_back();
        break;

      case '!':
        if(stack.size())
          getHeapValue() = stack.back();
        break;
#endif

        /* Extended Commands */
#if _ENABLE_EXTENSIONS
      case ')':
        getHeapValue() = 0;
        break;

      case ';':
        exit = true;
        break;

      case '~':
        ptr = getHeapValue();
        break;

      case '&':
        getHeapValue() = _ip.x;
        ptr++;
        getHeapValue() = _ip.y;
        ptr--;
        break;

      case '(':
        _ip.x = getHeapValue();
        ptr++;
        _ip.y = getHeapValue();
        ptr--;
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
    updateIP();
  }

#if defined _DEBUG
  ipDump();
  heapDump();
#endif

#if defined _DEBUG && _ENABLE_STACK
  stackDump();
#endif

  printf("\n\nProgram Execution Complete.\n%03u Instructions Processed.\n", iCount);
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  return 0;
}

