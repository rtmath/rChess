#ifndef GRAPHICS_H
#include "rchess.h"

#define ALPHA_MASK 0xffbad0

internal void
DrawTexture(back_buffer* Buffer, texture Texture,
	    int PosX, int PosY,
	    int ScaleX, int ScaleY) {
  // Assumes textures to be of same Width and Height
  int min_x = PosX;
  int min_y = PosY;
  int max_x = PosX + (Texture.Width  * ScaleX);
  int max_y = PosY + (Texture.Height * ScaleY);
  if (min_x < 0) { min_x = 0; }
  if (min_y < 0) { min_y = 0; }
  if (max_x > Buffer->Width)  { max_x = Buffer->Width; }
  if (max_y > Buffer->Height) { max_y = Buffer->Height; }
  
  uint8* Row = ((uint8*)Buffer->Memory + (min_y * Buffer->Pitch) + (min_x * Buffer->BytesPerPixel));

  int byte_size = 3;
  int tex_x = -1;
  int tex_y = -1;
  for (int y = min_y; y < max_y; ++y) {
    uint32* Pixel = (uint32*)Row;
    tex_y++;
    
    for (int x = min_x; x < max_x; ++x) {
      tex_x++;

      int pixel_data_index = (((tex_y / ScaleY) * Texture.Width * byte_size) + ((tex_x / ScaleX) * byte_size));
      byte* pixel_color = &Texture.Pixels[pixel_data_index];
      byte red   = *(pixel_color++);
      byte blue  = *(pixel_color++);
      byte green = *(pixel_color++);
      uint32 color = ((uint32) 0x00000000  | // Alpha
		      (uint32)(red   << 16)| // Red
		      (uint32)(blue  << 8) | // Green
		      (uint32)(green));      // Blue
      if (color == ALPHA_MASK) { *Pixel++; continue; }
      *Pixel++ = color;
    }
    tex_x = -1;
    Row += Buffer->Pitch;
  }
}

internal void
DrawTexture(back_buffer* Buffer, big_texture Texture,
	    int PosX, int PosY,
	    int ScaleX, int ScaleY) {
  // Assumes textures to be of same Width and Height
  int min_x = PosX;
  int min_y = PosY;
  int max_x = PosX + (Texture.Width  * ScaleX);
  int max_y = PosY + (Texture.Height * ScaleY);
  if (min_x < 0) { min_x = 0; }
  if (min_y < 0) { min_y = 0; }
  if (max_x > Buffer->Width)  { max_x = Buffer->Width; }
  if (max_y > Buffer->Height) { max_y = Buffer->Height; }
  
  uint8* Row = ((uint8*)Buffer->Memory + (min_y * Buffer->Pitch) + (min_x * Buffer->BytesPerPixel));

  int byte_size = 3;
  int tex_x = -1;
  int tex_y = -1;
  for (int y = min_y; y < max_y; ++y) {
    uint32* Pixel = (uint32*)Row;
    tex_y++;
    
    for (int x = min_x; x < max_x; ++x) {
      tex_x++;

      int pixel_data_index = (((tex_y / ScaleY) * Texture.Width * byte_size) + ((tex_x / ScaleX) * byte_size));
      byte* pixel_color = &Texture.Pixels[pixel_data_index];
      byte red   = *(pixel_color++);
      byte blue  = *(pixel_color++);
      byte green = *(pixel_color++);
      uint32 color = ((uint32) 0x00000000  | // Alpha
		      (uint32)(red   << 16)| // Red
		      (uint32)(blue  << 8) | // Green
		      (uint32)(green));      // Blue
      if (color == ALPHA_MASK) { *Pixel++; continue; }
      *Pixel++ = color;
    }
    tex_x = -1;
    Row += Buffer->Pitch;
  }
}

internal void
DrawTexture(back_buffer* Buffer, texture Texture,
	    int PosX, int PosY,
	    int ScaleXY) {
  DrawTexture(Buffer, Texture, PosX, PosY, ScaleXY, ScaleXY);
}

internal void
DrawTexture(back_buffer* Buffer, big_texture Texture,
	    int PosX, int PosY,
	    int ScaleXY) {
  DrawTexture(Buffer, Texture, PosX, PosY, ScaleXY, ScaleXY);
}

void DrawPieces(back_buffer* Buffer, chess_state* State, mailbox* Board, int Scale) {
  int board_texture_dims    = State->BoardTexPxVals.TotalTexture;
  int inner_texture_padding = State->BoardTexPxVals.InnerPadding;
  int piece_texture_dims    = State->BoardTexPxVals.PieceTexture;
  int square_dims           = State->BoardTexPxVals.BoardSquare;
  int padding               = State->BoardTexPxVals.SquarePadding;

  int starting_x = (Buffer->Width / 2) - (board_texture_dims / 2) + inner_texture_padding;
  int starting_y = board_texture_dims - inner_texture_padding - square_dims;

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      int pos_x = starting_x + (col * square_dims) + padding;
      int pos_y = starting_y - (row * square_dims) + padding;
      int piece = Board->Squares[(row * 8) + col];
      switch(piece) {
      case EMPTY:
      case OCCUP_SQ:
      case EMPTY_SQ:
      case UNUSED:
	continue;
      default:
	DrawTexture(Buffer, State->Pieces[piece], pos_x, pos_y, Scale);
	break;
      }
    }
  }
};


void DrawPiece(back_buffer* Buffer, chess_state* State, piece Piece, int X, int Y, int Scale) {
  switch(Piece) {
  case EMPTY:
  case OCCUP_SQ:
  case EMPTY_SQ:
  case UNUSED:
    return;
  default:
    DrawTexture(Buffer, State->Pieces[Piece], X, Y, Scale);
    return;
  }
}

internal void
DrawRectangle(back_buffer* Buffer,
	      int32 MinX, int32 MinY, int32 MaxX, int32 MaxY, uint32 Color) {
  if (MinX < 0) { MinX = 0; }
  if (MinY < 0) { MinY = 0; }
  if (MaxX > Buffer->Width) { MaxX = Buffer->Width; }
  if (MaxY > Buffer->Height) { MaxY = Buffer->Height; }

  uint8* Row = ((uint8*)Buffer->Memory +
		(MinX * Buffer->BytesPerPixel) +
		(MinY * Buffer->Pitch));
  
  for (int y = MinY; y < MaxY; ++y) {
    uint32* Pixel = (uint32*)Row;
    for (int x = MinX; x < MaxX; ++x) {
      *Pixel++ = Color;
    }

    Row += Buffer->Pitch;
  }
}

void ClearScreen(back_buffer* Buffer, uint32 Color) {
  DrawRectangle(Buffer, 0, 0, Buffer->Width, Buffer->Height, Color);
};

void DrawSquareOverlay(back_buffer* Buffer, bitboard ShouldDrawOverlay, int Scale, int Color) {
  int board_texture_dims = 256 * Scale;
  int inner_texture_padding = (board_texture_dims - (176 * Scale)) / 2;
  int piece_texture_dims = 20 * Scale;
  int square_dims = 22 * Scale;
  int padding = (square_dims - piece_texture_dims) / 2;

  int starting_x = (Buffer->Width / 2) - (board_texture_dims / 2) + inner_texture_padding;
  int starting_y = board_texture_dims - inner_texture_padding - square_dims;

  int i = 0;
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      int pos_x = starting_x + (col * square_dims);
      int pos_y = starting_y - (row * square_dims);
      if (ShouldDrawOverlay & 0x1ULL) {
	DrawRectangle(Buffer, pos_x, pos_y, pos_x + square_dims - padding, pos_y + square_dims - padding, Color);
      }
      ShouldDrawOverlay >>= 1;
    }
  }
}

void DrawSquareOverlay(back_buffer* Buffer, mailbox Mailbox, int Scale, int Color) {
  int board_texture_dims = 256 * Scale;
  int inner_texture_padding = (board_texture_dims - (176 * Scale)) / 2;
  int piece_texture_dims = 20 * Scale;
  int square_dims = 22 * Scale;
  int padding = (square_dims - piece_texture_dims) / 2;

  int starting_x = (Buffer->Width / 2) - (board_texture_dims / 2) + inner_texture_padding;
  int starting_y = board_texture_dims - inner_texture_padding - square_dims;

  int i = 0;
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      int pos_x = starting_x + (col * square_dims);
      int pos_y = starting_y - (row * square_dims);
      if (Mailbox.Squares[i++]) {
	DrawRectangle(Buffer, pos_x, pos_y, pos_x + square_dims - padding, pos_y + square_dims - padding, Color);
      }
    }
  }
}

#define GRAPHICS_H
#endif
