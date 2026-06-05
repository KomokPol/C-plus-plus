#pragma once

template <typename T>
T* Unique(T* start, T* end) {
  if (start == end) {
    return end;
  }
  T* temp = start;
  T* next = start + 1;
  while (next < end) {
    if (*next != *temp) {
      temp++;
      *temp = *next;
    }
    next++;
  }
  return temp + 1;
}