#ifndef ENGINE_FILE_H
#define ENGINE_FILE_H

#define ENGINE_FILE_CHUNK_SIZE (64 /* chars */)
#define ENGINE_FILE_GROWTH (4 /* times */)

typedef struct {
  size_t length;
  char *text;
  int error;
} engine_file_t;

#endif // ENGINE_FILE_H
