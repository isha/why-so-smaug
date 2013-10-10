#ifndef SCREEN_NAME_H_
#define SCREEN_NAME_H_

#include <time.h>
#include <stdlib.h>

#define FIRST_NAME_COUNT 10
#define ADJECTIVE_COUNT 14
#define LAST_NAME_COUNT 13

char *first_names[FIRST_NAME_COUNT] = {
  "Kevin ",
  "Isha ",
  "Rafael ",
  "Joey ",
  "Jeff ",
  "Bilbo ",
  "Richard ",
  "Lance ",
  "Romeo ",
  "Hulio "
};

char *adjectives[ADJECTIVE_COUNT] = {
  "the Fluffy ",
  "the Hammered ",
  "the Feisty ",
  "the Provocative ",
  "the Pesky ",
  "the Well endowed ",
  "the Sleepy ",
  "the Doped ",
  "the Innocent ",
  "the Easy ",
  "the Pimpin ",
  "the Hungry ",
  "the Abrasive ",
  "the Raging "
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

char * get_screen_name() {
  char * name;
  name = first_names[(unsigned int)alt_timestamp() % FIRST_NAME_COUNT];

  asprintf(&name, "%s%s", name, adjectives[(unsigned int)alt_timestamp() % ADJECTIVE_COUNT]);
  asprintf(&name, "%s%s", name, last_names[(unsigned int)alt_timestamp() % LAST_NAME_COUNT]);
  return name;
}
#endif
