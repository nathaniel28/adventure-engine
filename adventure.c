#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "area.h"
#include "commands.h"
#include "event.h"
#include "player.h"
#include "util.h"

Player *player;
Event **global_events;

bool small_room_event(char *cmd) {
  if (strcmp(cmd, "lift rug") == 0) {
    if (*event_status("rug lifted")) {
      printf("You already looked under the rug.\n");
    } else {
      add_interactable(player->currentarea, "key", "", TYPE_ITEM);
      printf("Lifting the rug reveals a key.\n");
      *event_status("rug lifted") = true;
    }
    return true;
  }
  if (strcmp(cmd, "go e") == 0 || strcmp(cmd, "go east") == 0) {
    if (search_inventory("key") != -1) {
      printf("You unlock the door.\n");
      do_command(cmd);
    } else {
      printf("You try to open the door, but it is locked.\n");
    }
    return true;
  }
  return false;
}

bool hallway_event(char *cmd) {
  if (strcmp(cmd, "take lantern") == 0) {
    if (search_inventory("ladder") != -1) {
      printf("Using the ladder, you can reach for the lantern.\n");
      do_command(cmd);
    } else {
      printf("You cannot reach it.\n");
    }
    return true;
  }
  return false;
}

bool large_room_event(char *cmd) {
  if (strcmp(cmd, "examine object") == 0) {
    if (search_inventory("ladder") != -1) {
      do_command(cmd);
    } else {
      printf("The ladder isn't here.\n");
    }
    return true;
  }
  if (strcmp(cmd, "go n") == 0 || strcmp(cmd, "go north") == 0) {
    if (search_inventory("lantern") != -1) {
      do_command(cmd);
    } else {
      printf("You cannot go that direction here.\n");
    }
    return true;
  }
  return false;
}

bool cavern_event(char *cmd) {
  if (strcmp(cmd, "take gemstone") == 0 || strcmp(cmd, "take diamond") == 0) {
    printf("Congratulations, you win.\n");
    exit(0);
  }
  return false;
}

int main() {
  
  Area *small_room;
  Area *hallway;
  Area *large_room;
  Area *cavern;
  
  small_room = make_area("Small Room");
  small_room->description = make_description("You stand in a small square room. You see a door to your east. You stand atop a comfortable rug.");
  add_global_event("rug lifted", false);

  add_interactable(small_room, "door", "It's a heavy wooden door.", TYPE_EXAMINABLE);
  add_interactable(small_room, "rug", "You see here a patterned rug. It's dark red.\\ There is a lump near the center, as if something was underneath.", TYPE_EXAMINABLE);

  add_event(small_room, &small_room_event);
  
  hallway = make_area("Hallway");
  hallway->description = make_description("You're in a hallway\\, lit by a single lantern, hanging high above\\. To the west, there is a door back to the small room. To the east, there is an archway leading to another room.");

  add_interactable(hallway, "lantern", "It's a metal lantern, hanging from a chain. It sways slightly, though you don't feel a breeze. You can't reach it.", TYPE_ITEM);
  add_interactable(hallway, "archway", "You glance at the stone archway. It looks sturdy.", TYPE_EXAMINABLE);
  
  add_event(hallway, &hallway_event);
  
  large_room = make_area("Dark Room");
  large_room->description = make_description("You can hardly see anything past the archway to your west that you just walked through.\\ You can just barely see some object on the south wall.\\ The lantern in the hallway flickers.\\Now illuminated, you notice the room is dusty and plain. The air tastes stale. You now see an exit to your north.");
  large_room->description->flags[3] = false;
  watch_array("lantern", (bool *[]) {&(large_room->description->flags[0]), &(large_room->description->flags[1]), &(large_room->description->flags[2]), &(large_room->description->flags[3]), NULL}, WATCH_TOGGLE);

  add_interactable(large_room, "archway", "Unsuprisingly, it hasn't changed.", TYPE_EXAMINABLE);
  add_interactable(large_room, "lantern", "The lantern glows softly, made dim through distance.", TYPE_EXAMINABLE);
  add_interactable(large_room, "object", "As you get closer, you realize it's a ladder.", TYPE_EXAMINABLE);
  add_interactable(large_room, "ladder", "As you get closer, you realize it's a ladder.", TYPE_ITEM);
  
  add_event(large_room, &large_room_event);

  cavern = make_area("End");
  cavern->description = make_description("You enter a large cavern. In the center, there lies a pedastle atop which lies a single gemstone.");

  add_interactable(cavern, "gemstone", "It's a small white gem.", TYPE_EXAMINABLE);
  add_interactable(cavern, "diamond", "It's a small white gem.", TYPE_EXAMINABLE);

  add_event(cavern, &cavern_event);
  
  //small_room
  link_areas(small_room, hallway, "east");
  
  //hallway
  link_areas(hallway, small_room, "west");
  link_areas(hallway, large_room, "east");
  
  //large_room
  link_areas(large_room, hallway, "west");
  link_areas(large_room, cavern, "north");

  //cavern
  link_areas(cavern, large_room, "south");

  player = init_player(small_room);
  
  printf("%s\n", player->currentarea->name);
  print_description(player->currentarea);
  char str[256];
  while (1) {
    str[255] = '\n';
    fgets(str, 256, stdin);
    if (str[255] != '\n') {
      printf("This command is too long.\n");
      continue;
    }
    if (strlen(str) < 2) continue;
    str[strlen(str) - 1] = '\0';
    if (player->currentarea->event != NULL) {
      if (player->currentarea->event(str))
	continue;
    }
    do_command(str);
  }
}
