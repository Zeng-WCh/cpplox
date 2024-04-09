#pragma once
#include <ostream>
#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

enum TokenType {
  tok_eof = -1,

  // One character
  tok_lparen = 257, // '(
  tok_rparen,       // ')
  tok_lbracket,     // '[
  tok_rbracket,     // ']
  tok_lbrace,       // '{'
  tok_rbrace,       // '}'
  tok_comma,        // ','
  tok_dot,          // '.'
  tok_colon,        // ':'
  tok_semicolon,    // ';'
  tok_plus,         // '+'
  tok_minus,        // '-'
  tok_star,         // '*'
  tok_slash,        // '/'
  tok_not,          // '!'
  tok_assign,       // '='
  tok_lt,           // '<'
  tok_gt,           // '>'

  // Multi character
  tok_ge, // '>='
  tok_le, // '<='
  tok_eq, // '=='
  tok_ne, // '!='

  // literals
  tok_ident,
  tok_number,
  tok_string,

  // keywords
  tok_and,    // 'and'
  tok_class,  // 'class'
  tok_else,   // 'else'
  tok_false,  // 'false'
  tok_func,   // 'func'
  tok_for,    // 'for'
  tok_if,     // 'if'
  tok_nil,    // 'nil'
  tok_or,     // 'or'
  tok_return, // 'return'
  tok_super,  // 'super'
  tok_this,   // 'this'
  tok_true,   // 'true'
  tok_var,    // 'var'
  tok_while,  // 'while'
  tok_list,   // 'list'

  // built-in functions
  tok_print, // 'print'
};

class Token {
private:
  std::string lexeme;
  TokenType type;
  std::size_t line, column, offset;

public:
  inline Token(TokenType type, std::size_t line, std::size_t column,
               std::size_t offset, const std::string &lexeme = "")
      : lexeme(lexeme), type(type), line(line), column(column), offset(offset) {
  }

  virtual inline ~Token() = default;

  inline std::string getLexeme() const { return lexeme; }

  inline TokenType getType() const { return type; }
  inline std::size_t getLine() const { return line; }
  inline std::size_t getColumn() const { return column; }
  inline std::size_t getOffset() const { return offset; }

  // friend std::ostream &operator<<(std::ostream &, const Token &);
};
std::ostream &operator<<(std::ostream &out, const Token &t);
std::ostream &operator<<(std::ostream &out, const TokenType &t);

// template <class T> class TokenWithValue : public Token {
// private:
//   T value;

// public:
//   inline TokenWithValue(TokenType type, std::size_t line, std::size_t column,
//                         std::size_t offset, const std::string &lexeme)
//       : Token(type, line, column, offset, lexeme) {}

//   inline T getValue() const { return value; }
// };

#endif