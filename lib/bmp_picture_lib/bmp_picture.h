#pragma once

#include <sandpile_model.h>
#include <cstdint>

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

bool SaveStateToBMP(const char* path, SandboxType* sandbox, uint32_t frame = 0);