#ifndef UI_HELPERS_H

#include "rchess.h"
#include "moves.h"

void BeginDragMode(chess_state* State, uint8 BoardCoord) {
  State->DraggedPieceOrigin = BoardCoord;
  State->Mailbox.Squares[BoardCoord] = EMPTY;
  State->DraggedPieceLegalMoves = PseudoLegalMoves(State->DraggedPiece, BoardCoord, State->Bitboards[EMPTY_SQ]);
  State->DraggedPieceLegalAttacks = PseudoLegalAttacks(State->DraggedPiece, BoardCoord, State->Bitboards[EMPTY_SQ], (State->BoardState.WhiteToMove) ? BlackOccupation(State->Bitboards) : WhiteOccupation(State->Bitboards) );
  uint8 EPTarget = State->BoardState.EnPassantTarget;
  bitboard EPAttacks = EnPassantAttacks(State->DraggedPiece, BoardCoord, EPTarget);
  State->DraggedPieceLegalAttacks |= EPAttacks;
  State->DragMode = true;
}

void ResetDraggedPiece(chess_state* State) {
  State->Mailbox.Squares[State->DraggedPieceOrigin] = State->DraggedPiece;
}

void EndDragMode(chess_state* State) {
  State->DragMode = false;
  State->DraggedPieceOrigin = 0;
  State->DraggedPiece = EMPTY;
  State->DraggedPieceLegalMoves = 0;
  State->DraggedPieceLegalAttacks = 0;
}

void LoadTextures(chess_state* State, memory* Memory, int BufferWidth) {
  LoadBigTextureFromBMP("../assets/board.bmp",     Memory, &State->BoardTex);
  LoadTextureFromBMP("../assets/black_pawn.bmp",   Memory, &State->Pieces[bPAWN]);
  LoadTextureFromBMP("../assets/black_bishop.bmp", Memory, &State->Pieces[bBISHOP]);
  LoadTextureFromBMP("../assets/black_knight.bmp", Memory, &State->Pieces[bKNIGHT]);
  LoadTextureFromBMP("../assets/black_rook.bmp",   Memory, &State->Pieces[bROOK]);
  LoadTextureFromBMP("../assets/black_queen.bmp",  Memory, &State->Pieces[bQUEEN]);
  LoadTextureFromBMP("../assets/black_king.bmp",   Memory, &State->Pieces[bKING]);
  LoadTextureFromBMP("../assets/white_pawn.bmp",   Memory, &State->Pieces[wPAWN]);
  LoadTextureFromBMP("../assets/white_bishop.bmp", Memory, &State->Pieces[wBISHOP]);
  LoadTextureFromBMP("../assets/white_knight.bmp", Memory, &State->Pieces[wKNIGHT]);
  LoadTextureFromBMP("../assets/white_rook.bmp",   Memory, &State->Pieces[wROOK]);
  LoadTextureFromBMP("../assets/white_queen.bmp",  Memory, &State->Pieces[wQUEEN]);
  LoadTextureFromBMP("../assets/white_king.bmp",   Memory, &State->Pieces[wKING]);

  State->TextureScale = 3;
    
  // The following board/texture values are hardcoded for the specific Board and Piece textures I am using.
  // The Board texture is 256x256 pixels and each piece is 20x20 pixels
  // The actual board in the texture is only 176x176, the other pixels are decoration
  State->BoardTexPxVals.TotalTexture  = 256 * State->TextureScale;
  State->BoardTexPxVals.InnerPadding  = ((256 - 176) * State->TextureScale) / 2;
  State->BoardTexPxVals.PieceTexture  = 20 * State->TextureScale;
  State->BoardTexPxVals.BoardSquare   = 22 * State->TextureScale;
  State->BoardTexPxVals.SquarePadding = ((22 - 20) * State->TextureScale) / 2;
  State->BoardTexPxVals.CenteredX     = (BufferWidth / 2) - (State->BoardTex.Width * State->TextureScale / 2);
    
  State->BoardTexDims.MinX = State->BoardTexPxVals.CenteredX + State->BoardTexPxVals.InnerPadding;
  State->BoardTexDims.MaxX = State->BoardTexDims.MinX + (176 * State->TextureScale);
  State->BoardTexDims.MinY = State->BoardTexPxVals.InnerPadding;
  State->BoardTexDims.MaxY = State->BoardTexPxVals.TotalTexture - State->BoardTexPxVals.InnerPadding;

  State->UndoButtonDims.MinX = 10;
  State->UndoButtonDims.MinY = 10;
  State->UndoButtonDims.MaxX = 100;
  State->UndoButtonDims.MaxY = 100;
}

#define UI_HELPERS_H
#endif
