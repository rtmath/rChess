#include <stdio.h>
#include "rchess.h"
#include "bmp.h"
#include "graphics.h"
#include "bitboard.h"
#include "undo.h"
#include "tests/test_bitboard.h"
#include "tests/test_fen.h"

// TODO: Castling | check that SideCanCastle and SquaresArentAttacked and SquaresAreEmpty

inline uint8
CalculateEnPassantTarget(piece P, uint8 POrigin, uint8 PDestination) {
  if (!(P == wPAWN || P == bPAWN)) { return 0; }
  if ((PDestination - POrigin) == 16) { return PDestination - 8; }
  if ((POrigin - PDestination) == 16) { return PDestination + 8; }
  return 0;
}

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

inline uint8
Rank(int n) {
  return n / 8;
}

inline uint8
File(int n) {
  return n % 8;
}

inline void
ChangeTurn(chess_state* S) {
  S->BoardState.WhiteToMove = !(S->BoardState.WhiteToMove);
}

bitboard MovesForPieceOnSquare(piece Type, uint8 BoardCoord, bitboard Pieces[], bool32 WhiteToMove) {
  bitboard PseudoQuiets   = PseudoLegalMoves(Type, BoardCoord, Pieces[EMPTY_SQ]);
  bitboard PseudoAttacks  = PseudoLegalAttacks(Type, BoardCoord, Pieces[EMPTY_SQ], (WhiteToMove) ? BlackOccupation(Pieces) : WhiteOccupation(Pieces) );
  return PseudoQuiets | PseudoAttacks;
}

inline bool32
NotEmpty(bitboard BB) {
  return BB > 0;
}

inline bool32
PieceAtLowestBit(bitboard BB) {
  return (BB & 0x1) > 0;
}

int GeneratePseudoLegalMoves(bitboard Pieces[], bool32 WhiteToMove, move Moves[], mailbox* Mailbox) {
  int MCounter = 0;
  for (piece P = PAWN; P <= KING; P++) {
    piece Piece = (WhiteToMove) ? (P | W_MASK) : P;
    bitboard BB = Pieces[Piece];

    uint8 Origin = 0;
    while (NotEmpty(BB)) {
      if (PieceAtLowestBit(BB)) {
	bitboard Mvs = MovesForPieceOnSquare(Piece, Origin, Pieces, WhiteToMove);
	
	uint8 Destination = 0;
	while (NotEmpty(Mvs)) {
	  if (PieceAtLowestBit(Mvs)) {
	    move M = {};
	    M.Origin = Origin;
	    M.Destination = Destination;
	    M.PieceType = Piece;
	    M.DestPieceType = Mailbox->Squares[Destination];

	    Moves[MCounter] = M;
	    MCounter++;
	  }
	  Destination++;
	  Mvs >>= 1;
	}
      }
      Origin++;
      BB >>= 1;
    }
  }
  return MCounter;
}

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

move Move(piece Type, piece DestType, uint8 Origin, uint8 Destination) {
  move M = {};
  M.PieceType = Type;
  M.DestPieceType = DestType;
  M.Origin = Origin;
  M.Destination = Destination;
  return M;
}

void MakeMove(chess_state* S, move M) {
  piece PieceType = M.PieceType;
  int Origin = M.Origin;
  int Destination = M.Destination;
  piece DestPieceType = M.DestPieceType;
  M.DestPieceLoc = Destination;

  bitboard* BBs = &S->Bitboards[0];
  
  ClearPiece(BBs, DestPieceType, Destination);
  MovePiece(BBs, PieceType, Origin, Destination);

  int CurrentEPTarget = S->BoardState.EnPassantTarget;
  if (EnPassant(Destination, CurrentEPTarget)) {
    if (CurrentEPTarget > H5) {
      M.DestPieceLoc = Destination - 8;
      M.DestPieceType = bPAWN;
      ClearPiece(&BBs[bPAWN], M.DestPieceLoc);
    } else {
      M.DestPieceLoc = Destination + 8;
      M.DestPieceType = wPAWN;
      ClearPiece(&BBs[wPAWN], M.DestPieceLoc);
    }
  }

  if (Promotion(PieceType, Destination)) {
    ClearPiece(&BBs[PieceType], Destination);
    // For now, assume promotion will promote to Q
    piece PromoPiece = (IsWhite(PieceType)) ? wQUEEN : bQUEEN;
    SetPiece(&BBs[PromoPiece], Destination);
    M.PromoPieceType = PromoPiece;
  }

  uint8 NewEPTarget = CalculateEnPassantTarget(PieceType, Origin, Destination);
  M.PrevEPTarget = CurrentEPTarget;
  M.NewEPTarget = NewEPTarget;
  S->BoardState.EnPassantTarget = NewEPTarget;

  //StoreMove(M);

  S->Bitboards[OCCUP_SQ] = CalculateOccupation(S->Bitboards);
  S->Bitboards[EMPTY_SQ] = ~S->Bitboards[OCCUP_SQ];

  ChangeTurn(S);
}

void UnmakeMove(chess_state* S, move M) {
  piece PieceType = M.PieceType;
  int Origin = M.Origin;
  int Destination = M.Destination;
  piece DestPieceType = M.DestPieceType;

  bitboard* BBs = &S->Bitboards[0];

  MovePiece(BBs, PieceType, Destination, Origin); // replace the moved piece
  SetPiece(BBs, DestPieceType, M.DestPieceLoc);   // replace a potentially capped piece

  if (M.PromoPieceType != EMPTY) {
    ClearPiece(BBs, M.PromoPieceType, M.DestPieceLoc);
  }

  S->BoardState.EnPassantTarget = M.PrevEPTarget;

  S->Bitboards[OCCUP_SQ] = CalculateOccupation(S->Bitboards);
  S->Bitboards[EMPTY_SQ] = ~S->Bitboards[OCCUP_SQ];

  ChangeTurn(S);
}

uint64 Perft(chess_state* State, int depth) {
  move MoveList[256];
  int NMoves = 0;
  uint64 nodes = 0;

  if (depth <= 0) { return 1ULL; }

  bool32 WhiteToMove = (depth % 2 == 1);

  NMoves = GeneratePseudoLegalMoves(State->Bitboards, WhiteToMove, MoveList, &State->Mailbox);
  for (int i = 0; i < NMoves; i++) {
    MakeMove(State, MoveList[i]);
    if (!KingIsInCheck(State, !WhiteToMove)) { // if the side that just moved isnt in check
      nodes += Perft(State, depth - 1);
    }
    UnmakeMove(State, MoveList[i]);
  }
  return nodes;
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

    for (int i = 0; i < 4; i++) {
      printf("Perft %i | %llu\n", i, Perft(State, i));
    }
  }
  
  int MouseX = Input->MouseX;
  int MouseY = Input->MouseY;
  bool32 LMB_pressed = Input->Buttons[0].EndedDown && Input->Buttons[0].JustTransitioned;
  bool32 LMB_released = (!Input->Buttons[0].EndedDown) && Input->Buttons[0].JustTransitioned;

  ClearScreen(Buffer, 0x00000000);

  if (LMB_pressed && WithinDims(MouseX, MouseY, State->UndoButtonDims)) {
    move M = UndoMove();
    UnmakeMove(State, M);
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
      MakeMove(State, M);
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
