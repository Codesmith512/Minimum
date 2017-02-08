#pragma once

/*
 * Manages the Stack
 */
#include <iostream>
#include <stdio.h>
#include <vector>

struct STACK
{
  /* Default Constructor */
  STACK()
    :data(),
    ptr(static_cast<POINTER_T>(-1))
  {

  }

  /* Peek Operation */
  CELL_T peek()
  {
    while(ptr >= data.size())
      data.resize(data.size() + MEM_CHUNK);
    return data[ptr];
  }

  /* Data Members */
  std::vector<CELL_T> data;
  POINTER_T ptr;

  /* Pop Operation */
  void pop()
  {
    ptr--;
  }

  /* Push Operation */
  void push(const CELL_T& val)
  {
    while(ptr >= data.size())
      data.resize(data.size() + MEM_CHUNK);
    data[++ptr] = val;
  }

  /* Dump Stack */
#if _ENABLE_DEBUGGING
  void dump()
  {
    std::cout << "\n\nProgram Stack Dump (uint8_t)|(char)\n::TOP::\n";
    for(size_t i = data.size() - 1; i < data.size(); i--)
    {
      if(i == ptr)
        printf("%4i | %c\t< sPtr\n", data[i], data[i]);
      else
        printf("%4i | %c\n", data[i], data[i]);
    }
    std::cout << "::BOT::\n\n";
    std::getc(stdin);
  }
#endif
} _stack;
