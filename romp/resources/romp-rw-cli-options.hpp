#pragma once

#include <cstddef>
#include <cstdint>
#include <gmpxx.h>
#include <string>
#include <vector>


enum trace_category_t { // to do for trace
  TC_VERBOSE
  TC_TRIPPED_STATES
  TC_RW_PATH
};

struct Options {
  u_int16_t threads = 0; // mpz_class ??
  u_int16_t depth = 0;//what has to be default value  ??
  u_int16_t random_walkers; //any default value needed if not provided ?
  u_int32_t rand_seed; 
};

extern Options options;

// input model's path
//extern std::string input_filename;
