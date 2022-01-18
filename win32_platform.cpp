#include "win32_platform.h"

global_var win32_back_buffer GlobalBackBuffer = {};
global_var bool32 Running;

PLATFORM_FREE_FILE_MEMORY(PlatformFreeFileMemory) {
  if (Memory) {
    VirtualFree(Memory, 0, MEM_RELEASE);
  }
}

PLATFORM_READ_ENTIRE_FILE(PlatformReadEntireFile) {
  read_file_result Result = {};
  HANDLE FileHandle = CreateFileA(Filename,
				  GENERIC_READ,
				  FILE_SHARE_READ,
				  0,
				  OPEN_EXISTING,
				  0, 0);
  if (FileHandle != INVALID_HANDLE_VALUE) {
    LARGE_INTEGER FileSize;
    if (GetFileSizeEx(FileHandle, &FileSize)) {
      uint32 FileSize32 = SafeTruncateUint64(FileSize.QuadPart);
      Result.Contents = VirtualAlloc(0, FileSize32, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
      if (Result.Contents) {
	DWORD BytesRead;
	if (ReadFile(FileHandle, Result.Contents, FileSize32, &BytesRead, 0) &&
	    (FileSize32 == BytesRead)) {

	  Result.ContentsSize = FileSize32;
	}
	else {
	  PlatformFreeFileMemory(Result.Contents);
	  Result.Contents = 0;
	}
      }
    };
    CloseHandle(FileHandle);
  }

  return Result;
}

PLATFORM_WRITE_ENTIRE_FILE(PlatformWriteEntireFile) {
  bool32 Result = false;
  HANDLE FileHandle = CreateFileA(Filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);

  if (FileHandle != INVALID_HANDLE_VALUE) {
    DWORD BytesWritten;
    if (WriteFile(FileHandle, Memory, MemorySize, &BytesWritten, 0)) {
      Result = (BytesWritten == MemorySize);
    }
    CloseHandle(FileHandle);
  }

  return Result;
}

internal win32_window_dimensions
Win32GetWindowDimensions(HWND window) {
  win32_window_dimensions Result;

  RECT ClientRect;
  GetClientRect(window, &ClientRect);
  Result.Width = ClientRect.right - ClientRect.left;
  Result.Height = ClientRect.bottom - ClientRect.top;
  return Result;
}

internal void
Win32ResizeDIBSection(win32_back_buffer* buffer, int width, int height) {
  if (buffer->Memory) {
    VirtualFree(buffer->Memory, 0, MEM_RELEASE);
  }

  buffer->BytesPerPixel = 4;

  buffer->Width = width;
  buffer->Height = height;
  buffer->Pitch = width * buffer->BytesPerPixel;

  buffer->Info.bmiHeader.biSize = sizeof(buffer->Info.bmiHeader);
  buffer->Info.bmiHeader.biHeight = -buffer->Height;
  buffer->Info.bmiHeader.biWidth = buffer->Width;
  buffer->Info.bmiHeader.biPlanes = 1;
  buffer->Info.bmiHeader.biBitCount = 32;
  buffer->Info.bmiHeader.biCompression = BI_RGB;

  int BitmapMemorySize = (width * height) * buffer->BytesPerPixel;
  buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
}

internal void
Win32DisplayBufferInWindow(win32_back_buffer* buffer, HDC device_context,
			   int window_width, int window_height) {
  StretchDIBits(device_context,
		0, 0, buffer->Width, buffer->Height,
		0, 0, buffer->Width, buffer->Height,
		buffer->Memory,
		&buffer->Info,
		DIB_RGB_COLORS, SRCCOPY
		);
}

internal void
Win32ProcessPendingMessages(input* Input) {
  MSG Message;
  while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE)) {
    if (Message.message == WM_QUIT) {
      Running = false;
    }

    switch (Message.message) {
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:
      {
	uint32 VKCode = Message.wParam;
	bool WasDown = ((Message.lParam & (1 << 30)) != 0);
	bool IsDown = ((Message.lParam & (1 << 31)) == 0);
	bool32 AltDown = (Message.lParam & (1 << 29));
	
	if (WasDown != IsDown) {
	  if (VKCode == VK_ESCAPE) {
	    Running = false;
	  }
	}

	if (AltDown && (VKCode == VK_F4)) {
	  Running = false;
	}
      }
    default:
      TranslateMessage(&Message);
      DispatchMessage(&Message);
    };
  }
}

internal void
Win32ProcessKeyboardMessage(button* Button, bool32 IsDown) {
  if (Button->EndedDown != IsDown) {
    Button->EndedDown = IsDown;
  }
}

LRESULT CALLBACK 
MainWindowCallBack(HWND Window,
		   UINT Message,
		   WPARAM wParam,
		   LPARAM lParam) 
{
  switch (Message)
  {
    case WM_SIZE: {
      win32_window_dimensions windims = Win32GetWindowDimensions(Window);
      Win32ResizeDIBSection(&GlobalBackBuffer, windims.Width, windims.Height);
    } break;
    case WM_DESTROY: {
      Running = false;
    } break;
    case WM_CLOSE: {
      Running = false;
    } break;
    case WM_PAINT: {
      PAINTSTRUCT Paint;
      HDC DeviceContext = BeginPaint(Window, &Paint);
      win32_window_dimensions windims = Win32GetWindowDimensions(Window);
      Win32DisplayBufferInWindow(&GlobalBackBuffer, DeviceContext, windims.Width, windims.Height);
      EndPaint(Window, &Paint);
    } break;
    default: {
    } break;
  }

  return DefWindowProc(Window, Message, wParam, lParam);
}

int CALLBACK
WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int CommandShow) {
  AllocConsole();
  #pragma warning(suppress : 4996)
  freopen("CONIN$", "r", stdin);
  #pragma warning(suppress : 4996)
  freopen("CONOUT$", "w", stdout);
  #pragma warning(suppress : 4996)
  freopen("CONOUT$", "w", stderr);

  WNDCLASS WindowClass = {};
  WindowClass.style = CS_HREDRAW|CS_VREDRAW;
  WindowClass.lpfnWndProc = MainWindowCallBack;
  WindowClass.hInstance = Instance;
  WindowClass.lpszClassName = "rChess";
  WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  RegisterClass(&WindowClass);

  HWND Window =
    CreateWindowEx(0, WindowClass.lpszClassName,
		   "rChess",
		   WS_OVERLAPPEDWINDOW|WS_VISIBLE,
		   CW_USEDEFAULT, CW_USEDEFAULT,
		   WINDOW_WIDTH, WINDOW_HEIGHT,
		   0, 0, Instance, 0);

  if (Window) {
    HDC DeviceContext = GetDC(Window);
    memory Memory = {};
    Memory.BlockSize = Megabytes(1);
    Memory.Block = 
      VirtualAlloc(0, (size_t)Memory.BlockSize,
		   MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    Memory.PlatformFreeFileMemory = PlatformFreeFileMemory;
    Memory.PlatformReadEntireFile = PlatformReadEntireFile;
    Memory.PlatformWriteEntireFile = PlatformWriteEntireFile;
    
    if (!Memory.Block) { return 69; }

    update_and_render* UpdateAndRender = 0;
    HMODULE LoadedDLL = LoadLibraryA("rChess.dll");
    if (LoadedDLL) {
      UpdateAndRender = (update_and_render*)GetProcAddress(LoadedDLL, "UpdateAndRender");
    }

    if (!UpdateAndRender) { return 69; }

    input Input[2] = {0};
    input* NewInput = &Input[0];
    input* OldInput = &Input[1];
    
    Running = true;
    while (Running) {
      for (int i = 0; i < ArrayCount(NewInput->Buttons); i++) {
        NewInput->Buttons[i].EndedDown = OldInput->Buttons[i].EndedDown;
      }

      Win32ProcessPendingMessages(NewInput);
      
      POINT MousePoint;
      GetCursorPos(&MousePoint);
      ScreenToClient(Window, &MousePoint);
      NewInput->MouseX = MousePoint.x;
      NewInput->MouseY = MousePoint.y;
      NewInput->MouseZ = 0;
      int high_bit = (1 << 15);
      Win32ProcessKeyboardMessage(&NewInput->Buttons[0],
				  GetKeyState(VK_LBUTTON) & high_bit);
      Win32ProcessKeyboardMessage(&NewInput->Buttons[1],
				  GetKeyState(VK_LBUTTON) & high_bit);
      Win32ProcessKeyboardMessage(&NewInput->Buttons[2],
				  GetKeyState(VK_LBUTTON) & high_bit);

      back_buffer bBuffer = {};
      bBuffer.Memory = GlobalBackBuffer.Memory;
      bBuffer.Width = GlobalBackBuffer.Width;
      bBuffer.Height = GlobalBackBuffer.Height;
      bBuffer.Pitch = GlobalBackBuffer.Pitch;
      bBuffer.BytesPerPixel = GlobalBackBuffer.BytesPerPixel;
      UpdateAndRender(Input, &Memory, &bBuffer);

      win32_window_dimensions windims = Win32GetWindowDimensions(Window);
      Win32DisplayBufferInWindow(&GlobalBackBuffer, DeviceContext,
				 windims.Width, windims.Height);

      input* Temp = NewInput;
      NewInput = OldInput;
      OldInput = Temp;
    }
  }
  return 0;
}
