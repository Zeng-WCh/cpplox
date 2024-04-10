#pragma once
#ifndef __PARSER_H__
#define __PARSER_H__

#include "scanner.h"
#include "token.h"

class Parser {
private:
  Scanner *scanner;

public:
  inline Parser(const std::string &filename) : scanner(new Scanner(filename)) {}
  inline Parser() : scanner(new Scanner()) {}

  Parser(const Parser &) = delete;
  Parser &operator=(const Parser &) = delete;
  Parser(Parser &&) = delete;

  inline ~Parser() {
    if (this->scanner) {
      delete this->scanner;
    }
  }
};

#endif