#ifndef SCREEN_NAME_H_
#define SCREEN_NAME_H_

#include <time.h>
#include <stdlib.h>

#define FIRST_NAME_COUNT 10
#define ADJECTIVE_COUNT 14
#define LAST_NAME_COUNT 13

char *first_names[FIRST_NAME_COUNT] = {
  "Kevin",
  "Isha",
  "Rafael",
  "Joey",
  "Jeff",
  "Bilbo",
  "Richard",
  "Lance",
  "Romeo",
  "Hulio"
};

char *adjectives[ADJECTIVE_COUNT] = {
  "Fluffy",
  "Hammered",
  "Feisty",
  "Provocative",
  "Pesky",
  "Well endowed",
  "Sleepy",
  "Doped",
  "Innocent",
  "Easy",
  "Pimpin",
  "Hungry",
  "Abrasive",
  "Raging"
};

char *last_names[LAST_NAME_COUNT] = {
  "Dwarve-lord",
  "Dragon-slayer",
  "Fiddler",
  "Moose-Rider",
  "Munchkin",
  "Beaver-Tamer",
  "Elf Friend",
  "Rogue",
  "Strong Arms",
  "Maiden",
  "Penguin",
  "Accident",
  "Balla"
};

char* get_screen_name() {
  char *name = " ";
  strcpy(name, first_names[rand() % FIRST_NAME_COUNT]);
  strcat(name, " the ");
  strcat(name, adjectives[rand() % ADJECTIVE_COUNT]);
  strcat(name, " ");
  strcat(name, last_names[rand() % LAST_NAME_COUNT]);
  printf("\nName returned %s", name);
  return name;
}

#endif
