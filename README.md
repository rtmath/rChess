# rChess

### A Humble Chess Engine

rChess strives to be a simple, readable, easy-to-understand chess engine in C++.

---
### Roadmap

Completed:
  * Board representation (mailbox and bitboard)
  * Pseudolegal move generation
  * FEN support
  * GUI + Drag & Drop controls

Currently:
  * Debugging Move Generation (Perft results mismatch)

Next Steps:
  * Move Search
  * Evaluation
  * Opening Book
  * and much, much more

---

##### Note on Project Structure
This project's structure derives from Casey Muratori's Handmade Hero, and deserves a brief overview.

win32_platform.cpp is responsible for Windows-specific tasks (window creation, message loop, file I/O, etc), and compiles as an .exe.<br>
rchess.cpp houses platform-independent application logic, and compiles as a .dll exporting one function: `UpdateAndRender()`.

During program execution, win32_platform loads the .dll, and as often as it can, calls `UpdateAndRender()`.

```
Each frame:
  UpdateAndRender receives pointers to the application's allocated memory, user input, and a pixel buffer
  UpdateAndRender handles user input, updates state, and populates the pixel buffer
  win32_platform blits the pixel buffer to the window via win32 API
```

When perusing the source code, my recommendation is to start from rChess.cpp's `UpdateAndRender()`, treating it as the program's entry point.
