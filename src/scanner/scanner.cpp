#include "scanner.h"
#include "token.h"
#include <cctype>
#include <iostream>

void Scanner::fill_buffer() {
  if (this->from_stdin) {
    std::getline(std::cin, this->buffer);
  }
}

void Scanner::advance() {}

void Scanner::clean() {}

int Scanner::next_char() {
  if (this->from_stdin) {
    if (this->buffer.empty()) {
      this->fill_buffer();
    } else if (this->offset == this->buffer.length()) {
      // buffer is full, clean the buffer and read another
      this->buffer.clear();
      this->offset = 0;
      this->fill_buffer();
    }

    // TODO: impl this
  } else {
    // TODO: impl this
  }

  return 0;
}
