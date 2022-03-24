#ifndef PRINT_HELPERS_H

#include "rchess.h"

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
    if (row == 7) { printf(" <- A1\n"); }
    putc('\n', stdout);
  }
  putc('\n', stdout);
}

void PrintMailbox(mailbox M) {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      int i = (row * 8) + col;
      printf("%i ", (M.Squares[i]) ? 1 : 0);
    };
    printf("\n");
  };
};

void PrintBoardState(board_state B, char* Name) {
  printf("|| %s\n", Name);
  printf("  WhiteToMove: %s\n", (B.WhiteToMove) ? "true" : "false");
  printf("  wCanCastleKingside: %u\n", B.wCanCastleKingside);
  printf("  wCanCastleQueenside: %u\n", B.wCanCastleQueenside);
  printf("  bCanCastleKingside: %u\n", B.bCanCastleKingside);
  printf("  bCanCastleQueenside: %u\n", B.bCanCastleQueenside);
  printf("  EnPassant Target Square index: %u\n", B.EnPassantTarget);
  printf("  Half Moves: %i\n", B.HalfMoves);
  printf("  Full Moves: %i\n", B.FullMoves);
};

void PrintMove(move M) {
  printf("== MOVE ==\n");
  printf("  PieceType %u\n",    M.PieceType);
  printf("  DestPieceType %u\n", M.DestPieceType);
  printf("  PromoPieceType %u\n", M.PromoPieceType);
  printf("  Origin %u\n", M.Origin);
  printf("  Destination %u\n", M.Destination);
  printf("  NewEPTarget: %u\n", M.NewEPTarget);
  printf("  PrevEPTarget: %u\n", M.PrevEPTarget);
  printf("  White Lost Castle K %u\n", M.wLostCastleKingside);
  printf("  White Lost Castle Q %u\n", M.wLostCastleQueenside);
  printf("  Black Lost Castle K %u\n", M.bLostCastleKingside);
  printf("  Black Lost Castle Q %u\n", M.bLostCastleQueenside);
  printf("==========\n");
};

void PrintPiece(piece P) {
  printf("%s", PieceNameLookup[P]);
}

void PrintPiece(piece P, char Label[]) {
  printf("  %s %s\n", Label, PieceNameLookup[P]);
};

void PrettyPrintMove(move M) {
  printf("== MOVE ==\n");
  printf("  Piece number: %u\n", M.PieceType);
  PrintPiece(M.PieceType, "PieceType");
  PrintPiece(M.DestPieceType, "DestPieceType");
  PrintPiece(M.PromoPieceType, "PromoPieceType");
  printf("  Origin %s\n", SquareLookup[M.Origin]);
  printf("  Destination %s\n", SquareLookup[M.Destination]);

  printf("==========\n");
};

#define PRINT_HELPERS_H
#endif
