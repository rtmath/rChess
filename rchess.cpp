#include <stdio.h>
#include "rchess.h"
#include "bmp.h"
#include "graphics.h"
#include "bitboard.h"
#include "undo.h"
#include "tests/test_bitboard.h"
#include "tests/test_fen.h"

//   En Passant
//     ) Bug: I seem to lose EP target after undoing a move
//     ) Bug: Undoing a move doesnt restore the captured piece after an EP capture

inline uint8 CalculateEnPassantTarget(piece P, uint8 POrigin, uint8 PDestination) {
  if (!(P == wPAWN || P == bPAWN)) { return 0; }
  if ((PDestination - POrigin) == 16) { return PDestination - 8; }
  if ((POrigin - PDestination) == 16) { return PDestination + 8; }
  return 0;
}

inline bool32
WithinDims(int x, int y, dims Dims) {
  return ((x >= Dims.MinX) && (y >= Dims.MinY) &&
	  (x <= Dims.MaxX) && (y <= Dims.MaxY));
};

inline bool32
PieceBelongsToPlayer(piece P, bool32 WhiteToMove) {
  if (P == EMPTY) { return false; }
  return IsWhite(P) == WhiteToMove;
};

inline uint8 Rank(int n) {
  return n / 8;
};

inline uint8 File(int n) {
  return n % 8;
};

void BeginDragMode(chess_state* State, uint8 BoardCoord) {
  State->DraggedPieceOrigin = BoardCoord;
  State->Mailbox.Squares[BoardCoord] = EMPTY;
  State->DraggedPieceLegalMoves = PseudoLegalMoves(State->DraggedPiece, BoardCoord, State->Bitboards[EMPTY_SQ]);
  State->DraggedPieceLegalAttacks = PseudoLegalAttacks(State->DraggedPiece, BoardCoord, State->Bitboards[EMPTY_SQ], (State->BoardState.WhiteToMove) ? BlackOccupation(State) : WhiteOccupation(State) );
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

inline bool32 Promotion(piece P, uint8 Destination) {
 return (PieceIsType(P, PAWN) &&
	 (Rank(Destination) == 7 || Rank(Destination) == 0));
}

inline bool32 EnPassant(uint8 Destination, uint8 EPTarget) {
  return (Destination == EPTarget) && (EPTarget != EMPTY);
}

extern "C" void UpdateAndRender(input* Input, memory* Memory, back_buffer* Buffer) {
  Assert(sizeof(chess_state) <= Memory->BlockSize);
  chess_state* State = (chess_state*)Memory->Block;
  
  if (!Memory->IsInitialized) {
    RunTests();
    LoadTextures(State, Memory, Buffer->Width);
    
    char* StartingFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    FENToBitboards(StartingFEN, &State->Bitboards[0], &State->BoardState);
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
    PrintMove(M);
    UpdateStateFromUndo(State, M);
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

      ClearPiece(&State->Bitboards[PieceAtDestination], BoardCoord);
      MovePiece(&State->Bitboards[State->DraggedPiece], Origin, Destination);
      
      int EPTarget = State->BoardState.EnPassantTarget;
      if (EnPassant(Destination, EPTarget)) {
	if (EPTarget > H5) {
          ClearPiece(&State->Bitboards[bPAWN], Destination - 8);
	} else {
	  ClearPiece(&State->Bitboards[wPAWN], Destination + 8);
	}
      }
      
      if (Promotion(State->DraggedPiece, Destination)) {
	ClearPiece(&State->Bitboards[State->DraggedPiece], Destination);
        // For now, assume promotion will promote to Q
        SetPiece( (IsWhite(State->DraggedPiece)) ? &State->Bitboards[wQUEEN] : &State->Bitboards[bQUEEN], Destination);
      }

      uint8 NewEPTarget = CalculateEnPassantTarget(State->DraggedPiece, Origin, Destination);
      
      move M = {};
      M.MovedPiece = State->DraggedPiece;
      M.CapturedPiece = PieceAtDestination;
      M.Origin = Origin;
      M.Destination = Destination;
      M.EnPassantTarget = NewEPTarget;
      StoreMove(M);

      State->Bitboards[OCCUP_SQ] = CalculateOccupation(State->Bitboards);
      State->Bitboards[EMPTY_SQ] = ~State->Bitboards[OCCUP_SQ];
      
      State->DraggedPieceOrigin = 0;
      State->BoardState.WhiteToMove = !State->BoardState.WhiteToMove;
      State->BoardState.EnPassantTarget = NewEPTarget;
    } else {
      ResetDraggedPiece(State);
    }

    EndDragMode(State);
    PopulateMailbox(&State->Bitboards[0], &State->Mailbox);
  }

  DrawTexture(Buffer, State->BoardTex, State->BoardTexPxVals.CenteredX, 0, State->TextureScale);
  DrawSquareOverlay(Buffer, State->Bitboards[EMPTY_SQ], State->TextureScale, 0x00AAAA00);
  DrawSquareOverlay(Buffer, State->DraggedPieceLegalAttacks, State->TextureScale, 0x00FF0000);
  DrawSquareOverlay(Buffer, State->DraggedPieceLegalMoves, State->TextureScale, 0x0000FF00);

  DrawPieces(Buffer, State, &State->Mailbox, State->TextureScale);

  DrawRectangle(Buffer, State->UndoButtonDims.MinX, State->UndoButtonDims.MinY, State->UndoButtonDims.MaxX, State->UndoButtonDims.MaxY, 0x00FF0000);
  
  if (State->DragMode) {
    DrawPiece(Buffer, State, State->DraggedPiece, MouseX - (State->BoardTexPxVals.BoardSquare / 2), MouseY - (State->BoardTexPxVals.BoardSquare / 2), State->TextureScale);
  }
}
