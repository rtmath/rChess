#ifndef BITBOARD_H

void PrintBinary(uint64 n) {
  byte rep[65] = {};
  uint64 mask = (1ULL << 63);
  for (int i = 0; i < 64; i++) {
    rep[i] = (n & mask) ? '1' : '0';
    n <<= 1;
  }
  rep[64] = '\0';
  printf("%s\n", rep);
}

void PrintBitboard(bitboard B) {
  uint64 mask = (1ULL << 63);
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      putc((B & mask) ? '1' : '0', stdout);
      putc(' ', stdout);
      B <<= 1;
    }
    putc('\n', stdout);
  }
  putc('\n', stdout);
}

inline uint64
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

inline void
ClearBit(uint64* target, unsigned char nth_place) {
  Assert(nth_place < 64);
  uint64 index = 0x1;
  *target &= ~(index << nth_place);
}

inline void
ToggleBit(uint64* target, unsigned char nth_place) {
  Assert(nth_place < 64);
  uint64 index = 0x1;
  *target ^= (index << nth_place);
}

inline bool32
CheckBit(uint64 n, unsigned char nth_place) {
  Assert(nth_place < 64);
  return !!((n >> nth_place) & 0x1ULL);
};

inline void
SetPiece(bitboard* B, int destination) {
  SetBit(B, destination);
};

inline void
MovePiece(bitboard* B, int origin, int destination) {
  ClearBit(B, origin);
  SetBit(B, destination);
};

// Dumb7Fills from chessprogrammingwiki
bitboard FloodFillN(bitboard Pieces, bitboard Empties) {
  bitboard flood = Pieces & Empties;
  flood |= Pieces = (Pieces << 8) & Empties;
  flood |= Pieces = (Pieces << 8) & Empties;
  flood |= Pieces = (Pieces << 8) & Empties;
  flood |= Pieces = (Pieces << 8) & Empties;
  flood |= Pieces = (Pieces << 8) & Empties;
  flood |= Pieces = (Pieces << 8) & Empties;
  flood |=          (Pieces << 8) & Empties;
  return flood;
}

bitboard FloodFillS(bitboard Pieces, bitboard Empties) {
  bitboard flood = Pieces & Empties;
  flood |= Pieces = (Pieces >> 8) & Empties;
  flood |= Pieces = (Pieces >> 8) & Empties;
  flood |= Pieces = (Pieces >> 8) & Empties;
  flood |= Pieces = (Pieces >> 8) & Empties;
  flood |= Pieces = (Pieces >> 8) & Empties;
  flood |= Pieces = (Pieces >> 8) & Empties;
  flood |=          (Pieces >> 8) & Empties;
  return flood;
}

bitboard FloodFillW(bitboard Pieces, bitboard Empties) {
  bitboard notH = 0x7F7F7F7F7F7F7F7F;
  Empties &= notH;
  bitboard flood = Pieces & Empties;
  flood |= Pieces = ((Pieces >> 1) & Empties);
  flood |= Pieces = ((Pieces >> 1) & Empties);
  flood |= Pieces = ((Pieces >> 1) & Empties);
  flood |= Pieces = ((Pieces >> 1) & Empties);
  flood |= Pieces = ((Pieces >> 1) & Empties);
  flood |= Pieces = ((Pieces >> 1) & Empties);
  flood |=          ((Pieces >> 1) & Empties);
  return flood & notH;
}

bitboard FloodFillE(bitboard Pieces, bitboard Empties) {
  bitboard notA = 0xFEFEFEFEFEFEFEFE;
  Empties &= notA;
  bitboard flood = Pieces & Empties;
  flood |= Pieces = (Pieces << 1) & Empties;
  flood |= Pieces = (Pieces << 1) & Empties;
  flood |= Pieces = (Pieces << 1) & Empties;
  flood |= Pieces = (Pieces << 1) & Empties;
  flood |= Pieces = (Pieces << 1) & Empties;
  flood |= Pieces = (Pieces << 1) & Empties;
  flood |=          (Pieces << 1) & Empties;
  return flood & notA;
}

// The chessprogramming wiki shows these functions returning (flood shift n) & notFile
// but this was causing the flood fills to ignore occupied squares one shift away.
// I replaced & notFile with & Empties, since Empties contains the notFile information
// from the earlier &
bitboard FloodFillNE(bitboard Pieces, bitboard Empties) {
  bitboard notA = 0xFEFEFEFEFEFEFEFE;
  bitboard flood = Pieces;
  Empties &= notA;
  flood |= Pieces = (Pieces << 9) & Empties;
  flood |= Pieces = (Pieces << 9) & Empties;
  flood |= Pieces = (Pieces << 9) & Empties;
  flood |= Pieces = (Pieces << 9) & Empties;
  flood |= Pieces = (Pieces << 9) & Empties;
  flood |=          (Pieces << 9) & Empties;
  return             (flood << 9) & Empties;
}

bitboard FloodFillSE(bitboard Pieces, bitboard Empties) {
  bitboard notA = 0xFEFEFEFEFEFEFEFE;
  bitboard flood = Pieces;
  Empties &= notA;
  flood |= Pieces = (Pieces >> 7) & Empties;
  flood |= Pieces = (Pieces >> 7) & Empties;
  flood |= Pieces = (Pieces >> 7) & Empties;
  flood |= Pieces = (Pieces >> 7) & Empties;
  flood |= Pieces = (Pieces >> 7) & Empties;
  flood |=          (Pieces >> 7) & Empties;
  return             (flood >> 7) & Empties;
}

bitboard FloodFillNW(bitboard Pieces, bitboard Empties) {
  bitboard notH = 0x7F7F7F7F7F7F7F7F;
  bitboard flood = Pieces;
  Empties &= notH;
  flood |= Pieces = (Pieces << 7) & Empties;
  flood |= Pieces = (Pieces << 7) & Empties;
  flood |= Pieces = (Pieces << 7) & Empties;
  flood |= Pieces = (Pieces << 7) & Empties;
  flood |= Pieces = (Pieces << 7) & Empties;
  flood |=          (Pieces << 7) & Empties;
  return             (flood << 7) & Empties;
}

bitboard FloodFillSW(bitboard Pieces, bitboard Empties) {
  bitboard notH = 0x7F7F7F7F7F7F7F7F;
  bitboard flood = Pieces;
  Empties &= notH;
  flood |= Pieces = (Pieces >> 9) & Empties;
  flood |= Pieces = (Pieces >> 9) & Empties;
  flood |= Pieces = (Pieces >> 9) & Empties;
  flood |= Pieces = (Pieces >> 9) & Empties;
  flood |= Pieces = (Pieces >> 9) & Empties;
  flood |=          (Pieces >> 9) & Empties;
  return             (flood >> 9) & Empties;
}

bitboard
PseudoLegalMoves(piece Piece, int PieceBoardCoord, bitboard EmptySquares) {
  bitboard legal_moves = 0;
  bitboard p = BitboardWithBitSet(0, PieceBoardCoord);
  
  switch (Piece) {
  case wPAWN: {
    legal_moves = (p << 8) & EmptySquares;
    if (PieceBoardCoord < A3) { legal_moves |= ((p << 16) & EmptySquares); }
  } break;
  case bPAWN: {
    legal_moves = (p >> 8) & EmptySquares;
    if (PieceBoardCoord > H6) { legal_moves |= ((p >> 16) & EmptySquares); }
  } break;
  case wBISHOP:
  case bBISHOP: {
    legal_moves = FloodFillNW(p, EmptySquares) |
                  FloodFillNE(p, EmptySquares) |
                  FloodFillSW(p, EmptySquares) |
                  FloodFillSE(p, EmptySquares);
  } break;
  case wKNIGHT:
  case bKNIGHT: {
    legal_moves = KnightMoves[PieceBoardCoord] & EmptySquares;
  } break;
  case wROOK:
  case bROOK: {
    legal_moves = FloodFillN(p, EmptySquares) |
                  FloodFillS(p, EmptySquares) |
                  FloodFillW(p, EmptySquares) |
                  FloodFillE(p, EmptySquares);
  } break;
  case wQUEEN:
  case bQUEEN: {
    legal_moves = FloodFillNW(p, EmptySquares) |
                  FloodFillNE(p, EmptySquares) |
                  FloodFillSW(p, EmptySquares) |
                  FloodFillSE(p, EmptySquares) |
                  FloodFillN(p, EmptySquares)  |
                  FloodFillS(p, EmptySquares)  |
                  FloodFillW(p, EmptySquares)  |
                  FloodFillE(p, EmptySquares);
  } break;
  case wKING:
  case bKING: {
    legal_moves = ((p << 8) & EmptySquares) | // North
                  ((p >> 8) & EmptySquares) | // South
                  ((p << 1) & EmptySquares) | // East
                  ((p >> 1) & EmptySquares) | // West
                  ((p << 9) & EmptySquares) | // NE
                  ((p >> 7) & EmptySquares) | // SE
                  ((p << 7) & EmptySquares) | // NW
                  ((p >> 9) & EmptySquares);  // SW
                   
  } break;
  }
  return legal_moves;
};

void SetBitboard(bitboard* Boards, piece Piece, int destination) {
  if (Piece == EMPTY) { return; }
  SetPiece(&Boards[Piece], destination);
}

void UpdateBitboard(bitboard* Boards, piece Piece, int origin, int destination) {
  if (Piece == EMPTY) { return; }
  MovePiece(&Boards[Piece], origin, destination);
}

inline void
ToMailbox(bitboard BB, piece PieceType, mailbox* Mailbox) {
  if (!BB) { return; }
  for (int i = 0; i < 64; i++) {
    if (BB & 0x1ULL) { Mailbox->Squares[i] = PieceType; }
    BB >>= 1;
  };
};

inline void
PopulateMailbox(bitboard* Bitboards, mailbox* Mailbox) {
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

#define BITBOARD_H
#endif
