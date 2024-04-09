#include "scanner.h"
#include "token.h"
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <ostream>
#include <unistd.h>

void File::read_a_chunk() {
  // Read a chunk of data from the file
  ssize_t bytes_read = read(this->fd, this->buffer, this->buffer_size);
  if (bytes_read == -1) {
    // Error
    fprintf(stderr, "Unable to read from file %s\n", this->filename.c_str());
    exit(EXIT_FAILURE);
  }
  if (bytes_read == 0) {
    // EOF, clean up
    memset(this->buffer, 0, this->buffer_size);
    this->current = this->buffer;
    this->end = this->buffer;
    *(this->current) = EOF;
    return;
  }

  this->current = this->buffer;
  this->end = this->buffer + bytes_read;
}

char File::next_char() {
  if (this->newline) {
    this->update_pos(this->newline);
    this->newline = false;
  }

  if (this->buffer_empty() && !this->eof()) {
    this->read_a_chunk();
  }

  if (this->eof()) {
    return EOF;
  }

  char c = *(this->current);
  this->current++;
  this->update_pos();
  if (c == '\n') {
    this->newline = true;
  }

  return c;
}

void File::update_pos(bool newline) {
  if (newline) {
    if (this->is_from_file())
      this->line++;
    this->column = 0;
  } else {
    this->column++;
  }
  this->offset++;
}

// return the current character without moving the file pointer
char File::peek() const { return *(this->current); }

int Scanner::get_char() { return this->f->next_char(); }

Token &Scanner::next_token() {
  while (isspace(this->lastchar)) {
    this->lastchar = this->get_char();
  }

  if (this->lastchar == EOF && this->tokens.back().getType() != tok_eof) {
    Token e = Token(tok_eof, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset());
    this->tokens.push_back(e);
    return this->tokens.back();
  }

  else if (this->lastchar == EOF) {
    return this->tokens.back();
  }

  if (this->lastchar == '{') {
    Token e = Token(tok_lbrace, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset(), "{");
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  if (this->lastchar == '}') {
    Token e = Token(tok_rbrace, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset(), "}");
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  if (this->lastchar == '(') {
    Token e = Token(tok_lparen, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset(), "(");
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  if (this->lastchar == ')') {
    Token e = Token(tok_rparen, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset(), ")");
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  if (this->lastchar == '[') {
    Token e = Token(tok_lbracket, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset(), "[");
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  if (this->lastchar == ']') {
    Token e = Token(tok_rbracket, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset(), "]");
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  if (this->lastchar == ',') {
    Token e = Token(tok_comma, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset(), ",");
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  if (this->lastchar == '.') {
    Token e = Token(tok_dot, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset(), ".");
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  if (this->lastchar == ':') {
    Token e = Token(tok_colon, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset(), ":");
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  if (this->lastchar == ';') {
    Token e = Token(tok_semicolon, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset(), ";");
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  if (this->lastchar == '+') {
    Token e = Token(tok_plus, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset(), "+");
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  if (this->lastchar == '-') {
    Token e = Token(tok_minus, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset(), "-");
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  if (this->lastchar == '*') {
    Token e = Token(tok_star, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset(), "*");
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  if (this->lastchar == '/') {
    Token e = Token(tok_slash, this->f->get_line(), this->f->get_column(),
                    this->f->get_offset(), "/");
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  if (this->lastchar == '!') {
    // Check for '!='
    std::size_t line = this->f->get_line();
    std::size_t column = this->f->get_column();
    std::size_t offset = this->f->get_offset();
    this->lastchar = this->get_char();
    if (this->lastchar == '=') {
      Token e = Token(tok_ne, line, column, offset, "!=");
      this->tokens.push_back(e);
      this->lastchar = this->get_char();
      return this->tokens.back();
    } else {
      Token e = Token(tok_not, line, column, offset, "!");
      this->tokens.push_back(e);
      return this->tokens.back();
    }
  }
  if (this->lastchar == '=') {
    // Check for '=='
    std::size_t line = this->f->get_line();
    std::size_t column = this->f->get_column();
    std::size_t offset = this->f->get_offset();
    this->lastchar = this->get_char();
    if (this->lastchar == '=') {
      Token e = Token(tok_eq, line, column, offset, "==");
      this->tokens.push_back(e);
      this->lastchar = this->get_char();
      return this->tokens.back();
    } else {
      Token e = Token(tok_assign, line, column, offset, "=");
      this->tokens.push_back(e);
      return this->tokens.back();
    }
  }
  if (this->lastchar == '<') {
    // Check for '<='
    std::size_t line = this->f->get_line();
    std::size_t column = this->f->get_column();
    std::size_t offset = this->f->get_offset();
    this->lastchar = this->get_char();
    if (this->lastchar == '=') {
      Token e = Token(tok_le, line, column, offset, "<=");
      this->tokens.push_back(e);
      this->lastchar = this->get_char();
      return this->tokens.back();
    } else {
      Token e = Token(tok_lt, line, column, offset, "<");
      this->tokens.push_back(e);
      return this->tokens.back();
    }
  }
  if (this->lastchar == '>') {
    // Check for '>='
    std::size_t line = this->f->get_line();
    std::size_t column = this->f->get_column();
    std::size_t offset = this->f->get_offset();
    this->lastchar = this->get_char();
    if (this->lastchar == '=') {
      Token e = Token(tok_ge, line, column, offset, ">=");
      this->tokens.push_back(e);
      this->lastchar = this->get_char();
      return this->tokens.back();
    } else {
      Token e = Token(tok_gt, line, column, offset, ">");
      this->tokens.push_back(e);
      return this->tokens.back();
    }
  }
  if (this->lastchar == '"') {
    std::string lexeme = "";
    std::size_t line = this->f->get_line();
    std::size_t column = this->f->get_column();
    std::size_t offset = this->f->get_offset();
    this->lastchar = this->get_char();
    while (this->lastchar != '"') {
      lexeme += this->lastchar;
      this->lastchar = this->get_char();
    }
    Token e = Token(tok_string, line, column, offset, lexeme);
    this->tokens.push_back(e);
    this->lastchar = this->get_char();
    return this->tokens.back();
  }
  // number
  if (std::isdigit(this->lastchar)) {
    std::string lexeme = "";
    std::size_t line = this->f->get_line();
    std::size_t column = this->f->get_column();
    std::size_t offset = this->f->get_offset();
    bool has_dot = false;
    while (std::isdigit(this->lastchar) || this->lastchar == '.') {
      if (this->lastchar == '.') {
        if (has_dot) {
          fprintf(stderr, "Invalid number format at line %lu, column %lu\n",
                  line, column);
          exit(EXIT_FAILURE);
        }
        has_dot = true;
      }
      lexeme += this->lastchar;
      this->lastchar = this->get_char();
    }

    // check for invalid number format
    if (lexeme.back() == '.') {
      fprintf(stderr, "Invalid number format at line %lu, column %lu\n", line,
              column);
      exit(EXIT_FAILURE);
    }

    Token e = Token(tok_number, line, column, offset, lexeme);
    this->tokens.push_back(e);
    return this->tokens.back();
  }

  // identifier of keyword
  if (std::isalpha(this->lastchar)) {
    std::string lexeme = "";
    std::size_t line = this->f->get_line();
    std::size_t column = this->f->get_column();
    std::size_t offset = this->f->get_offset();
    while (std::isalnum(this->lastchar) || this->lastchar == '_') {
      lexeme += this->lastchar;
      this->lastchar = this->get_char();
    }

    if (lexeme == "and") {
      Token e = Token(tok_and, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "class") {
      Token e = Token(tok_class, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "else") {
      Token e = Token(tok_else, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "false") {
      Token e = Token(tok_false, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "func") {
      Token e = Token(tok_func, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "for") {
      Token e = Token(tok_for, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "if") {
      Token e = Token(tok_if, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "nil") {
      Token e = Token(tok_nil, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "or") {
      Token e = Token(tok_or, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "return") {
      Token e = Token(tok_return, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "super") {
      Token e = Token(tok_super, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "this") {
      Token e = Token(tok_this, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "true") {
      Token e = Token(tok_true, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "var") {
      Token e = Token(tok_var, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "while") {
      Token e = Token(tok_while, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "list") {
      Token e = Token(tok_list, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else if (lexeme == "print") {
      Token e = Token(tok_print, line, column, offset, lexeme);
      this->tokens.push_back(e);
    } else {
      Token e = Token(tok_ident, line, column, offset, lexeme);
      this->tokens.push_back(e);
    }
    return this->tokens.back();
  }

  // unknown character
  if (this->lastchar != EOF) {
    fprintf(stderr, "Unknown character %c at line %lu, column %lu\n",
            this->lastchar, this->f->get_line(), this->f->get_column());
    exit(EXIT_FAILURE);
  }
  return this->tokens.back();
}

std::ostream &operator<<(std::ostream &out, const Token &t) {
  out << "<Line: " << t.getLine() << ", Column: " << t.getColumn()
      << ", Offset: " << t.getOffset() << ", Type: " << t.getType()
      << ", Lexeme: " << t.getLexeme() << ">";
  return out;
}

std::ostream &operator<<(std::ostream &out, const TokenType &t) {
  switch (t) {
  case tok_eof:
    out << "EOF";
    break;
  case tok_lbrace:
    out << "LBRACE";
    break;
  case tok_rbrace:
    out << "RBRACE";
    break;
  case tok_lparen:
    out << "LPAREN";
    break;
  case tok_rparen:
    out << "RPAREN";
    break;
  case tok_lbracket:
    out << "LBRACKET";
    break;
  case tok_rbracket:
    out << "RBRACKET";
    break;
  case tok_comma:
    out << "COMMA";
    break;
  case tok_dot:
    out << "DOT";
    break;
  case tok_colon:
    out << "COLON";
    break;
  case tok_semicolon:
    out << "SEMICOLON";
    break;
  case tok_plus:
    out << "PLUS";
    break;
  case tok_minus:
    out << "MINUS";
    break;
  case tok_star:
    out << "STAR";
    break;
  case tok_slash:
    out << "SLASH";
    break;
  case tok_not:
    out << "NOT";
    break;
  case tok_assign:
    out << "ASSIGN";
    break;
  case tok_lt:
    out << "LT";
    break;
  case tok_gt:
    out << "GT";
    break;
  case tok_ge:
    out << "GE";
    break;
  case tok_le:
    out << "LE";
    break;
  case tok_eq:
    out << "EQ";
    break;
  case tok_ne:
    out << "NE";
    break;
  case tok_ident:
    out << "IDENT";
    break;
  case tok_number:
    out << "NUMBER";
    break;
  case tok_string:
    out << "STRING";
    break;
  case tok_and:
    out << "AND";
    break;
  case tok_class:
    out << "CLASS";
    break;
  case tok_else:
    out << "ELSE";
    break;
  case tok_false:
    out << "FALSE";
    break;
  case tok_func:
    out << "FUNC";
    break;
  case tok_for:
    out << "FOR";
    break;
  case tok_if:
    out << "IF";
    break;
  case tok_nil:
    out << "NIL";
    break;
  case tok_or:
    out << "OR";
    break;
  case tok_return:
    out << "RETURN";
    break;
  case tok_super:
    out << "SUPER";
    break;
  case tok_this:
    out << "THIS";
    break;
  case tok_true:
    out << "TRUE";
    break;
  case tok_var:
    out << "VAR";
    break;
  case tok_while:
    out << "WHILE";
    break;
  case tok_list:
    out << "LIST";
    break;
  case tok_print:
    out << "PRINT";
    break;
  default:
    out << "UNKNOWN";
    break;
  }
  return out;
}