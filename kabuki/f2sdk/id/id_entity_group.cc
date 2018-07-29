/** kabuki::pro
    @file    $kabuki-toolkit/library/kt/id/id_entity_group.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#include <stdafx.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#include "../entity_group.h"

namespace _ {

EntityGroup::EntityGroup(const char* name)
    : name_(StringClone(name == nullptr ? "" : name)) {}

const char* EntityGroup::GetName() { return name_; }

void EntityGroup::SetName(const char* string) {
  name_ = StringClone(string);
}

void EntityGroup::ApplyPrivilege(const char* privileges) {
  // for (int i = 0; i < base.getNumAccounts (); i++)
  //    accounts[i].Role ().ApplyPrivileges (new_privileges);
}

int EntityGroup::Search(const char* string) { return 0; }

void EntityGroup::Print(Log& log) { log << "Group: " << name_ << " "; }

}       // namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2