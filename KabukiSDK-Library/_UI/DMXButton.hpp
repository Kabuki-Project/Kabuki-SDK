/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_UI/DMXButton.hpp
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
#include "DMXControl.hpp"
#include "ButtonControl.hpp"

namespace _UI {

class _KabukiSDK DMXButton : public DMXControl, public ButtonControl
{
    public:

    DMXButton  (const char* &newLabel = "", int newChannel = 0, int initValue = 0, int newMinValue = 0, 
        int newMaxValue = 255, int newWordSize = 8, int initAction = ButtonControl::Momentary, int newStepSize = 0, 
        double newDoublePressTime = ButtonControl::DefaultDoublePressTime);
    /*< Default constructor. */

    virtual ~DMXButton () {}
    /*< Virtual destructor. */

    int getMaxWordValue () const override;
    /*< gets the max value of a AVControl word. */

    const char* getLabel () const override;
    /*< gets the label. */

    int setLabel  (const char* newLabel) override; 
    /*< sets the label to the new label.
        @return gets 0 upon success and 1 if the newLabel is too 
            long.
        @see    Label::IsValid  (char). 
    */

    void toggle () override;
    /*< Toggles the state of the ButtonControl if type is latching. */

    void press () override {}
    /*< Triggered when a mode button is pressed. */

    void depress () override {}
    /*< Triggered when a mode button is depressed. */

    void doubleClick () override {}
    /*< Triggered when a user "double clicks" a button. */

    const char* getHeaderString () const override;
    /*< gets the header for toStringRow (). */

    const char* toStringRow () const override;
    /*< gets a column of the values without the labels. */
    
    inline void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */
};

}   //< namespace _UI


