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

// This is hardcoded for the specific Board and Piece textures I am using.
// The Board texture is 256x256 pixels and each piece is 20x20 pixels
// The actual board in the texture is only 176x176, and the other pixels are decoration
void DrawPieces(back_buffer* Buffer, chess_state* State, board* Board, int scale) {
  int board_texture_dims = 256 * scale;
  int inner_texture_padding = (board_texture_dims - (176 * scale)) / 2;
  int piece_texture_dims = 20 * scale;
  int square_dims = 22 * scale;
  int padding = (square_dims - piece_texture_dims) / 2;

  int starting_x = (Buffer->Width / 2) - (board_texture_dims / 2) + inner_texture_padding;
  int starting_y = board_texture_dims - inner_texture_padding - square_dims;

  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      int pos_x = starting_x + (col * square_dims) + padding;
      int pos_y = starting_y - (row * square_dims) + padding;
      switch(Board->Squares[(row * 8) + col]) {
      case EMPTY:
	continue;
      case wPAWN:
	DrawTexture(Buffer, State->WhitePawn, pos_x, pos_y, scale);
	break;
      case bPAWN:
	DrawTexture(Buffer, State->BlackPawn, pos_x, pos_y, scale);
	break;
      case wBISHOP:
	DrawTexture(Buffer, State->WhiteBishop, pos_x, pos_y, scale);
	break;
      case bBISHOP:
	DrawTexture(Buffer, State->BlackBishop, pos_x, pos_y, scale);
	break;
      case wKNIGHT:
	DrawTexture(Buffer, State->WhiteKnight, pos_x, pos_y, scale);
	break;
      case bKNIGHT:
	DrawTexture(Buffer, State->BlackKnight, pos_x, pos_y, scale);
	break;
      case wROOK:
	DrawTexture(Buffer, State->WhiteRook, pos_x, pos_y, scale);
	break;
      case bROOK:
	DrawTexture(Buffer, State->BlackRook, pos_x, pos_y, scale);
	break;
      case wQUEEN:
	DrawTexture(Buffer, State->WhiteQueen, pos_x, pos_y, scale);
	break;
      case bQUEEN:
	DrawTexture(Buffer, State->BlackQueen, pos_x, pos_y, scale);
	break;
      case wKING:
	DrawTexture(Buffer, State->WhiteKing, pos_x, pos_y, scale);
	break;
      case bKING:
	DrawTexture(Buffer, State->BlackKing, pos_x, pos_y, scale);
	break;
      }      
    }
  }
};

#define GRAPHICS_H
#endif
