#pragma once

/*
 * Manages the Instruction Pointer
 */
#include <stdio.h>

/* Direction of Instruction Pointer */
enum class IP_DIR : std::uint8_t
{
  RIGHT = 1,  // 001
  DOWN = 2,  // 010
  LEFT = 5,  // 101
  UP = 6,  // 110

  HORIZONTAL = 1, // 001
  VERTICAL = 2    // 010
};
inline bool operator&(const IP_DIR& a, const IP_DIR& b)
{ return static_cast<bool>((static_cast<uint8_t>(a) & static_cast<uint8_t>(b)) != 0); }

/* Instruction Coordinates */
struct IP
{
  /* Default Constructor */
  IP()
    :x(0),
    y(0),
    dir(IP_DIR::RIGHT)
  {

  }

  /* Data Members */
  PTR_SIZE x;
  PTR_SIZE y;
  IP_DIR dir;

  /* Updates x and y */
  void update()
  {
    switch(dir)
    {
    case IP_DIR::RIGHT:
      x++;
      break;

    case IP_DIR::DOWN:
      y++;
      break;

    case IP_DIR::LEFT:
      x--;
      break;

    case IP_DIR::UP:
      y--;
      break;
    }
  }

  /* Dump IP */
#if _ENABLE_DEBUGGING
  void dump()
  {
    printf("\n\nInstruction (%03u, %03u)\n", x, y);
    switch(dir)
    {
    case IP_DIR::RIGHT:
      printf("Direction: Right\n\n");
      break;

    case IP_DIR::DOWN:
      printf("Direction: Down\n\n");
      break;

    case IP_DIR::LEFT:
      printf("Direction: Left\n\n");
      break;

    case IP_DIR::UP:
      printf("Direction: Down\n\n");
      break;
    }
  }
#endif
} _ip;