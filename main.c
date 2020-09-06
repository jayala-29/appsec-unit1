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
  char * misspelled[MAX_MISSPELLED];
  FILE* txt_file;

  if (argc < 3 || argc > 3) {
    printf ("%s\n", "Incorrect argument count");
    return 1;
  }

  if (!load_dictionary(argv[2], hashtable)) {
    return 1;
  }

  txt_file = fopen(argv[1], "r");

  if (!txt_file) {
    printf("Could not open file\n");
    return 1;
  }

  int num_misspelled = check_words(txt_file, hashtable, misspelled);

  for (int i = 0; i < num_misspelled; i++) {
    printf ("%s\n", misspelled[i]);
  }

  for (int i = 0; i < HASH_SIZE; i++) {
    if (hashtable[i]) {
      free_map(hashtable[i]);
    }
  }

  for (int i = 0; i < num_misspelled; i++) {
    free(misspelled[i]);
  }

  fclose(txt_file);

  return 0;
}
