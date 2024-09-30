#include "../header_files/index.h"
#include <stdio.h>
#include "../header_files/Query.h"

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

// Choose the wanted type of query and threshold
void Choose_query_and_threshold(int word_count, char **words, inverted_index index, inverted_index_entry *DocumentIDs) {
    int query_type, threshold;
    for (int i = 0; i < word_count; i++) {
        printf("Since you are in test, please choose your query type: \n");
        printf("1. Query and return the documents with top K highest frequency\n");
        printf("2. Query and return the documents with top K lowest frequency\n");
        printf("3. Query and return the documents with frequency higher than a threshold K\n");
        printf("4. Query and return the documents with frequency lower than a threshold K\n");
        printf("5. Query and return the documents with top K%% highest frequency\n");
        printf("6. Query and return the documents with top K%% lowest frequency\n");
        scanf("%d", &query_type);
        clear_input_buffer();
        switch (query_type) {
            case 1: {
                printf("Input your threshold K: \n");
                scanf("%d", &threshold);
                clear_input_buffer();
                query_threshold_TopN_highest_results(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 2: {
                printf("Input your threshold K: \n");
                scanf("%d", &threshold);
                clear_input_buffer();
                query_threshold_TopN_lowest_results(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 3: {
                printf("Input your threshold K: \n");
                scanf("%d", &threshold);
                clear_input_buffer();
                query_threshold_minimal_frequency(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 4: {
                printf("Input your threshold K: \n");
                scanf("%d", &threshold);
                clear_input_buffer();
                query_threshold_maximal_frequency(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 5: {
                printf("Input your threshold K: \n");
                scanf("%d", &threshold);
                clear_input_buffer();
                query_threshold_maximal_percentage_frequency(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            case 6: {
                printf("Input your threshold K: \n");
                scanf("%d", &threshold);
                clear_input_buffer();
                query_threshold_minimal_percentage_frequency(words[i], &index, &DocumentIDs[i], threshold);
                break;
            }
            default:
                printf("Invalid query type.\n");
                DocumentIDs[i].doc_num = 0;
                break;
        }
    }
}
