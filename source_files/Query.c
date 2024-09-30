#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../header_files/stmr.h"
#include "../header_files/index.h"
#include "../header_files/functions_in_threshold_test.h"

#define MAX_INPUT_SIZE 100  // Maximum size for the input string
#define INITIAL_CAPACITY 10 // Initial capacity for the string array

//void esaInit(Esa *esa);
//void esaFree(Esa *esa);
//void esaSetDebugMode(Esa *esa, unsigned char mode);
//char esaStemToken(Esa *esa, char *token, int length);

void ReceiveUserInput(char **words, int *capacity, int *word_count){
    char input[MAX_INPUT_SIZE]; // Buffer to hold the input string
    char *token;                // Pointer for each word token
    const char delim[] = " ";   // Delimiter: space

    if (fgets(input, sizeof(input), stdin) != NULL) {
        // Remove newline character if present
        input[strcspn(input, "\n")] = '\0';
        // Tokenize the input string by space
        token = strtok(input, delim);
        while (token != NULL) {
            printf("Token: %s\n", token);
            int end = stem(token, 0, strlen(token) - 1);

            token[end + 1] = 0;
            printf("Stemmed Token: %s\n", token);
            // clean the token with stemming and stop words
            //token = esaStemToken(Esa *esa, token, int length);

            // Check if we need to expand the words array
            if (*word_count >= *capacity) {
                *capacity *= 2;
                char **new_words = realloc(words, *capacity * sizeof(char *));
                if (new_words == NULL) {
                    perror("Failed to reallocate memory");
                    return;
                }
                words = new_words;
            }

            // Allocate memory for the individual token and store it
            words[*word_count] = malloc(strlen(token) + 1); // +1 for null terminator
            if (words[*word_count] == NULL) {
                perror("Failed to allocate memory for token");
                return;
            }
            strcpy(words[*word_count], token);  // Copy token into the array

            // Move to the next token
            (*word_count)++;
            token = strtok(NULL, delim);
        }
    } else {
        printf("Error reading input.\n");
        return;
    }

}

// Search the word in the inverted index
void Query(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs){
    // Search the word in the inverted index
    int found = 0;
    for(int i=0; i<InvertedFileIndex->size; i++){
        if(strcmp(InvertedFileIndex->table[i].word, words) == 0){
            found = 1;
            // Found the word in the inverted index
            // Return the document IDs
            *DocumentIDs = InvertedFileIndex->table[i];
        }
    }
    // Word not found in the inverted index
    if (found == 0){
        DocumentIDs->doc_num = 0;
    }
}

// Set the thresholds to test the results
void query_threshold_TopN_highest_results(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold){
    int found = 0;
    for(int i=0; i<InvertedFileIndex->size; i++){
        if(strcmp(InvertedFileIndex->table[i].word, words) == 0){
            found = 1;
            *DocumentIDs = InvertedFileIndex->table[i];
            // Only return the Top N results based on the ordered frequencies
            quickSort_descending_order(DocumentIDs->doc_list, 0, DocumentIDs->doc_num-1);
            DocumentIDs->doc_num = threshold;
        }
    }
    // Word not found in the inverted index
    if (found == 0)
        DocumentIDs->doc_num = 0;
}

void query_threshold_TopN_lowest_results(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold){
    int found = 0;
    for(int i=0; i<InvertedFileIndex->size; i++){
        found = 1;
        if(strcmp(InvertedFileIndex->table[i].word, words) == 0){
            *DocumentIDs = InvertedFileIndex->table[i];
            // Only return the Top N results based on the ordered frequencies
            quickSort_ascending_order(DocumentIDs->doc_list, 0, DocumentIDs->doc_num-1);
            DocumentIDs->doc_num = threshold;
        }
    }
    // Word not found in the inverted index
    if (found == 0)
        DocumentIDs->doc_num = 0;
}

void query_threshold_maximal_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold){
    int found = 0;
    for(int i=0; i<InvertedFileIndex->size; i++){
        found = 1;
        if(strcmp(InvertedFileIndex->table[i].word, words) == 0){
            *DocumentIDs = InvertedFileIndex->table[i];
            // Only return the results whose frequency is greater than the threshold
            quickSort_ascending_order(DocumentIDs->doc_list, 0, DocumentIDs->doc_num-1);
            int count = 0;
            for(int j=0; j<DocumentIDs->doc_num; j++){
                if(DocumentIDs->doc_list[j].count > threshold){
                    count++;
                }
            }
            DocumentIDs->doc_num = count;
        }
    }
    // Word not found in the inverted index
    if(found == 0)
        DocumentIDs->doc_num = 0;
}

void query_threshold_minimal_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold){
    int found = 0;
    for(int i=0; i<InvertedFileIndex->size; i++){
        found = 1;
        if(strcmp(InvertedFileIndex->table[i].word, words) == 0){
            *DocumentIDs = InvertedFileIndex->table[i];
            // Only return the results whose frequency is greater than the threshold
            quickSort_descending_order(DocumentIDs->doc_list, 0, DocumentIDs->doc_num-1);
            int count = 0;
            for(int j=0; j<DocumentIDs->doc_num; j++){
                if(DocumentIDs->doc_list[j].count > threshold){
                    count++;
                }
            }
            DocumentIDs->doc_num = count;
        }
    }
    // Word not found in the inverted index
    if(found == 0)
        DocumentIDs->doc_num = 0;
}

void query_threshold_maximal_percentage_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold){
    int found = 0;
    for(int i=0; i<InvertedFileIndex->size; i++){
        if(strcmp(InvertedFileIndex->table[i].word, words) == 0){
            found = 1;
            *DocumentIDs = InvertedFileIndex->table[i];
            // Only return the Top N results based on the ordered frequencies
            quickSort_descending_order(DocumentIDs->doc_list, 0, DocumentIDs->doc_num-1);
            DocumentIDs->doc_num = threshold * DocumentIDs->doc_num / 100;
        }
    }
    // Word not found in the inverted index
    if (found == 0)
        DocumentIDs->doc_num = 0;
}

void query_threshold_minimal_percentage_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold){
    int found = 0;
    for(int i=0; i<InvertedFileIndex->size; i++){
        found = 1;
        if(strcmp(InvertedFileIndex->table[i].word, words) == 0){
            *DocumentIDs = InvertedFileIndex->table[i];
            // Only return the Top N results based on the ordered frequencies
            quickSort_ascending_order(DocumentIDs->doc_list, 0, DocumentIDs->doc_num-1);
            DocumentIDs->doc_num = threshold * DocumentIDs->doc_num / 100;
        }
    }
    // Word not found in the inverted index
    if (found == 0)
        DocumentIDs->doc_num = 0;
}

void format_filename(char *fullpath, char *formatted_filename) {
    char *filename_start = strrchr(fullpath, '/'); // Find the last '/'
    if (filename_start) {
        filename_start++; // Move past the '/'
    } else {
        filename_start = fullpath; // If no '/', start from the fullpath
    }

    char *dot = strrchr(filename_start, '.'); // Find the last '.'
    if (dot && strcmp(dot, ".txt") == 0) {
        *dot = '\0'; // Replace ".txt" with '\0' to remove the extension
    }

    // Copy the formatted filename
    strcpy(formatted_filename, filename_start);
}

void print_documents(inverted_index_entry *DocumentIDs, int word_count, char **words) {
    for (int i = 0; i < word_count; i++) {
        printf("Word: %s\n", words[i]);
        if (DocumentIDs[i].doc_num == 0) {
            printf("No documents found.\n");
        } else {
            for (int j = 0; j < DocumentIDs[i].doc_num; j++) {
                char formatted_filename[100]; // Adjust size as necessary
                format_filename(DocumentIDs[i].doc_list[j].filename, formatted_filename);
                printf("Doc ID: %s\t Count: %d\n", formatted_filename, DocumentIDs[i].doc_list[j].count);
            }
        }
        printf("------------------------------------------------------------------------------------------------------------\n");
        printf("Continue Your Query:\n");
    }
}