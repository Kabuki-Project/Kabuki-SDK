/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_bout.cc
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
    @desc    Slot implementation split into three files, slot.h, 
             monoid_rx.h, and monoid_tx.h because of the large read/write
             functions.
*/

#include <stdafx.h>
#include "../include/bout.h"
#include "../include/slot.h"
#include "../include/args.h"
#include "../include/address.h"

namespace _ {

const char* BoutErrorString (Bout::Error error) {
#if USING_CONSOLE
    static const char* strings[] = {
        "Buffer overflow",  //< 0
        "Locked Error"      //< 1
        "Room Error"        //< 2
    };
    if (error < 0 || error> Bout::RoomError)
        return strings[Bout::RoomError];
    return strings[error];
#else
    return kEmptyString;
#endif
}

const char* BoutStateString (Bout::State state) {
    static const char* strings[] = {
        "WritingState",
        "LockedState"
    };
    if (state >= Bout::LockedState)
        return strings[Bout::LockedState];
    return strings[state];
}

const Operation* BoutResult (Bout* bout, Bout::Error error) {
    // @todo Write me.
    return 0;
}

const Operation* BoutResult (Bout* bout, Bout::Error error, const uint_t* header) {
    // @todo Write me.
    return 0;
}

const Operation* BoutResult (Bout* bout, Bout::Error error, const uint_t* header,
    byte offset) {
    // @todo Write me.
    return 0;
}

const Operation* BoutResult (Bout* bout, Bout::Error error, const uint_t* header,
    byte offset, byte* address) {
    // @todo Write me.
    return 0;
}

Bout* BoutInit (byte* buffer, uint_t size) {
    if (size < kMinSlotSize) return nullptr;
    if (buffer == nullptr) return nullptr;

    Bout* bout = reinterpret_cast<Bout*> (buffer);
    bout->size = size - (4 * sizeof (uint_t));
    bout->start = 0;
    bout->stop = 0;
    bout->read = 0;

#if DEBUG_SCRIPT
    memset (&bout->buffer, '\0', size);
#endif
    return bout;
}

uint_t BoutSpace (Bout* bout) {
    if (bout == nullptr) return ~0;
    byte* txb_ptr = reinterpret_cast<byte*>(bout);
    return SlotSpace (txb_ptr + bout->start, txb_ptr + bout->stop,
        bout->size);
}

uint_t BoutBufferLength (Bout* bout) {
    if (bout == nullptr) return ~0;
    byte* base = &bout->buffer;
    return SlotLength (base + bout->start, base + bout->stop, bout->size);
}

byte* BoutEndAddress (Bout* bout) {
    return reinterpret_cast<byte*>(bout) + (4 * sizeof (uint_t)) + bout->size;
}

int BoutStreamByte (Bout* bout) {

    byte* begin = &bout->buffer,
        *end = begin + bout->size;
    byte* open = (byte*)begin + bout->read,
        *start = begin + bout->start,
        *cursor = start;

    int length = start < open?open - start + 1:
        (end - start) + (open - begin) + 2;

    if (length < 1) {
        BoutResult (bout, Bout::BufferOverflowError, Params<1, STX> (), 2, start);
        return -1;
    }
    byte b = *cursor;
    bout->stop = (++cursor > end)?static_cast<uint_t> (Diff (begin, end)):
        static_cast<uint_t> (Diff (begin, cursor));
    return 0;
}

void BoutPrint (Bout* bout) {
    PrintLine ('_');
    if (bout == nullptr) {
        printf ("| Bout null\n");
        return;
    }
    uint_t size = bout->size;
    printf ("| Bout 0x%p: size: %u, start: %u, stop: %u, read: %u\n", bout, size,
        bout->start, bout->stop, bout->read);
    PrintMemory (&bout->buffer, size);
}

const Operation* BoutWrite (Bout* bout, const uint_t* params, void** args) {
    //printf ("\n\n| Writing to %p\n", tx);
    if (bout == nullptr)
        return BoutResult (bout, Bout::RoomError);
    if (params == nullptr)
        return BoutResult (bout, Bout::RoomError);
    if (args == nullptr)
        return BoutResult (bout, Bout::RoomError);
    
    // Temp variables packed into groups of 8 bytes for memory alignment.
    byte type,
        ui1;
#if USING_2_BYTE_TYPES
    uint16_t ui2;
#endif
#if USING_4_BYTE_TYPES
    uint32_t ui4;
#endif
#if USING_8_BYTE_TYPES
    uint64_t ui8;
#endif

    const uint_t num_params = *params;
    ++params;
    if (num_params == 0) return 0;          //< Nothing to do.

    uint_t size = bout->size,               //< Size of the buffer.
        space,                              //< Space in the buffer.
        index,                              //< Index in the params.
        length;                             //< Length of a type to write.
    hash16_t hash = 0;                      //< 16-bit prime hash.
    const uint_t* param = params;               //< Pointer to the current param.
    // Convert the socket offsets to pointers.
    byte* begin = &bout->buffer,            //< Beginning of the buffer.
        * end   = begin + size,             //< End of the buffer.
        * start = begin + bout->start,      //< Start of the data.
        * stop  = begin + bout->stop;       //< Stop of the data.
    const byte* ui1_ptr;                    //< Pointer to a 1-byte type.
#if USING_2_BYTE_TYPES || USING_VARINT2
    const uint16_t* ui2_ptr;                //< Pointer to a 2-byte type.
#endif
#if USING_4_BYTE_TYPES || USING_VARINT4
    const uint32_t* ui4_ptr;                //< Pointer to a 4-byte type.
#endif
#if USING_8_BYTE_TYPES || USING_VARINT8
    const uint64_t* ui8_ptr;                //< Pointer to a 8-byte type.
#endif
#if USING_AR1 || USING_AR2 || USING_AR4 || USING_AR8
    uint_t array_type;                      //< Array type for writing arrays.
#endif

    space = SlotSpace (start, stop, size);

    //std::cout << "\n| space: " << space
    //          << "\n| address: \"";

    // Check if the buffer has enough room.
    if (space == 0)
        return BoutResult (bout, Bout::BufferOverflowError);
    --space;
    length = *param;   //< Load the max char length.
    ++param;

    // Write data.
    for (index = 0; index < num_params; ++index) {
        type = *param;
        ++param;
//#if DEBUG_SCRIPT
//        printf ("\n| %i:%u:%s start: %u, stop: %u hash: ", index, type, 
//                TypeString (type), Diff (begin, start), Diff (begin, stop));
//#endif
        switch (type) {
            case NIL:
                break;

            case SOH: //< _W_r_i_t_e__A_d_d_r_e_s_s__S_t_r_i_n_g________________
            case STX: //< _W_r_i_t_e__S_t_r_i_n_g_______________________________
                if (space == 0)
                    return BoutResult (bout, Bout::BufferOverflowError, params, index,
                                       start);
                --space;
                length = *param;   //< Load the max char length.
                ++param;

                //strings = reinterpret_cast<const byte**> (args);
                //printf ("\ntestStrings at after: 0x%p\nstring1: %s\n"
                //        "string2: %s\n", args, strings[0], strings[1]);

                // Load the source data pointer and increment args.fs
                ui1_ptr = reinterpret_cast<const byte*> (args[index]);

                //printf ("Before trying to print 0x%p: %s\n", ui1_ptr, ui1_ptr);
                //print ();
                //printf ("\nWriting chars: ");

                // We know we will always write at least one null-term byte.
                ui1 = *ui1_ptr;
                *stop = ui1;
                if (++stop >= end) stop -= size;
                hash = Hash16 (ui1, hash);
                //putchar (ui1);

                while (ui1 != 0) {
                    if (space-- == 0)
                        return BoutResult (bout, Bout::BufferOverflowError, params, index,
                                           start);
                    ++ui1_ptr;
                    ui1 = *ui1_ptr;     // Read byte.
                    hash = Hash16 (ui1, hash);

                    //putchar (ui1);

                    *stop = ui1;        // Write byte
                    if (++stop >= end) stop -= size;
                }
                if (type == SOH) {
                    *stop = ui1;        // Write byte
                    if (++stop >= end) stop -= size;
                    break;
                }
                //std::cout << '\n';

                break;

            case SI1: //< _W_r_i_t_e__8_-_b_i_t__T_y_p_e_s______________________
            case UI1:
            case BOL:
#if USING_1_BYTE_TYPES
                // Check if the buffer has enough room.
                if (space-- == 0)
                    return BoutResult (bout, Bout::BufferOverflowError, params, index,
                                        start);

                // Load pointer and read data to write.
                ui1_ptr = reinterpret_cast<const byte*> (args[index]);
                ui1 = *ui1_ptr;

                // Write data.
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;
                break;
#else
                goto TxInvalidType;
#endif
            case SI2: //< _W_r_i_t_e__1_6_-_b_i_t__T_y_p_e_s____________________
            case UI2:
            case HLF:
#if USING_2_BYTE_TYPES
                // Align the buffer to a word boundary and check if the 
                // buffer has enough room.
                if (space < sizeof (uint16_t))
                    return BoutResult (bout, Bout::BufferOverflowError, params, index,
                                       start);
                space -= sizeof (uint16_t);

                // Load pointer and value to write.
                ui2_ptr = reinterpret_cast<uint16_t*> (args[index]);
                ui2 = *ui2_ptr;

                // Write data.

                // Byte 1
                ui1 = (byte)ui2;
                *stop = ui1;
                if (++stop >= end) stop -= size;
                hash = Hash16 (ui1, hash);

                // Byte 2
                ui1 = (byte)(ui2 >> 8);
                *stop = ui1;
                if (++stop >= end) stop -= size;
                hash = Hash16 (ui1, hash);
                break;
#else
                goto TxInvalidType;
#endif

            case SI4: //< _W_r_i_t_e__3_2_-_b_i_t__T_y_p_e_s____________________________
            case UI4:
            case FLT:
            case TMS:
#if USING_4_BYTE_TYPES
                // Align the buffer to a word boundary and check if the buffer
                // has enough room.

                if (space < sizeof (uint32_t))
                    return BoutResult (bout, Bout::BufferOverflowError, params, index,
                                        start);
                space -= sizeof (uint64_t);

                // Load pointer and value to write.
                ui4_ptr = reinterpret_cast<uint32_t*> (args[index]);
                ui4 = *ui4_ptr;


                // Byte 1
                ui1 = (byte)ui4;
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;

                // Byte 2
                ui1 = (byte)(ui4 >> 8);
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;

                // Byte 3
                ui1 = (byte)(ui4 >> 16);
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;

                // Byte 4
                ui4 = (byte)(ui4 >> 24);
                *stop = ui1;
                hash = Hash16 (ui1, hash);
                if (++stop >= end) stop -= size;
                break;
#else
                goto TxInvalidType;
#endif

            case TMU: //< _W_r_i_t_e__6_4_-_b_i_t__T_y_p_e_s____________________________
            case SI8:
            case UI8:
            case DBL:
#if USING_8_BYTE_TYPES
                // Align the buffer to a word boundary and check if the buffer
                // has enough room.
                if (space < sizeof (uint64_t))
                    return BoutResult (bout, Bout::BufferOverflowError, params, index,
                                        start);
                space -= sizeof (uint64_t);

                // Load pointer and value to write.
                ui8_ptr = reinterpret_cast<uint64_t*> (args[index]);
                ui8 = *ui8_ptr;

                // Write data.

                // Byte 1
                ui1 = (byte)ui8;
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 2
                ui1 = (byte)(ui8 >> 8);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 3
                ui1 = (byte)(ui8 >> 16);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 4
                ui8 = (byte)(ui8 >> 24);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 5
                ui1 = (byte)(ui8 >> 32);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 6
                ui1 = (byte)(ui8 >> 40);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 7
                ui1 = (byte)(ui8 >> 48);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;

                // Byte 8
                ui8 = (byte)(ui8 >> 56);
                hash = Hash16 (ui1, hash);
                *stop = ui1;
                if (++stop >= end) stop -= size;
                break;
#else
                goto TxInvalidType;
#endif

            case SV2: //< _W_r_i_t_e__2_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t____________

#if USING_VARINT2
                // Load number to write and increment args.
                ui2_ptr = reinterpret_cast<const uint16_t*> (args[index]);
                ui2 = *ui2_ptr;
                // We are using the same code to print both signed and unsigned 
                // varints. In order to convert from a negative 2's complement 
                // signed integer to a transmittable format, we need to invert the 
                // bits and add 1. Then we just shift the bits left one and put the
                // sign bit in the LSB.
                ui2 = PackSignedVarint<uint16_t> (ui2);
                goto WriteVarint2;
            case UV2: //< _W_r_i_t_e__2_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_
                // Load next pointer value to write.
                ui2_ptr = reinterpret_cast<const uint16_t*> (args[index]);
                if (ui2_ptr == nullptr)
                    return BoutResult (bout, Bout::RoomError, params, index, start);
                ui2 = *ui2_ptr;

                WriteVarint2:
                {
                    // Byte 1
                    if (space-- == 0)
                        return BoutResult (bout, Bout::BufferOverflowError, params, index, start);
                    ui1 = ui2 & 0x7f;
                    ui2 = ui2 >> 7;
                    if (ui2 == 0) {
                        ui1 |= 0x80;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);

                    // Byte 2
                    if (--space == 0)
                        return BoutResult (bout, Bout::BufferOverflowError, params, index, start);
                    ui1 = ui2 & 0x7f;
                    ui2 = ui2 >> 7;
                    if (ui2 == 0) {
                        ui1 |= 0x80;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);

                    // Byte 3
                    if (--space == 0)
                        return BoutResult (bout, Bout::BufferOverflowError, params, index, start);
                    ui1 = ui2 & 0x7f;
                    ui1 |= 0x80;
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);
                    break;
                }
#else
            case UV2:
                goto TxInvalidType;
#endif
            case SV4: //< _W_r_i_t_e__4_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t____________

#if USING_VARINT4
                // Load number to write and increment args.
                ui4_ptr = reinterpret_cast<const uint32_t*> (args[index]);
                ui4 = *ui4_ptr;
                ui4 = PackSignedVarint<uint32_t> (ui4);
                goto WriteVarint4;
            case UV4: //< _W_r_i_t_e__4_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t_________
                // Load the 4-byte type to write to the buffer.
                ui4_ptr = reinterpret_cast<const uint32_t*> (args[index]);
                ui4 = *ui4_ptr;
                WriteVarint4:   //< Optimized manual do while loop.
                {
                    ui2 = 5;
                    if (space == 0) //< @todo Benchmark to space--
                        return BoutResult (bout, Bout::BufferOverflowError, params, index,
                                       start);
                    --space;    //< @todo Benchmark to space--
                    ui1 = ui4 & 0x7f;
                    ui4 = ui4 >> 7;
                    if (ui4 == 0) {
                        ui1 |= 0x80;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);
                    // This wont happen I don't think.
                    //if (--ui2 == 0)
                    //    return BoutResult (VarintOverflowError, params, index, start);

                    goto WriteVarint4;
                }
#else
            case UV4:
                goto TxInvalidType;
#endif
            case SV8: //< _W_r_i_t_e__8_-_b_y_t_e__S_i_g_n_e_d__V_a_r_i_n_t_____________
#if USING_VARINT8
                // Load number to write and increment args.
                ui8_ptr = reinterpret_cast<const uint64_t*> (args[index]);
                ui8 = *ui8_ptr;
                ui8 = PackSignedVarint<uint64_t> (ui8);
                goto WriteVarint8;
            case UV8: //< _W_r_i_t_e__8_-_b_y_t_e__U_n_s_i_g_n_e_d__V_a_r_i_n_t________
                // Load the 4-byte type to write to the buffer.
                ui8_ptr = reinterpret_cast<const uint64_t*> (args[index]);
                ui8 = *ui8_ptr;
                WriteVarint8:           //< Optimized manual do while loop.
                {
                    ui2 = 8;            //< The max number of varint bytes - 1.
                    if (space <= 9)     //< @todo Benchmark to space--
                        return BoutResult (bout, Bout::BufferOverflowError, params, index,
                                       start);
                    --space;            //< @todo Benchmark to space--
                    if (--ui2 == 0) {   //< It's the last byte not term bit.
                        ui1 = ui8 & 0xFF;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    ui1 = ui8 & 0x7f;   //< Take 7 bits at a time.
                    ui8 = ui8 >> 7;     //< 
                    if (ui8 == 0) {
                        ui1 |= 0x80;
                        *stop = ui1;
                        if (++stop >= end) stop -= size;
                        hash = Hash16 (ui1, hash);
                        break;
                    }
                    *stop = ui1;
                    if (++stop >= end) stop -= size;
                    hash = Hash16 (ui1, hash);

                    goto WriteVarint8;
                }
#else
            case UV8:
                goto TxInvalidType;
#endif

            case AR1:  //< _W_r_i_t_e__A_r_r_a_y_______________________________________

#if USING_AR1
                // Load pointer to data to write and get size.
                ui1_ptr = reinterpret_cast<const byte*> (args[index]);
                if (ui1_ptr == nullptr)
                    return BoutResult (bout, Bout::RoomError, params, index, start);
                length = *param++;
                array_type = *param++;
                length *= SizeOf (array_type); //< Calculate length of array.
                goto WriteBlock;
#else
                goto TxInvalidType;
#endif
              case AR2:  //< _W_r_i_t_e__A_r_r_a_y_2______________________________
#if USING_AR2
                // Load pointer to data to write and get size.
                ui1_ptr = reinterpret_cast<const byte*> (args[index]);
                if (ui1_ptr == nullptr)
                    return BoutResult (bout, Bout::RoomError, params, index, start);
                length = *param++;
                array_type = *param++;
                length *= SizeOf (array_type); //< Calculate length of array.
                //printf ("\nlength: %u\n", length);
                goto WriteBlock;
#else
                goto TxInvalidType;
#endif
              case AR4:  //< _W_r_i_t_e__A_r_r_a_y_4______________________________
#if USING_AR4
                // Load pointer to data to write and get size.
                ui1_ptr = reinterpret_cast<const byte*> (args[index]);
                if (ui1_ptr == nullptr)
                    return BoutResult (bout, Bout::RoomError, params, index, start);
                length = *param++;
                array_type = *param++;
                length *= SizeOf (array_type); //< Calculate length of array.
                goto WriteBlock;
#else
                goto TxInvalidType;
#endif
              case AR8:  //< _W_r_i_t_e__A_r_r_a_y_8______________________________
#if USING_AR8
                // Load pointer to data to write and get size.
                ui1_ptr = reinterpret_cast<const byte*> (args[index]);
                if (ui1_ptr == nullptr)
                    return BoutResult (bout, Bout::RoomError, params, index, start);
                length = *param++;
                array_type = *param++;
                length *= SizeOf (array_type); //< Calculate length of array.
                goto WriteBlock;
#else
                goto TxInvalidType;
#endif
              case ESC: //< _W_r_i_t_e__E_s_c_a_p_e_S_e_q_u_e_n_c_e_______________________
                // @todo Write Tx ESC algorithm.
                break;
              case FS:  //< _W_r_i_t_e__6_4_-_B_i_t__B_o_o_k____________________________

# if USING_BK8
                ui8_ptr = reinterpret_cast<const uint64_t*> (args[index]);
                if (ui8_ptr == nullptr)
                    return (Bout::RoomError, params, index, start);
                ui1_ptr = reinterpret_cast<byte*>(ui8_ptr);
                // Load size.
                ui8 = *ui8_ptr;
                length = static_cast<uint_t>(ui8);
                goto WriteBlock;
#else
                goto TxInvalidType;
#endif

              case GS:  //< _W_r_i_t_e__B_o_o_k_4_______________________________________

# if USING_BK4
                ui4_ptr = reinterpret_cast<const uint32_t*> (args[index]);
                if (ui4_ptr == nullptr)
                    return BoutResult (bout, Bout::RoomError, params, index, start);
                ui1_ptr = reinterpret_cast<byte*>(ui4_ptr);
                // Load size.
                ui4 = *ui4_ptr;
                length = static_cast<uint_t>(ui4);
                goto WriteBlock;
#else
                goto TxInvalidType;
#endif

              case RS:  //< _W_r_i_t_e__B_o_o_k_2_______________________________________

# if USING_BK2
                ui2_ptr = reinterpret_cast<const uint16_t*> (args[index]);
                if (ui2_ptr == nullptr)
                    return BoutResult (bout, Bout::RoomError, params, index, start);
                ui1_ptr = reinterpret_cast<byte*>(ui2_ptr);
                // Load size.
                ui2 = *ui2_ptr;
                length = static_cast<uint_t>(ui2);
                goto WriteBlock;
#else
                goto TxInvalidType;
#endif
              case US:
#if USING_BK2 || USING_BK4 || USING_BK8
                  ui1_ptr = reinterpret_cast<const byte*> (args[index]);
                  if (ui1_ptr == nullptr)
                      return BoutResult (bout, Bout::RoomError, params, index, start);
                  length = kUnitSize;
#else
                goto TxInvalidType;
#endif
#if USING_BK2 || USING_BK4 || USING_BK8 || USING_AR1 || USING_AR2 \
              || USING_AR4 || USING_AR8
                ui1_ptr = reinterpret_cast<const byte*> (args[index]);
                if (ui1_ptr == nullptr)
                    return BoutResult (bout, Bout::RoomError, params, index, start);
                length = kUnitSize;

                WriteBlock:
                if (space < length)
                    return BoutResult (bout, Bout::BufferOverflowError, params, index, start);
                if (length == 0)
                    break;          //< Not sure if this is an error.
                if (start + length >= end) {
                    for (; size - length > 0; --length) {
                        ui1 = *(ui1_ptr++);
                        hash = Hash16 (ui1, hash);
                        *stop = ui1;
                        ++stop;
                    }
                    stop = begin - 1;
                    for (; length > 0; --length) {
                        ui1 = *(ui1_ptr++);
                        hash = Hash16 (ui1, hash);
                        *stop = ui1;
                        ++stop;
                    }
                    break;
                }
                for (; length > 0; --length) {
                    ui1 = *(ui1_ptr++);
                    hash = Hash16 (ui1, hash);
                    *stop = ui1;
                    ++stop;
                }
                break;
#endif
            default:
                TxInvalidType:
                return BoutResult (bout, Bout::RoomError, params, index, start);
        }
    }

    return 0;
}

}       //< namespace _

