#include <stdio.h>
#include <string.h>

#include "event.h"
#include "util.h"

typedef struct {
  char *watching;
  bool **flags;
  int watch_type;
  
} Watcher;

Watcher **watchers;
int watcher_len = 0;

int events_len = 0;

void add_global_event(char *name, bool flag) {
  Event *new_event = safe_malloc(sizeof(Event));
  new_event->name = safe_strdup(name);
  new_event->status = flag;
  add_to_list((void ***) &global_events, (void *) new_event, events_len);
  events_len++;
}

bool* event_status(char *name) {
  for (int i = 0; i < events_len; i++) {
    if (strcmp(name, global_events[i]->name) == 0) {
      return &(global_events[i]->status);
    }
  }
  panic("Specified event not found.");
  return NULL; //This line will never run. It's here to calm down the compiler that doesn't know that panic() terminates the program.
}

void watch_array(char *item, bool *flag_p[], int instruction) {
  Watcher *new_watcher = safe_malloc(sizeof(Watcher));
  new_watcher->watching = safe_strdup(item);
  new_watcher->flags = flag_p;
  new_watcher->watch_type = instruction;
  add_to_list((void ***) &watchers, (void *) new_watcher, watcher_len);
  (watcher_len)++;
}

void watch(char *item, bool *flag, int instruction) {
  watch_array(item, (bool *[]) {flag}, instruction);
}

void update_watchers(char *item) {
  for (int i = 0; i < watcher_len; i++) {
    if (strcmp(item, watchers[i]->watching) == 0) {
      for (int j = 0; watchers[i]->flags[j] != NULL; j++) {
	switch (watchers[i]->watch_type) {
	case WATCH_SET_FALSE:
	  *(watchers[i]->flags[j]) = false;
	  break;
	case WATCH_SET_TRUE:
	  *(watchers[i]->flags[j]) = true;
	  break;
	case WATCH_TOGGLE:
	  *(watchers[i]->flags[j]) = !(*(watchers[i]->flags[j]));
	  break;
	default:
	  panic("Invalid watch type.");
	}
      }
      return;
    }
  }
}
