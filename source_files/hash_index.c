#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LEN 100 // 单词的最大长度
#define MAX_FILENAME_LEN 1000 // 文档名称的最大长度
#define MAX_DOCS 10000 // 每个单词出现的最大文档数
#define INITIAL_CAPACITY 1009 // 哈希表的大小（质数）

// 文档计数
typedef struct {
    int doc_id; // 文档ID
    int count; // 出现次数
    char filename[MAX_FILENAME_LEN]; // 文档名称
} doc_count;

// 反向索引条目
typedef struct inverted_index_entry {
    char word[MAX_WORD_LEN]; // 单词
    int doc_num; // 单词出现的文档数
    doc_count doc_list[MAX_DOCS]; // 单词出现的文档列表
    struct inverted_index_entry *next; // 链表指针
} inverted_index_entry;

// 反向索引表
typedef struct {
    int size; // 表的大小
    int capacity; // 表的容量
    inverted_index_entry **table; // 表的数组（哈希表）
} inverted_index;

// 哈希函数
unsigned int hash(const char *word) {
    unsigned int hash = 5381;
    int c;

    while ((c = *word++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

// 初始化反向索引表
void init_inverted_index(inverted_index *index) {
    index->size = 0;
    index->capacity = INITIAL_CAPACITY;
    index->table = (inverted_index_entry **)malloc(INITIAL_CAPACITY * sizeof(inverted_index_entry *));
    for (int i = 0; i < INITIAL_CAPACITY; i++) {
        index->table[i] = NULL;
    }
}

// 释放反向索引
void free_inverted_index(inverted_index *index) {
    for (int i = 0; i < index->capacity; i++) {
        inverted_index_entry *entry = index->table[i];
        while (entry) {
            inverted_index_entry *temp = entry;
            entry = entry->next;
            free(temp); //释放每个节点
        }
    }
    free(index->table);
    //释放hash表
}
// 扩容反向索引表
void expand_inverted_index(inverted_index *index) {
    int old_capacity = index->capacity;
    index->capacity *= 2; // 扩容两倍
    inverted_index_entry **new_table = (inverted_index_entry **)malloc(index->capacity * sizeof(inverted_index_entry *));
    for(int i = 0; i < old_capacity; i++){
        inverted_index_entry *entry = index->table[i];
        while(entry){
            inverted_index_entry *next = entry->next;
            // 计算新索引
            unsigned long idx = hash(entry->word) % index->capacity;
            entry->next = new_table[idx];
            new_table[idx] = entry;
            entry = next;
        }
    }
    free(index->table);
    index->table = new_table;
}

// 向反向索引中添加条目
void add_inverted_index_entry(inverted_index *index, char *word, int doc_id, char *filename) {
    unsigned int idx = hash(word) % index->capacity;
    inverted_index_entry *entry = index->table[idx];

    // 查找是否存在该单词
    while (entry) {
        if (strcmp(entry->word, word) == 0) {
            // 该单词已存在，更新文档列表
            for (int j = 0; j < entry->doc_num; j++) {
                if (entry->doc_list[j].doc_id == doc_id) {
                    // 找到对应条目，更新计数
                    entry->doc_list[j].count++;
                    return;
                }
            }
            // 未找到对应条目，添加新条目
            if (entry->doc_num >= MAX_DOCS) {
                printf("Error: too many documents for word %s\n", word);
                return;
            }
            strcpy(entry->doc_list[entry->doc_num].filename, filename);
            entry->doc_list[entry->doc_num].doc_id = doc_id;
            entry->doc_list[entry->doc_num].count = 1;
            entry->doc_num++;
            return;
        }
        entry = entry->next; // 继续查找下一个
    }

    // 该单词不存在，添加新条目
    if((float)index->size / index->capacity >= 0.7){
        expand_inverted_index(index);
        idx = hash(word) % index->capacity;
    }
    inverted_index_entry *new_entry = (inverted_index_entry *)malloc(sizeof(inverted_index_entry));
    strcpy(new_entry->word, word);
    strcpy(new_entry->doc_list[0].filename, filename);
    new_entry->doc_list[0].doc_id = doc_id;
    new_entry->doc_list[0].count = 1;
    new_entry->doc_num = 1;
    new_entry->next = index->table[idx]; // 插入到链表头
    index->table[idx] = new_entry; // 更新哈希表

    index->size++;
}

// 从文件中读取数据，构建反向索引
void build_inverted_index(inverted_index *index, char *filename[], int file_count) {
    for (int i = 0; i < file_count; i++) {
        FILE *fp = fopen(filename[i], "r");
        if (!fp) {
            printf("Error: cannot open file %s\n", filename[i]);
            continue;
        }
        char word[MAX_WORD_LEN];
        int doc_id = i;
        while (fscanf(fp, "%s", word) != EOF) {
            add_inverted_index_entry(index, word, doc_id, filename[i]);
        }
        fclose(fp);
    }
}

// 打印反向索引
void print_inverted_index(inverted_index *index) {
    for (int i = 0; i < index->capacity; i++) {
        inverted_index_entry *entry = index->table[i];
        while (entry) {
            printf("%s: ", entry->word);
            for (int j = 0; j < entry->doc_num; j++) {
                printf("Doc:%d & Document:%s (%d times)", entry->doc_list[j].doc_id, entry->doc_list[j].filename, entry->doc_list[j].count);
            }
            printf("\n");
            entry = entry->next; // 移动到下一个条目
        }
    }
}

int main() {
    inverted_index index;
    init_inverted_index(&index);
    // 从指定文件读取
    char *filenames[] = {"data.txt", "data2.txt"};
    int file_count = sizeof(filenames) / sizeof(filenames[0]);
    build_inverted_index(&index, filenames, file_count);
    print_inverted_index(&index);
    free_inverted_index(&index);
    return 0;
}
