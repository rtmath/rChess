#ifndef TEST_BITBOARD_H

#include "bitboard.h"

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
    if (!CheckBit(&actual, i)) {
      printf("TEST_CheckBit FAILED: Did not detect set bit %i.", i);
      result = false;
      break;
    }
    ClearBit(&actual, i);
    if (CheckBit(&actual, i)) {
      printf("TEST_CheckBit FAILED: Still detecting cleared bit %i.", i);
      result = false;
      break;
    }
  }
  return result;
}

#define TEST_BITBOARD_H
#endif
