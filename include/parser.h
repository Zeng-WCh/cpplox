#pragma once
#ifndef __PARSER_H__
#define __PARSER_H__

#include "ast.h"
#include "scanner.h"
#include "token.h"

class Parser {
private:
  Scanner *scanner;
  Program *ast;

protected:
  void logger_error(const char *fmt, ...);

  inline void match(Token current, TokenType expect) {
    if (current.getType() == expect) {
      // do nothing
    } else {
      this->logger_error("Syntax Error: <File:%s, Line: %zu, Column: %zu, "
                         "Offset: %zu>",
                         this->scanner->get_file(), current.getLine(),
                         current.getColumn(), current.getOffset());
      // exit(1);
    }
  }

  Declaration *parse_decl();

  ClassDeclaration *parse_class_decl();
  FunctionDeclaration *parse_func_decl();
  VariableDeclaration *parse_var_decl();
  Statement *parse_stmt();

public:
  inline Parser(const std::string &filename)
      : scanner(new Scanner(filename)), ast(nullptr) {}
  inline Parser() : scanner(new Scanner()), ast(nullptr) {}

  Parser(const Parser &) = delete;
  Parser &operator=(const Parser &) = delete;
  Parser(Parser &&) = delete;

  inline ~Parser() {
    if (this->scanner) {
      delete this->scanner;
    }
  }

  Ast *parse();
};

#endif