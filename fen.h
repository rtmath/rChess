#ifndef FEN_H

inline piece
PieceFromFEN(char c) {
  switch(c) {
  case 'r': { return bROOK; }
  case 'R': { return wROOK; }
  case 'n': { return bKNIGHT; }
  case 'N': { return wKNIGHT; }
  case 'b': { return bBISHOP; }
  case 'B': { return wBISHOP; }
  case 'q': { return bQUEEN; }
  case 'Q': { return wQUEEN; }
  case 'k': { return bKING; }
  case 'K': { return wKING; }
  case 'p': { return bPAWN; }
  case 'P': { return wPAWN; }
  default:  { return EMPTY; }
  }
}

inline bool32
IsPiece(char c) {
  switch(c) {
  case 'r':
  case 'R':
  case 'n':
  case 'N':
  case 'b':
  case 'B':
  case 'q':
  case 'Q':
  case 'k':
  case 'K':
  case 'p':
  case 'P': {
    return true;
  }
  default: {
    return false;
  }
  }
};

inline bool32
IsNum(char c) {
  return ((c >= '0') && (c <= '9'));
};

inline uint8 AsciiN(char c) {
  return c - '0';
};

void PopulateBitboardFromFEN(bitboard* Bitboards, char* FEN, int* fendex) {
  char c = FEN[*fendex];

  int square = A8;
  while (c > 0) {
    if (IsPiece(c)) {
      SetBit(&Bitboards[PieceFromFEN(c)], square++);
    } else if (IsNum(c)) {
      for (uint8 i = 0; i < AsciiN(c); i++) { square++; }
    }  else if (c == '/') {
      square -= 16; // Go to next row on board
    } else {
      (*fendex)++; // Move the FEN index over the whitespace
      break;
    }
    c = FEN[++(*fendex)];
  };
};

void SideToMoveFromFEN(board_state* Board, char* FEN, int* fendex) {
  char c = FEN[(*fendex)];
  (*fendex)+=2; // Skip current position and whitespace
  if (c == 'b') { Board->WhiteToMove = false; }
  else { Board->WhiteToMove = true; }
};

void CastlingAvailabilityFromFEN(board_state* Board, char* FEN, int* fendex) {
  char c = FEN[(*fendex)];
  while (c != '-' && c != ' ' && c != '\0') {
    switch(c) {
    case 'K': {
      Board->wCanCastleKingside = true;
    } break;
    case 'Q': {
       Board->wCanCastleQueenside = true;
    } break;
    case 'k': {
      Board->bCanCastleKingside = true;
    } break;
    case 'q': {
      Board->bCanCastleQueenside = true;
    } break;
    default: {
      break;
    }
    };
    c = FEN[++(*fendex)];
  }
  (*fendex)++;
};

void EnPassantFromFEN(board_state* Board, char* FEN, int* fendex) {
  char c = FEN[(*fendex)];
  if (c == '-') {
    Board->EnPassantTarget = 0; // This clashes with the index of the A1 square, but EP isnt possible on A1
    (*fendex) += 2; // Move FEN index beyond hyphen + whitespace
    return; 
  }
  switch(c) {
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
  case 'f':
  case 'g':
  case 'h': {
    uint8 file = c - 'a';
    c = FEN[++(*fendex)];
    uint8 rank = AsciiN(c);
    Board->EnPassantTarget = (8 * (rank - 1)) + (file);
    (*fendex)++; // Move FEN index beyond whitespace
    return;
  }
  default:
    Board->EnPassantTarget = 0;
    (*fendex) += 2; // Panic-increment the index if the default path is hit (it shouldnt be hit)
    return;
  }
};

void MoveCountFromFEN(uint8* board_move_counter, char* FEN, int* fendex) {
  uint8 places = 0;
  uint8 digits[3] = {};
  char c = FEN[(*fendex)];
  while (c != ' ' && c != '\0' && places < 3) {
    digits[places++] = AsciiN(c);
    c = FEN[++(*fendex)];
  }
  
  if (places == 1) {
    *board_move_counter = digits[0];
  } else if (places == 2) {
    *board_move_counter = (digits[0] * 10) + digits[1];
  } else if (places == 3) {
    *board_move_counter = (digits[0] * 100) + (digits[1] * 10) + digits[2];
  }
  (*fendex)++; // Skip whitespace
};

void FENToBitboards(char* FEN, bitboard* Bitboards, board_state* BoardState) {
  int index = 0;
  PopulateBitboardFromFEN(Bitboards, FEN, &index);
  SideToMoveFromFEN(BoardState, FEN, &index);
  CastlingAvailabilityFromFEN(BoardState, FEN, &index);
  EnPassantFromFEN(BoardState, FEN, &index);
  MoveCountFromFEN(&BoardState->HalfMoves, FEN, &index);
  MoveCountFromFEN(&BoardState->FullMoves, FEN, &index);
  Bitboards[OCCUP_SQ] = (Bitboards[wPAWN] |
			 Bitboards[wBISHOP] |
			 Bitboards[wKNIGHT] |
			 Bitboards[wROOK] |
			 Bitboards[wQUEEN] |
			 Bitboards[wKING] |
                         Bitboards[bPAWN] |
			 Bitboards[bBISHOP] |
			 Bitboards[bKNIGHT] |
			 Bitboards[bROOK] |
			 Bitboards[bQUEEN] |
			 Bitboards[bKING]);
  Bitboards[EMPTY_SQ] = ~(Bitboards[OCCUP_SQ]);
};

#define FEN_H
#endif
