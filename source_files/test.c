#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "../header_files/stmr.h"
#include "../header_files/index.h"
#include "../header_files/Query.h"
#include "../header_files/functions_in_threshold_test.h"

int main() {
    /* ==============================The First Part: Process the .txt files and build the inverted index============================= */

    inverted_index index;
    init_inverted_index(&index);

    char **filenames;
    const char *directory = "/Users/zhanyuxiao/Desktop/CLionProjects/ADS/Project1/MiniSearchEngine/txt";

    // Get all .txt files from the "txt" folder
    int file_count = get_txt_files(directory, &filenames);
    if (file_count == -1) {
        fprintf(stderr, "Error reading .txt files from directory: %s\n", directory);
        return 1;
    }

    // Build inverted index using all the retrieved filenames
    build_inverted_index(&index, filenames, file_count);

    // Print the built inverted index
    print_inverted_index(&index);



    /* ==============================The Second Part: Read User's Query and Return Related Document IDs============================= */
    char **words;   // Array of pointers to hold the tokens (words)
    int capacity = INITIAL_CAPACITY;
    int word_count = 0, first = 1;

    words = (char **)malloc(capacity * sizeof(char *));
    if (words == NULL) {
        perror("Failed to allocate memory for words array");
        return 1;
    }

    printf("Please Enter Your Query Here (input # if you want to stop the query): \n");
    do {
        word_count = 0;
//        if (first) {
//            first = 0;
//        } else {
//            for (int i = 0; i < word_count; i++) {
//                free(words[i]);
//            }
//            free(words);
//            words = (char **)malloc(capacity * sizeof(char *));
//            if (words == NULL) {
//                perror("Failed to allocate memory for words array");
//                return 1;
//            }
//            word_count = 0;
//        }
        //words = (char **)malloc(INITIAL_CAPACITY * sizeof(char *));
        //capacity = INITIAL_CAPACITY;
        ReceiveUserInput(words, &capacity, &word_count);
        inverted_index_entry *DocumentIDs = (inverted_index_entry*)malloc(word_count * sizeof(inverted_index_entry));
        Choose_query_and_threshold(word_count, words, index, DocumentIDs);
        print_documents(DocumentIDs, word_count, words);
    } while (strcmp(words[word_count - 1], "#") != 0);

    free_inverted_index(&index);

    return 0;
}