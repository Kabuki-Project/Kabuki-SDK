/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/text.h
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

#ifndef HEADER_FOR_SCRIPT_TEXT
#define HEADER_FOR_SCRIPT_TEXT

#include "strand.h"

#if SCRIPT_USING_TEXT

namespace _ {

/** A group of zero or more UTF-8 or ASCII strings.
    Text is intended to work similar to the std::cout using overloaded
    operator<<. The class uses a statically allocated buffer allowing you to
    put it on the stack. The default size is 141 to allow for 140 char_t(s).
*/
class Text {
    public:

    enum {
        kMixSize = 16,                   //< Min buffer size.
        kSize    = SCRIPT_TEXT_SIZE_MAX, //< Size of the Text buffer.
    };

    /** Constructor creates a Text from the given strand. */
    Text (const char* strand = "");

    /** Constructor copies the other Text buffer up the cursor. */
    Text (const Text& other);

    /** Clears the text buffer. */
    void Clear ();

    /** Gets the char count of the buffer in bytes. */
    int Length () const;

    /** Gets the char count of the buffer in bytes. */
    int Space () const;

    /** Deep copies the state of the other object. */
    void Clone (const Text& other);
    
    /** Gets a pointer to the beginning of the buffer. */
    char* GetBegin ();

    /** Gets a pointer to the current write location in the buffer_. */
    char* GetCursor ();

    /** Gets a pointer to the end of the buffer. */
    char* GetEnd ();

    /** Sets the cursor to the new value. 
        @return Returns false if the new_cursor is out of bounds. */
    bool SetCursor (char* new_cursor);

    /** Writes the given char to the buffer. */
    Text& Char (char c);

    /** Prints the given token a specified num_times. */
    Text& Line (char token = '-', const char* left_column_text = "\n|",
                int num_columns = 80);

    /** Prints an 80-char line of the char repeating with an underscore
        i.e. s_s_... */
    Text& Line (const char* strand, int num_columns = 80);

    /** Prints a bunch of new lines followed by the given message and a line of
    the specified token. */
    Text& LineBreak (const char* message, int top_bottom_margin,
                          char c = '-', int num_columns = 80);

    /** Prints a vertical tab with the given number of rows. */
    Text& Lines (int numRows = 10);

    /** Print's out the given pointer to the console. */
    Text& Pointer (const void* pointer);

    Text& Error (const char* message, const char* end_string = TextCR ());

    /** Prints the given byte in Hex.
        This function prints the hex in big endian. */
    Text& Hex (byte c);

    /** Print's out the given pointer to the console. */
    Text& Pointer (void* pointer);

    /** Prints the given value in hex format. */
    template<typename Type>
    Text& Hex (Type value) {
        char buffer[sizeof (Type) * 2 + 1];
        sprintf_s (buffer, "%x", &value);
        std::cout << "0x";

        int length = StrandLength (buffer);
        for (int i = 0; i < length; ++i)
            std::cout << '0';
        std::cout << buffer;
    }

    /** Prints an 80-char line of the number repeating with an underscore
    i.e. 1_1_... */
    Text& NumberLine (int index);

    /** Creates the format char for a key-value pair where the key is
        right-aligned to the column_width.
            @depreciated */
    //char PrintCreateKeyValueFormatText (char* string, char column_width,
    //                                    char type);
    /** 
    template<char kColumnWidth>
    Text& Hex (const char* header, Text&* value) {
        char formatText[12],
            nil_term_char = PrintCreateKeyValueFormatText (formatText,
                                                          kColumnWidth, 'p');
        printf (formatText, header, value);
    }*/

    /** 
    template<char kColumnWidth, typename Type>
    Text& SignedHex (const char* header, Type value) {
        char formatText[12],
            nullTermChar = PrintCreateKeyValueFormatText (formatText, kColumnWidth,
                                                          'i');
        printf ("formatText: %s\n", formatText);
        printf (formatText, header, value);
    }*/

    /** 
    template<char kColumnWidth, typename Type>
    Text& UnsignedHex (const char* header, Type value) {
        char formatText[12],
            nullTermChar = PrintCreateKeyValueFormatText (formatText, kColumnWidth,
                                                          'u');
        printf (formatText, header, value);
    }*/

    /** Prints the given value to the console and prompts the user to press any key
        to continue. 
    template<typename Type, const char* format>
    Type PrintReturn (Type value) {
        printf (format, value);
        return value;
    }*/

    /** Prints the array starting at the base_ptr with the given numElements with a
        header and given format. 
    template<typename Type>
    Text& Array (const char* header,
                 const char* format,
                 Type* base_ptr,
                 Type numElements) {
        PrintLine ();
        std::cout << header << ": numElements: " << numElements << '\n';
        for (Type i = 0; i < numElements; ++i) {
            printf (format, i);
            std::cout << ": ";
            printf (format, base_ptr[i]);
            std::cout << '\n';
        }
        std::cout << '\n';
        PrintLine ();
    }*/

    /** Prints an error message and pauses the system. */
    //Text& Pause (const char* string);

    /** Prints a new line. */
    Text& NewLine ();

    /** Prints the given char with a '|' at the beginning followed by a new line */
    Text& Bar (const char* input);

    /** Prints a line break. */
    Text& Break (const char* header = "\n_",
                             char c = '_',
                             int num_lines = 0,
                             int console_width = 80);

    /** Prints a line break with the given number of columns. */
    Text& ColumnBreak (int num_columns,
                                   char column_delimeter = '|',
                                   char break_char = '-',
                                   int width = 80);

    /** Prints the given char centered with a horizontal page bar to the left
        and right of the row.
        @param input The char to print.
        @param num_columns */
    Text& Centered (const char* input, int num_columns);

    /** Prints the given strand centered about the given width in console char
        rows.
        If char is too small to fit in the width, function will print as much of
        the char as it has room for with a "..." If the given width is less than
        5 then only dots will be printed. */

    /** Prints the given char centered. */
    Text& Centered (const char* input, int width, bool is_last,
                    char column_delimeter = '|');

    /** Prints the given char justified right.
        @param input The char to print.
        @param num_columns The number of columns per row. */
    Text& Right (const char* input, int num_columns);

    /** Prints the given input indented and bulleted with and '|' at the beginning
        and end of the line representing a page.

        @code
        print ("List Item 1", 0
        @endcode

        @param input The input to print.
        @param indent The number of tabs indent level.
        @param bullet The type of bullet to print.
        @param index  The index of the bullet in the list.
        @param tab_size The number of spaces per tab.
        @param num_columns The number of columns per line. */
    Text& Page (const char* input = "", int indentation = 0,
                char bullet = '*', int index = 0, int tab_size = 4,
                int num_columns = 80);

    /** Prints out the contents of the address to the debug stream. */
    Text& Memory (const void* address, const void* end);

    /** Prints out the contents of the address to the debug stream. */
    Text& Memory (const void* address, int size);

    /** Prints out the contents of the address to the debug stream. */
    //inline Text& Memory (const void* address, int num_bytes) {
    //    return Memory (address,
    //                   reinterpret_cast<const byte*> (address) + num_bytes);
    //}

    /** Prints out the contents of the address to the debug stream. */
    //inline Text& Memory (const void* address, uint num_bytes) {
    //    return Memory (address,
    //                   reinterpret_cast<const byte*> (address) + num_bytes);
    //}


    /** Prints the first whitespace or nil terminated token from the string.
        @param strand A nil-terminated string in ROM.
        @return A pointer to the end of the token or nil upon failure. */
    Text& Token (const char* strand);

    /** Prints the first whitespace or nil terminated token from the text.
        @param text     A nil-terminated string in RAM or ROM.
        @param strand_end End of the text buffer.
        @return A pointer to the end of the token or nil upon failure. */
    Text& Token (const char* text, const char* strand_end);

    /** Prints the given tm struct to the console. */
    Text& TimeStruct (tm* std_tm);

    /** Prints the given tm struct to the console. */
    Text& TimeStruct (tm* std_tm, int32_t microseconds);

    /** Prints the given time to the console. */
    Text& Time (time_t t);

    /** Prints the given Script Timestamp to the console. */
    Text& Timestamp (time_us_t timestamp);

    /** Prints the given time to the console. */
    Text& DateTime (time_t t);

    /** Prints out an error report to the stdout.
        @param e The error type.
        @param params      The parameter header.
        @param param_index The index in the params where the error occurred.
        @param source      The source buffer address.
    Text& Print (Expression::Error error, const uint_t* params, 
                 byte param_index, void* source); */

    /** Prints this Text to the stdout.
        @return Reference to this. */
    Text& Print ();

    /** Prints this Text to the stderr.
    @return Reference to this. */
    Text& Error ();

    /** Overloaded operator= clones the other Text. */
    Text& operator= (const Text& other);

    private:

    char* cursor_;        //< Buffer write cursor.
    char  buffer_[kSize]; //< Text buffer.

};  //< struct Text

    /** Prints the Text to the stdout. */
void Print (const char* strand);

/** Prints the Text to the stdout. */
void Print (Text& txt);

/** Prints the Text to the stdout. */
void Error (Text& txt);

}   //< namespace _

/** Operation << writes a nil-terminated UTF-8 or ASCII string to the 
    txt. */
inline _::Text& operator<< (std::ostream& out_stream, _::Text& txt) {
    out_stream << txt.GetBegin ();
    return txt;
}

/** Operation << writes a nil-terminated UTF-8 or ASCII string to the 
    txt. */
inline _::Text& operator<< (_::Text& text_a, _::Text& text_b) {
    // Do nothing.
    return text_a;
}

/** Operation << writes a nil-terminated UTF-8 or ASCII string to the 
    txt. */
inline _::Text& operator<< (_::Text& txt, const char* strand) {
    char* cursor = _::StrandWrite (txt.GetBegin (), txt.GetEnd (), strand);
    if (cursor == nullptr) {
        return txt;
    }
    txt.SetCursor (cursor + 1);
    return txt;
}

/** Operation << writes the given value to the txt. */
inline _::Text& operator<< (_::Text& txt, int8_t value) {
    char* cursor = _::StrandWrite (txt.GetBegin (), txt.GetEnd (), value);
    if (cursor == nullptr) {
        return txt;
    }
    txt.SetCursor (cursor + 1);
    return txt;
}

/** Operation << writes the given value to the txt. */
inline _::Text& operator<< (_::Text& txt, uint8_t value) {
    char* cursor = _::StrandWrite (txt.GetBegin (), txt.GetEnd (), value);
    if (cursor == nullptr) {
        return txt;
    }
    txt.SetCursor (cursor + 1);
    return txt;
}

/** Operation << writes the given value to the txt. */
inline _::Text& operator<< (_::Text& txt, int16_t value) {
    char* cursor = _::StrandWrite (txt.GetBegin (), txt.GetEnd (), value);
    if (cursor == nullptr) {
        return txt;
    }
    txt.SetCursor (cursor + 1);
    return txt;
}

/** Operation << writes the given value to the txt. */
inline _::Text& operator<< (_::Text& txt, uint16_t value) {
    char* cursor = _::StrandWrite (txt.GetBegin (), txt.GetEnd (), value);
    if (cursor == nullptr) {
        return txt;
    }
    txt.SetCursor (cursor + 1);
    return txt;
}

/** Operation << writes the given value to the txt. */
inline _::Text& operator<< (_::Text& txt, int32_t value) {
    char* cursor = _::StrandWrite (txt.GetBegin (), txt.GetEnd (), value);
    if (cursor == nullptr) {
        return txt;
    }
    txt.SetCursor (cursor + 1);
    return txt;
}

/** Operation << writes the given value to the txt. */
inline _::Text& operator<< (_::Text& txt, uint32_t value) {
    char* cursor = _::StrandWrite (txt.GetBegin (), txt.GetEnd (), value);
    if (cursor == nullptr) {
        return txt;
    }
    txt.SetCursor (cursor + 1);
    return txt;
}

/** Operation << writes the given value to the txt. */
inline _::Text& operator<< (_::Text& txt, int64_t value) {
    char* cursor = _::StrandWrite (txt.GetBegin (), txt.GetEnd (), value);
    if (cursor == nullptr) {
        return txt;
    }
    txt.SetCursor (cursor + 1);
    return txt;
}

/** Operation << writes the given value to the txt. */
inline _::Text& operator<< (_::Text& txt, uint64_t value) {
    char* cursor = _::StrandWrite (txt.GetBegin (), txt.GetEnd (), value);
    if (cursor == nullptr) {
        return txt;
    }
    txt.SetCursor (cursor + 1);
    return txt;
}

/** Operation << writes the given value to the txt. */
inline _::Text& operator<< (_::Text& txt, float value) {
    char* cursor = _::StrandWrite (txt.GetBegin (), txt.GetEnd (), value);
    if (cursor == nullptr) {
        return txt;
    }
    txt.SetCursor (cursor + 1);
    return txt;
}

/** Operation << writes the given value to the txt. */
inline _::Text& operator<< (_::Text& txt, double value) {
    char* cursor = _::StrandWrite (txt.GetBegin (), txt.GetEnd (), value);
    if (cursor == nullptr) {
        return txt;
    }
    txt.SetCursor (cursor + 1);
    return txt;
}

#endif  //< SCRIPT_USING_TEXT
#endif  //< HEADER_FOR_SCRIPT_TEXT
