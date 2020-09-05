#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

bool check_word(const char* word, hashmap_t hashtable[]) {

  int bucket = hash_function(word);
  hashmap_t cursor = hashtable[bucket];

  while (cursor) {
    if (!strcmp(word, cursor->word)) {
      return true;
    } else {
      cursor = cursor->next;
    }
  }

  char l_word[strlen(word)];

  for(int i = 0; i < strlen(word); i++) {
      l_word[i] = tolower(word[i]);
  }

  l_word[strlen(word)]='\0';

  bucket = hash_function(l_word);
  cursor = hashtable[bucket];

  while (cursor) {
    if (!strcmp(l_word, cursor->word)) {
      return true;
    } else {
      cursor = cursor->next;
    }
  }

  return false;
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {

  int bucket = 0;
  FILE* ls;

  char* entry = NULL;
  size_t len;
  ssize_t read_word;

  for (int i = 0; i < HASH_SIZE; i++) {
    hashtable[i] = NULL;
  }

  if (!(ls = fopen(dictionary_file, "r"))) {
    return false;
  }

  hashmap_t new_node = malloc(sizeof(node));
  new_node->next = NULL;

  while ((read_word = getline(&entry, &len, ls)) != EOF) {

    strncpy(new_node->word, entry, LENGTH);

    if (strlen(entry) > LENGTH) {
      new_node->word[LENGTH] = '\0';
    }

    bucket = hash_function(new_node->word);

    if (hashtable[bucket]) {
      new_node->next = hashtable[bucket];
    }

    hashtable[bucket] = new_node;

    new_node = malloc(sizeof(node));
    new_node->next = NULL;
  }

  free(new_node);
  fclose(ls);

  if (entry) {
    free(entry);
  }

  return true;
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {

  int num_misspelled = 0;
    
  /*
    While line in fp is not EOF (end of file):
        Read the line.
        Split the line on spaces.
        For each word in line:
            Remove punctuation from beginning and end of word.
            If not check_word(word):
                Append word to misspelled.
                Increment num_misspelled.
    Return num_misspelled.*/

  return num_misspelled;
}
