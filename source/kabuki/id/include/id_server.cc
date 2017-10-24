/** kabuki::pro
    @file    ~/source/kabuki/id/imp/id_server.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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
#include "../include/id_server.h"

namespace kabuki { namespace pro {

IdServer::IdServer ():
    num_events_ (0)
{
}

IdServer::~IdServer ()
{
}

int IdServer::NumEvents ()
{
    return num_events_;
}

int IdServer::RegisterKey (char* key)
{
    if (strlen (key) > MAX_KEY_LENGTH)
        return -1;
    return ids_.Add (key);
}

int IdServer::RegisterKey (const char* key)
{
    return RegisterKey (CloneString (key));
}

int IdServer::Find (const char* key)
{
    // @todo Replace me with hash table!
    for (int i = 0; i < ids_.GetCount (); ++i)
        if (!strcmp (key, ids_.Element (i)))
            return i;
    return -1;
}

int IdServer::RegisterEvent ()
{
    return num_events_++;
}

void IdServer::print ()
{
    std::cout << "\n| Id Server:"
              << "\n| num_events: " << num_events_
              << "\n| Registered Handles: \n";
    for (int i = 0; i < ids_.GetCount (); ++i)
        std::cout << "| " << i << ": " << ids_[i] << '\n';
}

}       //< id
}       //< kabuki