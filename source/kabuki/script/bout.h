/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/bout.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef HEADER_FOR___BOUT
#define HEADER_FOR___BOUT

#include "operation.h"

namespace _ {

/** A*B B-Output socket with a packed byte stream.
    A sequence of A*Bout expressions terminated by an ASCII CR char. All 
    sequences get evaluated down to an immutable set (by definition all sets are
    immutable), which can be an empty set.

    # Slot Format
    By default, all sequences are a set of one byte, all sets are linear, and 
    all sets may repeat some number of times.

    @code
    ESC VU8 1 BS CR
    @endcode
    A B-Output ring-buffer is identical in structure to an Bout ring-buffer, but
    the stop becomes volatile and start is not volatile. */
struct KABUKI Bout {

    /** List of B-Output Errors. */
    typedef enum {
        BufferOverflowError = 0,
        LockedError,
        kErrorRoom,
    } Error;

    /** List of the Finite Bout States. */
    typedef enum TxStates {
        WritingState = 0,             //< TxState 0: Most common state.
        LockedState,                  //< TxState 1: Locked.
    } State;

    uint_t size;           //< Size of the B-Output.
    volatile uint_t start; //< Starting index of the ring-buffer data.
    uint_t stop,           //< Stopping index of the ring-buffer data.
           read;           //< Address that the Bout device is reading from.
};

/** Get's the B-Output's buffer.*/
KABUKI byte* BoutBuffer (Bout* bout);

#if SCRIPT_USING_TEXT
KABUKI const char* BoutError (Bout::Error error);

/** Gets a a char for printing out the bout_state. */
KABUKI const char* BoutState (Bout::State state);

inline const char* BoutState (byte state) {
    return BoutState ((Bout::State)state);
}
#endif  //< SCRIPT_USING_TEXT

/** Used to return an erroneous result from a B-Output.

    @param error The error type.
    @return Returns a Static Error Operation Result. */
inline const Operation* BoutResult (Bout* bout, Bout::Error error) {
#if SCRIPT_DEBUG
    Text txt;
    txt << "\n| Bout " << BoutError (error) << " Error!" << txt.Print ();
#endif
    return reinterpret_cast<const Operation*> (1);
}

/** Used to return an erroneous result from a B-Output.
    @param  bout    The source Bout.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
inline const Operation* BoutResult (Bout* bout, Bout::Error error,
                                    const uint_t* header) {
#if SCRIPT_DEBUG
    std::cout << "\n| Bout " << BoutError (error) << " Error!";
#endif  //< MEMORY_PROFILE >= USE_MORE_ROM
    return reinterpret_cast<const Operation*> (1);
}

/** Used to return an erroneous result from a B-Output.
    @param  bout    The source Bout.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
inline const Operation* BoutResult (Bout* bout, Bout::Error error,
                                    const uint_t* header,
                                    uint_t offset) {
#if SCRIPT_DEBUG
    std::cout << "\n| Bout " << BoutError (error) << " Error!";
#endif  //< MEMORY_PROFILE >= USE_MORE_ROM
    return reinterpret_cast<const Operation*> (1);
}

/** Used to return an erroneous result from a B-Output.
    @param  bout    The source Bout.
    @param  error   The error type.
    @param  header  The B-Sequence Header.
    @param  offset  The offset to the type in error in the B-Sequence.
    @param  address The address of the byte in error.
    @return         Returns a Static Error Operation Result. */
inline const Operation* BoutResult (Bout* bout, Bout::Error error,
                                    const uint_t* header,
                                    uint_t offset,
                                    byte* address) {
#if SCRIPT_DEBUG
    std::cout << "\n| Bout " << BoutError (error) << " Error!";
#endif  //< MEMORY_PROFILE >= USE_MORE_ROM
    return reinterpret_cast<const Operation*> (1);
}

/** Initializes the B-Output buffer with the given buffer size. */
KABUKI Bout* BoutInit (uintptr_t* buffer, uint_t size);

/** Calculates the space left in the given ring buffer.
    @param  bout The B-Output buffer. */
KABUKI uint_t BoutSpace (Bout* bout);

/** Gets the B-Output. */
KABUKI uint_t BoutBufferLength (Bout* bout);

/** Gets the end address of the tx buffer. */
KABUKI byte* BoutEndAddress (Bout* bout);

/** Streams a B-Output byte.
    @param bout A B-Output abstract byte stream. */
KABUKI int BoutStreamByte (Bout* bout);

/** Writes a message with the given params to the given B-Output slot.
    @param bout   The B-Output socket to write to.
    @param params The escape sequence to write.
    @param args   The array of pointers to the stuff to write. */
KABUKI const Operation* BoutWrite (Bout* bout, const uint_t* params, 
                                   void** args);

/** Reads a message with the given params from the given B-Output slot.
    @param bout   The B-Output socket to write to.
    @param params The escape sequence to write.
    @param args   The array of pointers to the stuff to write. */
KABUKI const Operation* BoutRead (Bout* bout, const uint_t* params, 
                                  void** args);

/** Sends a connection message to the given address. */
KABUKI const Operation* BoutConnect (Bout* bout, const char* address);

/** Sends a connection message to the given address. */
KABUKI void BoutRingBell (Bout* bout, const char* address);

/** Sends a connection message to the given address. */
KABUKI void BoutAckBack (Bout* bout, const char* address);

#if SCRIPT_USING_TEXT
/** Prints the Bin to the Text.
    @param  bin The pin to print.
    @param  txt The Text to print the bin to.
    @return The txt. */
KABUKI Text& BoutPrint (Bout* bout, Text& txt);
#endif  //< SCRIPT_USING_TEXT

}       //< namespace _

#if SCRIPT_USING_TEXT
/** Prints out the bin to the txt. */
inline _::Text& operator<< (_::Text& txt, _::Bout* bout) {
    return txt << _::BoutPrint (bout, txt);
}
#endif  //< SCRIPT_USING_TEXT
#endif  //< HEADER_FOR___BOUT
