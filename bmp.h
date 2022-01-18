#ifndef BMP_H

internal unsigned int
interpret_bytes(byte* address, int amount) {
  //       Convert [0x42, 0x4D, 0x01, 0x00] to
  // (unsigned int) 0x00014D42
  unsigned int result = 0;
  for (int i = 0; i < amount; i++) {
    result |= (unsigned int)address[i] << (8 * i);
  }
  return result;
}

inline bool
unsupported_dib_header(unsigned int dib_size) {
  return dib_size != 40;
}

inline bool
unsupported_compression(unsigned int compression_code) {
  return compression_code != 0;
}

inline unsigned int
calculate_height(int file_length, int width) {
  return (file_length / (width * (3 + (width % 4))));
}

internal int
calculate_padding(int value) {
  #define CALCPAD_ALIGNMENT 4
  int modulo = value % CALCPAD_ALIGNMENT;
  if (modulo) {
    return (CALCPAD_ALIGNMENT - modulo);
  }
  return modulo;
  #undef CALCPAD_ALIGNMENT
}

// BMP-format data offsets
#define PIXEL_DATA 0xA
#define DIB_SIZE 0xE
#define BMP_WIDTH 0x12
#define BMP_HEIGHT 0x16
#define COMPRESSION_CODE 0x1E
#define BMP_BPP 0x1C
#define PIXEL_DATA_SIZE 0x22

internal void
ReadBMP(byte* bmp_contents, unsigned int bmp_length, byte* destination_pixel_memory, int* BmpWidth, int* BmpHeight) {
  const int RGB_BYTE_SIZE = 3;
  const int RGBA_BYTE_SIZE = 4;

  if (bmp_contents[0] != 0x42 || bmp_contents[1] != 0x4D) { return; } // Loaded file does not start with "BM", probably is not a bmp
  unsigned int bmp_size = interpret_bytes(&bmp_contents[2], 4);
  unsigned int bits_per_pixel = interpret_bytes(&bmp_contents[BMP_BPP], 4);
  unsigned int dib_size = interpret_bytes(&bmp_contents[DIB_SIZE], 4);
  unsigned int bmp_width = interpret_bytes(&bmp_contents[BMP_WIDTH], 4);
  unsigned int bmp_height = interpret_bytes(&bmp_contents[BMP_HEIGHT], 4);
  unsigned int pixel_data_loc = interpret_bytes(&bmp_contents[PIXEL_DATA], 4);
  if (unsupported_dib_header(dib_size)) { return; }
  if (unsupported_compression(interpret_bytes(&bmp_contents[COMPRESSION_CODE], 4))) { return; }

  if (bmp_size != bmp_length) {
    // Assume bmp width is correct, re-calculate bmp_height
    bmp_height = calculate_height(bmp_length, bmp_width);
  }
  
  *BmpWidth  = bmp_width;
  *BmpHeight = bmp_height;
  
  if (bits_per_pixel == 24) // BMP is RGB
  {
    int accumulating_index = 0;
    byte* pixel_data = (bmp_contents + pixel_data_loc);
    for (int i = bmp_height - 1; i >= 0; i--) {
      for (int j = 0; j < ((int32)bmp_width * RGB_BYTE_SIZE); j += RGB_BYTE_SIZE) {
	int index = j + (i * (bmp_width * RGB_BYTE_SIZE));
	// Convert LE to BE while looping
	destination_pixel_memory[accumulating_index++] = pixel_data[index + 2];
        destination_pixel_memory[accumulating_index++] = pixel_data[index + 1];
        destination_pixel_memory[accumulating_index++] = pixel_data[index];
      }
    }
  }
  else if (bits_per_pixel == 32) // BMP is RGBA
  {
    int accumulating_index = 0;
    for (int i = bmp_height - 1; i >= 0; i--) {
      for (int j = 0; j < ((int32)bmp_width * RGBA_BYTE_SIZE); j += RGBA_BYTE_SIZE) {
	int index = j + (i * (bmp_width * RGBA_BYTE_SIZE));
	// Convert LE to BE while looping
        destination_pixel_memory[accumulating_index++] = bmp_contents[index + 2]; // Blue
        destination_pixel_memory[accumulating_index++] = bmp_contents[index + 1]; // Green
        destination_pixel_memory[accumulating_index++] = bmp_contents[index];     // Red
        destination_pixel_memory[accumulating_index++] = bmp_contents[index + 3]; // Alpha
      }
    }
  } 
}

internal bool32
LoadTextureFromBMP(char* filename, memory* Memory, texture* Texture) {
  // I'm passing memory in here because I need the two File I/O function pointers
  read_file_result bmp = Memory->PlatformReadEntireFile(filename);
  if (bmp.ContentsSize > 0) {
    Assert(bmp.ContentsSize < Kilobytes(2));
    ReadBMP((byte*)bmp.Contents, bmp.ContentsSize, Texture->Pixels, &Texture->Width, &Texture->Height);
    Memory->PlatformFreeFileMemory(bmp.Contents);
    return true;
  }
  return false;
}

internal bool32
LoadBigTextureFromBMP(char* filename, memory* Memory, big_texture* Texture) {
  // I'm passing memory in here because I need the two File I/O function pointers
  read_file_result bmp = Memory->PlatformReadEntireFile(filename);
  if (bmp.ContentsSize > 0) {
    Assert(bmp.ContentsSize < Kilobytes(200));
    ReadBMP((byte*)bmp.Contents, bmp.ContentsSize, Texture->Pixels, &Texture->Width, &Texture->Height);
    Memory->PlatformFreeFileMemory(bmp.Contents);
    return true;
  }
  return false;
}

#define BMP_H
#endif
