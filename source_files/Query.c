#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../header_files/AllFunctions.h"

int Min(int a, int b){
    return a < b ? a : b;
}

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


// Helper function to find common document IDs between multiple queries
void find_common_documents(inverted_index_entry *DocumentIDs1, inverted_index_entry *DocumentIDs2) {
    int common_count = 0;

    // Create a new list for common documents
    doc_count common_docs[MAX_DOCS];

    for (int i = 0; i < DocumentIDs1->doc_num; i++) {
        for (int j = 0; j < DocumentIDs2->doc_num; j++) {
            if (strcmp(DocumentIDs1->doc_list[i].filename, DocumentIDs2->doc_list[j].filename) == 0) {
                // Common document found
                common_docs[common_count] = DocumentIDs1->doc_list[i];
                // Sum the counts of each document in the common list
                common_docs[common_count].count += DocumentIDs2->doc_list[j].count;
                common_count++;
                break;
            }
        }
    }

    // Update DocumentIDs1 with the common documents
    memcpy(DocumentIDs1->doc_list, common_docs, common_count * sizeof(doc_count));
    DocumentIDs1->doc_num = common_count;
}

// Debugging function to check for NULL or invalid pointers
void check_pointer(const char *name, void *ptr) {
    if (ptr == NULL) {
        printf("Error: %s is NULL\n", name);
    } else {
        printf("%s is valid\n", name);
    }
}

// Function to split a query into individual words, count the number of words, and process each one
void process_query(char* query, inverted_index *InvertedFileIndex, inverted_index_entry *finalDocumentIDs) {
    // Create a modifiable copy of the query string (strtok modifies the string in-place)
    char query_copy[MAX_INPUT_SIZE];

    if (strlen(query) >= MAX_INPUT_SIZE) {
        printf("Error: Query size exceeds buffer size.\n");
        return;  // Exit if the query is too large
    }

    strcpy(query_copy, query);  // Copy the original query to the modifiable buffer

    printf("Processing query: [%s]\n", query_copy);

    char *word;
    inverted_index_entry tempDocumentIDs;
    inverted_index_entry commonDocumentIDs;

    // Initialize the first query result
    commonDocumentIDs.doc_num = 0;

    // Split the query into words
    word = strtok(query_copy, " ");
    if (word == NULL) {
        printf("No words found in query.\n");
        return;  // If no words, return early
    }

    int first_word = 1;
    int word_count = 0;

    while (word != NULL) {
        word_count++;  // Count the number of words

        // Process the current word as a single word query
        process_single_word_query(word, InvertedFileIndex, &tempDocumentIDs);

        if (first_word) {
            // If it's the first word, initialize the commonDocumentIDs with its result
            commonDocumentIDs = tempDocumentIDs;
            first_word = 0;
        } else {
            // Find common document IDs between the previous results and the current word's results
            find_common_documents(&commonDocumentIDs, &tempDocumentIDs);
        }

        // Get the next word
        word = strtok(NULL, " ");
    }

    // If only one word in the query, directly use the results from that word
    if (word_count == 1) {
        *finalDocumentIDs = commonDocumentIDs;
    } else {
        // If multiple words, the results in commonDocumentIDs represent the common documents
        *finalDocumentIDs = commonDocumentIDs;
    }
    printf("------------------------------------------------------------------------------------------------------\n");
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
    // Process the query (whether it's single or multi-word)
    process_query(words, InvertedFileIndex, DocumentIDs);

    // Only return the Top N results based on the ordered frequencies
    quickSort_descending_order(DocumentIDs->doc_list, 0, DocumentIDs->doc_num-1);
    DocumentIDs->doc_num = Min(threshold, DocumentIDs->doc_num);
}

void query_threshold_TopN_lowest_results(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold){
    // Process the query (whether it's single or multi-word)
    process_query(words, InvertedFileIndex, DocumentIDs);

    // Only return the Top N results based on the ordered frequencies
    quickSort_ascending_order(DocumentIDs->doc_list, 0, DocumentIDs->doc_num-1);
    DocumentIDs->doc_num = Min(threshold, DocumentIDs->doc_num);
}

void query_threshold_maximal_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold){
    process_query(words, InvertedFileIndex, DocumentIDs);
    // Only return the results whose frequency is smaller than the threshold
    quickSort_ascending_order(DocumentIDs->doc_list, 0, DocumentIDs->doc_num-1);
    int count = 0;
    for(int j=0; j<DocumentIDs->doc_num; j++){
        if(DocumentIDs->doc_list[j].count > threshold){
            count++;
        }
    }
    DocumentIDs->doc_num = count;
}

void query_threshold_minimal_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold){
    process_query(words, InvertedFileIndex, DocumentIDs);
    // Only return the results whose frequency is larger than the threshold
    quickSort_descending_order(DocumentIDs->doc_list, 0, DocumentIDs->doc_num-1);
    int count = 0;
    for(int j=0; j<DocumentIDs->doc_num; j++){
        if(DocumentIDs->doc_list[j].count < threshold){
            count++;
        }
    }
    DocumentIDs->doc_num = count;
}

void query_threshold_maximal_percentage_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold){
    process_query(words, InvertedFileIndex, DocumentIDs);
    // Only return the results whose frequency is smaller than the threshold
    quickSort_ascending_order(DocumentIDs->doc_list, 0, DocumentIDs->doc_num-1);
    DocumentIDs->doc_num = threshold * DocumentIDs->doc_num / 100;
}

void query_threshold_minimal_percentage_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold){
    process_query(words, InvertedFileIndex, DocumentIDs);
    // Only return the results whose frequency is larger than the threshold
    quickSort_descending_order(DocumentIDs->doc_list, 0, DocumentIDs->doc_num-1);
    DocumentIDs->doc_num = threshold * DocumentIDs->doc_num / 100;
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



void print_documents(inverted_index_entry *DocumentIDs, int word_count) {
    if (word_count == 1) {
        if (DocumentIDs[0].doc_num == 0) {
            printf("No documents found.\n");
        } else {
            for (int j = 0; j < DocumentIDs[0].doc_num; j++) {
                char formatted_filename[100]; // Adjust size as necessary
                format_filename(DocumentIDs[0].doc_list[j].filename, formatted_filename);
                printf("Doc ID: %s\tWord: %s  Count: %d", formatted_filename, DocumentIDs[0].word, DocumentIDs[0].doc_list[j].count);
            }
        }
    } else {
        // Only print the Documents common to all words
        int final_common = 0;
        for (int j = 0; j < DocumentIDs[0].doc_num; j++) {
            int common = 1;
            for (int i = 1; i < word_count; i++) {
                int k = 0;
                while (k < DocumentIDs[i].doc_num) {
                    if (strcmp(DocumentIDs[0].doc_list[j].filename, DocumentIDs[i].doc_list[k].filename) == 0) {
                        break;
                    }
                    k++;
                }
                if (k == DocumentIDs[i].doc_num) {
                    common = 0;
                    final_common = 1;
                    break;
                }
            }
            if (common == 1) {
                char formatted_filename[100]; // Adjust size as necessary
                format_filename(DocumentIDs[0].doc_list[j].filename, formatted_filename);
                printf("Doc ID: %s\t", formatted_filename);
                for (int i = 0; i < word_count; i++) {
                    int k = 0;
                    while (k < DocumentIDs[i].doc_num) {
                        if (strcmp(DocumentIDs[0].doc_list[j].filename, DocumentIDs[i].doc_list[k].filename) == 0) {
                            break;
                        }
                        k++;
                    }
                    printf("Word: %s Count: %d\t", DocumentIDs[i].word, DocumentIDs[i].doc_list[k].count);
                }
            }
        }
        if (!final_common) {
            printf("No common documents found.\n");
        }
    }
    printf("\n------------------------------------------------------------------------------------------------------------\n");
    printf("Continue Your Query:\n");
}