#include <stdio.h>
#include "rchess.h"
#include "bmp.h"
#include "graphics.h"
#include "bitboard.h"
#include "undo.h"
#include "tests/test_bitboard.h"
#include "tests/test_fen.h"

// TODO:
//   Detect if King is in check
//   Pawn Promotions
//   En Passant pawn captures

inline bool32
WithinDims(int x, int y, dims Dims) {
  return ((x >= Dims.MinX) && (y >= Dims.MinY) &&
	  (x <= Dims.MaxX) && (y <= Dims.MaxY));
};

inline bool32
PieceBelongsToPlayer(piece P, bool32 WhiteToMove) {
  if (P == EMPTY) { return false; }
  return ((P & W_MASK) > 0) == WhiteToMove;
};

extern "C" void UpdateAndRender(input* Input, memory* Memory, back_buffer* Buffer) {
  Assert(sizeof(chess_state) <= Memory->BlockSize);
  chess_state* State = (chess_state*)Memory->Block;
  
  if (!Memory->IsInitialized) {
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
    State->BoardTexPxVals.CenteredX     = (Buffer->Width / 2) - (State->BoardTex.Width * State->TextureScale / 2);
    
    State->BoardTexDims.MinX = State->BoardTexPxVals.CenteredX + State->BoardTexPxVals.InnerPadding;
    State->BoardTexDims.MaxX = State->BoardTexDims.MinX + (176 * State->TextureScale);
    State->BoardTexDims.MinY = State->BoardTexPxVals.InnerPadding;
    State->BoardTexDims.MaxY = State->BoardTexPxVals.TotalTexture - State->BoardTexPxVals.InnerPadding;

    State->UndoButtonDims.MinX = 10;
    State->UndoButtonDims.MinY = 10;
    State->UndoButtonDims.MaxX = 100;
    State->UndoButtonDims.MaxY = 100;
    
    char* starting_position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    FENToBitboards(starting_position, &State->Bitboards[0], &State->BoardState);
    PopulateMailbox(&State->Bitboards[0], &State->Mailbox);

    Memory->IsInitialized = true;
  }
  
  int MouseX = Input->MouseX;
  int MouseY = Input->MouseY;
  bool32 LMB_pressed = Input->Buttons[0].EndedDown && Input->Buttons[0].JustTransitioned;
  bool32 LMB_released = (!Input->Buttons[0].EndedDown) && Input->Buttons[0].JustTransitioned;
  
  ClearScreen(Buffer, 0x00000000);

  if (LMB_pressed && WithinDims(MouseX, MouseY, State->UndoButtonDims)) {
    move M = UndoMove();
    UpdateStateFromUndo(State, M);
  }

  if (LMB_pressed && !State->DragMode &&
      WithinDims(MouseX, MouseY, State->BoardTexDims)) {
    int BoardX = ((MouseX - State->BoardTexDims.MinX) / State->BoardTexPxVals.BoardSquare);
    int BoardY = ((MouseY - State->BoardTexDims.MinY) / State->BoardTexPxVals.BoardSquare);
    int BoardCoord = (8 * (7 - BoardY)) + (BoardX);
    State->DraggedPiece = State->Mailbox.Squares[BoardCoord];
    if (PieceBelongsToPlayer(State->DraggedPiece, State->BoardState.WhiteToMove)) {
      State->DraggedPieceOrigin = BoardCoord;
      State->Mailbox.Squares[BoardCoord] = EMPTY;
      State->DraggedPieceLegalMoves = PseudoLegalMoves(State->DraggedPiece, BoardCoord, State->Bitboards[EMPTY_SQ]);
      bitboard WhiteOccupation = State->Bitboards[wPAWN] | State->Bitboards[wBISHOP] | State->Bitboards[wKNIGHT] | State->Bitboards[wROOK] | State->Bitboards[wQUEEN] | State->Bitboards[wKING];
      bitboard BlackOccupation = State->Bitboards[bPAWN] | State->Bitboards[bBISHOP] | State->Bitboards[bKNIGHT] | State->Bitboards[bROOK] | State->Bitboards[bQUEEN] | State->Bitboards[bKING];
      State->DraggedPieceLegalAttacks = PseudoLegalAttacks(State->DraggedPiece, BoardCoord, State->Bitboards[EMPTY_SQ], (State->BoardState.WhiteToMove) ? BlackOccupation : WhiteOccupation );
      State->DragMode = true;
    }
  }

  if (LMB_released && State->DragMode) {
    int BoardX = ((MouseX - State->BoardTexDims.MinX) / State->BoardTexPxVals.BoardSquare);
    int BoardY = ((MouseY - State->BoardTexDims.MinY) / State->BoardTexPxVals.BoardSquare);
    int BoardCoord = (8 * (7 - BoardY)) + (BoardX);
    if ((CheckBit(State->DraggedPieceLegalMoves, BoardCoord) ||
	 CheckBit(State->DraggedPieceLegalAttacks, BoardCoord)) &&
	WithinDims(MouseX, MouseY, State->BoardTexDims)) {
      int origin = State->DraggedPieceOrigin;
      int destination = BoardCoord;
      piece piece_at_destination = State->Mailbox.Squares[BoardCoord];
      
      ClearPiece(&State->Bitboards[piece_at_destination], BoardCoord);
      UpdateBitboard(&State->Bitboards[0], State->DraggedPiece, origin, destination);
      MovePiece(&State->Bitboards[EMPTY_SQ], destination, origin); // Inverse of moving a piece
      MovePiece(&State->Bitboards[OCCUP_SQ], origin, destination);

      move M = {};
      M.MovedPiece = State->DraggedPiece;
      M.CapturedPiece = piece_at_destination;
      M.Origin = origin;
      M.Destination = destination;
      StoreMove(M);
      
      State->Mailbox.Squares[BoardCoord] = State->DraggedPiece;
      State->DraggedPieceOrigin = 0;
      State->BoardState.WhiteToMove = !State->BoardState.WhiteToMove;
    } else {
      State->Mailbox.Squares[State->DraggedPieceOrigin] = State->DraggedPiece;
    }

    State->DragMode = false;
    State->DraggedPieceOrigin = 0;
    State->DraggedPiece = EMPTY;
    State->DraggedPieceLegalMoves = 0;
    State->DraggedPieceLegalAttacks = 0;
  }

  DrawTexture(Buffer, State->BoardTex, State->BoardTexPxVals.CenteredX, 0, State->TextureScale);
  DrawSquareOverlay(Buffer, State->DraggedPieceLegalAttacks, State->TextureScale, 0x00FF0000);  
  DrawSquareOverlay(Buffer, State->Bitboards[EMPTY_SQ], State->TextureScale, 0x00AAAA00);
  DrawSquareOverlay(Buffer, State->DraggedPieceLegalMoves, State->TextureScale, 0x0000FF00);

  DrawPieces(Buffer, State, &State->Mailbox, State->TextureScale);

  DrawRectangle(Buffer, State->UndoButtonDims.MinX, State->UndoButtonDims.MinY, State->UndoButtonDims.MaxX, State->UndoButtonDims.MaxY, 0x00FF0000);
  
  if (State->DragMode) {
    DrawPiece(Buffer, State, State->DraggedPiece, MouseX - (State->BoardTexPxVals.BoardSquare / 2), MouseY - (State->BoardTexPxVals.BoardSquare / 2), State->TextureScale);
  }
}
