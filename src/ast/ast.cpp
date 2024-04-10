#include "ast.h"

void Program::print(int indent) {
  for (auto decl : decls) {
    decl->print(indent);
  }
}