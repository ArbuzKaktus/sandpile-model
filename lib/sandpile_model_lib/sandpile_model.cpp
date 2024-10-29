
#include "sandpile_model.h"
#include <bmp_picture.h>
#include <parsing_args.h>
#include <reading_file.h>
// #include "../parsing_args_lib/parsing_args.h"
// #include "../reading_file_lib/reading_file.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string.h>
#include <iostream>

void SandStack::push(int16_t x, int16_t y) {
  Cell* new_cell = new Cell;
  new_cell->x = x;
  new_cell->y = y;
  if (size == 0){
    tail = new_cell;
    head = new_cell;
  } else{
    tail->next = new_cell;
    tail = new_cell;
  }
  ++size;
}

void SandStack::pop() {
  if (head == nullptr) {
    return;
  }
  if (size == 1){
    delete head;
    head = nullptr;
    tail = nullptr;
    --size;
    return;
  }
  Cell* temp = head;
  head = head->next;
  delete temp;
  --size;
}

void SandboxType::MakeAbroad(int16_t x, int16_t y) {
  uint64_t capacity;
  uint64_t new_size;
  if (x < x_min) {
    capacity = x_min - x + kAddToCapacity;
    new_size = size_x + 2*  capacity;
  } else if (x > x_max) {
    capacity = x - x_max + kAddToCapacity;
    new_size = size_x + 2*  capacity;
  }
  if (y < y_min) {
    capacity = y_min - y + kAddToCapacity;
    new_size = size_y + 2*  capacity;
  } else if (y > y_max) {
    capacity = y - y_max + kAddToCapacity;
    new_size = size_y + 2*  capacity;
  }
  for (size_t i = 0; i != size_x; ++i) {
    if (matrix[i] == nullptr) {
      continue;
    }
    uint64_t* new_arr = new uint64_t[new_size](0);
    std::copy(matrix[i], matrix[i] + size_y, new_arr + capacity);
    delete[] matrix[i];
    matrix[i] = new_arr;
  }
  uint64_t* *new_matrix = new uint64_t* [new_size](0);
  std::copy(matrix, matrix + size_x, new_matrix + capacity);
  delete[] matrix;
  matrix = new_matrix;
  size_x = new_size;
  size_y = new_size;
  x_min -= capacity;
  y_min -= capacity;
  x_max += capacity;
  y_max += capacity;
}

void SandboxType::AddSandpile(int16_t x, int16_t y, uint64_t sand_count, SandStack* sandpiles) {
  if (x < x_min || x > x_max || y < y_min || y > y_max) {
    MakeAbroad(x, y);
  }
  int16_t index_x = x - x_min;
  int16_t index_y = y - y_min;
  if (matrix[index_x] == nullptr) {
    matrix[index_x] = new uint64_t[size_y](0);
  }
  if (matrix[index_x][index_y] >= 4){
    matrix[index_x][index_y] += sand_count;
    return;
  }
  matrix[index_x][index_y] += sand_count;
  if (matrix[index_x][index_y] >= 4){
    sandpiles->push(x, y);
  }
}


// юра ты крутой
void SandboxType::MakeScrumbling(int16_t x, int16_t y, SandStack* sandpiles) {
  int16_t index_x = x - x_min;
  int16_t index_y = y - y_min;
  uint64_t add_sand = 1;
  matrix[index_x][index_y] -= 4;
  if (matrix[index_x][index_y] >= 4){
    sandpiles->push(x, y);
  }
  AddSandpile(x + 1, y, add_sand, sandpiles);
  AddSandpile(x, y + 1, add_sand, sandpiles);
  AddSandpile(x - 1, y, add_sand, sandpiles);
  AddSandpile(x, y - 1, add_sand, sandpiles);
}

void SandpileModel(Args* args) {
  SandboxType sandbox;
  SandStack sandpiles;
  uint32_t frame_num;
  uint64_t count = 0;
  size_t sandpiles_size;
  if (!ReadFile(&sandbox, args, &sandpiles)) {
    return;
  }
  while (count != args->max_iter && sandpiles.head != nullptr) {
    sandpiles_size = sandpiles.size;
    for (size_t i = 0; i != sandpiles_size; ++i) {
      sandbox.MakeScrumbling(sandpiles.head->x, sandpiles.head->y, &sandpiles);
      sandpiles.pop();
    }
    ++count;
    if (args->freq == 0) {
      continue;
    }
    if (count % args->freq == 0) {
      uint64_t frame_num = count / args->freq;
      if (!SaveStateToBMP(args->output_path, &sandbox, frame_num)) {
        return;
      }
    }
  }
  std::cout << "Sandpile Model is done!!!" << "\n";
  std::cout << "count of iterations " << count << '\n';
  SaveStateToBMP(args->output_path, &sandbox);
}