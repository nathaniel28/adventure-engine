#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

void panic(char *s) {
  printf("%s\n", s);
  exit(1);
}

void* safe_malloc(size_t size) {
  void *out = malloc(size);
  if (!out) panic("Out of memory.");
  return out;
}

char* safe_strdup(char *s) {
  char *out = strdup(s);
  if (!out) panic("Out of memory.");
  return out;
}

void add_to_list(void ***list, void *to_add, int len) {
  len++;
  *list = realloc(*list, sizeof(void **)*(len));
  if (!*list) panic("Out of memory.");
  (*list)[(len)-1] = to_add;
}

void remove_from_list(void ***list, int index, int len) { //does not free the pointer. it's up to the caller.
  len--;
  if (len > 0) {
    (*list)[index] = (*list)[len];
  }
  *list = realloc(*list, sizeof(void **)*(len));
  if (!*list && len > 0) panic("Out of memory.");
}

void split_string(char *str, char *spl, char ***list, int *len, int limit) {
  *list = safe_malloc(sizeof(char *));
  char *dup = safe_strdup(str);
  *list[0] = dup;
  *len = 1;
  for (int i = 0; dup[i] != '\0'; i++) {
    if (limit == 0) return;
    char *token = NULL;
    for (int j = 0; ; j++) {
      if (dup[i + j] != spl[j] || dup[j + i] == '\0') break;
      if (spl[j + 1] == '\0') {
        dup[i] = '\0';
        if (dup[i + j + 1] != '\0') token = &dup[i + j + 1];
      }
    }
    if (token != NULL) {
      *list = realloc(*list, (*len + 1)*sizeof(char *));
      if (!(*list)) panic("Out of memory.");
      (*list)[(*len)++] = token;
      limit--;
    }
  }
}
