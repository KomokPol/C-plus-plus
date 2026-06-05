#include <iostream>
#include "cstring.h"

size_t Strlen(const char* str) {
  size_t lenght = 0;
  while (*str != '\0') {
    lenght += 1;
    ++str;
  }
  return lenght;
}
int Strcmp(const char* first, const char* second) {
  while (*first != '\0') {
    if (*first > *second) {
      return 1;
    }
    if (*first < *second) {
      return -1;
    }
    ++first;
    ++second;
  }
  if (Strlen(first) < Strlen(second)) {
    return -1;
  }
  return 0;
}
int Strncmp(const char* first, const char* second, size_t count) {
  size_t c = 0;
  while (*first != '\0' && c < count) {
    if (*first > *second) {
      return 1;
    }
    if (*first < *second) {
      return -1;
    }
    ++first;
    ++second;
    ++c;
  }
  if ((Strlen(first) < Strlen(second)) && c != count) {
    return -1;
  }
  return 0;
}
char* Strcpy(char* dest, const char* src) {
  char* start = dest;
  while (*src != '\0') {
      *dest = *src;
      ++dest;
      ++src;
  }
  *dest = '\0';
  return start;
}
char* Strncpy(char* dest, const char* src, size_t count) {
  size_t c = 0;
  char* start = dest;
  while (*src != '\0' && c < count) {
      *dest = *src;
      ++dest;
      ++src;
      ++c;
  }
  while (c < count) {
    *dest = '\0';
    ++dest;
    ++c;
  }
  return start;
}
char* Strcat(char* dest, const char* src) {
  char* start = dest;
  while (*dest != '\0') {
    dest++;
  }
  while (*src != '\0') {
    *dest = *src;
    ++dest;
    ++src;
  }
  *dest = '\0';
  return start;
}
char* Strncat(char* dest, const char* src, size_t count) {
  size_t c = 0;
  char* start = dest;
  while (*dest != '\0') {
    dest++;
  }
  while (*src != '\0' && c < count) {
    *dest = *src;
    ++dest;
    ++src;
    ++c;
  }
  *dest = '\0';
  return start;
}
const char* Strchr(const char* str, char symbol) {
  while (*str != '\0') {
    if (*str == symbol) {
      return str;
      break;
    }
    ++str;
  }
  if (symbol == '\0') {
    return str;
  }
  return nullptr;
}
const char* Strrchr(const char* str, char symbol) {
  const char* ptr = nullptr;
  while (*str != '\0') {
    if (*str == symbol) {
      ptr = str;
    }
    ++str;
  }
  if (symbol == '\0') {
    return str;
  }
  return ptr;
}
size_t Strspn(const char* dest, const char* src) {
  size_t count = 0;
  while (*dest != '\0') {
    const char* ptr = src;
    bool flag = false;
    while (*ptr != '\0') {
      if (*dest == *ptr) {
        ++count;
        flag = true;
        break;
      }
      ++ptr;
    }
    if (!flag) {
      break;
    }
    ++dest;
  }
  return count;
}
size_t Strcspn(const char* dest, const char* src) {
  size_t count = 0;
  while (*dest != '\0') {
    const char* ptr = src;
    bool flag = false;
    while (*ptr != '\0') {
      if (*dest == *ptr) {
        flag = true;
        break;
      }
      ++ptr;
    }
    if (flag) {
      break;
    }
    ++dest;
    ++count;
  }
  return count;
}
const char* Strpbrk(const char* dest, const char* breakset) {
  const char* ptr = nullptr;
  while (*dest != '\0') {
    bool flag = false;
    while (*breakset != '\0') {
      if (*dest == *breakset) {
        flag = true;
        ptr = dest;
        break;
      }
      ++breakset;
    }
    if (flag) {
      break;
    }
    ++dest;
  }
  return ptr;
}
const char* Strstr(const char* str, const char* pattern) {
  const char* ptr = nullptr;
  if (*pattern == '\0') {
    return str;
  }
  while (*str != '\0') {
    if (*str == *pattern) {
      ptr = str;
      bool flag = false;
      const char* ptr1 = str;
      const char* ptr2 = pattern;
      while (*ptr2 != '\0') {
        if (*ptr1 != *ptr2) {
          ptr = nullptr;
          flag = true;
          break;
        }
        ++ptr2;
        ++ptr1;
      }
      if (!flag) {
        break;
      }
    }
    ++str;
  }
  return ptr;
}