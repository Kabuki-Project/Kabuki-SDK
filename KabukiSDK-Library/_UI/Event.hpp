/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_UI/Event.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/
 
#pragma once

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>

namespace _UI {

/* A event with a timestamp.
   An example of an Event is typically like a Control or menu funciton.
 */
class Event
{
    public:

    Event () : _timestamp  (0.0) {}
    //< Default constructor.
    
    virtual ~Event () {}
    //< Virtual destructor.

    double timestamp () const { return timestamp; }
    //< gets the timestamp of the Event.

    virtual void trigger () = 0;
    //< Triggers the event.
    
    void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */

    private:

    Timestamp timestamp;    //< The timestamp in milliseconds of the event.
};
}   //< namespace _UI

