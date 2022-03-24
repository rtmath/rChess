#ifndef TEST_FEN_H

#include "../fen.h"

bool32 Equal(board_state A, board_state B) {
  if ((A.WhiteToMove != B.WhiteToMove) ||
      (A.wCanCastleKingside != B.wCanCastleKingside) ||
      (A.wCanCastleQueenside != B.wCanCastleQueenside) ||
      (A.bCanCastleKingside != B.bCanCastleKingside) ||
      (A.bCanCastleQueenside != B.bCanCastleQueenside) ||
      (A.EnPassantTarget != B.EnPassantTarget) ||
      (A.HalfMoves != B.HalfMoves) ||
      (A.FullMoves != B.FullMoves)) {
    return false;
  }
  return true;
};

bool32 TEST_Equal() {
  bool32 result = true;
  board_state B = {};

  if (!Equal(B, B)) {
    printf("TEST_Equal FAILED! Same board != itself\n");
    result = false;
  }

  board_state X = {};
  board_state Y = {};
  X.HalfMoves = 20;
  if (Equal(X, Y)) {
    printf("TEST_Equal FAILED! Differing boards are passing compare test\n");
    result = false;
  }
  
  return result;
};

bool32 TEST_PopulateBoardFromFEN() {
  char* starting_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  piece ExpectedPieces[] = { // from 0 = A1 to 63 = H8
    wROOK, wKNIGHT, wBISHOP, wQUEEN, wKING, wBISHOP, wKNIGHT, wROOK,
    wPAWN, wPAWN, wPAWN, wPAWN, wPAWN, wPAWN, wPAWN, wPAWN,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    bPAWN, bPAWN, bPAWN, bPAWN, bPAWN, bPAWN, bPAWN, bPAWN,
    bROOK, bKNIGHT, bBISHOP, bQUEEN, bKING, bBISHOP, bKNIGHT, bROOK
  };
  bitboard Expected[16] = {};
  for (int i = 0; i < 64; i++) { SetPiece(Expected, ExpectedPieces[i], i); }
  
  bitboard Actual[16] = {};
  
  int fendex = 0;
  PopulateBitboardFromFEN(&Actual[0], starting_FEN, &fendex);

  bool32 result = true;
  for (int i = 0; i < 16; i++) {
    if (Expected[i] != Actual[i]) {
      result = false;
      printf("TEST_PopulateBoardFromFen FAILED @ Index %i: Expected (%llu) does not match Actual (%llu)\n", i, Expected[i], Actual[i]);
      printf("Actual: \n");
      PrintBitboard(Actual[i]);
      printf("Expected: \n");
      PrintBitboard(Expected[i]);
      break;
    }
  }
  
  return result;
};

bool32 TEST_SideToMoveFromFEN() {
  board_state Expected1 = {};
  Expected1.WhiteToMove = true;

  board_state Expected2 = {};
  Expected2.WhiteToMove = false;

  board_state Tests[] = { Expected1, Expected2 };
  char* FENS[] = { "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" ,
                   "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1" };

  bool32 result = true;
  for (int test = 0; test < ArrayCount(Tests); test++) {
    char* fen = FENS[test];
    int fendex = 44;
    board_state Expected = Tests[test];
    board_state Actual = {};
    SideToMoveFromFEN(&Actual, fen, &fendex);
    if (!Equal(Expected, Actual)) {
      result = false;
      printf("TEST_SideToMoveFromFEN() Test #%i FAILED!\n", test);
      PrintBoardState(Expected, "Expected State");
      PrintBoardState(Actual, "Actual State");
    }
  }
  return result;
}

bool32 TEST_CastlingAvailabilityFromFEN() {
  char* FENS[] = {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1",
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w kq - 0 1",
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQ - 0 1",
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w q - 0 1"
  };

  board_state Expected1 = {};
  Expected1.wCanCastleKingside  = true;
  Expected1.wCanCastleQueenside = true;
  Expected1.bCanCastleKingside  = true;
  Expected1.bCanCastleQueenside = true;

  board_state Expected2 = {};
  Expected2.wCanCastleKingside  = false;
  Expected2.wCanCastleQueenside = false;
  Expected2.bCanCastleKingside  = false;
  Expected2.bCanCastleQueenside = false;

  board_state Expected3 = {};
  Expected3.wCanCastleKingside  = false;
  Expected3.wCanCastleQueenside = false;
  Expected3.bCanCastleKingside  = true;
  Expected3.bCanCastleQueenside = true;

  board_state Expected4 = {};
  Expected4.wCanCastleKingside  = true;
  Expected4.wCanCastleQueenside = true;
  Expected4.bCanCastleKingside  = false;
  Expected4.bCanCastleQueenside = false;

  board_state Expected5 = {};
  Expected5.wCanCastleKingside  = false;
  Expected5.wCanCastleQueenside = false;
  Expected5.bCanCastleKingside  = false;
  Expected5.bCanCastleQueenside = true;

  board_state Tests[] = {
    Expected1,
    Expected2,
    Expected3,
    Expected4,
    Expected5
  };		     
  
  bool32 result = true;
  for (int test = 0; test < ArrayCount(Tests); test++) {
    board_state Expected = Tests[test];
    board_state Actual = {};
    int fendex = 46;
    CastlingAvailabilityFromFEN(&Actual, FENS[test], &fendex);
    if (!Equal(Expected, Actual)) {
      result = false;
      printf("TEST_CastleAvailabilityFromFEN() Test #%i FAILED!\n", test);
      PrintBoardState(Expected, "Expected State");
      PrintBoardState(Actual, "Actual State");
    }
  }
  return result;
}


bool32 TEST_EnPassantFromFEN() {
  char* FENS[] = {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
    "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
  };
  int fendices[] = { 51, 53, 55 };

  board_state Expected1 = {};
  Expected1.EnPassantTarget = 0;

  board_state Expected2 = {};
  Expected2.EnPassantTarget = E3;

  board_state Expected3 = {};
  Expected3.EnPassantTarget = C6;

  board_state Tests[] = { Expected1, Expected2, Expected3 };

  bool32 result = true;
  for (int test = 0; test < ArrayCount(Tests); test++) {
    board_state Expected = Tests[test];
    board_state Actual = {};
    int fendex = fendices[test];
    EnPassantFromFEN(&Actual, FENS[test], &fendex);
    if (!Equal(Expected, Actual)) {
      result = false;
      printf("TEST_EnPassantFromFEN() Test #%i FAILED!\n", test);
      PrintBoardState(Expected, "Expected State");
      PrintBoardState(Actual, "Actual State");
    }
  };
  
  return result;
}

bool32 TEST_MoveCountFromFEN() {
  char* FENS[] = {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 7 1",
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 42 1",
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 100 1"
  };

  board_state Expected1 = {};
  Expected1.HalfMoves = 0;

  board_state Expected2 = {};
  Expected2.HalfMoves = 7;

  board_state Expected3 = {};
  Expected3.HalfMoves = 42;

  board_state Expected4 = {};
  Expected4.HalfMoves = 100;

  board_state Tests[] = { Expected1, Expected2, Expected3, Expected4 };

  int test_count = 4;
  bool32 result = true;
  for (int test = 0; test < test_count; test++) {
    board_state Expected = Tests[test];
    board_state Actual = {};
    int fendex = 53;
    MoveCountFromFEN(&Actual.HalfMoves, FENS[test], &fendex);
  }
  
  return result;
}

bool32 TEST_FENToBitboards() {
  char* starting_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

  bitboard ExpectedBitboards[16] = {};
  board_state ExpectedState = {};
  piece ExpectedPieces[] = { // from 0 = A1 to 63 = H8
    wROOK, wKNIGHT, wBISHOP, wQUEEN, wKING, wBISHOP, wKNIGHT, wROOK,
    wPAWN, wPAWN, wPAWN, wPAWN, wPAWN, wPAWN, wPAWN, wPAWN,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    bPAWN, bPAWN, bPAWN, bPAWN, bPAWN, bPAWN, bPAWN, bPAWN,
    bROOK, bKNIGHT, bBISHOP, bQUEEN, bKING, bBISHOP, bKNIGHT, bROOK
  };
  ExpectedState.WhiteToMove = true;
  ExpectedState.wCanCastleKingside = true;
  ExpectedState.wCanCastleQueenside = true;
  ExpectedState.bCanCastleKingside = true;
  ExpectedState.bCanCastleQueenside = true;
  ExpectedState.EnPassantTarget = 0;
  ExpectedState.HalfMoves = 0;
  ExpectedState.FullMoves = 1;
  for (int i = 0; i < 64; i++) { SetPiece(ExpectedBitboards, ExpectedPieces[i], i); }

  ExpectedBitboards[OCCUP_SQ] = (ExpectedBitboards[wPAWN]   |
				 ExpectedBitboards[wBISHOP] |
				 ExpectedBitboards[wKNIGHT] |
				 ExpectedBitboards[wROOK]   |
				 ExpectedBitboards[wQUEEN]  |
				 ExpectedBitboards[wKING]   |
				 ExpectedBitboards[bPAWN]   |
				 ExpectedBitboards[bBISHOP] |
				 ExpectedBitboards[bKNIGHT] |
				 ExpectedBitboards[bROOK]   |
				 ExpectedBitboards[bQUEEN]  |
				 ExpectedBitboards[bKING]);

  ExpectedBitboards[EMPTY_SQ] = ~(ExpectedBitboards[OCCUP_SQ]);
  
  bitboard ActualBitboards[16] = {};
  board_state ActualState = {};

  FENToBitboards(starting_FEN, ActualBitboards, &ActualState);
  bool32 result = true;
  if (!Equal(ExpectedState, ActualState)) {
    result = false;
    printf("TEST_FENToBitboards() FAILED! State mismatch\n");
    PrintBoardState(ExpectedState, "Expected State");
    PrintBoardState(ActualState, "Actual State");
  };
  for (int i = 0; i < 16; i++) {
    if (ExpectedBitboards[i] != ActualBitboards[i]) {
      result = false;
      printf("TEST_TO_FENToBitboards() FAILED! Bitboard %i mismatch\n", i);
      printf("Expected: \n");
      PrintBitboard(ExpectedBitboards[i]);
      printf("Actual: \n");
      PrintBitboard(ActualBitboards[i]);
    }
  }
  return result;
};

#define TEST_FEN_H
#endif
