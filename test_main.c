#include <check.h>
#include "dictionary.h"
#include <stdlib.h>

#define DICTIONARY "./txt_files/wordlist.txt"
#define TESTDICT "./txt_files/test_worlist.txt"

START_TEST(test_dictionary_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    ck_assert(load_dictionary(TESTDICT, hashtable));
    char* expected[4];
    expected[0] = "first";
    expected[1] = "second";
    expected[2] = "third";
    expected[3] = "test";
    for (int i = 0; i < 4; i++)
      ck_assert(check_word(expected[i], hashtable));
}
END_TEST

START_TEST(test_check_word_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    const char* correct_word = "Justice";
    const char* punctuation_word_2 = "pl.ace";
    ck_assert(check_word(correct_word, hashtable));
    ck_assert(!check_word(punctuation_word_2, hashtable));
    // Test here: What if a word begins and ends with "?
    // Note: this is done in check_words, NOT check_word
    FILE *fp = fopen("./txt_files/includes_quotes.txt", "r");
    char* expected[1];
    expected[0] = "includin";
    char *misspelled[MAX_MISSPELLED];
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 1);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0, NULL);
}
END_TEST

START_TEST(test_check_words_normal)
{
    hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
    char* expected[3];
    expected[0] = "sogn";
    expected[1] = "skyn";
    expected[2] = "betta";
    char *misspelled[MAX_MISSPELLED];
    FILE *fp = fopen("./txt_files/test1.txt", "r");
    int num_misspelled = check_words(fp, hashtable, misspelled);
    ck_assert(num_misspelled == 3);
    bool test = strlen(misspelled[0]) == strlen(expected[0]);
    int len1 = strlen(misspelled[0]);
    int len2 = strlen(expected[0]);
    ck_assert_msg(test, "%d!=%d", len1, len2);
    ck_assert_msg(strcmp(misspelled[0], expected[0]) == 0, NULL);
    ck_assert_msg(strcmp(misspelled[1], expected[1]) == 0, NULL);
    ck_assert_msg(strcmp(misspelled[2], expected[2]) == 0, NULL);
}
END_TEST

Suite *
check_word_suite(void)
{
    Suite * suite;
    TCase * check_word_case;
    suite = suite_create("running tests for the three functions...");
    check_word_case = tcase_create("Core");
    tcase_add_test(check_word_case, test_dictionary_normal);
    tcase_add_test(check_word_case, test_check_word_normal);
    tcase_add_test(check_word_case, test_check_words_normal);
    suite_add_tcase(suite, check_word_case);

    return suite;
}

int
main(void)
{
    int failed;
    Suite *suite;
    SRunner *runner;
    
    suite = check_word_suite();
    runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
