#ifndef TEST_FEN_H

#include "../fen.h"

void PrintBoard(board B, char* Name) {
  printf("|| %s\n", Name);
  printf("  WhiteToMove: %s\n", (B.WhiteToMove) ? "true" : "false");
  printf("  wCanCastleKingside: %u\n", B.wCanCastleKingside);
  printf("  wCanCastleQueenside: %u\n", B.wCanCastleQueenside);
  printf("  bCanCastleKingside: %u\n", B.bCanCastleKingside);
  printf("  bCanCastleQueenside: %u\n", B.bCanCastleQueenside);
  printf("  EnPassant Target Square index: %u\n", B.EnPassantTarget);
  printf("  Half Moves: %i\n", B.HalfMoves);
  printf("  Full Moves: %i\n", B.FullMoves);
  printf("  I'm not printing squares here unless I need to.\n");
};

void PrintBoard(board B) {
  PrintBoard(B, "Board");
}

bool32 Equal(board A, board B) {
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
  for (int i = 0; i < 64; i++) {
    if (A.Squares[i] != B.Squares[i]) {
      return false;
    }
  }
  return true;
};

bool32 TEST_Equal() {
  bool32 result = true;
  board B = {};

  if (!Equal(B, B)) {
    printf("TEST_Equal FAILED! Same board != itself\n");
    result = false;
  }

  board X = {};
  board Y = {};
  X.HalfMoves = 20;
  if (Equal(X, Y)) {
    printf("TEST_Equal FAILED! Differing boards are passing compare test\n");
    result = false;
  }

  for (int i = 0; i < 64; i++) {
    X.Squares[i] = i % 6;
    Y.Squares[i] = i % 7;
  }
  if (Equal(X, Y)) {
    printf("TEST_Equal FAILED! Boards with different squares are passing compare test\n");
    result = false;
  };

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
  board Expected = {};
  for (int i = 0; i < 64; i++) { Expected.Squares[i] = ExpectedPieces[i]; }
  
  board Actual = {};
  int fendex = 0;
  PopulateBoardFromFEN(&Actual, starting_FEN, &fendex);

  bool32 result = true;
  for (int i = 0; i < 64; i++) {
    if (Expected.Squares[i] != Actual.Squares[i]) {
      result = false;
      printf("TEST_PopulateBoardFromFen FAILED @ Index %i: Expected (%u) does not match Actual (%u)\n", i, Expected.Squares[i], Actual.Squares[i]);
      break;
    };
  };
  
  return result;
};

bool32 TEST_SideToMoveFromFEN() {
  board Expected1 = {};
  Expected1.WhiteToMove = true;

  board Expected2 = {};
  Expected2.WhiteToMove = false;

  board Boards[] = { Expected1, Expected2 };
  char* FENS[] = { "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" ,
                   "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1" };
  int test = 0;
  int test_count = 2;

  bool32 result = true;
  for (test; test < test_count; test++) {
    char* fen = FENS[test];
    int fendex = 44;
    board Expected = Boards[test];
    board Actual = {};
    SideToMoveFromFEN(&Actual, fen, &fendex);
    if (!Equal(Expected, Actual)) {
      result = false;
      printf("TEST_SideToMoveFromFEN() Test #%i FAILED!\n", test);
      PrintBoard(Expected, "Expected Board");
      PrintBoard(Actual, "Actual Board");
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

  board Expected1 = {};
  Expected1.wCanCastleKingside  = true;
  Expected1.wCanCastleQueenside = true;
  Expected1.bCanCastleKingside  = true;
  Expected1.bCanCastleQueenside = true;

  board Expected2 = {};
  Expected2.wCanCastleKingside  = false;
  Expected2.wCanCastleQueenside = false;
  Expected2.bCanCastleKingside  = false;
  Expected2.bCanCastleQueenside = false;

  board Expected3 = {};
  Expected3.wCanCastleKingside  = false;
  Expected3.wCanCastleQueenside = false;
  Expected3.bCanCastleKingside  = true;
  Expected3.bCanCastleQueenside = true;

  board Expected4 = {};
  Expected4.wCanCastleKingside  = true;
  Expected4.wCanCastleQueenside = true;
  Expected4.bCanCastleKingside  = false;
  Expected4.bCanCastleQueenside = false;

  board Expected5 = {};
  Expected5.wCanCastleKingside  = false;
  Expected5.wCanCastleQueenside = false;
  Expected5.bCanCastleKingside  = false;
  Expected5.bCanCastleQueenside = true;

  board Boards[] = {
    Expected1,
    Expected2,
    Expected3,
    Expected4,
    Expected5
  };		     
  
  int test = 0;
  int test_count = 5;
  bool32 result = true;
  for (test; test < test_count; test++) {
    board Expected = Boards[test];
    board Actual = {};
    int fendex = 46;
    CastlingAvailabilityFromFEN(&Actual, FENS[test], &fendex);
    if (!Equal(Expected, Actual)) {
      result = false;
      printf("TEST_CastleAvailabilityFromFEN() Test #%i FAILED!\n", test);
      PrintBoard(Expected, "Expected Board");
      PrintBoard(Actual, "Actual Board");
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

  board Expected1 = {};
  Expected1.EnPassantTarget = 0;

  board Expected2 = {};
  Expected2.EnPassantTarget = E3;

  board Expected3 = {};
  Expected3.EnPassantTarget = C6;

  board Boards[] = { Expected1, Expected2, Expected3 };

  int test_count = 3;
  bool32 result = true;
  for (int test = 0; test < test_count; test++) {
    board Expected = Boards[test];
    board Actual = {};
    int fendex = fendices[test];
    EnPassantFromFEN(&Actual, FENS[test], &fendex);
    if (!Equal(Expected, Actual)) {
      result = false;
      printf("TEST_EnPassantFromFEN() Test #%i FAILED!\n", test);
      PrintBoard(Expected, "Expected Board");
      PrintBoard(Actual, "Actual Board");
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

  board Expected1 = {};
  Expected1.HalfMoves = 0;

  board Expected2 = {};
  Expected2.HalfMoves = 7;

  board Expected3 = {};
  Expected3.HalfMoves = 42;

  board Expected4 = {};
  Expected4.HalfMoves = 100;

  board Boards[] = { Expected1, Expected2, Expected3, Expected4 };

  int test_count = 4;
  bool32 result = true;
  for (int test = 0; test < test_count; test++) {
    board Expected = Boards[test];
    board Actual = {};
    int fendex = 53;
    MoveCountFromFEN(&Actual.HalfMoves, FENS[test], &fendex);
  }
  
  return result;
}

bool32 TEST_FENToBoard() {
  char* starting_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

  board Expected = {};
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
  Expected.WhiteToMove = true;
  Expected.wCanCastleKingside = true;
  Expected.wCanCastleQueenside = true;
  Expected.bCanCastleKingside = true;
  Expected.bCanCastleQueenside = true;
  Expected.EnPassantTarget = 0;
  Expected.HalfMoves = 0;
  Expected.FullMoves = 1;
  for (int i = 0; i < 64; i++) { Expected.Squares[i] = ExpectedPieces[i]; }

  board Actual = {};

  FENToBoard(starting_FEN, &Actual);
  bool32 result = true;
  if (!Equal(Expected, Actual)) {
    result = false;
    printf("TEST_FENToBoard() FAILED!\n");
    PrintBoard(Expected, "Expected Board");
    PrintBoard(Actual, "Actual Board");
  };
  return result;
};

#define TEST_FEN_H
#endif
