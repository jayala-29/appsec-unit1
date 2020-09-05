#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

void free_map(hashmap_t n) {
  if (n->next) {
    free_map(n->next);
  }
  free(n);
  return;
}


int main(int argc, char* argv[]) {

  hashmap_t hashtable[HASH_SIZE];
  //char * misspelled[MAX_MISSPELLED];

  if (argc < 3 || argc > 3) {
    printf ("%s\n", "Incorrect argument count");
    return -1;
  }

  if (!load_dictionary(argv[2], hashtable)) {
    return -2;
  }

  for (int i = 0; i < HASH_SIZE; i++) {
    if (hashtable[i]) {
      printf ("%s\n", hashtable[i]->word);
    }
  }

  printf("%i\n", check_word("Hello\n", hashtable));

  for (int i = 0; i < HASH_SIZE; i++) {
    if (hashtable[i]) {
      free_map(hashtable[i]);
    }
  }

  return 0;
}
