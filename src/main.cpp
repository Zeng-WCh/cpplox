#include "scanner.h"
#include <iostream>

using namespace std;

const char *msg = "Usage: %s [input file]\n";

int main(int argc, const char **argv) {
  Scanner *sc = nullptr;

  if (argc == 1) {
    // file = new File();
    sc = new Scanner();
  } else if (argc == 2) {
    // file = new File(argv[1]);
    sc = new Scanner(argv[1]);
  } else {
    fprintf(stderr, msg, argv[0]);
    return 1;
  }

  while (!sc->is_eof()) {
    Token &t = sc->next_token();
    cout << t << endl;
  }

  if (sc) {
    delete sc;
  }

  return 0;
}