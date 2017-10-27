/** kabuki::id
    @file    ~/source/kabuki/id/include/password.h
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

#ifndef KABUKI_ID_PASSWORD_H
#define KABUKI_ID_PASSWORD_H

#include "module_config.h"

namespace kabuki { namespace id {

/** An account password. 
    @todo Add salt!
*/
class KABUKI Password {
    public:
    
    enum {
        kMinLength      = 4,   //< The minimum length of a password.
        kMaxLength      = 255, //< The maximum length of a password.
    };

    /** Attempts to create a password from the given password with the default format. 
        If the password does not match the default format, the default password will be used.
    */
    Password (const char* password = "Password");

    /** Gets the password key. */
    const char* GetKey ();

    /** Attempts to set the password and returns a non-zero error message upon failure. */
    const char* SetKey (const char* password);

    /** Gets true if the given password is valid. */
    static bool IsValid (const char* password);

    /** Returns true if this password equals the given one. */
    bool Equals (const char* string);

    /** Returns true if this password equals the given one. */
    bool Equals (const Password& p);

    /** Prints this object to a expression. */
    void Print (_::Log& log);

    private:

    char*    password_; //< Unencrypted password.
};

}       //< namespace id
}       //< namespace kabuki
#endif  //< KABUKI_ID_PASSWORD_H
