#pragma once
#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <string>

enum TokenType {
  tok_eof = -1,
  // types
  tok_var = 256,
  tok_func,

  // keywords
  tok_class,
  tok_this,
  tok_super,

  // control flow
  tok_if,
  tok_else,
  tok_while,
  tok_for,
  tok_break,
  tok_continue,
  tok_return,

  // modules
  tok_import,
  tok_as,
  toko_from,

  tok_colon,     // ':'
  tok_semicolon, // ';'
  // operators
  tok_assign,     // '='
  tok_gr,         // '>'
  tok_less,       // '<'
  tok_ge,         // '>='
  tok_le,         // '<='
  tok_and,        // 'and'
  tok_or,         // 'or'
  tok_eq,         // '=='
  tok_ne,         // '!='
  tok_add,        // '+'
  tok_sub,        // '-'
  tok_mul,        // '*'
  tok_div,        // '/'
  tok_self_add,   // '++'
  tok_self_sub,   // '--'
  tok_add_assign, // '+='
  tok_sub_assign, // '-='
  tok_mul_assign, // '*='
  tok_div_assign, // '/='
  tok_dot,        // '.'
  tok_comma,      // ','
  tok_lp,         // '('
  tok_rp,         // ')'
  tok_lbracket,   // '['
  tok_rbracket,   // ']'
  tok_lbrace,     // '{'
  tok_rbrace,     // '}'

  // literals
  tok_true,
  tok_false,
  tok_nil, // works as null in C
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
};

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