#include "charpointer_func.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>

uint64_t GetSandCount(char* arr, size_t* i) {
  char str[20]('\0');
  uint64_t value = 0;
  size_t count = 0;
  size_t j = 0;
  size_t digit = 0;
  while (arr[*i] != '\n' && arr[*i] != '\0') {
    str[j] = arr[*i];
    ++*i;
    ++j;
  }
  ++*i;
  while (count != j) {
    value = value*  10 + (str[count] - '0');
    count++;
  }
  return value;
}

int16_t GetCoordinate(char* arr, size_t* i) {
  char str[20]('\0');
  size_t j = 0;
  while (arr[*i] != '\t') {
    str[j] = arr[*i];
    ++*i;
    ++j;
  }
  ++*i;
  return std::stoi(str);
}