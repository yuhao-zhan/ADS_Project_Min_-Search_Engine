#include <stdio.h>
#include <string.h>
#include "/opt/homebrew/Cellar/json-c/0.18/include/json-c/json.h"
#include "../header_files/AllFunctions.h"

// Swap function to swap two doc_count elements
void swap(doc_count* a, doc_count* b) {
    doc_count temp = *a;
    *a = *b;
    *b = temp;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Discard characters until a newline or EOF is encountered
    }
}

// Function to parse the JSON data and load expected results into a structure
void load_expected_results(const char *filename, expected_result_entry *expected_results, int *num_entries) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }
//    else {
//        printf("File opened successfully\n");
//    }

    // Allocate dynamic buffer for reading the file size instead of a static one
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL) {
        perror("Failed to allocate memory for reading JSON file");
        fclose(fp);
        return;
    }

    // Read the file into a buffer and ensure null-terminated string
    fread(buffer, sizeof(char), file_size, fp);
    buffer[file_size] = '\0';  // Ensure null-termination
    fclose(fp);

    // Parse JSON data
    struct json_object *parsed_json = json_tokener_parse(buffer);
    if (parsed_json == NULL) {
        printf("Error parsing JSON data\n");
        free(buffer);  // Free buffer in case of error
        return;
    }

    struct json_object *one_word_array;
    struct json_object *multi_word_array;

    // Check if the expected fields exist in the JSON structure
    if (!json_object_object_get_ex(parsed_json, "one_word", &one_word_array)) {
        printf("Error: 'one_word' not found in JSON\n");
        free(buffer);  // Free buffer in case of error
        return;
    }
    if (!json_object_object_get_ex(parsed_json, "two_word", &multi_word_array)) {
        printf("Error: 'two_word' not found in JSON\n");
        free(buffer);  // Free buffer in case of error
        return;
    }

    int one_word_count = json_object_array_length(one_word_array);
    int multi_word_count = json_object_array_length(multi_word_array);

    *num_entries = one_word_count + multi_word_count;

    // Parsing one-word queries
    for (int i = 0; i < one_word_count; i++) {
        struct json_object *entry = json_object_array_get_idx(one_word_array, i);
        if (entry == NULL) {
            printf("Error parsing one-word entry at index %d\n", i);
            continue;
        }

        struct json_object *query, *expected_result_ID;
        json_object_object_get_ex(entry, "query", &query);
        json_object_object_get_ex(entry, "expected_result_ID", &expected_result_ID);

        if (query == NULL || expected_result_ID == NULL) {
            printf("Error: Missing fields in one-word entry at index %d\n", i);
            continue;
        }

        // Store query
        strcpy(expected_results[i].query, json_object_get_string(query));

        // Store document IDs and frequencies
        int doc_count = json_object_array_length(expected_result_ID);
        expected_results[i].doc_count = doc_count;

        for (int j = 0; j < doc_count; j++) {
            struct json_object *doc_id = json_object_array_get_idx(expected_result_ID, j);
            if (doc_id == NULL) {
                printf("Error parsing document ID at index %d\n", j);
                continue;
            }
            strcpy(expected_results[i].expected_result_ID[j], json_object_get_string(doc_id));
        }
    }

    printf("Start parsing two-word queries\n");
    // Parsing two-word queries
    for (int i = 0; i < multi_word_count; i++) {
        struct json_object *entry = json_object_array_get_idx(multi_word_array, i);
        if (entry == NULL) {
            printf("Error parsing two-word entry at index %d\n", i);
            continue;
        }

        struct json_object *query, *expected_result_ID;
        json_object_object_get_ex(entry, "query", &query);
        json_object_object_get_ex(entry, "expected_result_ID", &expected_result_ID);

        if (query == NULL || expected_result_ID == NULL) {
            printf("Error: Missing fields in two-word entry at index %d\n", i);
            continue;
        }

        // Store query
        strcpy(expected_results[one_word_count + i].query, json_object_get_string(query));
        printf("Query: %s\n", expected_results[one_word_count + i].query);

        // Store document IDs and frequencies for each word
        int doc_count = json_object_array_length(expected_result_ID);
        expected_results[one_word_count + i].doc_count = doc_count;

        for (int j = 0; j < doc_count; j++) {
            struct json_object *doc_id = json_object_array_get_idx(expected_result_ID, j);
            if (doc_id == NULL) {
                printf("Error parsing document ID at index %d\n", j);
                continue;
            }
            strcpy(expected_results[one_word_count + i].expected_result_ID[j], json_object_get_string(doc_id));
        }
    }

    // Free the allocated buffer after parsing
    free(buffer);
}


// Function to compute precision and recall for document IDs
void compute_precision_recall_for_ID(char **expected_docs, int expected_doc_count, char **returned_docs, int returned_doc_count, float *precision, float *recall) {
    int true_positives = 0;

    // Ensure both expected_docs and returned_docs are not null
    if (expected_docs == NULL || returned_docs == NULL) {
        fprintf(stderr, "Error: expected_docs or returned_docs is NULL\n");
        return;
    }

    // Debugging to check if the docs are properly initialized
    for (int i = 0; i < expected_doc_count; i++) {
        if (expected_docs[i] == NULL) {
            printf("Error: expected_docs[%d] is NULL\n", i);
        }
    }

    for (int i = 0; i < returned_doc_count; i++) {
        if (returned_docs[i] == NULL) {
            printf("Error: returned_docs[%d] is NULL\n", i);
        }
    }

    // Compute true positives
    for (int i = 0; i < returned_doc_count; i++) {
        for (int j = 0; j < expected_doc_count; j++) {
            if (returned_docs[i] != NULL && expected_docs[j] != NULL) {
                if (strcmp(returned_docs[i], expected_docs[j]) == 0) {
                    true_positives++;
                }
            }
        }
    }

    // Calculate precision and recall
    *precision = (true_positives > 0) ? (float) true_positives / (float) returned_doc_count : 0;
    *recall = (true_positives > 0) ? (float) true_positives / (float) expected_doc_count : 0;

    // If both true_positives and returned_doc_count are 0, set precision and recall to 1
    if (expected_doc_count == 0 && returned_doc_count == 0) {
        *precision = 1;
        *recall = 1;
    }

    // Print the results
    printf("The precision is %.2f and the recall is %.2f\n", *precision, *recall);
}

// Partition function for Quick Sort in descending order
int partition_descending_order(doc_count arr[], int low, int high) {
    int pivot = arr[high].count; // Choose the last element as the pivot
    int i = low - 1; // Index of the smaller element

    for (int j = low; j < high; j++) {
        // If current element's count is greater than or equal to the pivot (descending order)
        if (arr[j].count >= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    // Swap the pivot element with the element at i+1
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Partition function for Quick Sort in ascending order
int partition_ascending_order(doc_count arr[], int low, int high) {
    int pivot = arr[high].count; // Choose the last element as the pivot
    int i = low - 1; // Index of the smaller element

    for (int j = low; j < high; j++) {
        // If current element's count is greater than or equal to the pivot (descending order)
        if (arr[j].count <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    // Swap the pivot element with the element at i+1
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Quick Sort function to sort the array in descending order
void quickSort_descending_order(doc_count arr[], int low, int high) {
    if (low < high) {
        int pi = partition_descending_order(arr, low, high); // pi is the partitioning index

        // Recursively sort elements before and after partition
        quickSort_descending_order(arr, low, pi - 1);
        quickSort_descending_order(arr, pi + 1, high);
    }
}

// Quick Sort function to sort the array in ascending order
void quickSort_ascending_order(doc_count arr[], int low, int high) {
    if (low < high) {
        int pi = partition_ascending_order(arr, low, high); // pi is the partitioning index

        // Recursively sort elements before and after partition
        quickSort_ascending_order(arr, low, pi - 1);
        quickSort_ascending_order(arr, pi + 1, high);
    }
}


// Helper function to stem the word and return the corresponding document IDs
void process_single_word_query(char* word, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs) {
    int found = 0;

    // Stem the word
    int end = stem(word, 0, strlen(word) - 1);
    word[end + 1] = 0;
    //printf("Stemmed word: [%s]\n", word);

    // Find the stemmed word in the inverted index
    for (int i = 0; i < InvertedFileIndex->size; i++) {
        //printf("Comparing word %s with %s\n", InvertedFileIndex->table[i].word, stemmed_word);
        if (strcmp(InvertedFileIndex->table[i].word, word) == 0) {
            *DocumentIDs = InvertedFileIndex->table[i];
            found = 1;
            break;  // Return immediately once found
        }
    }

    // If word not found, set doc_num to 0
    if (!found) {
        DocumentIDs->doc_num = 0;
    }
}

// Choose the wanted type of query and threshold
void Choose_query_and_threshold(int word_count, char **words, inverted_index index, inverted_index_entry *DocumentIDs) {
    int query_type, threshold;
    printf("Since you are in test, please choose your query type: \n");
    printf("1. Query and return the documents with top K highest frequency\n");
    printf("2. Query and return the documents with top K lowest frequency\n");
    printf("3. Query and return the documents with frequency higher than a threshold K\n");
    printf("4. Query and return the documents with frequency lower than a threshold K\n");
    printf("5. Query and return the documents with top K%% highest frequency\n");
    printf("6. Query and return the documents with top K%% lowest frequency\n");
    scanf("%d", &query_type);
    clear_input_buffer();
    printf("Input your threshold K: \n");
    scanf("%d", &threshold);
    clear_input_buffer();

    for (int i = 0; i < word_count; i++) {
        switch (query_type) {
            case 1: {
                query_threshold_TopN_highest_results(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 2: {
                query_threshold_TopN_lowest_results(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 3: {
                query_threshold_minimal_frequency(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 4: {
                query_threshold_maximal_frequency(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 5: {
                query_threshold_maximal_percentage_frequency(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 6: {
                query_threshold_minimal_percentage_frequency(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            default:
                printf("Invalid query type.\n");
                DocumentIDs[i].doc_num = 0;
                break;
        }
        printf("After processing the word %s, the number of document IDs is %d: \n", words[i], DocumentIDs[i].doc_num);
    }
}

// Choose the wanted type of query and threshold
void Choose_query_and_threshold_for_test(int word_count, char **words, inverted_index index, inverted_index_entry *DocumentIDs, int query_type, int threshold) {
    for (int i = 0; i < word_count; i++) {
        switch (query_type) {
            case 1: {
                query_threshold_TopN_highest_results(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 2: {
                query_threshold_TopN_lowest_results(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 3: {
                query_threshold_minimal_frequency(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 4: {
                query_threshold_maximal_frequency(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 5: {
                query_threshold_maximal_percentage_frequency(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 6: {
                query_threshold_minimal_percentage_frequency(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            default:
                printf("Invalid query type.\n");
                DocumentIDs[i].doc_num = 0;
                break;
        }
        printf("After processing the word %s, the number of document IDs is %d: \n", words[i], DocumentIDs[i].doc_num);
    }
}

// Helper function to format the document ID (enclose in quotes if it contains a comma)
char* format_doc_id(char* doc_id) {
    static char formatted_doc_id[1024];  // Buffer for formatted document ID
    if (strchr(doc_id, ',') != NULL) {
        // If the doc_id contains a comma, enclose it in double quotes
        snprintf(formatted_doc_id, sizeof(formatted_doc_id), "\"%s\"", doc_id);
    } else {
        // If no comma, just return the doc_id as-is
        snprintf(formatted_doc_id, sizeof(formatted_doc_id), "%s", doc_id);
    }
    return formatted_doc_id;
}

// Function to export data to CSV file
void export_to_csv(FILE *fp, const char *query, char **expected_docs, int expected_doc_count, char **returned_docs, int returned_doc_count, float precision, float recall) {
    int max_docs = (expected_doc_count > returned_doc_count) ? expected_doc_count : returned_doc_count;

    // Write the first row with the query, precision, recall, and the first doc_id
    fprintf(fp, "%s,%s,%d,%s,%d,%.2f,%.2f\n",
            query,
            expected_doc_count > 0 ? format_doc_id(expected_docs[0]) : "",  // First expected doc ID
            expected_doc_count,  // Expected doc count (only in the first row)
            returned_doc_count > 0 ? format_doc_id(returned_docs[0]) : "",  // First returned doc ID
            returned_doc_count,  // Returned doc count (only in the first row)
            precision, recall);

    // For each subsequent doc_id, write them on separate lines but leave other columns empty
    for (int i = 1; i < max_docs; i++) {
        fprintf(fp, ",");

        if (i < expected_doc_count) {
            fprintf(fp, "%s,", format_doc_id(expected_docs[i]));  // Write the expected doc_id if available
        } else {
            fprintf(fp, ",");  // Empty expected doc_id cell
        }

        fprintf(fp, ",");  // Leave the expected_doc_num column empty for subsequent rows

        if (i < returned_doc_count) {
            fprintf(fp, "%s\n", format_doc_id(returned_docs[i]));  // Write the returned doc_id if available
        } else {
            fprintf(fp, "\n");  // Empty returned doc_id cell
        }
    }
}


