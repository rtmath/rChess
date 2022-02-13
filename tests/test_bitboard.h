#ifndef TEST_BITBOARD_H

#include "bitboard.h"

uint64 Power(uint8 base, uint8 exp) {
  if (base == 0 || exp == 0) return 0;
  uint64 result = 1;
  for (int i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}

bool32 TEST_SetBit() {
  bool32 result = true;
  for (int i = 0; i < 64; i++) {
    uint64 expected = (i == 0) ? 1 : Power(2, i);
    uint64 actual = 0;
    SetBit(&actual, i);
    if (expected != actual) {
      printf("TEST_SetBit FAILED: Expected (%llu) does not match actual (%llu)\n",
	     expected, actual);
      result = false;
      break;
    }
  }
  return result;
}

bool32 TEST_ClearBit() {
  bool32 result = true;
  for (int i = 0; i < 64; i++) {
    uint64 expected = (i == 0) ? ~1 : ~(Power(2, i));
    uint64 actual = ~0ULL;
    ClearBit(&actual, i);
    if (expected != actual) {
      printf("TEST_ClearBit FAILED: Expected (%llu) does not match actual (%llu)\n",
	     expected, actual);
      result = false;
      break;
    }
  }
  return result;
}

bool32 TEST_ToggleBit() {
  bool32 result = true;
  for (int i = 0; i < 64; i++) {
    uint64 expected_on = (i == 0) ? 1 : Power(2, i);
    uint64 expected_off = 0;
    uint64 actual = 0;
    ToggleBit(&actual, i);
    if (actual != expected_on) {
      printf("TEST_ToggleBit On FAILED: Expected (%llu) does not match actual (%llu)\n",
	     expected_on, actual);
      result = false;
      break;
    }
    ToggleBit(&actual, i);
    if (actual != expected_off) {
      printf("TEST_ToggleBit Off FAILED: Expected (%llu) does not match actual (%llu)\n",
	     expected_off, actual);
      result = false;
      break;
    }
  }
  return result;
}

bool32 TEST_CheckBit() {
  bool32 result = true;
  for (int i = 0; i < 64; i++) {
    uint64 actual = 0;
    SetBit(&actual, i);
    if (!CheckBit(actual, i)) {
      printf("TEST_CheckBit FAILED: Did not detect set bit %i.", i);
      result = false;
      break;
    }
    ClearBit(&actual, i);
    if (CheckBit(actual, i)) {
      printf("TEST_CheckBit FAILED: Still detecting cleared bit %i.", i);
      result = false;
      break;
    }
  }
  return result;
}

bool32 TEST_BitScanForward() {
  bool32 result = true;
  for (int i = 0; i < 64; i++) {
    uint64 tester = BitboardWithBitSet(0, i);

    int expected = i;
    int actual = BitScanForward(tester);

    if (expected != actual) {
      result = false;
      printf("TEST_BitScanForward FAILED: Expected i (%i) did not match actual i (%i)\n", expected, actual);
    }
  }
  return result;
}

bool32 TEST_BitScanReverse() {
  bool32 result = true;
  for (int i = 0; i < 64; i++) {
    uint64 tester = BitboardWithBitSet(0, i);

    int expected = i;
    int actual = BitScanReverse(tester);

    if (expected != actual) {
      result = false;
      printf("TEST_BitScanReverse FAILED: Expected i (%i) did not match actual i (%i)\n", expected, actual);
    }
  }
  return result;
}

bool32 TEST_PieceIsType() {
  bool32 result = true;

  piece Types[] = { PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING };
  piece White[] = { wPAWN, wBISHOP, wKNIGHT, wROOK, wQUEEN, wKING };
  piece Black[] = { bPAWN, bBISHOP, bKNIGHT, bROOK, bQUEEN, bKING };

  for (int i = 0; i < 6; i++) {
    bool32 actualW = PieceIsType(White[i], Types[i]);
    bool32 actualB = PieceIsType(Black[i], Types[i]);
    if (!actualW) { printf("TEST_PieceIsType FAILED: Expected white piece %i was not detected as type %i\n", White[i], Types[i]); result = false; }
    if (!actualB) { printf("TEST_PieceIsType FAILED: Expected black piece %i was not detected as type %i\n", Black[i], Types[i]); result = false; }
  }
  
  return result;
}

#define TEST_BITBOARD_H
#endif
