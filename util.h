#include <stdbool.h>

extern void panic(char *s);

extern void* safe_malloc(size_t size);

extern char* safe_strdup(char *s);

extern void add_to_list(void ***list, void *to_add, int len);

extern void remove_from_list(void ***list, int index, int len);

extern void split_string(char *str, char *spl, char ***list, int *len, int limit);
