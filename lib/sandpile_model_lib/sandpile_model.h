#pragma once

#include <parsing_args.h>
#include <cstddef>
#include <cstdint>


struct Cell {
  int16_t x;
  int16_t y;
  Cell* next = nullptr;
};

struct SandStack {
  Cell* head = nullptr;
  Cell* tail = nullptr;
  size_t size = 0;
  void push(int16_t x, int16_t y);
  void pop();
};

struct SandboxType {
  const size_t kStartSize = 100;
  const size_t kAddToCapacity = 64;
  int16_t size_x = kStartSize;
  int16_t size_y = kStartSize;
  int16_t x_max = kStartSize / 2;
  int16_t y_max = kStartSize / 2;
  int16_t x_min = -(kStartSize / 2 - 1);
  int16_t y_min = -(kStartSize / 2 - 1);
  int16_t centre_x = size_x / 2;
  int16_t centre_y = size_y / 2;
  uint64_t* *matrix = new uint64_t* [size_x](0);
  void MakeAbroad(int16_t x, int16_t y);
  void AddSandpile(int16_t x, int16_t y, uint64_t sand_count, SandStack* sandpiles);
  void MakeScrumbling(int16_t x, int16_t y, SandStack* sandpiles);
};

void SandpileModel(Args* args);
