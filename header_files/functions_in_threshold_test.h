#ifndef MINISEARCHENGINE_FUNCTIONS_IN_THRESHOLD_TEST_H
#define MINISEARCHENGINE_FUNCTIONS_IN_THRESHOLD_TEST_H

#include "index.h"

void swap(doc_count* a, doc_count* b);
void clear_input_buffer();
int partition_descending_order(doc_count arr[], int low, int high);
int partition_ascending_order(doc_count arr[], int low, int high);
void quickSort_descending_order(doc_count arr[], int low, int high);
void quickSort_ascending_order(doc_count arr[], int low, int high);
void Choose_query_and_threshold(int word_count, char **words, inverted_index index, inverted_index_entry *DocumentIDs);

#endif //MINISEARCHENGINE_FUNCTIONS_IN_THRESHOLD_TEST_H
