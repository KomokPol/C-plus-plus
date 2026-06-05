#pragma once

template <typename T>
void Reverse(T* start, T* end) {
  end--;
  while (start < end) {
    T temp = *end;
    *end = *start;
    *start = temp;
    start++;
    end--;
  }
}

template <typename T>
void Rotate(T* start, T* mid, T* end) {
  Reverse(start, end);
  auto len = end - start;
  auto k = mid - start;
  mid = start + (len - k);
  Reverse(start, mid);
  Reverse(mid, end);
}