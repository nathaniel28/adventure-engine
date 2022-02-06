#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "area.h"
#include "commands.h"
#include "event.h"
#include "player.h"
#include "util.h"

bool is_vowel(char c) {
  return strchr("aeiouAEIOU", c) != NULL;
}

/*
int search_interactables(Area *area, char *name, char *type) {
  for (int i = 0; i < area->i_len; i++) {
    if (strcmp(area->interactables[i], name) == 0 && strcmp(area->i_type[i], type) == 0) {
      return i;
    }
  }
  return -1;
}
*/

void cmd_go(char **args, int a_len) {
  if (a_len < 2) return;
  char *abbr[] = {"n", "s", "e", "w", "ne", "nw", "se", "sw"};
  char *norm[][2] = {{"", "north"}, {"", "south"}, {"", "east"}, {"", "west"}, {"", "northeast"}, {"", "northwest"}, {"", "southeast"}, {"", "southwest"}}; //empty string needed because cmd_go expects args to equal {"go", "<direction>"} (though it never checks if args[0] is "go", since do_command() does this) 
  for (int i = 0; i < 8; i++) { //iterates through abbr looking for turning args[1] into the non abbriviated version, if applicable
    if (strcmp(args[1], abbr[i]) == 0) {
      cmd_go(norm[i], 2);
      return;
    }
  }
  for (int i = 0; i < player->currentarea->c_len; i++) {
    if (strcmp(args[1], player->currentarea->c_directions[i]) == 0) {
      player->currentarea = player->currentarea->connections[i];
      printf("You enter the %s.\n\n%s\n", player->currentarea->name, player->currentarea->name);
      if (player->currentarea->event != NULL) {
	player->currentarea->event("");
      }
      print_description(player->currentarea);
      return;
    }
  }
  printf("You cannot go that direction here.\n");
}

/*
void move_to_list(void ***to_move, int tm_len, void ***other_list, int ot_len) { //use this for cmd_take() and cmd_drop()?

}
*/

void cmd_take(char **args, int a_len) { //make a move_to_list function? anything besides this...
  if (a_len < 2) return;
  for (int i = 0; i < player->currentarea->i_len; i++) {
    if (strcmp(args[1], player->currentarea->interactables[i]->name) == 0 && player->currentarea->interactables[i]->type == TYPE_ITEM) {
      add_to_list((void ***) &(player->inventory), (void *) player->currentarea->interactables[i], player->i_len);
      (player->i_len)++;
      remove_from_list((void ***) &(player->currentarea->interactables), i, player->currentarea->i_len);
      (player->currentarea->i_len)--;
      printf("You added a%s %s to your bag.\n", is_vowel(args[1][0]) ? "n" : "", args[1]);
      update_watchers(args[1]);
      return;
    }
  }
  printf("You can't take a%s %s.\n", is_vowel(args[1][0]) ? "n" : "", args[1]);
}
 
void cmd_drop(char **args, int a_len) {
  if (a_len < 2) return;
  for (int i = 0; i < player->i_len; i++) {
    if (strcmp(args[1], player->inventory[i]->name) == 0) {
      player->inventory[i]->is_dropped_item = true;
      add_to_list((void ***) &(player->currentarea->interactables), (void *) player->inventory[i], player->currentarea->i_len);
      (player->currentarea->i_len)++;
      remove_from_list((void ***) &(player->inventory), i, player->i_len);
      (player->i_len)--;
      printf("You dropped your %s.\n", args[1]);
      update_watchers(args[1]);
      return;
    }
  }
  printf("You don't have a%s %s.\n", is_vowel(args[1][0]) ? "n" : "", args[1]);
}

void cmd_examine(char **args, int a_len) {
  if (a_len < 2) return;
  for (int i = 0; i < player->currentarea->i_len; i++) {
    if (strcmp(args[1], player->currentarea->interactables[i]->name) == 0) {
      for (int j = 0; j < player->currentarea->interactables[i]->description->f_len; j++) { //a near copy of this for loop and code within can be found in area.c, so should it become a function?
	if (player->currentarea->interactables[i]->description->flags[j]) {
	  printf("%s", player->currentarea->interactables[i]->description->text[j]);
	}
      }
      printf("\n");
      return;
    }
  }
  printf("You can't examine a%s %s.\n", is_vowel(args[1][0]) ? "n" : "", args[1]);
}

void cmd_look(char **args, int a_len) {
  print_description(player->currentarea);
}

void cmd_inventory(char **args, int a_len) {
  if (player->i_len == 0) {
    printf("You don't have any items right now.\n");
    return;
  }
  printf("You have %d item%s.\n", player->i_len, (player->i_len > 1) ? "s" : "");
  for (int i = 0; i < player->i_len; i++) {
    printf("  %d) %s\n", i + 1, player->inventory[i]->name);
  }
}

void cmd_clear(char **args, int a_len) {
  system("clear");
}

void cmd_exit(char **args, int a_len) {
  printf("So long.\n");
  exit(0);
}

void cmd_help(char **args, int a_len) {
  printf(
	 "go (direction) - Moves you to the area in the direction you specify.\n"
	 "take (object) - Adds an object to your inventory.\n"
	 "drop (object) - Removes an object in your inventory.\n"
	 "examine (object) - Gives a description of something.\n"
	 "look - Gives the description of the area you are in.\n"
	 "inventory - Lists the items you have in your bag.\n"
	 "clear - Clears the screen.\n"
	 "exit - Terminates the program.\n"
	 "\nNote that this is not the full list of commands-- and just because it says you can't do something somewhere doesn't mean you can't do it everywhere.\n\n"
	 );
}

void area_stats(Area *area) {
  printf("Address: %p\nName: %s\n", area, area->name);
  for (int i = 0; i < area->description->f_len; i++) {
    printf("%s: %s\n", area->description->flags[i] ? "true" : "false", area->description->text[i]);
  }
  printf("Connections:\n");
  for (int i = 0; i < area->c_len; i++) {
    printf("%s: %p: %s\n", area->c_directions[i], area->connections[i], area->connections[i]->name);
  }
  for (int i = 0; i < area->i_len; i++) {
    printf("%d: %s\n", area->interactables[i]->type, area->interactables[i]->name);
  }
  printf("%p event\n", area->event);
}

void cmd_stats() {
  if (!player->debug_mode) {
    printf("You aren't allowed to do this.\n");
    return;
  }
  area_stats(player->currentarea);
}

Command commands[] = {
  {"go", &cmd_go},
  {"take", &cmd_take},
  {"get", &cmd_take},
  {"drop", &cmd_drop},
  {"examine", &cmd_examine},
  {"look", &cmd_look},
  {"inventory", &cmd_inventory},
  {"bag", &cmd_inventory},
  {"clear", &cmd_clear},
  {"exit", &cmd_exit},
  {"quit", &cmd_exit},
  {"help", &cmd_help},
  
  {"stats", &cmd_stats},
  
  {NULL, NULL}
};

void do_command(char *cmd) {
  for (int i = 0; cmd[i] != '\0'; i++) {
    cmd[i] = tolower(cmd[i]);
  }
  char **args = NULL;
  int a_len = 0;
  split_string(cmd, " ", &args, &a_len, 1);
  if (a_len == 0) {
    return;
  }
  for (int i = 0; commands[i].name != NULL; i++) {
    if (strcmp(args[0], commands[i].name) == 0) {
      (commands[i].function)(args, a_len);
      break;
    }
  }
  free(args[0]); //originally malloced as one thing.
  free(args);
}
