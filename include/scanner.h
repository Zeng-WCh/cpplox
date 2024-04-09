#pragma once

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "token.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

class Scanner {
private:
  std::string source;
  std::vector<Token> tokens;
  std::size_t line, column, offset;
  std::ifstream input;

  bool from_stdin;

  /**
   * buffer is used to store the current line of input
   * only used in interactive
   */
  std::string buffer;
  int lastChar;
  std::size_t idx;

protected:
  void fill_buffer();
  /**
   * Internal method, used to fill the tokens
   */
  void advance();
  void advance_in_buffer();
  void advance_in_file();

  int next_char();

public:
  inline Scanner()
      : source("-"), tokens(), line(0), column(0), offset(0), input(),
        from_stdin(true), buffer(), lastChar(' ') {}

  inline Scanner(const std::string &filename)
      : source(filename), tokens(), line(0), column(0), offset(0),
        from_stdin(false), buffer(), lastChar(' ') {
    this->input.open(this->source, std::ios::in);
    if (!this->input.is_open()) {
      fprintf(stderr, "Unable to open file: %s\n", filename.c_str());
      exit(1);
    }
  }

  inline ~Scanner() {
    if ((!this->from_stdin) && this->input.is_open()) {
      this->input.close();
    }
  }

  inline std::size_t getLine() const { return this->line; }
  inline std::size_t getColumn() const { return this->column; }
  inline std::size_t getOffset() const { return this->offset; }

  Token &next();

  void clean();
};

#endif