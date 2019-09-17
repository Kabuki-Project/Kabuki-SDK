/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki.toolkit.git
@file    /tests/pch.cc
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-9 Cale McCollough; all right reserved (R).
This Source Code Form is subject to the terms of the Mozilla Public License,
v. 2.0. If a copy of the MPL was not distributed with this file, You can
obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <pch.h>

#if SEAM != SEAM_N

#include "gui/tests/all.inl"
#include "hypertext/tests/all.inl"
#include "pro/tests/all.inl"
#include "tek/tests/all.inl"
#include "touch/tests/all.inl"
#include "who/tests/all.inl"

using kabuki::toolkit;

int main(int arg_count, char** args) {
  enum { kSize = 1024 };
  char _log[kSize];
  return ::_::TestTree<who::Test>(arg_count, args, _log, kSize);
}
#endif
