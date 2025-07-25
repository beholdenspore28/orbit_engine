#include "engine.h"

#define ENGINE_FILE_CHUNK_SIZE (64 /* chars */)
#define ENGINE_FILE_GROWTH (4 /* times */)

struct engine_file engine_file_load_as_string(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    struct engine_file ret;
    ret.error = 1;
    ret.text = NULL;
    return ret;
  }
  size_t alloc = ENGINE_FILE_CHUNK_SIZE * ENGINE_FILE_GROWTH;
  char *buf = (char *)malloc(alloc);
  size_t length = 0;
  while (!feof(file)) {
    if (alloc - length <= ENGINE_FILE_CHUNK_SIZE + 1) {
      alloc += ENGINE_FILE_CHUNK_SIZE;
      alloc *= ENGINE_FILE_GROWTH;
      buf = (char *)realloc((void *)buf, alloc);
    }
    int got = fread((void *)&buf[length], 1, ENGINE_FILE_CHUNK_SIZE, file);
    length += got;
    if (got != ENGINE_FILE_CHUNK_SIZE) {
      break;
    }
  }
  buf[length] = '\0';
  fclose(file);
  struct engine_file ret;
  ret.text = buf;
  ret.length = length;
  ret.error = 0;
  return ret;
}

void engine_file_free(const struct engine_file file) { free(file.text); }
