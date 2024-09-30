#ifndef MINISEARCHENGINE_QUERY_H
#define MINISEARCHENGINE_QUERY_H

#include "index.h"
void ReceiveUserInput(char **words, int *capacity, int *word_count);
void Query(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs);
void query_threshold_TopN_highest_results(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold);
void query_threshold_TopN_lowest_results(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold);
void query_threshold_minimal_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold);
void query_threshold_maximal_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold);
void query_threshold_maximal_percentage_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold);
void query_threshold_minimal_percentage_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold);
void format_filename(char *fullpath, char *formatted_filename);
void print_documents(inverted_index_entry *DocumentIDs, int word_count, char **words);

#define MAX_INPUT_SIZE 100  // Maximum size for the input string
#define INITIAL_CAPACITY 10 // Initial capacity for the string array

#endif //MINISEARCHENGINE_QUERY_H
