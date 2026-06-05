#pragma once

template <typename T, typename U = T> 
U* Copy(T* src_begin, T* src_end, U* dest) {
  while (src_begin != src_end) {
    *dest = *src_begin;
    src_begin++;
    dest++;
  }
  return dest;
}
template <typename T, typename U = T> 
U* CopyBackward(T* src_begin, T* src_end, U* dest) {
  while (src_end != src_begin) {
    src_end--;
    dest--;
    *dest = *src_end;
  }
  return dest;
}