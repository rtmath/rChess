#include <stdio.h>
#include "rchess.h"
#include "bmp.h"
#include "graphics.h"
#include "bitboard.h"
#include "tests/test_bitboard.h"
#include "tests/test_fen.h"

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
    TEST_FENToBoard();
    
    LoadBigTextureFromBMP("../assets/board.bmp", Memory, &State->BoardTexture);
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

    char* starting_position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    FENToBoard(starting_position, &State->Board);

    static int scale = 3;
    static int centered_board_x = (Buffer->Width / 2) - (State->BoardTexture.Width * scale / 2);
    DrawTexture(Buffer, State->BoardTexture, centered_board_x, 0, scale, scale);

    DrawPieces(Buffer, State, &State->Board, scale); // Don't need to draw pieces every frame, sticking this here for the time being
    
    Memory->IsInitialized = true;
  }


}
