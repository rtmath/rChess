#ifndef MOVES_H

#include "bitboard.h"

inline uint8
Rank(int n) {
  return n / 8;
}

inline uint8
File(int n) {
  return n % 8;
}

inline void
ChangeTurn(chess_state* S) {
  S->BoardState.WhiteToMove = !(S->BoardState.WhiteToMove);
}

inline uint8
CalculateEnPassantTarget(piece P, uint8 POrigin, uint8 PDestination) {
  if (!(P == wPAWN || P == bPAWN)) { return 0; }
  if ((PDestination - POrigin) == 16) { return PDestination - 8; }
  if ((POrigin - PDestination) == 16) { return PDestination + 8; }
  return 0;
}

inline bool32
EnPassant(uint8 Destination, uint8 EPTarget) {
  return (Destination == EPTarget) && (EPTarget != EMPTY);
}

inline bool32
EnPassant(move M) {
  return M.EPCapturedPieceLoc != EMPTY;
}

inline bool32
NotEmpty(bitboard BB) {
  return BB > 0;
}

inline bool32
PieceAtLowestBit(bitboard BB) {
  return (BB & 0x1) > 0;
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
    if (PieceBoardCoord < A3 && (legal_moves > 0)) { legal_moves |= ((p << 16) & EmptySquares); }
  } break;
  case bPAWN: {
    legal_moves = (p >> 8) & EmptySquares;
    if (PieceBoardCoord > H6 && (legal_moves > 0)) { legal_moves |= ((p >> 16) & EmptySquares); }
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

inline bitboard GetKnightAttacks(bitboard Knights) {
  bitboard result = EMPTY;
  uint8 coord = A1;
  while (NotEmpty(Knights)) {
    if (PieceAtLowestBit(Knights)) {
      result |= KnightMoves[coord];
    }

    coord++;
    Knights >>= 1;
  }

  return result;
}

inline bool32
WhiteKingIsInCheck(chess_state* State) {
  bitboard KingPos = State->Bitboards[wKING];
  bitboard Empties = State->Bitboards[EMPTY_SQ];
  bitboard DiagPieces = State->Bitboards[bBISHOP] | State->Bitboards[bQUEEN];
  bitboard NSEWPieces = State->Bitboards[bROOK] | State->Bitboards[bQUEEN];

  bitboard pawn_checks = (((State->Bitboards[bPAWN] >> 9) & notH) & KingPos) |
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

  bitboard pawn_checks = (((State->Bitboards[wPAWN] >> 9) & notH) & KingPos) |
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

bitboard MovesForPieceOnSquare(piece Type, uint8 BoardCoord, bitboard Pieces[], bool32 WhiteToMove) {
  bitboard PseudoQuiets   = PseudoLegalMoves(Type, BoardCoord, Pieces[EMPTY_SQ]);
  bitboard PseudoAttacks  = PseudoLegalAttacks(Type, BoardCoord, Pieces[EMPTY_SQ], (WhiteToMove) ? BlackOccupation(Pieces) : WhiteOccupation(Pieces) );
  return PseudoQuiets | PseudoAttacks;
}

bitboard DEBUG_MovesForPieceOnSquare(piece Type, uint8 BoardCoord, bitboard Pieces[], bool32 WhiteToMove, int depth) {
  bitboard PseudoQuiets   = PseudoLegalMoves(Type, BoardCoord, Pieces[EMPTY_SQ]);
  bitboard PseudoAttacks  = PseudoLegalAttacks(Type, BoardCoord, Pieces[EMPTY_SQ], (WhiteToMove) ? BlackOccupation(Pieces) : WhiteOccupation(Pieces) );
  /* TODO: REMOVE THIS DEBUG FUNCTION
  if (BoardCoord == A3 && (Type == wKNIGHT) && depth == 1) {
    printf("Quiets\n");
    PrintBitboard(PseudoQuiets);
    if (PseudoAttacks) { printf("Attacks\n"); PrintBitboard(PseudoAttacks); }
  }
  */
  return PseudoQuiets | PseudoAttacks;
}

int GeneratePseudoLegalMoves(bitboard Pieces[], bool32 WhiteToMove, uint8 EPTarget, move Moves[], mailbox* Mailbox, int depth) {
  int MCounter = 0;
  for (piece P = PAWN; P <= KING; P++) {
    piece Piece = (WhiteToMove) ? (P | W_MASK) : P;
    bitboard BB = Pieces[Piece];

    uint8 Origin = A1;
    while (NotEmpty(BB)) {
      if (PieceAtLowestBit(BB)) {
	bitboard Mvs = DEBUG_MovesForPieceOnSquare(Piece, Origin, Pieces, WhiteToMove, depth);
	
	uint8 Destination = A1;
	while (NotEmpty(Mvs)) {
	  if (PieceAtLowestBit(Mvs)) {
	    move M = {};
	    M.Origin = Origin;
	    M.Destination = Destination;
	    M.PieceType = Piece;
	    M.DestPieceType = Mailbox->Squares[Destination];
	    uint8 NewEPTarget = CalculateEnPassantTarget(M.PieceType, M.Origin, M.Destination);
            M.PrevEPTarget = EMPTY;
            M.NewEPTarget = NewEPTarget;

	    if (EnPassant(M.Destination, EPTarget)) {
	      if (EPTarget > H5) {
		M.EPCapturedPieceLoc = M.Destination - 8;
		M.DestPieceType = bPAWN;
	      } else {
		M.EPCapturedPieceLoc = M.Destination + 8;
		M.DestPieceType = wPAWN;
	      }
	    }

	    Moves[MCounter] = M;
	    MCounter++;
	  }
	  Destination++;
	  Mvs >>= 1;
	}
      }
      Origin++;
      BB >>= 1;
    }
  }
  return MCounter;
}

inline bool32
Promotion(piece P, uint8 Destination) {
 return (PieceIsType(P, PAWN) &&
	 (Rank(Destination) == 7 || Rank(Destination) == 0));
}

move Move(piece Type, piece DestType, uint8 Origin, uint8 Destination) {
  move M = {};
  M.PieceType = Type;
  M.DestPieceType = DestType;
  M.Origin = Origin;
  M.Destination = Destination;
  return M;
}

void MakeMove(chess_state* S, move* M) {
  bitboard* BBs = &S->Bitboards[0];
  
  ClearPiece(BBs, M->DestPieceType, M->Destination);
  MovePiece(BBs, M->PieceType, M->Origin, M->Destination);

  int CurrentEPTarget = S->BoardState.EnPassantTarget;

  if (EnPassant(M->Destination, CurrentEPTarget)) {
    if (CurrentEPTarget > H5) {
      M->EPCapturedPieceLoc = M->Destination - 8;
      M->DestPieceType = bPAWN;
      ClearPiece(&BBs[bPAWN], M->EPCapturedPieceLoc);
    } else {
      M->EPCapturedPieceLoc = M->Destination + 8;
      M->DestPieceType = wPAWN;
      ClearPiece(&BBs[wPAWN], M->EPCapturedPieceLoc);
    }
  }

  if (Promotion(M->PieceType, M->Destination)) {
    ClearPiece(&BBs[M->PieceType], M->Destination);
    // For now, assume promotion will promote to Q
    piece PromoPiece = (IsWhite(M->PieceType)) ? wQUEEN : bQUEEN;
    SetPiece(&BBs[PromoPiece], M->Destination);
    M->PromoPieceType = PromoPiece;
  }

  uint8 NewEPTarget = CalculateEnPassantTarget(M->PieceType, M->Origin, M->Destination);
  M->PrevEPTarget = CurrentEPTarget;
  M->NewEPTarget = NewEPTarget;
  S->BoardState.EnPassantTarget = NewEPTarget;

  //StoreMove(M);

  S->Bitboards[OCCUP_SQ] = CalculateOccupation(S->Bitboards);
  S->Bitboards[EMPTY_SQ] = ~S->Bitboards[OCCUP_SQ];

  ChangeTurn(S);
}

void UnmakeMove(chess_state* S, move M) {
  bitboard* BBs = &S->Bitboards[0];

  MovePiece(BBs, M.PieceType, M.Destination, M.Origin); // replace the moved piece
  SetPiece(BBs, M.DestPieceType, ( EnPassant(M) ? M.EPCapturedPieceLoc : M.Destination ));   // replace a potentially capped piece

  if (M.PromoPieceType != EMPTY) {
    ClearPiece(BBs, M.PromoPieceType, M.Destination);
  }

  S->BoardState.EnPassantTarget = M.PrevEPTarget;

  S->Bitboards[OCCUP_SQ] = CalculateOccupation(S->Bitboards);
  S->Bitboards[EMPTY_SQ] = ~S->Bitboards[OCCUP_SQ];

  ChangeTurn(S);
}

#define MOVES_H
#endif
