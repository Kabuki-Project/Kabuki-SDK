/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/benchmark.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <stdafx.h>

#if SEAM >= SEAM_00_00_00__00_01
#ifndef INCLUDED_F2_BENCHMARK
#define INCLUDED_F2_BENCHMARK 1

#include "test.h"

namespace _ {

class BenchmarkCase {
 public:
  /* Constructs an object from the given values. */
  BenchmarkCase(const char* name, SeamTree* cases, int count);

  /* Runs the benchmark. */
  TestResult Run(const char* args);

  /* Gets the number of cases in this benchmark. */
  int GetCount();

 private:
  const char* name;  //< Name of this benchmark case.
  SeamTree* cases;      //< Array of pointers of BenchmarkCase(s).
  int count;         //< Number of test cases.
};

/* A Benchmark composed of one or more BenchmarkCase(s). */
class Benchmark {
 public:
  /* Constructs an object from the given values. */
  Benchmark(const char* name, const char* filename, BenchmarkCase* groups,
            int count);

  /* Runs the benchmark.
  @return If args is nil, result will contain the name as */
  TestResult Run(const char* args);

 private:
  const char *name,       //< Name of the benchmark.
      *filename;          //< Filename including .csv file extention.
  BenchmarkCase* groups;  //< Array of pointers of BenchmarkCase(s).
  int count,              //< Number of test cases.
      columns_max;        //< Max test cases in any BenchmarkCase(s).
};

}  // namespace _

#endif  //< INCLUDED_F2_BENCHMARK
#endif  //< #if SEAM >= SEAM_00_00_00__00_01
