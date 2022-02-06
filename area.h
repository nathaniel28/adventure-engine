#ifndef AREA_H
#define AREA_H

#define TYPE_EXAMINABLE 0
#define TYPE_ITEM 1

#include <stdbool.h>

typedef struct {
  char **text;
  bool *flags;
  int f_len;
} Description;

typedef struct {
  char *name;
  Description *description;
  int type;
  bool is_dropped_item;
} Interactable;

typedef struct Area {
  char *name;

  Description *description;
  
  struct Area **connections;
  char **c_directions;
  int c_len;
  
  Interactable **interactables;
  int i_len;
  
  bool (*event)(char *cmd);
  
} Area;

extern Area* make_area(char *name);

extern Description* make_description(char *text);

extern Interactable* make_interactable(char *name, Description *description, int type);

extern void add_interactable(Area *area, char *i, char *text, int type);

extern void link_areas(Area *here, Area *there, char *dir);

extern void add_event(Area *area, bool (*e)(char *cmd));

extern void print_description(Area *area);

#endif
