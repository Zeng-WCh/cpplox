#pragma once
#ifndef __AST_H__
#define __AST_H__

#include <vector>

class Stmts;

class Ast {
public:
  virtual ~Ast() = 0;
  virtual void print(int indent) = 0;
};

inline Ast::~Ast() {
  // Make the compiler happy
}

class Program : Ast {
private:
  std::vector<Stmts *> statements;

public:
  Program() = default;
  ~Program();
};

#endif