#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

bool check_word(const char* word, hashmap_t hashtable[]) {

  char l_word[strlen(word)];

  strncpy(l_word, word, LENGTH);

  if (strlen(word) > LENGTH) {
    l_word[LENGTH] = '\0';
  }
  
  int bucket = hash_function(word);
  hashmap_t cursor = hashtable[bucket];

  while (cursor) {
    if (!strcmp(l_word, cursor->word)) {
      return true;
    } else {
      cursor = cursor->next;
    }
  }

  for(int i = 0; i < strlen(word); i++) {
      l_word[i] = tolower(word[i]);
  }

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
    } else {
      new_node->word[strlen(entry)-1] = '\0';
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

  char* line = NULL;
  size_t len;
  ssize_t read_line;

  while ((read_line = getline(&line, &len, fp)) != EOF) {

    char str[strlen(line)];
    strcpy(str,line);
    char delim[] = " ";
    char* ptr = strtok(str, delim);
    int emp;

    while (ptr) {

      emp = 0;

      char strr[strlen(ptr)];
      strcpy(strr, ptr);

      char *ps1 = strr;

      while (!isalpha(ps1[0]) && !isdigit(ps1[0])) {
        *ps1 = *(ps1+1);
        ps1++;
        if (!strlen(ps1)) {
          emp = 1;
          break;
        }
      }

      if (emp) {
        ptr = strtok(NULL, delim);
        continue;
      } 

      while (!isalpha(ps1[strlen(ps1)-1]) && !isdigit(ps1[strlen(ps1)-1])) {
        ps1[strlen(ps1)-1] = '\0';
      } 

      char str2[strlen(ps1)];
      strcpy(str2, ps1);

      if (strlen(str2) > LENGTH) {
        str2[LENGTH] = '\0';
      }

      if (isdigit(str2[0])) {
        for (int i = 1; i < strlen(str2); i++) {
          if (!isdigit(str2[i])) {
            misspelled[num_misspelled] = malloc(sizeof(char) * strlen(str2) + 1);
            strcpy(misspelled[num_misspelled], str2);
            num_misspelled++;
            break;
          }
        }
        ptr = strtok(NULL, delim);
        continue;
      } 
        
      if (!check_word(str2, hashtable)) {
        if (num_misspelled > MAX_MISSPELLED) {
          printf("Maxmimum misspelled words reached, exiting...");
          break;
        }
        misspelled[num_misspelled] = malloc(sizeof(char) * strlen(str2) + 1);
        strcpy(misspelled[num_misspelled], str2);
        num_misspelled++;
      }

      ptr = strtok(NULL, delim);
    }
  }

  free(line);

  return num_misspelled;
}
