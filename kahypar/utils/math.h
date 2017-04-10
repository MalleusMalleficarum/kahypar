/*******************************************************************************
 * This file is part of KaHyPar.
 *
 * Copyright (C) 2016 Sebastian Schlag <sebastian.schlag@kit.edu>
 * Copyright (C) 2016 Yaroslav Akhremtsev <yaroslav.akhremtsev@kit.edu>
 *
 * KaHyPar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * KaHyPar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KaHyPar.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#pragma once

#if defined(_MSC_VER)
#include <intrin.h>
#else
#include <x86intrin.h>
#endif

#include <cstddef>
#include <random>
#include <type_traits>
#include <utility>
#include <vector>

#include "kahypar/macros.h"

namespace kahypar {
namespace math {
// See: Warren, Henry S. Hacker's Delight (Second Edition), p.61
template <typename T>
static inline T nextPowerOfTwoCeiled(T n) {
  static_assert(std::is_integral<T>::value, "Integer required.");
  static_assert(std::is_unsigned<T>::value, "Incompatible type");
  --n;
  for (size_t k = 1; k != 8 * sizeof(n); k <<= 1) {
    n |= n >> k;
  }
  return n + 1;
}

template <typename T, typename U>
static inline T nearestMultipleOf(T num, U multiple) {
  return (num + multiple - 1) & ~(multiple - 1);
}

template <typename T>
inline double median(const std::vector<T>& vec) {
  double median = 0.0;
  if ((vec.size() % 2) == 0) {
    median = static_cast<double>((vec[vec.size() / 2] + vec[(vec.size() / 2) - 1])) / 2.0;
  } else {
    median = vec[vec.size() / 2];
  }
  return median;
}

// based on: http://mathalope.co.uk/2014/07/18/accelerated-c-solution-to-exercise-3-2/
template <typename T>
inline std::pair<double, double> firstAndThirdQuartile(const std::vector<T>& vec) {
  if (vec.size() > 1) {
    const size_t size_mod_4 = vec.size() % 4;
    const size_t M = vec.size() / 2;
    const size_t ML = M / 2;
    const size_t MU = M + ML;
    double first_quartile = 0.0;
    double third_quartile = 0.0;
    if (size_mod_4 == 0 || size_mod_4 == 1) {
      first_quartile = (vec[ML] + vec[ML - 1]) / 2;
      third_quartile = (vec[MU] + vec[MU - 1]) / 2;
    } else if (size_mod_4 == 2 || size_mod_4 == 3) {
      first_quartile = vec[ML];
      third_quartile = vec[MU];
    }
    return std::make_pair(first_quartile, third_quartile);
  } else {
    return std::make_pair(0.0, 0.0);
  }
}

template <typename T>
static inline T crc32(const T& x) {
  return _mm_crc32_u32((size_t)28475421, x);
}

template <typename T>
static inline T identity(const T& x) {
  return x;
}

template <typename T>
static inline T cs2(const T& x) {
  return x * x;
}

template <typename T>
static inline T hash(const T& x) {
  return cs2(x);
}

template <typename Key>
class MurmurHash {
 public:
  using HashValue = uint64_t;

  explicit MurmurHash(const uint32_t seed = 0) :
    _seed(seed)
  { }

  void reset(const uint32_t seed) {
    _seed = seed;
  }

  inline HashValue operator() (const Key& key) const {
    return hash(reinterpret_cast<const void*>(&key), sizeof(key), _seed);
  }

 private:
  inline uint64_t hash(const void* key, const uint32_t len, const uint32_t seed) const {
    const uint64_t m = 0xc6a4a7935bd1e995;
    const int r = 47;

    uint64_t h = seed ^ (len * m);

    const uint64_t* data = (const uint64_t*)key;
    const uint64_t* end = data + (len / 8);

    while (data != end) {
      uint64_t k = *data++;

      k *= m;
      k ^= k >> r;
      k *= m;

      h ^= k;
      h *= m;
    }

    const unsigned char* data2 = (const unsigned char*)data;

    switch (len & 7) {
      case 7: h ^= uint64_t(data2[6]) << 48;
      case 6: h ^= uint64_t(data2[5]) << 40;
      case 5: h ^= uint64_t(data2[4]) << 32;
      case 4: h ^= uint64_t(data2[3]) << 24;
      case 3: h ^= uint64_t(data2[2]) << 16;
      case 2: h ^= uint64_t(data2[1]) << 8;
      case 1: h ^= uint64_t(data2[0]);
        h *= m;
    }

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
  }

  uint32_t _seed;
};

/*#define XXH_PRIVATE_API
#include "xxhash.h"
template <typename Key>
struct XXHash {
  using HashValue = uint64_t;

  KAHYPAR_ATTRIBUTE_ALWAYS_INLINE uint64_t operator() (const Key k) const {
    return XXH64(&k, sizeof(Key), _seed);
  }

  void reset(const uint64_t seed) {
    _seed = seed;
  }

 private:
  uint64_t _seed;
};*/
}  // namespace math
}  // namespace kahypar
