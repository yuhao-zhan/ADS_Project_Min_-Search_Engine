//
// Created by 湛雨潇 on 9/29/24.
//

#ifndef MINISEARCHENGINE_INDEX_H
#define MINISEARCHENGINE_INDEX_H
#define MAX_FILENAME_LEN 1000 //文档名称的最大长度

#define MAX_WORD_LEN 100 //单词的最大长度
#define MAX_DOCS 10000 //每个单词出现的最大文档数

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


void init_inverted_index(inverted_index *index);
void free_inverted_index(inverted_index *index);
void add_inverted_index_entry(inverted_index *index, char *word, int doc_id);
void build_inverted_index(inverted_index *index, char *filename[],int file_count);
void print_inverted_index(inverted_index *index);
int get_txt_files(const char *directory, char ***filenames);

#endif //MINISEARCHENGINE_INDEX_H
