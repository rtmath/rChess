#ifndef BITBOARD_H

uint64 Power(uint8 base, uint8 exp) {
  if (base == 0 || exp == 0) return 0;
  uint64 result = 1;
  for (int i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}

void PrintBinary(uint64 n) {
  byte rep[65] = {};
  uint64 mask = (1ULL << 63);
  for (int i = 0; i < 64; i++) {
    rep[i] = (n & mask) ? '1' : '0';
    n <<= 1;
  }
  rep[64] = '\0';
  printf("%s\n", rep);
}

inline void
SetBit(uint64* target, unsigned char nth_place) {
  Assert(nth_place < 64);
  uint64 index = 0x1;
  *target |= (index << nth_place);
}

inline void
ClearBit(uint64* target, unsigned char nth_place) {
  Assert(nth_place < 64);
  uint64 index = 0x1;
  *target &= ~(index << nth_place);
}

inline void
ToggleBit(uint64* target, unsigned char nth_place) {
  Assert(nth_place < 64);
  uint64 index = 0x1;
  *target ^= (index << nth_place);
}

inline bool
CheckBit(uint64* target, unsigned char nth_place) {
  Assert(nth_place < 64);
  uint64 index = 0x1;
  return !!(*target >> nth_place) & index;
}

#define BITBOARD_H
#endif
