#include <stdio.h>
#include "rchess.h"
#include "bmp.h"
#include "graphics.h"
#include "bitboard.h"
#include "undo.h"
#include "print_helpers.h"
#include "ui_helpers.h"
#include "tests/test_bitboard.h"
#include "tests/test_fen.h"
#include "tests/test_moves.h"

// TODO: Castling | check that SideCanCastle and SquaresArentAttacked and SquaresAreEmpty

inline bool32
WithinDims(int x, int y, dims Dims) {
  return ((x >= Dims.MinX) && (y >= Dims.MinY) &&
	  (x <= Dims.MaxX) && (y <= Dims.MaxY));
}

inline bool32
PieceBelongsToPlayer(piece P, bool32 WhiteToMove) {
  if (P == EMPTY) { return false; }
  return (IsWhite(P) == WhiteToMove);
}

void CopyMailbox(mailbox* Src, mailbox* Dst) {
  for (int i = 0; i < 64; i++) {
    Dst->Squares[i] = Src->Squares[i];
  }
}

void CompareMailboxes(mailbox* A, mailbox* B) {
  for (int i = 0; i < 64; i++) {
    if (A->Squares[i] != B->Squares[i]) {
      printf("Mailbox mismatch on square %s\n", SquareLookup[i]);
    }
  }
}

void CopyBoardState(board_state* Src, board_state* Dst) {
  Dst->WhiteToMove = Src->WhiteToMove;
  Dst->wCanCastleKingside = Src->wCanCastleKingside;
  Dst->wCanCastleQueenside = Src->wCanCastleQueenside;
  Dst->bCanCastleKingside = Src->bCanCastleKingside;
  Dst->bCanCastleQueenside = Src->bCanCastleQueenside;
  Dst->EnPassantTarget = Src->EnPassantTarget;
  Dst->HalfMoves = Src->HalfMoves;
  Dst->FullMoves = Src->FullMoves;
};

bool32 CompareBoardState(board_state* Src, board_state* Dst) {
  bool32 result = true;
  
  result = result && (Dst->WhiteToMove == Src->WhiteToMove);
  result = result && (Dst->wCanCastleKingside == Src->wCanCastleKingside);
  result = result && (Dst->wCanCastleQueenside == Src->wCanCastleQueenside);
  result = result && (Dst->bCanCastleKingside == Src->bCanCastleKingside);
  result = result && (Dst->bCanCastleQueenside == Src->bCanCastleQueenside);
  result = result && (Dst->EnPassantTarget == Src->EnPassantTarget);
  result = result && (Dst->HalfMoves == Src->HalfMoves);
  result = result && (Dst->FullMoves == Src->FullMoves);

  if (!result) { PrintBoardState(*Src, "Src"); PrintBoardState(*Dst, "Dst"); }

  return result;
};

void RunTests() {
  TEST_SetBit();
  TEST_ClearBit();
  TEST_ToggleBit();
  TEST_CheckBit();
  TEST_Equal();
  TEST_PopulateBoardFromFEN();
  TEST_SideToMoveFromFEN();
  TEST_CastlingAvailabilityFromFEN();
  TEST_EnPassantFromFEN();
  TEST_MoveCountFromFEN();
  TEST_FENToBitboards();
  TEST_BitScanForward();
  TEST_BitScanReverse();
  TEST_PieceIsType();
  TEST_PopCount();

  TEST_Moves();
}

/*
uint64 Perft(chess_state* State, int depth) {
  move MoveList[256];
  int NMoves = 0;
  uint64 nodes = 0;

  if (depth <= 0) { return 1ULL; }

  bool32 WhiteToMove = (depth % 2 == 1);

  NMoves = GeneratePseudoLegalMoves(State->Bitboards, WhiteToMove, State->BoardState.EnPassantTarget, MoveList, &State->Mailbox);
  for (int i = 0; i < NMoves; i++) {
    bitboard DEBUG_STATE1[16] = {};
    CopyBitboards(State->Bitboards, DEBUG_STATE1);
    
    MakeMove(State, MoveList[i]);
    PopulateMailbox(&State->Bitboards[0], &State->Mailbox);
    if (!KingIsInCheck(State, !WhiteToMove)) { // if the side that just moved isnt in check
      nodes += Perft(State, depth - 1);
    }
    UnmakeMove(State, MoveList[i]);
    PopulateMailbox(&State->Bitboards[0], &State->Mailbox);
    //CompareBitboards(State->Bitboards, DEBUG_STATE1, depth, nodes, MoveList[i]);
  }
  return nodes;
}
*/

uint64 PerftDivide(chess_state* S, int StartingDepth, int Depth) {
  move MoveList[256] = {};
  uint64 MoveCount[256] = {};
  int NMoves = 0;
  uint64 nodes = 0;

  if (Depth <= 0) { return 1ULL; }

  NMoves = GeneratePseudoLegalMoves(S->Bitboards, S->BoardState.WhiteToMove, S->BoardState.EnPassantTarget, MoveList, &S->Mailbox, Depth);
  
  for (int i = 0; i < NMoves; i++) {    
    MakeMove(S, &MoveList[i]);
    PopulateMailbox(&S->Bitboards[0], &S->Mailbox);
    
    if (!KingIsInCheck(S, !S->BoardState.WhiteToMove)) { // if the side that just moved isnt in check
      MoveCount[i] = PerftDivide(S, StartingDepth, Depth - 1);
      nodes += MoveCount[i];
    }
    
    UnmakeMove(S, MoveList[i]);
    PopulateMailbox(&S->Bitboards[0], &S->Mailbox);
  }
  
  if (Depth == StartingDepth) {
    for (int i = 0; i < NMoves; i++) {
      printf("%s%s %llu moves\n", SquareLookup[MoveList[i].Origin],
                                  SquareLookup[MoveList[i].Destination], MoveCount[i]);
    }
  }
  
  return nodes;
}

uint64 PerftDivide(chess_state* S, int Depth) {
  return PerftDivide(S, Depth, Depth);
}

extern "C"
void UpdateAndRender(input* Input, memory* Memory, back_buffer* Buffer) {
  Assert(sizeof(chess_state) <= Memory->BlockSize);
  chess_state* State = (chess_state*)Memory->Block;
  
  if (!Memory->IsInitialized) {
    RunTests();
    LoadTextures(State, Memory, Buffer->Width);
    
    char* StartingFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    FENToBitboards(StartingFEN, &State->Bitboards[0], &State->BoardState);
    PopulateMailbox(&State->Bitboards[0], &State->Mailbox);

    Memory->IsInitialized = true;
    
    printf("Perft %i | %llu\n", 4, PerftDivide(State, 4));
  }
  
  int MouseX = Input->MouseX;
  int MouseY = Input->MouseY;
  bool32 LMB_pressed = Input->Buttons[0].EndedDown && Input->Buttons[0].JustTransitioned;
  bool32 LMB_released = (!Input->Buttons[0].EndedDown) && Input->Buttons[0].JustTransitioned;

  ClearScreen(Buffer, 0x00000000);

  if (LMB_pressed && WithinDims(MouseX, MouseY, State->UndoButtonDims)) {
    //move M = UndoMove();
    //UnmakeMove(State, M);
    PopulateMailbox(&State->Bitboards[0], &State->Mailbox);
  }

  if (LMB_pressed && !State->DragMode &&
      WithinDims(MouseX, MouseY, State->BoardTexDims)) {
    int BoardX = ((MouseX - State->BoardTexDims.MinX) / State->BoardTexPxVals.BoardSquare);
    int BoardY = ((MouseY - State->BoardTexDims.MinY) / State->BoardTexPxVals.BoardSquare);
    int BoardCoord = (8 * (7 - BoardY)) + (BoardX);
    State->DraggedPiece = State->Mailbox.Squares[BoardCoord];
    if (PieceBelongsToPlayer(State->DraggedPiece, State->BoardState.WhiteToMove)) {
      BeginDragMode(State, BoardCoord);
    }
  }

  if (LMB_released && State->DragMode) {
    int BoardX = ((MouseX - State->BoardTexDims.MinX) / State->BoardTexPxVals.BoardSquare);
    int BoardY = ((MouseY - State->BoardTexDims.MinY) / State->BoardTexPxVals.BoardSquare);
    int BoardCoord = (8 * (7 - BoardY)) + (BoardX);
    if ((CheckBit(State->DraggedPieceLegalMoves, BoardCoord) ||
	 CheckBit(State->DraggedPieceLegalAttacks, BoardCoord)) &&
	WithinDims(MouseX, MouseY, State->BoardTexDims)) {
      int Origin = State->DraggedPieceOrigin;
      int Destination = BoardCoord;
      piece PieceAtDestination = State->Mailbox.Squares[BoardCoord];
      
      move M = Move(State->DraggedPiece, PieceAtDestination, Origin, Destination);
      MakeMove(State, &M);
      if (KingIsInCheck(State, !State->BoardState.WhiteToMove)) {
        UnmakeMove(State, M);
      }
      
      State->DraggedPieceOrigin = 0;
    } else {
      ResetDraggedPiece(State);
    }

    EndDragMode(State);
    PopulateMailbox(&State->Bitboards[0], &State->Mailbox);
  }

  DrawTexture(Buffer, State->BoardTex, State->BoardTexPxVals.CenteredX, 0, State->TextureScale);
  //DrawSquareOverlay(Buffer, State->Bitboards[EMPTY_SQ], State->TextureScale, 0x00AAAA00);
  DrawSquareOverlay(Buffer, State->DraggedPieceLegalAttacks, State->TextureScale, 0x00FF0000);
  DrawSquareOverlay(Buffer, State->DraggedPieceLegalMoves, State->TextureScale, 0x0000FF00);

  DrawPieces(Buffer, State, &State->Mailbox, State->TextureScale);

  DrawRectangle(Buffer, State->UndoButtonDims.MinX, State->UndoButtonDims.MinY, State->UndoButtonDims.MaxX, State->UndoButtonDims.MaxY, 0x00FF0000);
  
  if (State->DragMode) {
    DrawPiece(Buffer, State, State->DraggedPiece, MouseX - (State->BoardTexPxVals.BoardSquare / 2), MouseY - (State->BoardTexPxVals.BoardSquare / 2), State->TextureScale);
  }
}
