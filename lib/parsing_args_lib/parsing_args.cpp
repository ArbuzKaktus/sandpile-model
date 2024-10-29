#include "parsing_args.h"
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>

bool IsNum(const char* arg) {
  for (size_t i = 0; arg[i] != '\0'; ++i) {
    if (!std::isdigit(arg[i])) {
      std::cerr << "Invalid value of Argument" << std::endl;
      return false;
    }
  }
  return true;
}

bool ReadInput(Args* args, char* *argv, size_t argc, size_t i) {
  if (!strcmp(argv[i], "--input") || !strcmp(argv[i], "-i")) {
    if (argv[i + 1][0] == '-') {
      std::cerr << "Invalid input path" << std::endl;
      return false;
    }
    args->input_path = argv[i + 1];
    return true;
  }
  return true;
}

bool ReadOutput(Args* args, char* *argv, size_t argc, size_t i) {
  if (!strcmp(argv[i], "--output") || !strcmp(argv[i], "-o")) {
    if (argv[i + 1][0] == '-') {
      std::cerr << "Invalid output path" << std::endl;
      return false;
    }
    args->output_path = argv[i + 1];
    return true;
  }
  return true;
}

bool ReadMaxIteration(Args* args, char* *argv, size_t argc, size_t i) {
  if (!strcmp(argv[i], "--max-iter") || !strcmp(argv[i], "-m")) {
    if (!IsNum(argv[i + 1])) {
      return false;
    }
    int64_t value = atoi(argv[i + 1]);
    if (value < 0) {
      std::cerr << "Value of argument must be positive" << std::endl;
      return false;
    }
    args->max_iter = value;
    return true;
  }
  return true;
}

bool ReadFreq(Args* args, char* *argv, size_t argc, size_t i) {
  if (!strcmp(argv[i], "--freq") || !strcmp(argv[i], "-f")) {
    if (!IsNum(argv[i + 1])) {
      return false;
    }
    int64_t value = atoi(argv[i + 1]);
    if (value < 0) {
      std::cerr << "Value of argument must be positive" << std::endl;
      return false;
    }
    args->freq = value;
    return true;
  }
  return true;
}

bool ReadAllArgs(Args* args, char* *argv, size_t argc) {
  if (argc % 2 != 1) {
    std::cerr << "Not enough arguments for start programm" << std::endl;
    return false;
  }
  for (size_t i = 1; i != argc; i += 2) {
    if (ReadInput(args, argv, argc, i)*  ReadOutput(args, argv, argc, i)* 
            ReadFreq(args, argv, argc, i)* 
            ReadMaxIteration(args, argv, argc, i) == 0) {
      return false;
    }
  }
  return true;
}
