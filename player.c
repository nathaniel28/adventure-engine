#include <stdlib.h>
#include <string.h>

#include "player.h"
#include "util.h"

Player* init_player(Area *starting_area) {
  Player *new_player = malloc(sizeof(Player));
  if (!new_player) panic("out of memory");
  
  new_player->currentarea = starting_area;

  new_player->inventory = NULL;
  new_player->i_len = 0;
  
  new_player->debug_mode = true;
  
  return new_player;
}

int search_inventory(char *item) {
  for (int i = 0; i < player->i_len; i++) {
    if (strcmp(player->inventory[i]->name, item) == 0) {
      return i;
    }
  }
  return -1;
}
