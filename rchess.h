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
  void*  Contents;
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

struct pixel_values {
  int TotalTexture;  // size in one dimension (256x256 = 256)
  int InnerPadding;  // No. pixels until actual board begins
  int PieceTexture;  // size in one dimension (20x20 = 20)
  int BoardSquare;   // size in one dimension (22x22 = 22)
  int SquarePadding; // No. pixels to offset pieces when drawing the board
  int CenteredX;     // The leftmost X value when the texture is centered
};

struct dims {
  int MinX;
  int MinY;
  int MaxX;
  int MaxY;
};

struct texture {
  int  Width;
  int  Height;
  byte Pixels[Kilobytes(4)];
};

struct big_texture {
  int  Width;
  int  Height;
  byte Pixels[Kilobytes(200)];
};

struct board_state {
  bool32 WhiteToMove;
  bool32 wCanCastleKingside;
  bool32 wCanCastleQueenside;
  bool32 bCanCastleKingside;
  bool32 bCanCastleQueenside;
  uint8 EnPassantTarget;
  uint8 HalfMoves;
  uint8 FullMoves; // according to Chess.com, only one game (Nikolic vs Arsovic 1989) exceeded the max value of a uint8 (269 moves). Anyone who plays a game of equivalent length on this engine will be sad.
};

struct mailbox {
  piece Squares[64]; // 0..63 | A1, A2, .., H7, H8 | Little Endian Rank File mapping
};

struct chess_state {
  bitboard Bitboards[16];
  board_state BoardState;
  mailbox Mailbox;

  // Input
  bool32 DragMode;
  piece DraggedPiece;
  int DraggedPieceOrigin;
  bitboard DraggedPieceLegalMoves;
  
  // GUI
  big_texture  BoardTex;
  dims         BoardTexDims;
  pixel_values BoardTexPxVals;
  texture Pieces[16];
  int TextureScale;
};

#define RCHESS_H
#endif
