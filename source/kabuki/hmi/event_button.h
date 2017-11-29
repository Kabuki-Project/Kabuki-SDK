/** Kabuki Starship
    @file    ~/Source/_hmi/Buttons/EventButton.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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


#include <KabukiTheater-Config.h>
#include "ControlLayer.h"
#include "../Button.h"
#include "ControlLayer.h"
#include "Event.h"

namespace _hmi { namespace Buttons {

class EventButton: public Button
/** A button that triggers an Event.
    A buttun Event is triggered on the press function.
*/
{
    public:
    
    EventButton  (const char* initLabel = "", int initAction = Button::Momentary);
	/*< Default constructor. */
	
    EventButton  (const EventButton& initState);
    //< Copy constructor.
    
    ~EventButton ();
    //< Destructor.

    void press  (const ControlLayer &cl);
    //< The action that gets performed when this button gets pressed.
    
    void depress  (const ControlLayer &cl);
    //< The action that gets performed when this button gets double pressed.
    
    void doublePressed  (const ControlLayer &cl);
    //< The action that gets performed when this button double pressed.

    void trigger ();
    //< Event interface implementation.
};
}   //< namespace Buttons
}   //< namespace _hmi
