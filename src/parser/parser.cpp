#include "parser.h"
#include "ast.h"
#include "token.h"

#include <cstdarg>
#include <cstdio>

void Parser::logger_error(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, fmt, args);
  va_end(args);
}

Ast *Parser::parse() {
  // if EOF, return ast
  Token t = this->scanner->next_token();

  while (t.getType() != TokenType::tok_eof) {
    this->scanner->stop_scanning();
    Declaration *decl = this->parse_decl();
    if (this->ast)
      this->ast->add_decl(decl);
    else {
      this->ast = new Program();
      this->ast->add_decl(decl);
    }
  }

  return this->ast;
}

Declaration *Parser::parse_decl() {
  Token e = this->scanner->next_token();
  switch (e.getType()) {
  case tok_class:
    this->scanner->stop_scanning();
    return this->parse_class_decl();
  case tok_func:
    this->scanner->stop_scanning();
    return this->parse_func_decl();
  case tok_var:
    this->scanner->stop_scanning();
    return this->parse_var_decl();
  default:
    this->scanner->stop_scanning();
    return this->parse_stmt();
  }

  return nullptr;
}

ClassDeclaration *Parser::parse_class_decl() {
  Token e = this->scanner->next_token();

  if (e.getType() != tok_class) {
  }

  return nullptr;
}