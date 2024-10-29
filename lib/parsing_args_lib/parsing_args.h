#pragma once

#include <cstddef>
#include <cstdint>

struct Args {
  const char* input_path = nullptr;
  const char* output_path = nullptr;
  int64_t max_iter = -1;
  uint32_t freq = 0;
};

bool ReadAllArgs(Args* args, char* *argv, size_t argc);