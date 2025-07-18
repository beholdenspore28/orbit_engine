#ifndef ENGINE_LIST_H
#define ENGINE_LIST_H

#include <stdlib.h>

#define LIST_INITIAL_CAPACITY (1)

typedef size_t list_size;

typedef struct {
  list_size capacity;
  list_size count;
} list_meta_data;

#define DECLARE_AND_DEFINE_LIST(type)                                          \
  DECLARE_LIST(type)                                                           \
  DEFINE_LIST(type)

#define DECLARE_LIST(type)                                                     \
  ENGINE_NOWARN("-Wunused-function", typedef type *list_##type;                \
                                                                               \
                list_size list_##type##_count(const list_##type list);         \
                                                                               \
                list_##type list_##type##_alloc(void);                         \
                                                                               \
                void list_##type##_free(list_##type list);                     \
                                                                               \
                void list_##type##_add(list_##type *list, const type element); \
                                                                               \
                list_##type list_##type##_alloc_from_array(list_##type other,  \
                                                           list_size length);  \
                                                                               \
                void list_##type##_remove_at(const list_##type list,           \
                                             const list_size index);)

#define DEFINE_LIST(type)                                                      \
                                                                               \
  ENGINE_NOWARN(                                                               \
      "-Wunused-function",                                                     \
      list_size list_##type##_count(const list_##type list) {                  \
        list_meta_data *data = ((list_meta_data *)(list)) - 1;                 \
        return data->count;                                                    \
      }                                                                        \
                                                                               \
      list_##type list_##type##_alloc(void) {                                  \
        void *ptr = malloc(sizeof(list_meta_data) +                            \
                           (sizeof(type) * LIST_INITIAL_CAPACITY));            \
        list_meta_data *data = (list_meta_data *)ptr;                          \
        data->capacity = LIST_INITIAL_CAPACITY;                                \
        data->count = 0;                                                       \
                                                                               \
        return (list_##type)(data + 1);                                        \
      }                                                                        \
                                                                               \
      void list_##type##_free(list_##type list) {                              \
        list_meta_data *data = ((list_meta_data *)(list)) - 1;                 \
        free(data);                                                            \
      }                                                                        \
                                                                               \
      void list_##type##_add(list_##type *list, const type element) {          \
        list_meta_data *data = ((list_meta_data *)(*list)) - 1;                \
        if (data->count >= data->capacity) {                                   \
          const list_size new_capacity = data->count * 2 + 1;                  \
          data = realloc(data, sizeof(list_meta_data) +                        \
                                   (sizeof(type) * new_capacity));             \
          data->capacity = new_capacity;                                       \
          *list = (list_##type)(data + 1);                                     \
        }                                                                      \
        (*list)[data->count] = element;                                        \
        data->count++;                                                         \
      }                                                                        \
                                                                               \
      list_##type list_##type##_alloc_from_array(list_##type other,            \
                                                 list_size length) {           \
        list_##type l = list_##type##_alloc();                                 \
        for (list_size i = 0; i < length; i++) {                               \
          list_##type##_add(&l, other[i]);                                     \
        }                                                                      \
        return l;                                                              \
      }                                                                        \
                                                                               \
      void list_##type##_remove_at(const list_##type list,                     \
                                   const list_size index) {                    \
        if (!list)                                                             \
          return;                                                              \
        list_meta_data *data = ((list_meta_data *)(list)) - 1;                 \
        (list)[index] = (list)[data->count - 1];                               \
        data->count--;                                                         \
      })

#endif // ENGINE_LIST_H
