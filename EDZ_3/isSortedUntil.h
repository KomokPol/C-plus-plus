#pragma once

template <typename T>
T* IsSortedUntil(T* start, T* end) {
  while (start != end) {
    T* element = start + 1;
    if (*element < *start) {
      return element;
    }
    start++;
  }
  return end;
}