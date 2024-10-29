#include <sandpile_model.h>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
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
  int j = 0;
  while (arr[*i] != '\t') {
    str[j] = arr[*i];
    ++*i;
    ++j;
  }
  ++*i;
  return std::stoi(str);
}

bool ReadFile(SandboxType* sandbox, Args* args, SandStack* sandpiles) {
  std::fstream file(args->input_path);
  if (!file.is_open()) {
    std::cerr << "ERROR WHILE READING FILE";
    return false;
  }
  const size_t kMaxSize = 33; // 2*  size of max int16t + size of max uint64 + 2*  '\t' + '\n'
  char str[kMaxSize]('\0');
  size_t i;
  int16_t x;
  int16_t y;
  uint64_t sand_count;
  while (!file.eof()) {
    i = 0;
    file.getline(str, kMaxSize);
    if (str[0] == '\0') {
      break;
    }
    x = GetCoordinate(str, &i);
    y = GetCoordinate(str, &i);
    sand_count = GetSandCount(str, &i);
    sandbox->AddSandpile(x, y, sand_count, sandpiles);
  }
  file.close();
  return true;
}
