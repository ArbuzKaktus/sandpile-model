#include "bmp_picture.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sandpile_model.h>


const Color palette[5] = {
    {255, 255, 255}, {0, 255, 0}, {128, 0, 128}, {255, 255, 0}, {0, 0, 0}};

uint8_t GetColorIndex(uint64_t value) {
  if (value > 3) {
    value = 4;
  }
  return value;
}

void Makebytes(char* str, int32_t value, size_t count) {
  const int32_t byte = 256;
  for (size_t i = 0; i != count; ++i) {
    str[i] = value % byte;
    value = value >> 8;
  }
}

bool SaveStateToBMP(const char* path, SandboxType* sandbox, uint32_t frame) {
  const size_t kOutputPathSize = 300;
  const size_t kNumSize = 20;
  const size_t kBmpHeaderSize = 14;
  const size_t kDibHeaderSize = 40;
  const size_t kColorTableSize = 16;
  const int32_t kResolution = 2835;
  const int32_t kPlanes = 1;
  const int32_t kBitsInPixel = 4;
  const int32_t kCompression = 0;
  const int32_t kImportantColors = 0;
  const int32_t kColors = 16;
  uint64_t width = sandbox->size_y;
  uint64_t height = sandbox->size_x;
  int32_t row_size = ((width + 1) / 2 + 3) & (~3);
  int32_t file_size =
      kBmpHeaderSize + kDibHeaderSize + kColorTableSize + row_size*  height;
  int32_t image_size = row_size*  height;
  int32_t offset = kBmpHeaderSize + kDibHeaderSize + kColorTableSize;
  char* header_str = new char[4];
  char* header_str2 = new char[2];
  char* row_str = new char[row_size](0);
  char filename[kOutputPathSize](0);
  char num[kNumSize](0);

  if (!std::filesystem::exists(path)) {
    std::filesystem::create_directories(path);
  }
  if (frame == 0) {
    char out[] = {'f', 'i', 'n', 'a', 'l', '\0'};
    strcat_s(filename, path);
    strcat_s(filename, out);
    strcat_s(filename, ".bmp\0");
  } else {
    char out[] = {'f', 'r', 'a', 'm', 'e', '_', '\0'};
    sprintf(num, "%d", frame);
    strcat_s(filename, path);
    strcat_s(filename, out);
    strcat_s(filename, num);
    strcat_s(filename, ".bmp\0");
  }
  std::ofstream file;
  file.open(filename, std::ios::out | std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "ERROR While writing bmp picture";
    return false;
  }
  file.put('B').put('M');
  Makebytes(header_str, file_size, 4);
  file.write(header_str, 4);
  file.write("\0\0\0\0", 4);
  Makebytes(header_str, offset, 4);
  file.write(header_str, 4);
  Makebytes(header_str, kDibHeaderSize, 4);
  file.write(header_str, 4);
  Makebytes(header_str, width, 4);
  file.write(header_str, 4);
  Makebytes(header_str, height, 4);
  file.write(header_str, 4);
  Makebytes(header_str2, kPlanes, 2);
  file.write(header_str2, 2);
  Makebytes(header_str2, kBitsInPixel, 2);
  file.write(header_str2, 2);
  Makebytes(header_str, kCompression, 4);
  file.write(header_str, 4);
  Makebytes(header_str, image_size, 4);
  file.write(header_str, 4);
  Makebytes(header_str, kResolution, 4);
  file.write(header_str, 4);
  Makebytes(header_str, kResolution, 4);
  file.write(header_str, 4);
  Makebytes(header_str, kColors, 4);
  file.write(header_str, 4);
  Makebytes(header_str, kImportantColors, 4);
  file.write(header_str, 4);
  for (int i = 0; i != 5; ++i) {
    file.put(palette[i].b).put(palette[i].g).put(palette[i].r).put(0);
  }
  for (int i = 5; i != 16; ++i) {
    file.write("\0\0\0\0", 4);
  }
  for (int x = 0; x != height; ++x) {
    char* row_data = new char[row_size];
    for (int y = 0; y < row_size; ++y) {
      row_data[y] = 0;
    }
    for (int y = 0; y != width; y += 2) {
      uint8_t first_pixel = 0;
      uint8_t second_pixel = 0;
      if (sandbox->matrix[y] != nullptr) {
        first_pixel = GetColorIndex(sandbox->matrix[y][height - x - 1]);
      }
      if (y + 1 < width) {
        if (sandbox->matrix[y + 1] != nullptr) {
          second_pixel = GetColorIndex(sandbox->matrix[y + 1][height - x - 1]);
        }
      }
      uint8_t byte = (first_pixel << 4) | second_pixel;
      row_data[y >> 1] = byte;
    }
    file.write(row_data, row_size);
  }
  file.close();
  return true;
}