#ifndef RCHESS_H

#include "typedefs.h"

#define ArrayCount(Array) (sizeof(Array) / sizeof(Array[0]))
#define Assert(Expression) if(!(Expression)) {*(int*)0 = 0;}

#define Kilobytes(value) ((value)*1024LL)
#define Megabytes(value) ((Kilobytes(value) * 1024LL))
#define Gigabytes(value) ((Megabytes(value) * 1024LL))
#define Terabytes(value) ((Gigabytes(value) * 1024LL))

struct read_file_result {
  uint32 ContentsSize;
  void* Contents;
};

#define PLATFORM_FREE_FILE_MEMORY(name) void name(void* Memory)
typedef PLATFORM_FREE_FILE_MEMORY(platform_free_file_memory);

#define PLATFORM_READ_ENTIRE_FILE(name) read_file_result name(char* Filename)
typedef PLATFORM_READ_ENTIRE_FILE(platform_read_entire_file);

#define PLATFORM_WRITE_ENTIRE_FILE(name) bool32 name(char* Filename, uint32 MemorySize, void* Memory)
typedef PLATFORM_WRITE_ENTIRE_FILE(platform_write_entire_file);

inline uint32
SafeTruncateUint64(uint64 value) {
  Assert(value <= 0xFFFFFFFF);
  uint32 Result = (uint32)value;
  return Result;
};

struct memory {
  bool32 IsInitialized;
  uint64 BlockSize;
  void*  Block;  
  platform_free_file_memory*  PlatformFreeFileMemory;
  platform_read_entire_file*  PlatformReadEntireFile;
  platform_write_entire_file* PlatformWriteEntireFile;
};

struct button {
  bool32 EndedDown;
};

struct input {
  int MouseX;
  int MouseY;
  int MouseZ;
  button Buttons[3];
};

struct back_buffer {
  void* Memory;
  int Width;
  int Height;
  int Pitch;
  int BytesPerPixel;
};

struct texture {
  int  Width;
  int  Height;
  byte Pixels[Kilobytes(4)];
};

struct big_texture {
  int Width;
  int Height;
  byte Pixels[Kilobytes(200)];
};
  
struct chess_state {
  big_texture Board;
  texture BlackBishop;
  texture BlackKing;
  texture BlackKnight;
  texture BlackPawn;
  texture BlackQueen;
  texture BlackRook;
  texture WhiteBishop;
  texture WhiteKing;
  texture WhiteKnight;
  texture WhitePawn;
  texture WhiteQueen;
  texture WhiteRook;
};

#define RCHESS_H
#endif
