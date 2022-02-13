#ifndef UNDO_H
#include "rchess.h"

struct undo_stack {
  uint8 Index;
  move  Moves[256];
};

global_var undo_stack UndoStack = {};

void StoreMove(move M) {
  UndoStack.Moves[UndoStack.Index++] = M;
}

move UndoMove() {
  if (UndoStack.Index == 0) { return {}; }
  return UndoStack.Moves[--UndoStack.Index];
}

void UpdateStateFromUndo(chess_state* State, move Undo) {
  if (Undo.wLostCastleKingside) { State->BoardState.wCanCastleKingside = true; }
  if (Undo.wLostCastleQueenside) { State->BoardState.wCanCastleQueenside = true; }
  if (Undo.bLostCastleKingside) { State->BoardState.bCanCastleKingside = true; }
  if (Undo.bLostCastleQueenside) { State->BoardState.bCanCastleQueenside = true; }
  MovePiece(State->Bitboards, Undo.MovedPiece, Undo.Destination, Undo.Origin);
  SetPiece(State->Bitboards, Undo.CapturedPiece, Undo.Destination);
  State->Bitboards[OCCUP_SQ] = CalculateOccupation(State->Bitboards);
  State->Bitboards[EMPTY_SQ] = ~State->Bitboards[OCCUP_SQ];
  PopulateMailbox(&State->Bitboards[0], &State->Mailbox);
  State->BoardState.WhiteToMove = !State->BoardState.WhiteToMove;
  State->BoardState.EnPassantTarget = Undo.EnPassantTarget;
  // TODO: Halfmoves, Fullmoves
}

#define UNDO_H
#endif
