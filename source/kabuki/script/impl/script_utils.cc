/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_utils.cc
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
#include "../include/utils.h"

namespace _ {

void PrintLine (const char* string) {
    std::cout << '\n' << string << '\n';
}

void PrintDebug (const char* message, 
                 const char* end_string,
                 const char* begin_string)
{
#if DEBUG
    std::cout << begin_string << message << end_string;
#endif
}

void PrintError (const char* message, const char* end_string) {
#if DEBUG
    std::cout << ErrorHeader << message << end_string;
#endif
}

void PrintDebugPointer (const char* message, const void* address) {
#if DEBUG
    printf ("| Error at address 0x%p: %s\n", address, message);
#endif
}

void PrintDebugHex (const char* message, char value) {
#if DEBUG
    printf ("| %s '%c':0x%x\n", message, value, value);
#endif
}

int StringLength (const char* string, char delimiter) {
    if (string == nullptr)
        return -1;
    int count = 0;

    char c; c = *string;
    while (c)
        c = *(++string);
    return count;
}

const char* DuplicateString (const char* string, char delimiter) {
    int length = StringLength (string);
    if (length < 0)
        string = "Error duplicating char.";
    char* buffer = new char[length + 1];
    memcpy_s (buffer, length + 1, string, length + 1);
    return buffer;
}

void DestroyDuplicate (const byte* string) {
    if (string == nullptr) return;
}

void Delete (void* buffer) {
    if (buffer == nullptr) return;
    delete[] buffer;
}

byte NibbleToLowerCaseHex (byte b) {
    if (b > 15) return 'f';
    if (b > 9)
        return b + ('a' - 10);
    return b + '0';
}

byte NibbleToUpperCaseHex (byte b) {
    if (b > 15) return 'F';
    if (b > 9) return b + ('A' - 10);
    return b + '0';
}

uint16_t ToLowerCaseHex (byte b) {
    uint16_t value = NibbleToLowerCaseHex (b & 0xf);
    value = value << 8;
    value |= NibbleToLowerCaseHex (b >> 4);
    return value;
}

uint16_t ToUpperCaseHex (byte b) {
    uint16_t value = NibbleToUpperCaseHex (b & 0xf);
    value = value << 8;
    value |= NibbleToUpperCaseHex (b >> 4);
    return value;
}

int ToByte (byte c) {
    //printf ("toByte (byte c): %c ", c);
    if (c < '0') return -1;
    if (c >= 'a') {
        if (c > 'f') return -1;
        //printf ("output!: %i \n", c - ('a' - 10));
        return c - ('a' - 10);
    }
    if (c >= 'A') {
        if (c > 'F') return -1;
        return c - ('A' - 10);
    }
    if (c > '9') return -1;
    //printf ("output: %i \n", c - ('a' - 10));
    return c - '0';
}

int ToByte (uint16_t h) {
    // Note: This works but the upper and lower are switched... pay no 
    // attention to the man behind the curtain #HandWave
    //printf ("toByte (uint16_t c): %c%c\n", (byte) (h << 8), (byte)h);
    int lowerValue = ToByte (static_cast<byte> (h >> 8));
    //printf (" lowerValue: %i \n", lowerValue);
    if (lowerValue < 0) return -1;

    int upperValue = ToByte (static_cast<byte> (h));
    if (upperValue < 0) return -1;
    //printf (" upperValue: %i \nlowerValue | (upperValue << 4): %i\n",
    //	upperValue, lowerValue | (upperValue << 4));

    //PrintLine ();
    return lowerValue | (upperValue << 4);
}

uintptr_t Diff (void* begin, void* end) {
    return static_cast<uintptr_t>(reinterpret_cast<byte*> (end) -
                                  reinterpret_cast<byte*> (begin));
}

uintptr_t Diff (const void* begin, const void* end) {
    byte* start = static_cast<byte*> (const_cast<void*> (begin)),
        *stop = static_cast<byte*> (const_cast<void*> (end));
    return static_cast<uintptr_t> (stop - start);
}

hash16_t Hash16 (char c, hash16_t hash) {
    //PrintHex (c);
    hash16_t cprime = c * 65521;
    return cprime + hash;
}

hash16_t Hash16 (const char* string, hash16_t hash) {
    byte c = *string;
    while (c) {
        hash = Hash16 (c, hash);
        ++string;
        c = *string;
    }
    return hash;
}

hash32_t Hash32 (char c, hash32_t hash) {
    hash32_t cprime = c * 4294967291;
    return cprime + hash;
}

hash16_t Hash32 (const char* string, hash32_t hash) {
    byte c = *string;
    while (c) {
        hash = Hash32 (c, hash);
        ++string;
        c = *string;
    }
    return hash;
}

hash64_t Hash64 (char c, hash64_t hash) {
    hash64_t cprime = c * 18446744073709551557;
    return cprime + hash;
}

hash64_t Hash64 (const char* string, hash64_t hash) {
    byte c = *string;
    while (c) {
        hash = Hash64 (c, hash);
        ++string;
        c = *string;
    }
    return hash;
}

void PrintHex (byte c) {
    uint16_t chars = ToUpperCaseHex (c);
    putchar ((char)chars);
    putchar ((char)(chars >> 8));
    putchar (' ');
}

void PrintCentered (const char* string, int width) {
    if (width < 2) {
        //? Not sure if this is an error.
        return;
    }
    // We need to leave at least one space to the left and right of 
    int length = StringLength (string);
    if (length < width - 2) {
        // We need to write the ....
        if (length < 4) {
            // Then we're just going to write the first few letters.
            for (; length >= 0; --length) {
                std::cout << '\n';
            }
        }
    }
    int offset = (width - length) >> 1; //< >> 1 to /2
    for (int i = 0; i < offset; ++i)
        std::cout << '\n';
    printf (string);
    for (offset = width - length - offset; offset <= width; ++offset)
        std::cout << '\n';
}

void PrintChar (char c) {
    if ((c > 0 && c < ' ') || c == 127) {
        std::cout << 1;
        return;
    }
    std::cout << c;
}

void PrintLine (char token, int column_width) {
    std::cout << '\n';
    for (int i = 0; i < column_width; ++i)
        std::cout << token;
    std::cout << '\n';
}

void PrintLine (const char* start_string, char token, int length) {
    std::cout << '\n' << start_string;
    for (int i = 0; i < 79; ++i)
        std::cout << token;
    std::cout << '\n';
}

void PrintLines (int numRows) {
    std::cout << '\r';
    for (int i = 0; i < numRows - 1; ++i)
        std::cout << '\n';
}

void PrintMemory (const void* address, const void* end) {
    printf ("|%i", 0);
    // Print columns
    for (int i = 8; i <= 66; i += 8)
        printf ("%8i", i);
    std::cout << '\n';
    std::cout << '|';
    for (int i = 0; i < 65; ++i)
        std::cout << '_';
    std::cout << '\n';

    const char* chars = reinterpret_cast<const char*> (address);
    char temp;
    while (chars < end) {
        std::cout << '|';
        for (int i = 0; i < 64; ++i) {
            temp = *chars;
            if (chars >= end)
                temp = 'x';
            PrintChar (temp);
            ++chars;
        }
        printf ("| 0x%p\n", chars - 64);
    }
    std::cout << '|';
    for (int i = 0; i < 64; ++i)
        std::cout << '_';
    printf ("| 0x%p\n", chars + Diff (address, end));
}

void PrintMemory (const void* address, size_t size) {
    const char* end = reinterpret_cast<const char*>(address) + size;
    PrintMemory (address, end);
}

char CreateKeyValueFormatString (char* string, char column_width, char type) {
    char hundreds = (column_width / 100),
        decimal = (column_width % 10),
        tens = (column_width - hundreds - decimal) / 10;
    string[0] = '%';
    if (column_width <  10) {
        string[1] = decimal + '0';
        string[2] = 's';
        string[3] = ':';
        string[4] = ' ';
        string[5] = '0';
        string[6] = 'x';
        string[7] = '%';
        string[8] = 'p';
        string[9] = '\n';
        return 0;
    } else if (column_width < 100) {
        string[1] = tens + '0';
        string[2] = decimal + '0';
        string[3] = 's';
        string[4] = ':';
        string[5] = ' ';
        string[6] = '0';
        string[7] = 'x';
        string[8] = '%';
        string[9] = 'p';
        string[10] = '\n';
        return 0;
    }
    string[1] = hundreds + '0';
    string[2] = tens + '0';
    string[3] = decimal + '0';
    string[4] = 's';
    string[5] = ':';
    string[6] = ' ';
    string[7] = '0';
    string[8] = 'x';
    string[9] = '%';
    string[10] = 'p';
    string[11] = '\n';
    return 0;
}

void PrintNumberLine (index index) {
    std::cout << '\n';
    enum { MaxBufferSize = (sizeof (int) == 2) ? 7 : (sizeof (int) == 4) ? 11 : 128 };
    char buffer[MaxBufferSize];
    sprintf_s (buffer, MaxBufferSize, "%u", index);
    int length = StringLength (buffer),
        i,
        lettersLeft = 80 % (length + 1);

    for (i = 0; i < 80; i += (length + 1))
        printf ("%s_", buffer);
    for (int j = 0; j < lettersLeft; ++j)
        std::cout << buffer[j];
    std::cout << '\n';
}

void PrintStringLine (const char* string) {
    PrintLine ();
    int length = StringLength (string),
        i;

    for (i = 0; i < 80; i += (length + 1))
        std::cout << string << '_';
    for (int j = 0; j < 80 % (length + 1); ++j)
        std::cout << string[j];
    PrintLine ();
}

void PrintPause (const char* string) {
#if DEBUG
    std::cout << "\n" << string << "\n";
    system ("PAUSE");
#endif
}

void PrintNL () {
    std::cout << '\n';
}

void PrintLineBreak (const char* message, int top_bottom_margin,
                            char c, int num_columns) {
    PrintLines (top_bottom_margin);
    std::cout << message;
    PrintLine (c, num_columns);
}

void CopyString (char* destination, const char* source,
                        char delimeter) {
    if (destination == nullptr)
        return;
    if (source == nullptr)
        return;
    char c = *source;
    ++source;
    while (c != delimeter) {
        *destination = c;
        ++destination;
        c = *source;
        ++source;
    }
    *destination = '\0';
}

char* CloneString (const char* input, char delimeter) {
    if (input == nullptr)
        input = "";
    size_t length = StringLength (input);
    char* clone = new char[length + 1];
    CopyString (clone, input);
    return clone;
}

void PrintBar (const char* input) {
    std::cout << "\n| " << input << '\n';
}

void PrintBreak (const char* header, char c, int num_lines, 
                 int console_width) {
    for (int i = 0; i < num_lines; ++i)
        std::cout << '\n';
    std::cout << header;
    int length = StringLength (header);
    for (int i = 0; i < console_width - length; ++i)
        std::cout << c;
    std::cout << '\n';
}

void PrintCentered (const char* input, int width, bool is_last,
                    char column_delimeter)
{
    if (width < 1)
        return;
    std::cout << column_delimeter;
    int length = strlen (input),
        num_spaces;

    if (width < length) {
        char format[32];
        sprintf_s (format, 32, "%%%i", is_last ? width - 2 : width - 1);
        std::cout << column_delimeter;
        printf (format, input);
        if (is_last)
            std::cout << column_delimeter << '\n';
    }
    num_spaces = width - length - 1 - (is_last ? 1 : 0);
    for (int i = 0; i < num_spaces / 2; ++i)
        std::cout << ' ';
    std::cout << input;
    for (int i = 0; i < num_spaces - (num_spaces / 2); ++i)
        std::cout << ' ';
    if (is_last)
        std::cout << column_delimeter << '\n';
}

void PrintColumnBreak (int num_columns, char column_delimeter, char break_char, 
                       int width) {
    int column_width = width / num_columns;
    for (int i = 0; i < num_columns - 1; ++i) {
        std::cout << column_delimeter;
        for (int j = 0; j < column_width - 1; ++j)
            std::cout << break_char;
    }
    std::cout << column_delimeter;
    for (int j = 0; j < column_width - 2; ++j)
        std::cout << break_char;
    std::cout << column_delimeter << '\n';
}

const char* FindEndOfRow (const char* input, int num_columns) {
    char c;
    // Scroll to the end of the line.
    while (c = *input++) {
        if (num_columns-- < 0) // We've reached the end.
        {
            // Scroll left till we hit whitespace (if any).
            while (!isspace (c))
                c = *(--input);
            // Then scroll past the whitespace.
            while (isspace (c));
            c = *(--input);
            return input;
        }
    }
    return --input;
}

void PrintPageCentered (const char* input, int num_columns) {
    char c;   //< The current char.
    if (num_columns <= 4) // We need at least 4 columns for this to work.
        return;
    if (input == nullptr)
        return;
    do {
        const char* end_row = FindEndOfRow (input, num_columns);
        size_t row_length = end_row - input,
            num_left_spaces = (num_columns / 2) - (row_length / 2),
            i;
        // Print left whitespace.
        for (i = num_left_spaces; i != 0; --i)
            std::cout << ' ';
        // Print input.
        for (i = row_length; i != 0; --i)
            std::cout << *input++;
        for (i = num_columns - num_left_spaces - row_length; i != 0; --i)
            std::cout << ' ';
        // Increment the input pointer and scroll past the whitespace
        input += row_length;
        while (isspace (c = *input++));
    } while (c);
}

void PrintPageRight (const char* input, int num_columns) {
    char c;  //< The current char.

    if (num_columns <= 4) // We need at least 4 columns for this to work.
        return;
    if (input == nullptr)
        return;
    do {
        const char* end_row = FindEndOfRow (input, num_columns);
        size_t row_length = end_row - input,
            num_left_spaces = num_columns - row_length,
            i;
        // Print left whitespace.
        for (i = num_left_spaces; i != 0; --i)
            std::cout << ' ';
        // Print input.
        for (i = row_length; i != 0; --i)
            std::cout << *input++;
        // Increment the input pointer and scroll past the whitespace
        input += row_length;
        while (isspace (c = *input++));
    } while (c);
}

void PrintPage (const char* input, int indentation,
                       char bullet, index index, int tab_size,
                       int num_columns) {
    num_columns -= 4;
    std::cout << "| ";
    int cursor; //< The column number of the cursor.
    char c = *input++,  //< The current char.
        buffer[15];     //< The bullet buffer.
    if (!c || input == nullptr)  //< It's an empty input.
    {
        for (int i = num_columns; i > 0; --i)
            std::cout << ' ';
        std::cout << "|\n";
        return;
    }

    // Make the input for the bullet.
    if (isdigit (bullet)) // Then we have to print a number bullet.
    {
        sprintf_s (&buffer[0], 15, "%i", index);
        //< + 2 for "| " - 2 for the bullet offset.
        char format[16];
        sprintf_s (&format[1], 16, "%%%us", indentation * tab_size);
        printf (format, buffer);
    } else if (isalpha (bullet)) // Then it's a lower case bullet.
    {
        cursor = 0;
        bullet += index % 26;  //< 
        for (; index > 0; index -= 26)
            buffer[cursor++] = bullet;
        buffer[cursor] = '\0';
    } else {
        buffer[0] = bullet;
        buffer[1] = 0;
    }
    // Prints the char.
    int num_spaces = tab_size * indentation;
    while (c) {
        for (cursor = num_spaces; cursor > 0; --cursor)
            std::cout << ' ';
        cursor = num_spaces;
        while (c && (cursor++ < num_columns)) {
            std::cout << c;
            c = *input++;
        }
        std::cout << " |\n";
    }
    std::cout << '\n';
}

const char* NextNonNumberString (const char* input) {
    char c = *input;
    if (c == '-')           // It might be negative.
    {
        c = *(++input);
        if (!isdigit (c))   // but it's not.
            return input - 1;
        c = *(++input);
    }
    while (isdigit (c))
        c = *(++input);
    return input;
}

char* NextNonNumber (char* input) {
    return (char*)NextNonNumberString (input);
}

const char* SkipLeadingZerosString (const char* input) {
    if (input == nullptr)
        return nullptr;
    char c = *input,
        d;
    if (c != '0')
        return input;
    d = *(++input);
    // We know the first char is a '0' so just loop till c and d aren't '0'.
    while (c == d) {
        c = d;
        d = *(++input);
    }
    if (!isdigit (d))       //< Special case for "...00" char.
        return input - 1;
    return input;
}

char* SkipLeadingZeros (char* input) {
    return (char*)SkipLeadingZerosString (input);
}

const char* SkipSpacesString (const char* input) {
    if (input == nullptr)
        return 0;
    //std::cout << "\nSkipping spaces: ";
    char c = *input;
    while (c) {
        //std::cout << '.';
        if (!isspace (c))
            return input;
        c = *(++input);
    }
    return input;
}

char* SkipSpaces (char* input) {
    return (char*)SkipSpacesString (input);
}

const char* EndOfTokenString (const char* input) {
    if (input == nullptr)
        return nullptr;
    input = SkipSpacesString (input);

    char c = *input;
    while (c) {
        if (isspace (c))
            return input + 1;
        c = *(++input);
    }
    return input;
}

char* EndOfToken (char* input) {
    return (char*)EndOfTokenString (input);
}

const char* EndOfString (const char* input, char delimiter) {
    char c = *input;
    while (c) {
        if (c == delimiter)
            return input;
        c = *(++input);
    }
    return input;
}

char* EndOf (char* input, char delimiter) {
    return (char*)EndOfString (input, delimiter);
}

const char* CompareTokenString (const char* input, const char* token) {
    if (input == nullptr)
        return nullptr;
    if (token == nullptr)
        return nullptr;

    input = SkipSpacesString (input);
    token = SkipSpacesString (token);

    char a = *input,
        b = *token;
    if (b == 0) // Nothing to do.
        return nullptr;
    while (a && !isspace (a)) {
        //std::cout << "> a: " << a << " b: " << b << '\n';
        if (a != b) {
            return nullptr;
        }
        if (!b)
            return nullptr;
        a = *(++input);
        b = *(++token);
    }
    //std::cout << "> Query hit!\n";
    return input;
}

char* CompareToken (const char* input, const char* query) {
    return (char*)CompareTokenString (input, query);
}

const char* CompareString (const char* input, const char* query,
                           char delimiter) {
    if (input == nullptr)
        return nullptr;
    if (query == nullptr)
        return nullptr;

    input = SkipSpacesString (input);
    query = SkipSpacesString (query);

    char string = *input,
        t = *query;
    if (string == 0)          // Nothing to do.
        return nullptr;
    if (t == 0)          // Nothing to do.
        return nullptr;
    if (t == delimiter) // Nothing to do.
        return nullptr;
    while (!isspace (string)) {
        if (string != t)
            return nullptr;
        if (t == delimiter) // Special case for null-term char.
            return nullptr;
        string = *(++input);
        t = *(++query);
    }
    return input; //< Query hit!
}

char* Compare (char* source, const char* query, char delimiter) {
    return (char*)CompareString (source, query, delimiter);
}

const char* ParseString (const char* input, char* destination,
                                int buffer_size, char delimiter) {
    //std::cout << "> parse_string buffer_size: " << buffer_size
    //          << " delimiter " << delimiter << "\n> ";
    if (input == nullptr) {
        std::cout << "| input == nullptr\n";
        return nullptr;
    }
    if (destination == nullptr) {
        std::cout << "| destination == nullptr\n";
        return nullptr;
    }
    if (buffer_size < 1) {
        std::cout << "| buffer_size < 1\n";
        return nullptr;
    }
    //std::cout << delimiter;
    char c = *input;       //< Current char from input.
    while (c) {
        if (c == delimiter) {
            //std::cout << delimiter << " parsed.\n";
            *destination = 0;
            if (delimiter == 0)
                return input;
            return input + 1;
        }
        //std::cout << c;
        if (buffer_size-- < 1) {
            std::cout << "\n| Buffer overflow!\n";
            return nullptr;
        }
        *destination++ = c;
        c = *(++input);
    }
    //std::cout << delimiter << " done.\n";
    *destination = 0;
    if (delimiter == 0)
        return input;
    return input + 1;
}

char* Parse (char* input, char* destination, int buffer_size,
                    char delimiter) {
    return (char*)ParseString (input, destination, buffer_size, delimiter);
}

char* ParseToken (char* input) {
    if (input == nullptr)
        return nullptr;

    input = SkipSpaces (input);

    char c = *input;
    while (c && !isspace (c)) {
        c = *(++input);
    }
    *input = 0;   //< Mark the null-term char.
    return input; //< This is the start of the next token or whitespace.
}

const char* FindString (const char* input, const char* query, char delimiter) {
    if (input == nullptr)
        return nullptr;
    if (query == nullptr)
        return nullptr;

    char string = *input,    //< Current input char.
        t = *query,     //< Current query char.
        c = t;          //< The first char of the query we're searching for.
    if (c == delimiter) //< We're not allowing empty queries.
        return nullptr;
    const char* start_of_query,
        //*cursor;             //< This was uninitialized???
        *cursor = input;    //< @todo Is this working?

    query = SkipSpacesString (query);

    // Scroll through each char and match it to the query char.
    while (string) {
        if (string == c) // The first char matches:
        {
            // Setup to compare the strings;
            start_of_query = input;
            cursor = query;
            t = c;
            // check the rest of the char:
            while (string == t) {
                string = *(++input);
                t = *(++cursor);
                if (t == delimiter)        // Once we've reached the delimiter
                    return start_of_query;  // it's a match!
                if (!string) // We've reached the end of char without a hit.
                    return nullptr;
            }
        }
        // The char did not match so repeat the process for each char. 
        string = *(++input);
        t = *(++cursor);

    }
    // If we haven't found it by now it's not in the input.
    return nullptr;
}

char* Find(char* input, const char* query, char delimiter) {
    return (char*)FindString (input, query, delimiter);
}

const char* ParseIntString (const char* input, int* value) {
    if (input == nullptr)
        return nullptr;
    if (value == nullptr)
        return nullptr;
    if (!sscanf_s (input, "%i", value))
        return nullptr;
    return EndOfTokenString (input);
}

char* ParseInt (char* input, int* value) {
    return (char*)ParseIntString (input, value);
}

const char* ParseFloatString (const char* input, float * value) {
    if (input == nullptr)
        return nullptr;
    if (value == nullptr)
        return nullptr;
    if (!sscanf_s (input, "%f", value))
        return nullptr;
    return EndOfTokenString (input);
}

char* ParseFloat (char* input, float* value) {
    return (char*)ParseFloatString (input, value);
}

bool IsToken (const char* input) {
    if (input == nullptr)
        return false;
    char c = *input;
    if (!isspace (c))
        return false;
    if (isdigit (c))
        return false;
    while (c) {
        if (isspace (c))
            return false;
        c = *(++input);
    }
    return true;
}

}       //< namespace _
