#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "../header_files/AllFunctions.h"

int main() {
    /* ==============================The First Part: Process the .txt files and build the inverted index============================= */

    // Initialize the inverted index
    inverted_index index;

    init_inverted_index(&index);

    char **filenames;
    const char *directory = "../txt";

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




    /* ==============================The Second Part: Test the precision and recall of returned document IDs and the expected results============================= */

    // Structure to store expected results
    expected_result_entry expected_results[200];
    int num_entries = 0;

    // Load the expected results from JSON file
    load_expected_results("../search_output.json", expected_results, &num_entries);
    char **test_words = (char **)malloc(num_entries * sizeof(char *));
    if (test_words == NULL) {
        perror("Failed to allocate memory for test_words array");
        return 1;
    }

    // Assign the query words to the test_words array
    for (int i = 0; i < num_entries; i++) {
        test_words[i] = (char *)malloc(MAX_INPUT_SIZE * sizeof(char));
        if (test_words[i] == NULL) {
            perror("Failed to allocate memory for test_words[i]");
            return 1;
        }
        strcpy(test_words[i], expected_results[i].query);
    }

    inverted_index_entry *test_documentIDs = (inverted_index_entry*)malloc(num_entries * sizeof(inverted_index_entry));
    // Use our search engine to return the document IDs for each query

    // Test the search engine with different thresholds
    for (int i = 1; i <= 6; i++){
        Choose_query_and_threshold_for_test(num_entries, test_words, index, test_documentIDs, i, 1);
        export_precision_and_recall_to_csv_file(num_entries, expected_results, test_documentIDs, i, 1);
        printf("Successfully export results to .csv file [Query TYpe: %d | Threshold: 1]", i);
        Choose_query_and_threshold_for_test(num_entries, test_words, index, test_documentIDs, i, 5);
        export_precision_and_recall_to_csv_file(num_entries, expected_results, test_documentIDs, i, 5);
        printf("Successfully export results to .csv file [Query TYpe: %d | Threshold: 5]", i);
        Choose_query_and_threshold_for_test(num_entries, test_words, index, test_documentIDs, i, 10);
        export_precision_and_recall_to_csv_file(num_entries, expected_results, test_documentIDs, i, 10);
        printf("Successfully export results to .csv file [Query TYpe: %d | Threshold: 10]", i);
        Choose_query_and_threshold_for_test(num_entries, test_words, index, test_documentIDs, i, 25);
        export_precision_and_recall_to_csv_file(num_entries, expected_results, test_documentIDs, i, 25);
        printf("Successfully export results to .csv file [Query TYpe: %d | Threshold: 25]", i);
        Choose_query_and_threshold_for_test(num_entries, test_words, index, test_documentIDs, i, 50);
        export_precision_and_recall_to_csv_file(num_entries, expected_results, test_documentIDs, i, 50);
        printf("Successfully export results to .csv file [Query TYpe: %d | Threshold: 50]", i);
        Choose_query_and_threshold_for_test(num_entries, test_words, index, test_documentIDs, i, 75);
        export_precision_and_recall_to_csv_file(num_entries, expected_results, test_documentIDs, i, 75);
        printf("Successfully export results to .csv file [Query TYpe: %d | Threshold: 75]", i);
        Choose_query_and_threshold_for_test(num_entries, test_words, index, test_documentIDs, i, 100);
        export_precision_and_recall_to_csv_file(num_entries, expected_results, test_documentIDs, i, 100);
        printf("Successfully export results to .csv file [Query TYpe: %d | Threshold: 100]", i);
    }

// Don't forget to free memory for test_words
    for (int i = 0; i < num_entries; i++) {
        free(test_words[i]);
    }
    free(test_words);
    free(test_documentIDs);

//
//
//
//    /* ==============================The Third Part: Read User's Query and Return Related Document IDs============================= */
//    char **words;   // Array of pointers to hold the tokens (words)
//    int capacity = INITIAL_CAPACITY;
//    int word_count = 0;
//
//    words = (char **)malloc(capacity * sizeof(char *));
//    if (words == NULL) {
//        perror("Failed to allocate memory for words array");
//        return 1;
//    }
//
//    // Prompt user for input
//    printf("Please Enter Your Query Here (input # if you want to stop the query): \n");
//    do {
//        word_count = 0;
//        ReceiveUserInput(words, &capacity, &word_count);
//        inverted_index_entry *DocumentIDs = (inverted_index_entry*)malloc(word_count * sizeof(inverted_index_entry));
//        Choose_query_and_threshold(word_count, words, index, DocumentIDs);
//        print_documents(DocumentIDs, word_count);
//    } while (strcmp(words[word_count - 1], "#") != 0);

    free_inverted_index(&index);

    return 0;
}