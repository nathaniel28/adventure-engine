#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

#include "area.h"

typedef struct {
  Area *currentarea;
  
  Interactable **inventory;
  int i_len;
  
  bool debug_mode;
} Player;

extern Player *player;

extern Player* init_player(Area *starting_area);

extern int search_inventory(char *item);

#endif
