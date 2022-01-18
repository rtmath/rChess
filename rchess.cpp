#include "rchess.h"
#include "bmp.h"
#include "graphics.h"

extern "C" void UpdateAndRender(input* Input, memory* Memory, back_buffer* Buffer) {
  Assert(sizeof(chess_state) <= Memory->BlockSize);
  chess_state* State = (chess_state*)Memory->Block;
  
  if (!Memory->IsInitialized) {
    LoadBigTextureFromBMP("../assets/board.bmp", Memory, &State->Board);
    LoadTextureFromBMP("../assets/black_bishop.bmp", Memory, &State->BlackBishop);
    
    LoadTextureFromBMP("../assets/black_king.bmp", Memory, &State->BlackKing);
    LoadTextureFromBMP("../assets/black_knight.bmp", Memory, &State->BlackKnight);
    LoadTextureFromBMP("../assets/black_pawn.bmp", Memory, &State->BlackPawn);
    LoadTextureFromBMP("../assets/black_queen.bmp", Memory, &State->BlackQueen);
    LoadTextureFromBMP("../assets/black_rook.bmp", Memory, &State->BlackRook);
    LoadTextureFromBMP("../assets/white_bishop.bmp", Memory, &State->WhiteBishop);
    LoadTextureFromBMP("../assets/white_king.bmp", Memory, &State->WhiteKing);
    LoadTextureFromBMP("../assets/white_knight.bmp", Memory, &State->WhiteKnight);
    LoadTextureFromBMP("../assets/white_pawn.bmp", Memory, &State->WhitePawn);
    LoadTextureFromBMP("../assets/white_queen.bmp", Memory, &State->WhiteQueen);
    LoadTextureFromBMP("../assets/white_rook.bmp", Memory, &State->WhiteRook);
    
    Memory->IsInitialized = true;
  }

  static int scale = 3;
  static int centered_board_x = (Buffer->Width / 2) - (State->Board.Width * scale / 2);
  DrawTexture(Buffer, State->Board, centered_board_x, 0, scale, scale);

  DrawTexture(Buffer, State->BlackBishop, 500, 0, scale);
  DrawTexture(Buffer, State->BlackKing, 500, 100, scale);
  DrawTexture(Buffer, State->BlackKnight, 500, 200, scale);
  DrawTexture(Buffer, State->BlackPawn, 500, 300, scale);
  DrawTexture(Buffer, State->BlackQueen, 500, 400, scale);
  DrawTexture(Buffer, State->BlackRook, 500, 500, scale);
  DrawTexture(Buffer, State->WhiteBishop, 700, 0, scale);
  DrawTexture(Buffer, State->WhiteKing, 700, 100, scale);
  DrawTexture(Buffer, State->WhiteKnight, 700, 200, scale);
  DrawTexture(Buffer, State->WhitePawn, 700, 300, scale);
  DrawTexture(Buffer, State->WhiteQueen, 700, 400, scale);
  DrawTexture(Buffer, State->WhiteRook, 700, 500, scale);
}
