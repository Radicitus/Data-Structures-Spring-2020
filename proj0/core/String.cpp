// String.cpp
//
// ICS 46 Spring 2020
// Project #0: Getting to Know the ICS 46 VM
//
// Implement all of your String member functions in this file.
//
// Note that the entire standard library -- both the C Standard
// Library and the C++ Standard Library -- is off-limits for this
// task, as the goal is to exercise your low-level implementation
// skills (pointers, memory management, and so on).

#include "String.hpp"
#include "OutOfBoundsException.hpp"


String::String() {
    num_chars = 0;
    characters = new char[1];
    characters[0] = '\0';
}

String::String(const char *chars) {
    int size = 0;
    while (chars[size] != '\0')
    {
        size++;
    }
    characters = new char[size + 1];
    for (int i = 0; i <= size; ++i) {
        characters[i] = chars[i];
    }
    characters[size] = '\0';
    num_chars = size;
}

String::String(const String &s) {
    characters = s.characters;
    num_chars = s.num_chars;
}

String::~String() noexcept {
    delete[] characters;
}

String &String::operator=(const String &s) {
    delete[] characters;
    num_chars = s.length();
    
    characters = new char[num_chars + 1];
    characters[num_chars] = '\0';

    for (int i = 0; i < num_chars; ++i) {
        characters[i] = s.characters[i];
    }

    return *this;
}

void String::append(const String &s) {
    unsigned int new_size = num_chars + s.length();
    char * new_characters = new char[new_size + 1];

    for (int i = 0; i < num_chars; ++i) {
        new_characters[i] = characters[i];
    }

    for (int i = 0; i < s.length(); ++i) {
        new_characters[i + num_chars] = s.at(i);
    }

    new_characters[new_size] = '\0';

    delete[] characters;
    characters = new_characters;
    num_chars = new_size;
}

char String::at(unsigned int index) const {
    if (index >= num_chars || index < 0)
    {
        throw OutOfBoundsException{};
    }
    return characters[index];
}

char &String::at(unsigned int index) {
    if (index >= num_chars || index < 0)
    {
        throw OutOfBoundsException{};
    }
    return characters[index];
}

void String::clear() {
    delete[] characters;
    characters = new char{'\0'};
    num_chars = 0;
}

int String::compareTo(const String &s) const noexcept {
    for (int i = 0; i < num_chars; ++i) {
        //Check if this has reached last char of s
        if (i == s.num_chars) {
            return 1;
        }
        //Check if current char for this is less than s
        if (characters[i] < s.characters[i]) {
            return -1;
        }
        //Check if current char for this is greater than s
        if (characters[i] > s.characters[i]) {
            return 1;
        }
    }

    if (num_chars == s.num_chars) {
        return 0;
    } else {
        return -1;
    }
}

String String::concatenate(const String &s) const {
    unsigned int new_size = num_chars + s.length();

    char * new_characters = new char[new_size + 1];

    for (int i = 0; i < num_chars; ++i) {
        new_characters[i] = characters[i];
    }

    for (int i = 0; i < s.length(); ++i) {
        new_characters[i + num_chars] = s.at(i);
    }

    new_characters[new_size] = '\0';

    String * to_return = new String();
    delete[] to_return->characters;
    to_return->characters = new_characters;
    to_return->num_chars = new_size;

    return * to_return;
}

bool String::contains(const String &substring) const noexcept {
    int compare_index = 0;
    bool streak = false;

    for (int i = 0; i < num_chars; ++i) {
        char current_char = characters[i];
        char current_substr_char = substring.characters[compare_index];
        
        if (compare_index + 1 == substring.length() && current_char == current_substr_char) {
            return true;
        }
        if (streak && current_char == current_substr_char) {
            compare_index++;
        }
        if (!streak && current_char == current_substr_char) {
            compare_index++;
            streak = true;
        }
        if (streak && current_char != current_substr_char) {
            compare_index = 0;
            streak = false;
        }
    }
    return false;
}

bool String::equals(const String &s) const noexcept {
    if (length() != s.length()) {
        return false;
    }

    for (int i = 0; i < num_chars; ++i) {
        if (characters[i] != s.characters[i]) {
            return false;
        }
    }

    return true;
}

int String::find(const String &substring) const noexcept {
    int start_index = -1;
    int compare_index = 0;
    bool streak = false;

    for (int i = 0; i < num_chars; ++i) {
        char current_char = characters[i];
        char current_substr_char = substring.characters[compare_index];
        
        if (compare_index + 1 == substring.length() && current_char == current_substr_char) {
            return start_index;
        }
        if (streak && current_char == current_substr_char) {
            compare_index++;
        }
        if (!streak && current_char == current_substr_char) {
            compare_index++;
            streak = true;
            start_index = i;
        }
        if (streak && current_char != current_substr_char) {
            compare_index = 0;
            streak = false;
            start_index = -1;
        }
    }
    return start_index;
}

bool String::isEmpty() const noexcept {
    return num_chars == 0;
}

unsigned int String::length() const noexcept {
    return num_chars;
}

String String::substring(unsigned int startIndex, unsigned int endIndex) const {
    if (startIndex < 0 || endIndex >= num_chars) {
        throw OutOfBoundsException{};
    }
    if (startIndex > endIndex) {
        throw OutOfBoundsException{};
    }
    if (startIndex == endIndex) {
        return "";
    }

    String * new_substring = new String();
    new_substring->num_chars = endIndex - startIndex;
    delete[] new_substring->characters;
    char * new_characters = new char[num_chars + 1];
    new_characters[num_chars] = '\0';
    new_substring->characters = new_characters;

    for (int i = startIndex; i < endIndex; ++i) {
        new_substring->characters[i - startIndex] = characters[i];
    }

    return * new_substring;
}

const char *String::toChars() const noexcept {
    char * toReturn = new char[num_chars + 1];
    for (int i = 0; i < num_chars; ++i) {
        toReturn[i] = characters[i];
    }
    toReturn[num_chars] = '\0';
    return toReturn;
}








