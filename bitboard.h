#ifndef BITBOARD_H

#include "print_helpers.h"

inline bitboard
BitboardWithBitSet(bitboard n, unsigned char nth_place) {
  Assert(nth_place < 64);
  return n | (0x1ULL << nth_place);
}

inline void
SetBit(uint64* target, unsigned char nth_place) {
  Assert(nth_place < 64);
  uint64 index = 0x1;
  *target |= (index << nth_place);
}

inline bitboard
SetBit(bitboard BB, unsigned char nth_place) {
  Assert(nth_place < 64);
  uint64 index = 0x1;
  return BB |= (index << nth_place);
}

inline bitboard
ClearBit(bitboard bb, unsigned char nth_place) {
  uint64 index = 0x1;
  return (bb & ~(index << nth_place));
}

inline void
ClearBit(uint64* target, unsigned char nth_place) {
  Assert(nth_place < 64);
  uint64 index = 0x1;
  *target &= ~(index << nth_place);
}

inline void
ToggleBit(uint64* target, unsigned char nth_place) {
  Assert(nth_place < 64);
  uint64 index = 0x1;  *target ^= (index << nth_place);
}

inline bool32
CheckBit(uint64 n, unsigned char nth_place) {
  Assert(nth_place < 64);
  return !!((n >> nth_place) & 0x1ULL);
}

inline bool32
IsWhite(piece P) {
  return (P & W_MASK) > 0;
}

inline bool32
PieceIsType(piece P, piece Type) {
  P = P & (~W_MASK); // remove color identifier from P
  return P == (P & Type);
}

inline void
SetPiece(bitboard* B, int Destination) {
  SetBit(B, Destination);
}

inline void
ClearPiece(bitboard* B, int Location) {
  ClearBit(B, Location);
}

inline void
ClearPiece(bitboard* Boards, piece Piece, int Location) {
  if (Piece == EMPTY) { return; }
  ClearBit(&Boards[Piece], Location);
}

inline void
MovePiece(bitboard* B, int Origin, int Destination) {
  ClearBit(B, Origin);
  SetBit(B, Destination);
}

void MovePiece(bitboard* Boards, piece Piece, int Origin, int Destination) {
  if (Piece == EMPTY) { return; }
  MovePiece(&Boards[Piece], Origin, Destination);
}

void SetPiece(bitboard* Boards, piece Piece, int Destination) {
  if (Piece == EMPTY) { return; }
  SetPiece(&Boards[Piece], Destination);
}

inline void
ToMailbox(bitboard BB, piece PieceType, mailbox* Mailbox) {
  if (!BB) { return; }
  for (int i = 0; i < 64; i++) {
    if (BB & 0x1ULL) { Mailbox->Squares[i] = PieceType; }
    BB >>= 1;
  }
}

inline void
PopulateMailbox(bitboard* Bitboards, mailbox* Mailbox) {
  *Mailbox = {};
  ToMailbox(Bitboards[bPAWN],     bPAWN, Mailbox);
  ToMailbox(Bitboards[bBISHOP], bBISHOP, Mailbox);
  ToMailbox(Bitboards[bKNIGHT], bKNIGHT, Mailbox);
  ToMailbox(Bitboards[bROOK],     bROOK, Mailbox);
  ToMailbox(Bitboards[bQUEEN],   bQUEEN, Mailbox);
  ToMailbox(Bitboards[bKING],     bKING, Mailbox);
  ToMailbox(Bitboards[wPAWN],     wPAWN, Mailbox);
  ToMailbox(Bitboards[wBISHOP], wBISHOP, Mailbox);
  ToMailbox(Bitboards[wKNIGHT], wKNIGHT, Mailbox);
  ToMailbox(Bitboards[wROOK],     wROOK, Mailbox);
  ToMailbox(Bitboards[wQUEEN],   wQUEEN, Mailbox);
  ToMailbox(Bitboards[wKING],     wKING, Mailbox);
}

inline int BitScanForward(bitboard B) {
  // Naively locate Least Significant 1 bit position
  if (!B) { return -1; }
  for (int i = 0; i < 64; i++) {
    if (B & 0x1ULL) { return i; }
    B >>= 1;
  }
  return -1;
}

inline int BitScanReverse(bitboard B) {
  // Naively locate Most Significant 1 bit position
  if (!B) { return -1; }
  uint64 mask = (0x1ULL << 63);
  for (int i = 63; i >= 0; i--) {
    if (B & mask) { return i; }
    B <<= 1;
  };
  return -1;
}

uint8 PopCount(bitboard B) {
  uint8 count = 0;
  while (B > 0) {
    if ((B & 0x1ULL) > 0) {
      count++;
    }
    B >>= 1;
  }
  return count;
}

inline void
CopyBitboards(bitboard Src[], bitboard Dest[]) {
  for (int i = EMPTY; i < UNUSED; i++) {
    Dest[i] = Src[i];
  }
}

inline void
CompareBitboards(bitboard A[], bitboard B[]) {
  for (int i = EMPTY; i < UNUSED; i++) {
    if (A[i] != B[i]) {
      printf("Boards mismatch on piece ");
      PrintPiece(i);
      printf("\n");
      printf("\n == State->Bitboards\n");
      PrintBitboard(A[i]);
      printf("\n == DEBUG_STATE1\n");
      PrintBitboard(B[i]);
    }
  }
}

// JUST FOR DEBUGGING
inline void
CompareBitboards(bitboard A[], bitboard B[], int depth, uint64 nodes, move m) {
  for (int i = EMPTY; i < UNUSED; i++) {
    if (A[i] != B[i]) {
      PrintPiece(i);
      printf(" BOARD MISMATCH! at depth %i nodes %llu\n", depth, nodes);
      printf("\n == State->Bitboards\n");
      PrintBitboard(A[i]);
      printf("\n == DEBUG_STATE1\n");
      PrintBitboard(B[i]);
      PrettyPrintMove(m);
    }
  }
}

#define BITBOARD_H
#endif
