#ifndef WIN32_PLATFORM_H
#include <stdio.h>
#include <windows.h>
#include "typedefs.h"
#include "rchess.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define UPDATE_AND_RENDER(name) void name(input* Input, memory* Memory, back_buffer *Buffer)
typedef UPDATE_AND_RENDER(update_and_render);

struct win32_window_dimensions {
  int Width;
  int Height;
};

struct win32_back_buffer {
  BITMAPINFO Info;
  void* Memory;
  int Width;
  int Height;
  int Pitch;
  int BytesPerPixel;
};

#define WIN32_PLATFORM_H
#endif
