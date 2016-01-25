#pragma once

/*
 * Manages the Heap
 */
#include <iostream>
#include <stdio.h>
#include <vector>

struct HEAP
{
  /* Default Constructor */
  HEAP()
    :data(),
    ptr(0)
  {

  }

  /* Data Members */
  std::vector<PTR_SIZE> data;
  PTR_SIZE ptr;

  /* Access heap */
  PTR_SIZE& value()
  {
    while(ptr >= data.size())
      data.resize(data.size() + MEM_CHUNK);
    return data[ptr];
  }

  /* Dump heap */
#if _ENABLE_DEBUGGING
  void dump()
  {
    std::cout << "\n\nProgram data Dump (uint8_t)";
    for(size_t i = 0; i < data.size(); i++)
    {
      if(i % 0x10 == 0) printf("\n0x%x\t| ", i);
      printf(" %03i", data[i]);
    }
    printf("\nPtr: 0x%02x [%03i]", ptr, value());
    std::cout << "\n\n";
    std::getc(stdin);

    std::cout << "\n\nProgram data Dump (char)";
    for(size_t i = 0; i < data.size(); i++)
    {
      if(i % 0x10 == 0) printf("\n0x%x\t| ", i);
      printf("%c ", data[i]);
    }
    printf("\nPtr: 0x%02x [%c]", ptr, value());
    std::cout << "\n\n";
    std::getc(stdin);
  }
#endif
} _heap;