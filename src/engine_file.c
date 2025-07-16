#include "engine.h"

engine_file_t engine_file_load_as_string(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    engine_file_t ret;
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
  engine_file_t ret;
  ret.text = buf;
  ret.length = length;
  ret.error = 0;
  return ret;
}

void engine_file_free(const engine_file_t file) { free(file.text); }
