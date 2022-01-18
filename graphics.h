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

#define GRAPHICS_H
#endif
