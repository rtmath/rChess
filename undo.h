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

#define UNDO_H
#endif
