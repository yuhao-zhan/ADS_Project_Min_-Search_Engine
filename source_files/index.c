#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../header_files/stmr.h"

#define MAX_WORD_LEN 100 //单词的最大长度
#define MAX_FILENAME_LEN 1000 //文档名称的最大长度
#define MAX_DOCS 10000 //每个单词出现的最大文档数

//文档计数
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
//初始化反向索引表
void init_inverted_index(inverted_index *index){
    index->size = 0;
    index->capacity = 1000;
    index->table = (inverted_index_entry*)malloc(index->capacity * sizeof(inverted_index_entry));
}
//释放反向索引
void free_inverted_index(inverted_index *index){
    free(index->table);
}
//向反向索引中添加条目
void add_inverted_index_entry(inverted_index *index, char *word, int doc_id, char *filename){
    //查找是否存在该单词
    for(int i=0; i<index->size; i++){
        if(strcmp(index->table[i].word,word) == 0){
            //该单词已存在，更新文档列表
            for(int j=0; j<index->table[i].doc_num; j++){
                if(index->table[i].doc_list[j].doc_id == doc_id){
                    //找到对应条目，更新计数
                    index->table[i].doc_list[j].count++;
                    return;
                }
            }
            //未找到对应条目，添加新条目
            if(index->table[i].doc_num >= MAX_DOCS){
                printf("Error: too many documents for word %s\n", word);
                return;
            }
            strcpy(index->table[i].doc_list[index->table[i].doc_num].filename, filename);
            index->table[i].doc_list[index->table[i].doc_num].doc_id = doc_id;
            index->table[i].doc_list[index->table[i].doc_num].count = 1;
            index->table[i].doc_num++;
            return;
        }
    }
    //该单词不存在，添加新条目
    //如果表已满，扩容
    if(index->size >= index->capacity){
        index->capacity *= 2;
        index->table = (inverted_index_entry*)realloc(index->table, index->capacity * sizeof(inverted_index_entry));
    }
    //添加新条目
    strcpy(index->table[index->size].word, word);
    strcpy(index->table[index->size].doc_list[0].filename, filename);
    index->table[index->size].doc_list[0].doc_id = doc_id;
    index->table[index->size].doc_list[0].count = 1;
    index->table[index->size].doc_num = 1;
    index->size++;
}
//从文件中读取数据，构建反向索引
void build_inverted_index(inverted_index *index, char *filename[],int file_count){
    for(int i=0; i<file_count; i++){
        FILE *fp = fopen(filename[i], "r");
        printf("Processing: %s\n", filename[i]);
        if(!fp){
            printf("Error: cannot open file %s\n", filename[i]);
            continue;
        }
        char word[MAX_WORD_LEN];
        int doc_id = i;
        while(fscanf(fp, "%s", word)!=EOF){
            int end = stem(word, 0, strlen(word) - 1);
            word[end + 1] = 0;
            add_inverted_index_entry(index, word, doc_id, filename[i]);
        }
        fclose(fp);
    }
}
//打印反向索引
void print_inverted_index(inverted_index *index){
    for(int i = 0; i < index->size; i++){
        printf("%s: ", index->table[i].word);
        for(int j = 0; j < index->table[i].doc_num; j++){
            printf("Doc %d Document %s (%d times) ", index->table[i].doc_list[j].doc_id, index->table[i].doc_list[j].filename, index->table[i].doc_list[j].count);
        }
        printf("\n");
    }
}

// Function to read all .txt files from the "txt" directory
int get_txt_files(const char *directory, char ***filenames) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    // Open the directory
    if ((dir = opendir(directory)) == NULL) {
        perror("opendir() error");
        return -1;
    }

    // Count the number of .txt files
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".txt")) {
            count++;
        }
    }

    // Allocate memory for filenames array
    *filenames = malloc(count * sizeof(char *));
    if (*filenames == NULL) {
        perror("malloc() error");
        closedir(dir);
        return -1;
    }

    // Reset the directory stream to read filenames
    rewinddir(dir);
    int i = 0;

    // Store .txt filenames
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".txt")) {
            // Construct full path by appending the directory and filename
            //printf("%s\n", entry->d_name);
            (*filenames)[i] = malloc(strlen(directory) + strlen(entry->d_name) + 2); // +2 for '/' and '\0'
            if ((*filenames)[i] == NULL) {
                perror("malloc() error");
                closedir(dir);
                return -1;
            }
            sprintf((*filenames)[i], "%s/%s", directory, entry->d_name);
            i++;
        }
    }

    closedir(dir);
    return count;
}

/*int main(){
    inverted_index index;
    init_inverted_index(&index);
    //从指定文件读取
    char *filenames[] = {"txt/A Lover's Complaint.txt"};
    int file_count = sizeof(filenames)/sizeof(filenames[0]);
    build_inverted_index(&index, filenames, file_count);
    print_inverted_index(&index);
    free_inverted_index(&index);
    return 0;
}*/
