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
    ptr(0)
  {

  }

  /* Peek Operation */
  PTR_SIZE peek()
  {
    while(ptr >= data.size())
      data.resize(data.size() + MEM_CHUNK);
    return data[ptr];
  }

  /* Data Members */
  std::vector<PTR_SIZE> data;
  PTR_SIZE ptr;

  /* Pop Operation */
  void pop()
  {
    ptr--;
  }

  /* Push Operation */
  void push(const PTR_SIZE& val)
  {
    while(ptr >= data.size())
      data.resize(data.size() + MEM_CHUNK);
    data[ptr++] = val;
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
