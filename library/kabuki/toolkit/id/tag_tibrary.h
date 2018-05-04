/** Kabuki Toolkit
    @file    ~/source/id/tag_library.h
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

#ifndef KABUKI_ID_TAGLIBRARY_H
#define KABUKI_ID_TAGLIBRARY_H

#include "tag.h"
#include "../../data/array.h"

namespace kabuki { namespace id {

using namespace kabuki;

/** A container of strings sorted alphabetically.
    This object owns the memory for the strings. Each time a patch is added,
    each tag is added, a pointer to the char is passed back.
*/
class TagLibrary
{
    public:

    /** Creates an empty tag library. */
    TagLibrary ();

    /** Gets the tag char, and adds it to the collection if it doesn't exist.
        @return Gets null if the tags list doesn't contain the Tag, and 
            non-null if the Tag was added successfully. */
    char GetOrAddTag (char tag);
    
    /** Sorts the tags alphabetically for fast binary search. */
    void Sort ();
    
    /** Gets the number of tags. */
    int GetNumTags ();

    /** Prints this object to a Expression. */
    _::Text& Print (_::Text& txt);
    
    private:
    
    data::Array<id::Tag*> tags; //< Collection of tag strings.
};
}       //< namespace id
}       //< namespace kabuki
#endif  //< KABUKI_ID_TAGLIBRARY_H