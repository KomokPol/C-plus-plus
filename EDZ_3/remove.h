#pragma once

template <typename T>
T* Remove(T* start, T* end, T value) {
  if (start == end) {
    return end;
  }
  T* temp = start;
  while (start < end) {
    if (*start != value) {
      *temp = *start;
      temp++;
    }
    start++;
  }
  return temp;
}