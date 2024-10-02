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




    /* ==============================The Second Part: Test the precision and recall of returned document IDs and the expected results============================= */

    // Structure to store expected results
    expected_result_entry expected_results[100];
    int num_entries = 0;

    // Load the expected results from JSON file
    load_expected_results("/Users/zhanyuxiao/Desktop/CLionProjects/ADS/Project1/pythonProject/search_engine_test_data.json", expected_results, &num_entries);
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
    Choose_query_and_threshold(num_entries, test_words, index, test_documentIDs);

    // Export the result to a CSV file
    FILE *fp = fopen("../results.csv", "w");  // Open the CSV file in append mode
    if (fp == NULL) {
        perror("Failed to open CSV file");
        return 1;
    }

    // Write the CSV header row
    fprintf(fp, "query,expected doc_ids,expected doc_num,returned doc_ids,returned doc_num,precision,recall\n");

    for (int i = 0; i < num_entries; i++) {
        // Allocate memory for returned document IDs
        char **returned_IDs = (char **)malloc(test_documentIDs[i].doc_num * sizeof(char *));
        if (returned_IDs == NULL) {
            perror("Failed to allocate memory for returned_IDs");
            return 1;
        }

        // Allocate memory for each document ID string and copy the filename
        for (int j = 0; j < test_documentIDs[i].doc_num; j++) {
            returned_IDs[j] = (char *)malloc(MAX_FILENAME_LEN * sizeof(char));
            if (returned_IDs[j] == NULL) {
                perror("Failed to allocate memory for returned_IDs[j]");
                return 1;
            }

            format_filename(test_documentIDs[i].doc_list[j].filename, returned_IDs[j]);
        }

        // Create a temporary pointer array for expected_result_ID (which is a 2D array)
        char **expected_docs = (char **)malloc(expected_results[i].doc_count * sizeof(char *));
        if (expected_docs == NULL) {
            perror("Failed to allocate memory for expected_docs");
            return 1;
        }
        for (int j = 0; j < expected_results[i].doc_count; j++) {
            expected_docs[j] = expected_results[i].expected_result_ID[j];  // Point to each row of the 2D array
        }

        // Compute the precision and recall of the returned document IDs
        float precision = 0, recall = 0;

        // Debugging output to check input data
        printf("Computing precision and recall of the query [%s] \n", expected_results[i].query);
        // If the returned document count is greater than 0, compute precision and recall
        if (test_documentIDs[i].doc_num > 0) {
            compute_precision_recall_for_ID(expected_docs, expected_results[i].doc_count, returned_IDs,
                                            test_documentIDs[i].doc_num, &precision, &recall);

            // Write the result to the CSV file
            export_to_csv(fp, expected_results[i].query, expected_docs, expected_results[i].doc_count, returned_IDs,
                          test_documentIDs[i].doc_num, precision, recall);
        }
        // If the returned document count is 0, set precision and recall to 0
        else {
            // Write the result to the CSV file
            export_to_csv(fp, expected_results[i].query, expected_docs, expected_results[i].doc_count, returned_IDs,
                          test_documentIDs[i].doc_num, 0, 0);
        }

        free(expected_docs);

        // Free memory for each returned ID string
        for (int j = 0; j < test_documentIDs[i].doc_num; j++) {
            free(returned_IDs[j]);
        }
        free(returned_IDs);
    }

    // Close the CSV file
    fclose(fp);


// Don't forget to free memory for test_words
    for (int i = 0; i < num_entries; i++) {
        free(test_words[i]);
    }
    free(test_words);
    free(test_documentIDs);




    /* ==============================The Third Part: Read User's Query and Return Related Document IDs============================= */
    char **words;   // Array of pointers to hold the tokens (words)
    int capacity = INITIAL_CAPACITY;
    int word_count = 0;

    words = (char **)malloc(capacity * sizeof(char *));
    if (words == NULL) {
        perror("Failed to allocate memory for words array");
        return 1;
    }

    // Prompt user for input
    printf("Please Enter Your Query Here (input # if you want to stop the query): \n");
    do {
        word_count = 0;
        ReceiveUserInput(words, &capacity, &word_count);
        inverted_index_entry *DocumentIDs = (inverted_index_entry*)malloc(word_count * sizeof(inverted_index_entry));
        Choose_query_and_threshold(word_count, words, index, DocumentIDs);
        print_documents(DocumentIDs, word_count);
    } while (strcmp(words[word_count - 1], "#") != 0);

    free_inverted_index(&index);

    return 0;
}