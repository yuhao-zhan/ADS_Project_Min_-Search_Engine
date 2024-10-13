## 伪代码
1. 定义数据结构 Define Data Structures
```
STRUCT DocumentList
    INTEGER doc_id
    STRING filename
    INTEGER count
    DocumentList next

STRUCT InvertedIndexEntry
    STRING word
    DocumentList head

STRUCT InvertedIndex
    HASH_TABLE<string, InvertedIndexEntry>

```
2. 初始化倒排索引 Initialize Inverted Index
```
FUNCTION find_inverted_index_entry(INVERTED_INDEX index, STRING word)
    IF word IN index THEN
        entry = index[word]
        PRINT "Results for " + word
        current = entry.head
        WHILE current IS NOT NULL DO
            PRINT "Document ID: " + current.doc_id + ", Filename: " + current.filename + ", Count: " + current.count
            current = current.next
        END WHILE
    ELSE
        PRINT "No results found for " + word
    END IF
END FUNCTION

```
3. 添加倒排索引条目 Add Inverted Index Entry
```
FUNCTION add_inverted_index_entry(INVERTED_INDEX index, STRING word, INTEGER doc_id, STRING filename)
    IF word NOT IN index THEN
        CREATE new_entry
        new_entry.word = word
        new_entry.head = NULL
        index[word] = new_entry
    END IF

    entry = index[word]
    current = entry.head
    WHILE current IS NOT NULL DO
        IF current.doc_id == doc_id THEN
            current.count += 1
            RETURN
        END IF
        current = current.next
    END WHILE

    CREATE new_doc
    new_doc.doc_id = doc_id
    new_doc.filename = filename
    new_doc.count = 1
    new_doc.next = entry.head
    entry.head = new_doc
END FUNCTION

```
4. 构建倒排索引 Build Inverted Index
```
FUNCTION build_inverted_index(INVERTED_INDEX index, ARRAY OF STRING filenames)
    FOR EACH filename IN filenames DO
        OPEN file
        doc_id = GET_DOCUMENT_ID(filename)

        WHILE NOT END_OF_FILE DO
            STRING word
            READ word FROM file
            NORMALIZE(word)
            add_inverted_index_entry(index, word, doc_id, filename)
        END WHILE

        CLOSE file
    END FOR
END FUNCTION

```
5. 查找倒排索引条目 Find Inverted Index Entry
```
FUNCTION find_inverted_index_entry(INVERTED_INDEX index, STRING word)
    IF word IN index THEN
        entry = index[word]
        PRINT "Results for " + word
        current = entry.head
        WHILE current IS NOT NULL DO
            PRINT "Document ID: " + current.doc_id + ", Filename: " + current.filename + ", Count: " + current.count
            current = current.next
        END WHILE
    ELSE
        PRINT "No results found for " + word
    END IF
END FUNCTION

```
### **DJB2**
```
hash = 5381 for each character c in the string: hash = ((hash << 5) + hash) + ord(c) // hash * 33 + c return hash
```
**Advantages**
- **Simplicity and Ease of Implementation:**  
    The algorithm has a simple structure, making it easy to understand and implement, which is suitable for rapid development.
- **Good Performance:**  
    In most cases, DJB2 can quickly compute hash values and is suitable for handling large amounts of data.
- **Low Collision Rate:**  
    For general strings, DJB2 has a relatively low collision rate, especially performing well on random datasets.
**Disadvantages**
- **Collision Issues:**  
    Although the collision rate is low, it can still occur, particularly when dealing with specific patterns or similar strings.
- **Input Characteristics Affect Performance:**  
    For certain specific types of input (such as repeated characters or specific patterns), hashing performance may degrade.
## **HASH**
基本结构已介绍，对字符串进行hash算法如上，将计算得到idx对capacity求余，得到其坐标。如冲突，使用while(entry)循环遍历链表，对现有条目进行扩充。若没有，增加新条目。
涉及到扩容时需要申请一块更大的空间进行重哈希，代码如下
The basic structure has been introduced. For the hash algorithm on strings, the index is calculated by taking the hash value and performing a modulus operation with the capacity to obtain its coordinate. In case of a collision, a `while(entry)` loop is used to traverse the linked list and expand existing entries. If there are no existing entries, a new entry is added.
When resizing is involved, it is necessary to allocate a larger space for rehashing. The code is as follows:
```c
void expand_inverted_index(inverted_index *index) {
    int old_capacity = index->capacity;
    index->capacity *= 2;
    inverted_index_entry **new_table = (inverted_index_entry **)malloc(index->capacity * sizeof(inverted_index_entry *));
    for(int i = 0; i < index->capacity; i++)
        new_table[i] = NULL;
    for(int i = 0; i < old_capacity; i++){
        inverted_index_entry *entry = index->table[i];
        while(entry){
            inverted_index_entry *next = entry->next;
            unsigned long idx = hash(entry->word) % index->capacity;
            entry->next = new_table[idx];
            new_table[idx] = entry;
            entry = next;
        }
    }
    free(index->table);
    index->table = new_table;
}
```
如此可以保证hash_table满足数据量大的情况。
This ensures that the hash table can handle large amounts of data. 
查找时，先计算其hash值，寻找对应词条，可以将时间复杂度优化到O(1)
When searching, the hash value is calculated first to find the corresponding entry, which can optimize the time complexity to O(1).
```c
void find_inverted_index_entry(inverted_index *index, char *word) {
    unsigned long idx = hash(word) % index->capacity;
    inverted_index_entry* entry = index->table[idx];
    while (entry) {
        if (strcmp(entry->word, word) == 0) {
            printf("Word: %s\n", entry->word);
            printf("Documents:\n");
            for (int i = 0; i < entry->doc_num; i++) {
                printf("  - %s (Doc ID: %d, Count: %d)\n",
                       entry->doc_list[i].filename,
                       entry->doc_list[i].doc_id,
                       entry->doc_list[i].count);
            }
            return;
        }
        entry = entry->next;
    }
    printf("Word '%s' not found in the index.\n", word);
}
```
对stem处理过后的文件进行试读入和搜索，功能良好
When reading and searching the stemmed files the function works well.
![[hash_index.jpg]]