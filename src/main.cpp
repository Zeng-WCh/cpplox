#include "scanner.h"
#include <iostream>

using namespace std;

const char *msg = "Usage: %s [input file]\n";

int main(int argc, const char **argv) {
  switch (argc) {
  case 1:
    // interactive mode
    printf("Enter a string to scan: ");
    break;
  case 2:
    // file mode
    // FIXME: implement file mode
    fprintf(stderr, "FIXME: file mode not implemented\n");
    return 64;
  default:
    fprintf(stderr, msg, argv[0]);
    return 64;
  }

  return 0;
}