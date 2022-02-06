#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "area.h"
#include "player.h"
#include "util.h"

Area* make_area(char *name) {
  Area *new_area = safe_malloc(sizeof(Area));
  new_area->name = safe_strdup(name);

  //probably should use calloc
  
  new_area->description = NULL;

  new_area->connections = NULL;
  new_area->c_directions = NULL;
  new_area->c_len = 0;

  new_area->interactables = NULL;
  new_area->i_len = 0;
  
  new_area->event = NULL;
  
  return new_area;
}

Description* make_description(char *text) {
  Description *new_description = safe_malloc(sizeof(Description));
  char **split = NULL;
  int len = 0;
  split_string(text, "\\", &split, &len, -1);
  new_description->text = split;
  new_description->flags = safe_malloc(sizeof(bool)*len);
  new_description->f_len = len;
  for (int i = 0; i < len; i++) {
    new_description->flags[i] = true;
  }
  return new_description;
}

Interactable* make_interactable(char *name, Description *description, int type) {
  Interactable *new_interactable = safe_malloc(sizeof(Interactable));
  new_interactable->name = safe_strdup(name);
  new_interactable->description = description;
  new_interactable->type = type;
  new_interactable->is_dropped_item = false;
  return new_interactable;
}

void add_interactable(Area *area, char *name, char *text, int type) {
  Description *description = make_description(text);
  Interactable *interactable = make_interactable(name, description, type);
  add_to_list((void ***) &area->interactables, (void *) interactable, area->i_len);
  (area->i_len)++;
}

void link_areas(Area *here, Area *there, char *dir) {
  add_to_list((void ***) &here->connections, (void *) there, here->c_len);
  add_to_list((void ***) &here->c_directions, (void *) dir, here->c_len);
  (here->c_len)++;
}

void add_event(Area *area, bool (*e)(char *cmd)) { //get rid of this?
  area->event = e;
}

void print_description(Area *area) {
  for (int i = 0; i < area->description->f_len; i++) { //a near copy of this for loop and code within can be found in commands.c, so should it become a function?
    if (area->description->flags[i]) {
      printf("%s", area->description->text[i]);
    }
  }
  int dropped_items_found = 0;
  for (int i = 0; i < area->i_len; i++) {
    if (area->interactables[i]->is_dropped_item) {
      if (dropped_items_found == 0) printf(" You have dropped the following item(s) here:\n");
      printf("%d) %s\n", ++dropped_items_found, area->interactables[i]->name);
    }
  }
  printf("\n");
}
