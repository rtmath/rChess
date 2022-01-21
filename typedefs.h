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
// the top 4 most-significant bits are currently unused
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

#define TYPEDEFS_H
#endif
