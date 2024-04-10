#pragma once

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "token.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>

class File {
private:
  static constexpr std::size_t FILE_BUF_SIZE = 64;
  const std::string filename;
  int fd;
  char *buffer;
  std::size_t buffer_size;
  char *current, *end;
  std::size_t line, column, offset;

  bool newline;

protected:
  // Check if buffer is empty
  inline bool buffer_empty() const { return this->current == this->end; }
  // Check if EOF
  inline bool eof() const { return *(this->current) == EOF; }
  // Read a buf_size chunk of data from the file
  void read_a_chunk();

  void update_pos(bool newline = false);

public:
  inline File(const std::string &filename)
      : filename(filename), fd(-1), buffer(nullptr), buffer_size(FILE_BUF_SIZE),
        current(nullptr), end(nullptr), line(1), column(0), offset(0),
        newline(false) {
    if (filename == "-") {
      this->fd = STDIN_FILENO;
    } else {
      // Open file (read-only mode
      this->fd = open(filename.c_str(), O_RDONLY);
      if (this->fd == -1) {
        // Error
        fprintf(stderr, "Unable to open file %s, using input...\n",
                filename.c_str());
        // change to stdin
        fd = STDIN_FILENO;
      }
    }

    this->buffer = (char *)malloc(sizeof(char) * this->buffer_size);

    if (this->buffer == nullptr) {
      // Error
      fprintf(stderr, "Unable to allocate memory for file buffer\n");
      exit(EXIT_FAILURE);
    }

    memset(this->buffer, 0, this->buffer_size);

    this->current = this->buffer;
    this->end = this->buffer;
  }

  inline File() : File("-") {}

  File(const File &) = delete;
  File(const File &&) = delete;
  File &operator=(const File &) = delete;

  inline ~File() {
    if (this->fd != STDIN_FILENO && this->fd != -1) {
      close(this->fd);
    }
    free(this->buffer);
  }

  inline bool is_from_file() const { return this->fd != STDIN_FILENO; }
  inline std::size_t get_line() const { return this->line; }
  inline std::size_t get_column() const { return this->column; }
  inline std::size_t get_offset() const { return this->offset; }

  inline bool is_eof() const { return this->eof(); }

  char next_char();

  char peek() const;
};

class Scanner {
private:
  File *f;
  std::vector<Token> tokens;
  int lastchar;

protected:
  int get_char();

public:
  inline Scanner(const std::string &filename)
      : f(new File(filename)), lastchar(' ') {}

  inline Scanner() : f(new File()), lastchar(' ') {}

  Scanner(const Scanner &) = delete;
  Scanner(const Scanner &&) = delete;
  Scanner &operator=(const Scanner &) = delete;

  inline ~Scanner() { delete this->f; }

  Token &next_token();
  Token &peek_token() const;

  inline bool is_eof() const { return this->f->is_eof(); }
};

#endif