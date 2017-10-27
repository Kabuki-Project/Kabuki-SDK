/** kabuki::id
    @file    ~/source/kabuki/id/include/account.h
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

#pragma once
#include <stdafx.h>

#ifndef KABUKI_ID_ACCOUNT_H
#define KABUKI_ID_ACCOUNT_H

#include "roster.h"
#include "handle.h"
#include "password.h"
#include "user.h"

namespace kabuki { namespace id {

/** An account with a char and password. 
    @todo Integrate with OS accounts.
*/
class KABUKI Account
{
    public:
    
    /** Constructs an Account with the given char. */
    Account (const char* username);

    /** Constructs an account from the given char and password. */
    Account (const char* username, const char* password);
    
    /** Returns true if this is a valid account. */
    bool IsValid ();
    
    /** Gets a reference to the name char.  */
    const char* GetName ();
    
    /** Sets the name to the char. */
    bool SetName (const char* string);

    /** Returns true if this account requires a password. */
    bool RequiresPassword ();
    
    /** Sets the password to the new char. */
    void SetPassword (const char* string);

    /** Gets a reverence to the user list. */
    Roster& GetUsers ();

    /** Allows another user to be attached to this account. */
    void AddUser (User& user);

    /** Prints this object to the log. */
    void Print (_::Log& log);

    private:
    
    Handle   name_;     //< Account handle/name.
    Password password_; //< Admin password for this account.
    Roster   users_;    //< Account Roster describes User Roles.
};

}       //< namespace id
}       //< namespace kabuki
#endif  //< KABUKI_ID_ACCOUNT_H
