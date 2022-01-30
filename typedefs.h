#ifndef TYPEDEFS_H

#include <stdint.h>

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef int32 bool32;

typedef float real32;
typedef double real64;

typedef unsigned char byte;

typedef uint8 piece;
typedef uint64 bitboard;

// Disambiguate "static" keyword, internal is intended to confine the function to its TU
#define global_var static
#define internal   static

// Square Coordinates to Index mapping, in Little Endian Rank File form
#define A1 0
#define A2 8
#define A3 16
#define A4 24
#define A5 32
#define A6 40
#define A7 48
#define A8 56

#define B1 A1 + 1
#define B2 A2 + 1
#define B3 A3 + 1
#define B4 A4 + 1
#define B5 A5 + 1
#define B6 A6 + 1
#define B7 A7 + 1
#define B8 A8 + 1

#define C1 B1 + 1
#define C2 B2 + 1
#define C3 B3 + 1
#define C4 B4 + 1
#define C5 B5 + 1
#define C6 B6 + 1
#define C7 B7 + 1
#define C8 B8 + 1

#define D1 C1 + 1
#define D2 C2 + 1
#define D3 C3 + 1
#define D4 C4 + 1
#define D5 C5 + 1
#define D6 C6 + 1
#define D7 C7 + 1
#define D8 C8 + 1

#define E1 D1 + 1
#define E2 D2 + 1
#define E3 D3 + 1
#define E4 D4 + 1
#define E5 D5 + 1
#define E6 D6 + 1
#define E7 D7 + 1
#define E8 D8 + 1

#define F1 E1 + 1
#define F2 E2 + 1
#define F3 E3 + 1
#define F4 E4 + 1
#define F5 E5 + 1
#define F6 E6 + 1
#define F7 E7 + 1
#define F8 E8 + 1

#define G1 F1 + 1
#define G2 F2 + 1
#define G3 F3 + 1
#define G4 F4 + 1
#define G5 F5 + 1
#define G6 F6 + 1
#define G7 F7 + 1
#define G8 F8 + 1

#define H1 G1 + 1
#define H2 G2 + 1
#define H3 G3 + 1
#define H4 G4 + 1
#define H5 G5 + 1
#define H6 G6 + 1
#define H7 G7 + 1
#define H8 G8 + 1



// Constants for Piece Representation
// the first three bits represent the piece, the fourth bit indicates if it's white's piece
// These piece constants are also used to index into State->Bitboards for their corresponding board based on piece type
/*
      BIN  Piece    BB Index

      0000 EMPTY     "
      0001 bPAWN     "
      0010 bBISHOP   "
      0011 bKNIGHT   "
      0100 bROOK     "
      0101 bQUEEN    "
      0110 bKING     "
      0111           OCCUP_SQ
      1000 W_MASK    EMPTY_SQ
      1001 wPAWN     "
      1010 wBISHOP   "
      1011 wKNIGHT   "
      1100 wROOK     "
      1101 wQUEEN    "
      1110 wKING     "
      1111           UNUSED
*/

#define EMPTY  0x0
#define PAWN   0x1
#define BISHOP 0x2
#define KNIGHT 0x3
#define ROOK   0x4
#define QUEEN  0x5
#define KING   0x6
#define W_MASK 0x8
#define wPAWN   (PAWN   | W_MASK)
#define wBISHOP (BISHOP | W_MASK)
#define wKNIGHT (KNIGHT | W_MASK)
#define wROOK   (ROOK   | W_MASK)
#define wQUEEN  (QUEEN  | W_MASK)
#define wKING   (KING   | W_MASK)
#define bPAWN   PAWN
#define bBISHOP BISHOP
#define bKNIGHT KNIGHT
#define bROOK   ROOK
#define bQUEEN  QUEEN
#define bKING   KING

// State->Bitboard indices that don't map to a piece
#define OCCUP_SQ 0x7
#define EMPTY_SQ 0x8
#define UNUSED   0xF

static char* SquareLookup[] = {
  "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1", 
  "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
  "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3",
  "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
  "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5",
  "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
  "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7",
  "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"
};

static bitboard KnightMoves[64] = {
  0x0000000000020400, // A1
  0x0000000000050800, // B1
  0x00000000000A1100, // C1
  0x0000000000142200, // D1
  0x0000000000284400, // E1
  0x0000000000508800, // F1
  0x0000000000A01000, // G1
  0x0000000000402000, // H1
  0x0000000002040004, // A2
  0x0000000005080008, // B2
  0x000000000A110011, // C2
  0x0000000014220022, // D2
  0x0000000028440044, // E2
  0x0000000050880088, // F2
  0x00000000A0100010, // G2
  0x0000000040200020, // H2
  0x0000000204000402, // A3  
  0x0000000508000805, // B3
  0x0000000A1100110A, // C3
  0x0000001422002214, // D3
  0x0000002844004428, // E3
  0x0000005088008850, // F3
  0x000000A0100010A0, // G3
  0x0000004020002040, // H3
  0x0000020400040200, // A4
  0x0000050800080500, // B4
  0x00000A1100110A00, // C4
  0x0000142200221400, // D4
  0x0000284400442800, // E4
  0x0000508800885000, // F4
  0x0000A0100010A000, // G4
  0x0000402000204000, // H4  
  0x0002040004020000, // A5
  0x0005080008050000, // B5  
  0x000A1100110A0000, // C5
  0x0014220022140000, // D5
  0x0028440044280000, // E5
  0x0050880088500000, // F5
  0x00A0100010A00000, // G5  
  0x0040200020400000, // H5
  0x0204000402000000, // A6
  0x0508000805000000, // B6
  0x0A1100110A000000, // C6
  0x1422002214000000, // D6
  0x2844004428000000, // E6
  0x5088008850000000, // F6
  0xA0100010A0000000, // G6
  0x4020002040000000, // H6  
  0x0400040200000000, // A7  
  0x0800080500000000, // B7
  0x1100110A00000000, // C7
  0x2200221400000000, // D7  
  0x4400442800000000, // E7
  0x8800885000000000, // F7
  0x100010A000000000, // G7
  0x2000204000000000, // H7
  0x0004020000000000, // A8
  0x0008050000000000, // B8
  0x00110A0000000000, // C8
  0x0022140000000000, // D8
  0x0044280000000000, // E8  
  0x0088500000000000, // F8
  0x0010A00000000000, // G8
  0x0020400000000000  // H8
};

#define TYPEDEFS_H
#endif
