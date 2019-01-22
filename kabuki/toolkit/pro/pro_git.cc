/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/pro/pro_git.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_PRO_1
#include "cgit.h"

namespace _ {

Git::Git() : reserved(0) {}

const Op* Git::Star(wchar_t index, Expr* expr) {
  void* args[1];
  enum { kSize = 254 };
  CH1 buffer[kSize + 1];
  switch (index) {
    case '?': {
      static const Op kThis = {"Sloth", OpFirst(' '), OpLast(' '), "git",
                               '}',     ';',          ' ',         false,
                               nullptr, nullptr,      nullptr};
      return &kThis;
    }
    case ' ': {
      static const Op kClone = {
          "clone", Params<1, TKN, 255>(),
          0,       "Push Operation that occurs upon updating the toolkit.",
          '}',     ';',
          ' ',     false,
          0,       0,
          0};
      if (!expr) return &kClone;
      Print(buffer, buffer + 255, "git clone ");
      if (!ExprArgs(expr, kClone, Args(args, &buffer[10]))) return expr->result;
      system(&buffer[9]);
      return 0;
    }
  }
  return nullptr;
}

}  //< namespace _
#endif
