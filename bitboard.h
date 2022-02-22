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
SetPiece(bitboard* B, int destination) {
  SetBit(B, destination);
}

inline void
ClearPiece(bitboard* B, int location) {
  ClearBit(B, location);
}

inline void
ClearPiece(bitboard* Boards, piece Piece, int location) {
  if (Piece == EMPTY) { return; }
  ClearBit(&Boards[Piece], location);
}

inline void
MovePiece(bitboard* B, int origin, int destination) {
  ClearBit(B, origin);
  SetBit(B, destination);
}

void MovePiece(bitboard* Boards, piece Piece, int origin, int destination) {
  if (Piece == EMPTY) { return; }
  MovePiece(&Boards[Piece], origin, destination);
}

void SetPiece(bitboard* Boards, piece Piece, int destination) {
  if (Piece == EMPTY) { return; }
  SetPiece(&Boards[Piece], destination);
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

bitboard FloodFillNE(bitboard Pieces, bitboard Empties) {
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

bitboard FloodAttacksS(bitboard Pieces, bitboard Empties) {
  bitboard flood = Pieces;
  flood |= Pieces = (Pieces >> 8) & Empties;
  flood |= Pieces = (Pieces >> 8) & Empties;
  flood |= Pieces = (Pieces >> 8) & Empties;
  flood |= Pieces = (Pieces >> 8) & Empties;
  flood |= Pieces = (Pieces >> 8) & Empties;
  flood |=          (Pieces >> 8) & Empties;
  return             (flood >> 8);
}

bitboard FloodAttacksN(bitboard Pieces, bitboard Empties) {
  bitboard flood = Pieces;
  flood |= Pieces = (Pieces << 8) & Empties;
  flood |= Pieces = (Pieces << 8) & Empties;
  flood |= Pieces = (Pieces << 8) & Empties;
  flood |= Pieces = (Pieces << 8) & Empties;
  flood |= Pieces = (Pieces << 8) & Empties;
  flood |=          (Pieces << 8) & Empties;
  return             (flood << 8);
}

bitboard FloodAttacksE(bitboard Pieces, bitboard Empties) {
  bitboard flood = Pieces;
  Empties &= notA;
  flood |= Pieces = (Pieces << 1) & Empties;
  flood |= Pieces = (Pieces << 1) & Empties;
  flood |= Pieces = (Pieces << 1) & Empties;
  flood |= Pieces = (Pieces << 1) & Empties;
  flood |= Pieces = (Pieces << 1) & Empties;
  flood |=          (Pieces << 1) & Empties;
  return             (flood << 1) & notA;
}

bitboard FloodAttacksNE(bitboard Pieces, bitboard Empties) {
  bitboard flood = Pieces;
  Empties &= notA;
  flood |= Pieces = (Pieces << 9) & Empties;
  flood |= Pieces = (Pieces << 9) & Empties;
  flood |= Pieces = (Pieces << 9) & Empties;
  flood |= Pieces = (Pieces << 9) & Empties;
  flood |= Pieces = (Pieces << 9) & Empties;
  flood |=          (Pieces << 9) & Empties;
  return             (flood << 9) & notA;
}

bitboard FloodAttacksSE(bitboard Pieces, bitboard Empties) {
  bitboard flood = Pieces;
  Empties &= notA;
  flood |= Pieces = (Pieces >> 7) & Empties;
  flood |= Pieces = (Pieces >> 7) & Empties;
  flood |= Pieces = (Pieces >> 7) & Empties;
  flood |= Pieces = (Pieces >> 7) & Empties;
  flood |= Pieces = (Pieces >> 7) & Empties;
  flood |=          (Pieces >> 7) & Empties;
  return             (flood >> 7) & notA;
}

bitboard FloodAttacksW(bitboard Pieces, bitboard Empties) {
  bitboard flood = Pieces;
  Empties &= notH;
  flood |= Pieces = (Pieces >> 1) & Empties;
  flood |= Pieces = (Pieces >> 1) & Empties;
  flood |= Pieces = (Pieces >> 1) & Empties;
  flood |= Pieces = (Pieces >> 1) & Empties;
  flood |= Pieces = (Pieces >> 1) & Empties;
  flood |=          (Pieces >> 1) & Empties;
  return             (flood >> 1) & notH;
}

bitboard FloodAttacksSW(bitboard Pieces, bitboard Empties) {
  bitboard flood = Pieces;
  Empties &= notH;
  flood |= Pieces = (Pieces >> 9) & Empties;
  flood |= Pieces = (Pieces >> 9) & Empties;
  flood |= Pieces = (Pieces >> 9) & Empties;
  flood |= Pieces = (Pieces >> 9) & Empties;
  flood |= Pieces = (Pieces >> 9) & Empties;
  flood |=          (Pieces >> 9) & Empties;
  return             (flood >> 9) & notH;
}

bitboard FloodAttacksNW(bitboard Pieces, bitboard Empties) {
  bitboard flood = Pieces;
  Empties &= notH;
  flood |= Pieces = (Pieces << 7) & Empties;
  flood |= Pieces = (Pieces << 7) & Empties;
  flood |= Pieces = (Pieces << 7) & Empties;
  flood |= Pieces = (Pieces << 7) & Empties;
  flood |= Pieces = (Pieces << 7) & Empties;
  flood |=          (Pieces << 7) & Empties;
  return             (flood << 7) & notH;
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
    legal_moves = ((p << 8) & EmptySquares)        | // North
                  ((p >> 8) & EmptySquares)        | // South
                  ((p << 1) & EmptySquares & notA) | // East
                  ((p >> 1) & EmptySquares & notH) | // West
                  ((p << 9) & EmptySquares & notA) | // NE
                  ((p >> 7) & EmptySquares & notA) | // SE
                  ((p << 7) & EmptySquares & notH) | // NW
                  ((p >> 9) & EmptySquares & notH);  // SW
  } break;
  }
  return legal_moves;
}

bitboard
PseudoLegalAttacks(piece Piece, int PieceBoardCoord, bitboard Empties, bitboard EnemyOccupation) {
  bitboard legal_attacks = 0;
  bitboard p = BitboardWithBitSet(0, PieceBoardCoord);
  switch (Piece) {
  case wPAWN: {
    legal_attacks = (((p << 7) & notH & EnemyOccupation) | // NW
                     ((p << 9) & notA & EnemyOccupation)); // NE
  } break;
  case bPAWN: {
    legal_attacks = (((p >> 9) & notH & EnemyOccupation) | // SW
		     ((p >> 7) & notA & EnemyOccupation)); // SE
  } break;
  case wBISHOP:
  case bBISHOP: {
    legal_attacks =  (FloodAttacksNW(p, Empties) |
                      FloodAttacksNE(p, Empties) |
                      FloodAttacksSW(p, Empties) |
                      FloodAttacksSE(p, Empties)) & EnemyOccupation;
  } break;
  case wKNIGHT:
  case bKNIGHT: {
    legal_attacks = KnightMoves[PieceBoardCoord] & EnemyOccupation;
  } break;
  case wROOK:
  case bROOK: {
    legal_attacks = (FloodAttacksN(p, Empties) |
		     FloodAttacksS(p, Empties) |
		     FloodAttacksE(p, Empties) |
		     FloodAttacksW(p, Empties)) & EnemyOccupation;
  } break;
  case wQUEEN:
  case bQUEEN: {
    legal_attacks = (FloodAttacksNW(p, Empties) |
                     FloodAttacksNE(p, Empties) |
                     FloodAttacksSW(p, Empties) |
                     FloodAttacksSE(p, Empties) |
                     FloodAttacksN(p, Empties)  |
                     FloodAttacksS(p, Empties)  |
                     FloodAttacksW(p, Empties)  |
                     FloodAttacksE(p, Empties)) & EnemyOccupation;
  } break;
  case wKING:
  case bKING: {
    legal_attacks = ((p << 8) & EnemyOccupation)        | // North
                    ((p >> 8) & EnemyOccupation)        | // South
                    ((p << 1) & EnemyOccupation & notA) | // East
                    ((p >> 1) & EnemyOccupation & notH) | // West
                    ((p << 9) & EnemyOccupation & notA) | // NE
                    ((p >> 7) & EnemyOccupation & notA) | // SE
                    ((p << 7) & EnemyOccupation & notH) | // NW
                    ((p >> 9) & EnemyOccupation & notH);  // SW
  } break;
  }
  return legal_attacks;
}

inline bitboard GetKnightAttacks(bitboard Knights) {
  // This assumes maximum of 2 knights which is incorrect
  // TODO: Change to while loop (while Knights, Knights >>= 1, etc etc)
  int knight1_coord = BitScanReverse(Knights);
  int knight2_coord = BitScanForward(Knights);
  bitboard result = 0ULL;
  if (knight1_coord > 0) { result |= (KnightMoves[knight1_coord]); }
  if (knight2_coord > 0) { result |= (KnightMoves[knight2_coord]); }
  return result;
}

inline bitboard EnPassantAttacks(piece P, uint8 Origin, uint8 EPTarget) {
  bitboard p  = BitboardWithBitSet(EMPTY, Origin);
  bitboard ep = BitboardWithBitSet(EMPTY, EPTarget);
  if (IsWhite(P)) { return ((p << 7) & notH & ep) | ((p << 9) & notA & ep); }
  return ((p >> 9) & notH & ep) | ((p >> 7) & notA & ep);
}

inline bitboard WhiteOccupation(bitboard BBs[]) {
  return BBs[wPAWN]   |
         BBs[wBISHOP] |
         BBs[wKNIGHT] |
         BBs[wROOK]   |
         BBs[wQUEEN]  |
         BBs[wKING];
}

inline bitboard BlackOccupation(bitboard BBs[]) {
  return BBs[bPAWN]   |
         BBs[bBISHOP] |
         BBs[bKNIGHT] |
         BBs[bROOK]   |
         BBs[bQUEEN]  |
         BBs[bKING];
}

inline bitboard
CalculateOccupation(bitboard Bitboards[]) {
  return BlackOccupation(Bitboards) | WhiteOccupation(Bitboards);
}

inline bool32
WhiteKingIsInCheck(chess_state* State) {
  bitboard KingPos = State->Bitboards[wKING];
  bitboard Empties = State->Bitboards[EMPTY_SQ];
  bitboard DiagPieces = State->Bitboards[bBISHOP] | State->Bitboards[bQUEEN];
  bitboard NSEWPieces = State->Bitboards[bROOK] | State->Bitboards[bQUEEN];

  bitboard pawn_checks = (((State->Bitboards[bPAWN] >> 9) & notH) & KingPos) ||
                         (((State->Bitboards[bPAWN] >> 7) & notA) & KingPos);
  bitboard diagonal_checks = (FloodAttacksNW(DiagPieces, Empties) |
                              FloodAttacksNE(DiagPieces, Empties) |
                              FloodAttacksSW(DiagPieces, Empties) |
                              FloodAttacksSE(DiagPieces, Empties)) & KingPos;
  bitboard nsew_checks = (FloodAttacksN(NSEWPieces, Empties) |
		          FloodAttacksS(NSEWPieces, Empties) |
		          FloodAttacksE(NSEWPieces, Empties) |
		          FloodAttacksW(NSEWPieces, Empties)) & KingPos;
  bitboard knight_checks = GetKnightAttacks(State->Bitboards[bKNIGHT]) & KingPos;

  return pawn_checks || diagonal_checks || nsew_checks || knight_checks;
}

inline bool32
BlackKingIsInCheck(chess_state* State) {
  bitboard KingPos = State->Bitboards[bKING];
  bitboard Empties = State->Bitboards[EMPTY_SQ];
  bitboard DiagPieces = State->Bitboards[wBISHOP] | State->Bitboards[wQUEEN];
  bitboard NSEWPieces = State->Bitboards[wROOK] | State->Bitboards[wQUEEN];

  bitboard pawn_checks = (((State->Bitboards[wPAWN] >> 9) & notH) & KingPos) ||
                         (((State->Bitboards[wPAWN] >> 7) & notA) & KingPos);
  bitboard diagonal_checks = (FloodAttacksNW(DiagPieces, Empties) |
                              FloodAttacksNE(DiagPieces, Empties) |
                              FloodAttacksSW(DiagPieces, Empties) |
                              FloodAttacksSE(DiagPieces, Empties)) & KingPos;
  bitboard nsew_checks = (FloodAttacksN(NSEWPieces, Empties) |
		          FloodAttacksS(NSEWPieces, Empties) |
		          FloodAttacksE(NSEWPieces, Empties) |
		          FloodAttacksW(NSEWPieces, Empties)) & KingPos;
  bitboard knight_checks = GetKnightAttacks(State->Bitboards[wKNIGHT]) & KingPos;

  return pawn_checks || diagonal_checks || nsew_checks || knight_checks;
}

inline bool32
KingIsInCheck(chess_state* State) {
  if (State->BoardState.WhiteToMove) { return WhiteKingIsInCheck(State); }
  return BlackKingIsInCheck(State);
}

inline bool32
KingIsInCheck(chess_state* State, bool32 WhiteToMove) {
  if (WhiteToMove) { return WhiteKingIsInCheck(State); }
  return BlackKingIsInCheck(State);
}

#define BITBOARD_H
#endif
