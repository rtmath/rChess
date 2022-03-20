#ifndef TEST_BITBOARD_H

#include "bitboard.h"

uint64 Power(uint8 base, uint8 exp) {
  if (base == 0) return 0;
  if ( exp == 0) return 1;
  uint64 result = 1;
  for (int i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}

bool32 TEST_SetBit() {
  bool32 result = true;
  for (int i = 0; i < 64; i++) {
    uint64 expected = (i == 0) ? 1 : Power(2, i);
    uint64 actual = 0;
    SetBit(&actual, i);
    if (expected != actual) {
      printf("TEST_SetBit FAILED: Expected (%llu) does not match actual (%llu)\n",
	     expected, actual);
      result = false;
      break;
    }
  }
  return result;
}

bool32 TEST_ClearBit() {
  bool32 result = true;
  for (int i = 0; i < 64; i++) {
    uint64 expected = (i == 0) ? ~1 : ~(Power(2, i));
    uint64 actual = ~0ULL;
    ClearBit(&actual, i);
    if (expected != actual) {
      printf("TEST_ClearBit FAILED: Expected (%llu) does not match actual (%llu)\n",
	     expected, actual);
      result = false;
      break;
    }
  }
  return result;
}

bool32 TEST_ToggleBit() {
  bool32 result = true;
  for (int i = 0; i < 64; i++) {
    uint64 expected_on = (i == 0) ? 1 : Power(2, i);
    uint64 expected_off = 0;
    uint64 actual = 0;
    ToggleBit(&actual, i);
    if (actual != expected_on) {
      printf("TEST_ToggleBit On FAILED: Expected (%llu) does not match actual (%llu)\n",
	     expected_on, actual);
      result = false;
      break;
    }
    ToggleBit(&actual, i);
    if (actual != expected_off) {
      printf("TEST_ToggleBit Off FAILED: Expected (%llu) does not match actual (%llu)\n",
	     expected_off, actual);
      result = false;
      break;
    }
  }
  return result;
}

bool32 TEST_CheckBit() {
  bool32 result = true;
  for (int i = 0; i < 64; i++) {
    uint64 actual = 0;
    SetBit(&actual, i);
    if (!CheckBit(actual, i)) {
      printf("TEST_CheckBit FAILED: Did not detect set bit %i.", i);
      result = false;
      break;
    }
    ClearBit(&actual, i);
    if (CheckBit(actual, i)) {
      printf("TEST_CheckBit FAILED: Still detecting cleared bit %i.", i);
      result = false;
      break;
    }
  }
  return result;
}

bool32 TEST_PopCount() {
  bool32 result = true;
  for (uint8 i = 0; i < 64; i++) {
    bitboard BB = 0;
    for (uint8 j = 0; j < i; j++) {
      BB = SetBit(BB, j);
    }
    uint8 Expected = i;
    uint8 Actual = PopCount(BB);
    if (Expected != Actual) { printf("TEST_PopCount FAILED! Expected (%i) does not match actual (%i)\n", Expected, Actual); result = false;}
  }
  return result;
}

bool32 TEST_BitScanForward() {
  bool32 result = true;
  for (int i = 0; i < 64; i++) {
    uint64 tester = BitboardWithBitSet(0, i);

    int expected = i;
    int actual = BitScanForward(tester);

    if (expected != actual) {
      result = false;
      printf("TEST_BitScanForward FAILED: Expected i (%i) did not match actual i (%i)\n", expected, actual);
    }
  }
  return result;
}

bool32 TEST_BitScanReverse() {
  bool32 result = true;
  for (int i = 0; i < 64; i++) {
    uint64 tester = BitboardWithBitSet(0, i);

    int expected = i;
    int actual = BitScanReverse(tester);

    if (expected != actual) {
      result = false;
      printf("TEST_BitScanReverse FAILED: Expected i (%i) did not match actual i (%i)\n", expected, actual);
    }
  }
  return result;
}

bool32 TEST_PieceIsType() {
  bool32 result = true;

  piece Types[] = { PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING };
  piece White[] = { wPAWN, wBISHOP, wKNIGHT, wROOK, wQUEEN, wKING };
  piece Black[] = { bPAWN, bBISHOP, bKNIGHT, bROOK, bQUEEN, bKING };

  for (int i = 0; i < 6; i++) {
    bool32 actualW = PieceIsType(White[i], Types[i]);
    bool32 actualB = PieceIsType(Black[i], Types[i]);
    if (!actualW) { printf("TEST_PieceIsType FAILED: Expected white piece %i was not detected as type %i\n", White[i], Types[i]); result = false; }
    if (!actualB) { printf("TEST_PieceIsType FAILED: Expected black piece %i was not detected as type %i\n", Black[i], Types[i]); result = false; }
  }
  
  return result;
}

bool32 PawnCanMoveTwoSquares(piece Pawn, uint8 Origin) {
  int Direction = (IsWhite(Pawn)) ? 8 : -8;
  bitboard EmptyBoard = BitboardWithBitSet(EMPTY, Origin);
  bitboard ExpectedMoves = BitboardWithBitSet(EMPTY, Origin + Direction);
  ExpectedMoves = BitboardWithBitSet(ExpectedMoves, Origin + Direction + Direction);
  bitboard ActualMoves = PseudoLegalMoves(Pawn, Origin, ~EmptyBoard);

  if (ExpectedMoves != ActualMoves) { return false; }
  return true;
}

bool32 TEST_PawnMovesOnEmptyBoard() {
  bool32 result = true;
  uint8 TestWOrigins[] = { A2, B2, C2, D2, E2, F2, G2, H2,
			   A3, B3, C3, D3, E3, F3, G3, H3 };
  uint8 TestBOrigins[] = { A7, B7, C7, D7, E7, F7, G7, H7,
			   A6, B6, C6, D6, E6, F6, G6, H6 };
  bool32 Expected[] = {  true,  true,  true,  true,  true,  true,  true,  true,
			 false, false, false, false, false, false, false, false };

  for (int i = 0; i < 16; i++) {
    bool32 WMoves = PawnCanMoveTwoSquares(wPAWN, TestWOrigins[i]);
    bool32 BMoves = PawnCanMoveTwoSquares(bPAWN, TestBOrigins[i]);
    if (WMoves != Expected[i]) { printf("TEST_PLPawnMoves FAILED! %s at Position %s actual (%i) does not match expected (%i)\n", PieceNameLookup[wPAWN], SquareLookup[TestWOrigins[i]], Expected[i], WMoves); result = false; }
    if (BMoves != Expected[i]) { printf("TEST_PLPawnMoves FAILED! %s at Position %s actual (%i) does not match expected (%i)\n", PieceNameLookup[bPAWN], SquareLookup[TestBOrigins[i]], Expected[i], BMoves); result = false; }
  }
  return result;
}

uint8 CornerSquares[] = {
  A1,                         H1,
  A8,                         H8
};

uint8 EdgeSquares[] = {
      B1, C1, D1, E1, F1, G1,
  A2,                         H2,
  A3,                         H3,
  A4,                         H4,
  A5,                         H5,
  A6,                         H6,
  A7,                         H7,
      B8, C8, D8, E8, F8, G8
};

uint8 InnerEdgeSquares[] = {
      B2, C2, D2, E2, F2, G2,
      B3,                 G3,
      B4,                 G4,
      B5,                 G5,
      B6,                 G6,
      B7, C7, D7, E7, F7, G7
};		     

uint8 NonEdgeSquares[] = {
          C3, D3, E3, F3,
          C4,         F4,
          C5,         F5,
          C6, D6, E6, F6
};

uint8 CenterSquares[] = {
              D4, E4,
              D5, E5
};

bool32 ExpectedMoves(piece P, uint8 Squares[], int SquareCount, uint8 ExpectedMoves) {
  bool32 result = true;
  for (int i = 0; i < SquareCount; i++) {
    bitboard BB = BitboardWithBitSet(EMPTY, Squares[i]);
    bitboard Moves = PseudoLegalMoves(P, Squares[i], ~BB);
    uint8 Actual = PopCount(Moves);
    if (ExpectedMoves != Actual) {
      printf("ExpectedMoves FAILED! %s on square %s Expected Moves (%u) does not match Actual Moves (%u)\n",
	     PieceNameLookup[P], SquareLookup[Squares[i]], ExpectedMoves, Actual);
      result = false;
    };
  }
  return result;
}

bool32 TEST_BishopMovesOnEmptyBoard(piece P) {
  bool32 result = true;

  result = result && ExpectedMoves(P, CornerSquares, ArrayCount(CornerSquares), 7);
  result = result && ExpectedMoves(P, EdgeSquares, ArrayCount(EdgeSquares), 7);
  result = result && ExpectedMoves(P, InnerEdgeSquares, ArrayCount(InnerEdgeSquares), 9);
  result = result && ExpectedMoves(P, NonEdgeSquares, ArrayCount(NonEdgeSquares), 11);
  result = result && ExpectedMoves(P, CenterSquares, ArrayCount(CenterSquares), 13);

  return result;
}

bool32 TEST_KnightMovesOnEmptyBoard(piece P) {
  bool32 result = true;

  uint8 Expecting2[] = { A1, A8, H1, H8 };
  uint8 Expecting3[] = { A2, B1, A7, B8, G1, H2, G8, H7 };
  uint8 Expecting4[] = { A3, A4, A5, A6,
			 H3, H4, H5, H6,
			 C1, D1, E1, F1,
			 C8, D8, E8, F8,
			 B2, B7, G2, G7 };
  uint8 Expecting6[] = {     C2, D2, E2, F2,
			 B3,                G3,
			 B4,                G4,
			 B5,                G5,
			 B6,                G6,
			     C7, D7, E7, F7    };
  uint8 Expecting8[] = {     C3, D3, E3, F3,
                             C4, D4, E4, F4,
                             C5, D5, E5, F5,
                             C6, D6, E6, F6    };

  result = result && ExpectedMoves(P, Expecting2, ArrayCount(Expecting2), 2);
  result = result && ExpectedMoves(P, Expecting3, ArrayCount(Expecting3), 3);
  result = result && ExpectedMoves(P, Expecting4, ArrayCount(Expecting4), 4);
  result = result && ExpectedMoves(P, Expecting6, ArrayCount(Expecting6), 6);
  result = result && ExpectedMoves(P, Expecting8, ArrayCount(Expecting8), 8);

  return result;
}

bool32 TEST_RookMovesOnEmptyBoard(piece P) {
  bool32 result = true;

  result = result && ExpectedMoves(P, CornerSquares, ArrayCount(CornerSquares), 14);
  result = result && ExpectedMoves(P, EdgeSquares, ArrayCount(EdgeSquares), 14);
  result = result && ExpectedMoves(P, InnerEdgeSquares, ArrayCount(InnerEdgeSquares), 14);
  result = result && ExpectedMoves(P, NonEdgeSquares, ArrayCount(NonEdgeSquares), 14);
  result = result && ExpectedMoves(P, CenterSquares, ArrayCount(CenterSquares), 14);

  return result;
}

bool32 TEST_QueenMovesOnEmptyBoard(piece P) {

}

bool32 TEST_KingMovesOnEmptyBoard(piece P) {

}

bool32 TEST_Moves() {
  TEST_PawnMovesOnEmptyBoard();
  TEST_BishopMovesOnEmptyBoard(wBISHOP);
  TEST_BishopMovesOnEmptyBoard(bBISHOP);
  TEST_KnightMovesOnEmptyBoard(wKNIGHT);
  TEST_KnightMovesOnEmptyBoard(bKNIGHT);
  TEST_RookMovesOnEmptyBoard(wROOK);
  TEST_RookMovesOnEmptyBoard(bROOK);
  TEST_QueenMovesOnEmptyBoard(wQUEEN);
  TEST_QueenMovesOnEmptyBoard(bQUEEN);
  TEST_KingMovesOnEmptyBoard(wKING);
  TEST_KingMovesOnEmptyBoard(bKING);
  
  return true;
}

#define TEST_BITBOARD_H
#endif
