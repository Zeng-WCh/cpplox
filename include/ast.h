#pragma once
#ifndef __AST_H__
#define __AST_H__

#include <string>
#include <vector>

// https://craftinginterpreters-zh.vercel.app/appendix-i.html

class Declaration;
class Func;
class Expr;
class LogicAnd;
class Call;
class LogicOr;
class Equality;
class Comparison;
class Addition;
class Term;
class Factor;
class Unary;
class Primary;
class Call;
class Arguments;
class CallTail;
class Parameters;

class Ast {
public:
  virtual ~Ast() = 0;
  virtual void print(int indent) = 0;
};

inline Ast::~Ast() {
  // Make the compiler happy
}

class Program : public Ast {
private:
  std::vector<Declaration *> decls;

public:
  Program() = default;
  ~Program();

  void print(int indent) override;
};

class Declaration : public Ast {
public:
  virtual ~Declaration() = 0;
  virtual void print(int indent) = 0;
};

inline Declaration::~Declaration() {
  // Make the compiler happy
}

class ClassDeclaration : public Declaration {
private:
  std::vector<Func *> methods;
  std::string name, parent;

public:
  ClassDeclaration() = default;
  ~ClassDeclaration();

  void print(int indent) override;
};

class FunctionDeclaration : public Declaration {
private:
  Func *func;

public:
  FunctionDeclaration() = default;
  ~FunctionDeclaration();

  void print(int indent) override;
};

class VariableDeclaration : public Declaration {
private:
  std::string name;
  Expr *init_expr;

public:
  VariableDeclaration() = default;
  ~VariableDeclaration();

  void print(int indent) override;
};

class Statement : public Declaration {
public:
  virtual ~Statement() = 0;
  virtual void print(int indent) = 0;
};

class ExprStmt : public Statement {
private:
  Expr *expr;

public:
  ExprStmt() = default;
  ~ExprStmt();

  void print(int indent) override;
};

class ForStmt : public Statement {
private:
  // init can be ether a VariableDeclaration or an ExprStmt
  VariableDeclaration *init_var;
  ExprStmt *init_expr;

  Expr *cond, *update;
  Statement *body;

public:
  ForStmt() = default;
  ~ForStmt();

  void print(int indent) override;
};

class IfStmt : public Statement {
private:
  Expr *cond;
  Statement *then_stmt, *else_stmt;

public:
  IfStmt() = default;
  ~IfStmt();

  void print(int indent) override;
};

class PrintStmt : public Statement {
private:
  Expr *expr;

public:
  PrintStmt() = default;
  ~PrintStmt();

  void print(int indent) override;
};

class ReturnStmt : public Statement {
private:
  Expr *expr;

public:
  ReturnStmt() = default;
  ~ReturnStmt();

  void print(int indent) override;
};

class WhileStmt : public Statement {
private:
  Expr *cond;
  Statement *body;

public:
  WhileStmt() = default;
  ~WhileStmt();

  void print(int indent) override;
};

class Block : public Statement {
private:
  std::vector<Declaration *> stmts;

public:
  Block() = default;
  ~Block();

  void print(int indent) override;
};

class Expr : public Ast {
public:
  virtual ~Expr() = 0;
  virtual void print(int indent) = 0;
};

class Assignment : public Expr {
private:
  Call *call;
  std::string name;
  Assignment *value;
  LogicOr *logic_or;

public:
  Assignment() = default;
  ~Assignment();

  void print(int indent) override;
};

class LogicOr : public Ast {
private:
  LogicAnd *left;
  std::vector<LogicAnd *> right;

public:
  LogicOr() = default;
  ~LogicOr();

  void print(int indent) override;
};

class LogicAnd : public Ast {
private:
  Equality *left;
  std::vector<Equality *> right;

public:
  LogicAnd() = default;
  ~LogicAnd();

  void print(int indent) override;
};

class Equality : public Ast {
public:
  enum EqualityOp { NOT_EQUAL, EQUAL };

private:
  Comparison *left;
  std::vector<Comparison *> right;
  std::vector<EqualityOp> ops;

public:
  Equality() = default;
  ~Equality();

  void print(int indent) override;
};

class Comparison : public Ast {
public:
  enum ComparisonOp { GREATER, GREATER_EQUAL, LESS, LESS_EQUAL };

private:
  Addition *left;
  std::vector<Addition *> right;
  std::vector<ComparisonOp> ops;

public:
  Comparison() = default;
  ~Comparison();

  void print(int indent) override;
};

class Term : public Ast {
public:
  enum TermOp { ADD, MINUS };

private:
  Factor *left;
  std::vector<Factor *> right;
  std::vector<TermOp> ops;

public:
  Term() = default;
  ~Term();

  void print(int indent) override;
};

class Factor : public Ast {
public:
  enum FactorOp { DIVIDE, MULTI };

private:
  Unary *left;
  std::vector<Unary *> right;
  std::vector<FactorOp> ops;

public:
  Factor() = default;
  ~Factor();

  void print(int indent) override;
};

class Unary : public Ast {
public:
  enum UnaryOps { NOT, NEGATIVE };

private:
  UnaryOps prefix;
  Unary *value_unary;
  Call *value_call;

public:
  Unary() = default;
  ~Unary();

  void print(int indent) override;
};

class Call : public Ast {
private:
  Primary *primary;
  std::vector<CallTail *> tails;

public:
  Call() = default;
  ~Call();

  void print(int indent) override;
};

class CallTail : public Ast {
public:
  virtual ~CallTail() = 0;
  virtual void print(int) = 0;
};

inline CallTail::~CallTail() {}

class CallArgs : public CallTail {
private:
  Arguments *arg;

public:
  CallArgs() = default;
  ~CallArgs();

  void print(int indent) override;
};

class CallField : public CallTail {
private:
  std::string name;

public:
  CallField() = default;
  ~CallField();

  void print(int indent) override;
};

class Primary : public Ast {
public:
  virtual ~Primary() = 0;
  virtual void print(int) = 0;
};

inline Primary::~Primary() {}

class TruePrimary : public Primary {
public:
  inline ~TruePrimary(){};
  void print(int indent) override;
};

class FalsePrimary : public Primary {
public:
  inline ~FalsePrimary(){};
  void print(int indent) override;
};

class NilPrimary : public Primary {
public:
  inline ~NilPrimary(){};
  void print(int indent) override;
};

class NumberPrimary : public Primary {
private:
  double val;

public:
  inline NumberPrimary(double val) : val(val){};
  inline ~NumberPrimary() {}

  void print(int indent) override;
};

class ThisPrimary : public Primary {
public:
  inline ~ThisPrimary(){};
  void print(int indent) override;
};

class ExprPrimary : public Primary {
private:
  Expr *expr;

public:
  ExprPrimary() = default;
  ~ExprPrimary();

  void print(int indent) override;
};

class SuperPrimary : public Primary {
private:
  std::string name;

public:
  SuperPrimary() = default;
  ~SuperPrimary();

  void print(int indent) override;
};

class Func : public Ast {
private:
  std::string name;
  Parameters *params;
  Block *body;

public:
  Func() = default;
  ~Func();

  void print(int indent) override;
};

class Parameters : public Ast {
private:
  std::vector<std::string> params;

public:
  Parameters() = default;
  ~Parameters();

  void print(int indent) override;
};

class Arguments : public Ast {
private:
  std::vector<Expr *> args;

public:
  Arguments() = default;
  ~Arguments();

  void print(int indent) override;
};

// For performance reason, we use visitor pattern
class Visitor {
public:
  void visit(Program *node);
  void visit(ClassDeclaration *node);
  void visit(FunctionDeclaration *node);
  void visit(VariableDeclaration *node);
  void visit(Statement *node);
};

#endif