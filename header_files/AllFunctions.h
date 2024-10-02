#ifndef MINISEARCHENGINE_ALLFUNCTIONS_H
#define MINISEARCHENGINE_ALLFUNCTIONS_H
#include <stdio.h>

#define MAX_FILENAME_LEN 200 //文档名称的最大长度
#define MAX_INPUT_SIZE 100  // Maximum size for the input string
#define INITIAL_CAPACITY 10 // Initial capacity for the string array
#define MAX_WORD_LEN 100 //单词的最大长度
#define MAX_DOCS 100 //每个单词出现的最大文档数

#define TRUE 1
#define FALSE 0

int stem(char *p, int index, int position);

typedef struct{
    int doc_id; //文档ID
    int count; //出现次数
    char filename[MAX_FILENAME_LEN]; //文档名称
} doc_count;
//反向索引条目
typedef struct{
    char word[MAX_WORD_LEN]; //单词
    int doc_num; //单词出现的文档数
    doc_count doc_list[MAX_DOCS]; //单词出现的文档列表
} inverted_index_entry;
//反向索引表
typedef struct{
    int size; //表的大小
    int capacity; //表的容量
    inverted_index_entry *table; //表的数组
} inverted_index;

typedef struct {
    char query[MAX_WORD_LEN];
    char expected_result_ID[MAX_DOCS][MAX_FILENAME_LEN];
    int expected_result_frequency[MAX_DOCS];
    int doc_count;
} expected_result_entry;

void init_inverted_index(inverted_index *index);
void free_inverted_index(inverted_index *index);
void add_inverted_index_entry(inverted_index *index, char *word, int doc_id, char *filename);
void build_inverted_index(inverted_index *index, char *filename[],int file_count);
void print_inverted_index(inverted_index *index);
int get_txt_files(const char *directory, char ***filenames);
void ReceiveUserInput(char **words, int *capacity, int *word_count);
void Query(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs);
void query_threshold_TopN_highest_results(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold);
void query_threshold_TopN_lowest_results(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold);
void query_threshold_minimal_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold);
void query_threshold_maximal_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold);
void query_threshold_maximal_percentage_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold);
void query_threshold_minimal_percentage_frequency(char* words, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs, int threshold);
void format_filename(char *fullpath, char *formatted_filename);
void print_documents(inverted_index_entry *DocumentIDs, int word_count);
void load_expected_results(const char *filename, expected_result_entry *expected_results, int *num_entries);
void swap(doc_count* a, doc_count* b);
void clear_input_buffer();
int partition_descending_order(doc_count arr[], int low, int high);
int partition_ascending_order(doc_count arr[], int low, int high);
void quickSort_descending_order(doc_count arr[], int low, int high);
void quickSort_ascending_order(doc_count arr[], int low, int high);
void Choose_query_and_threshold(int word_count, char **words, inverted_index index, inverted_index_entry *DocumentIDs);
void compute_precision_recall_for_ID(char **expected_docs, int expected_doc_count, char **returned_docs, int returned_doc_count, float *precision, float *recall);
void process_single_word_query(char* word, inverted_index *InvertedFileIndex, inverted_index_entry *DocumentIDs);
void export_to_csv(FILE *fp, const char *query, char **expected_docs, int expected_doc_count, char **returned_docs, int returned_doc_count, float precision, float recall);


#endif
