#include <stdbool.h>

#define WATCH_SET_FALSE 0
#define WATCH_SET_TRUE 1
#define WATCH_TOGGLE 2

typedef struct {
  char *name;
  bool status;
} Event;

extern Event **global_events;

extern void add_global_event(char *name, bool flag);

extern bool* event_status(char *name);

extern void watch_array(char *item, bool *flag_p[], int instruction);

extern void watch(char *item, bool *flag, int instruction);

extern void update_watchers(char *item);
