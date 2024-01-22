/*
MIT License

Copyright (c) 2021 Brian T. Park

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Arduino.h> // pgm_read_byte()
#include "copyReplace.h"

namespace ace_common {

void copyReplaceChar(char* dst, size_t dstSize, const char* src,
    char oldChar, char newChar) {
  char c;
  while ((c = *src++) != '\0' && dstSize > 0) {
    if (c == oldChar) {
      if (newChar == '\0') continue;
      c = newChar;
    }
    *dst++ = c;
    dstSize--;
  }

  if (dstSize == 0) {
    --dst;
  }
  *dst = '\0';
}

void copyReplaceChar(char* dst, size_t dstSize, const __FlashStringHelper* src,
    char oldChar, char newChar) {
  char c;
  const char* s = (const char*) src;
  while ((c = (char) pgm_read_byte(s++)) != '\0' && dstSize > 0) {
    if (c == oldChar) {
      if (newChar == '\0') continue;
      c = newChar;
    }
    *dst++ = c;
    dstSize--;
  }

  if (dstSize == 0) {
    --dst;
  }
  *dst = '\0';
}

// 4 overloaded versions of copyReplaceString() below, from the 2 types of `src`
// and 2 types of `newString`.

void copyReplaceString(char* dst, size_t dstSize, const char* src,
    char oldChar, const char* newString) {
  char c;
  while ((c = *src++) != '\0' && dstSize > 0) {
    if (c == oldChar) {
      const char* s = newString;
      while (*s != '\0' && dstSize > 0) {
        *dst++ = *s++;
        dstSize--;
      }
    } else {
      *dst++ = c;
      dstSize--;
    }
  }

  if (dstSize == 0) {
    --dst;
  }
  *dst = '\0';
}

void copyReplaceString(char* dst, size_t dstSize, const char* src,
    char oldChar, const __FlashStringHelper* newString) {
  char c;
  while ((c = *src++) != '\0' && dstSize > 0) {
    if (c == oldChar) {
      const char* s = (const char*) newString;
      while ((char) pgm_read_byte(s) != '\0' && dstSize > 0) {
        *dst++ = (char) pgm_read_byte(s++);
        dstSize--;
      }
    } else {
      *dst++ = c;
      dstSize--;
    }
  }

  if (dstSize == 0) {
    --dst;
  }
  *dst = '\0';
}

void copyReplaceString(char* dst, size_t dstSize,
    const __FlashStringHelper* src, char oldChar, const char* newString) {
  char c;
  const char* ss = (const char*) src;
  while ((c = (char) pgm_read_byte(ss++)) != '\0' && dstSize > 0) {
    if (c == oldChar) {
      const char* s = (const char*) newString;
      while (*s != '\0' && dstSize > 0) {
        *dst++ = *s++;
        dstSize--;
      }
    } else {
      *dst++ = c;
      dstSize--;
    }
  }

  if (dstSize == 0) {
    --dst;
  }
  *dst = '\0';
}

void copyReplaceString(char* dst, size_t dstSize,
    const __FlashStringHelper* src, char oldChar,
    const __FlashStringHelper* newString) {
  char c;
  const char* ss = (const char*) src;
  while ((c = (char) pgm_read_byte(ss++)) != '\0' && dstSize > 0) {
    if (c == oldChar) {
      const char* s = (const char*) newString;
      while ((char) pgm_read_byte(s) != '\0' && dstSize > 0) {
        *dst++ = (char) pgm_read_byte(s++);
        dstSize--;
      }
    } else {
      *dst++ = c;
      dstSize--;
    }
  }

  if (dstSize == 0) {
    --dst;
  }
  *dst = '\0';
}

}
