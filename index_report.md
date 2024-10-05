# 1. Structure Analysis
### 1.1 `inverted_index_entry` Structure
This structure is used to store each entry in the inverted index, containing the following members:

- **`char word[MAX_WORD_LEN]`**: Stores the word.
- **`document doc_list[MAX_DOCS]`**: Document list representing the documents where the word appears.
- **`int doc_num`**: Records the number of documents.
- **`struct inverted_index_entry *next`**: Pointer to the next entry, used for the linked list structure.
1.2 inverted_index Structure
This structure represents the entire inverted index, containing the following members:

inverted_index_entry **table: Hash table storing pointers to inverted_index_entry.
int size: The current number of entries stored in the index.
int capacity: The capacity of the hash table.
2. Algorithm Analysis
2.1 Adding Entries
The main steps of the add_inverted_index_entry function are:

Calculate the hash value of the word and determine the index position.
Check if there is already an entry for the word in the hash table:
If it exists, check if the document list already contains the document. If yes, increment the count; if not, add the new document information.
If the word does not exist, create a new entry and insert it at the head of the linked list.
If the load factor of the hash table exceeds 0.7, expand the hash table.
2.2 Building the Index
The steps of the build_inverted_index function are:

Iterate through the file list and open each file one by one.
Read words from the file and call add_inverted_index_entry to add them to the index.
2.3 Search Functionality
The steps of the find_inverted_index_entry function are:

Calculate the hash value of the word and look for the corresponding entry.
If found, print all related document information; otherwise, output a not found message.
3. Time and Space Complexity Analysis
3.1 Time Complexity
Adding Entries (add_inverted_index_entry):

Average case: O(1) (due to the properties of the hash table), worst case: O(n) (when many collisions occur).
Building the Index (build_inverted_index):

O(m * k), where m is the number of files and k is the number of words in each file. Each word requires a call to the add entry function.
Finding Entries (find_inverted_index_entry):

Average case: O(1), worst case: O(n) (again due to collisions).
3.2 Space Complexity
Inverted Index Storage:

The space complexity for the inverted index is O(n), where n is the number of unique words. If each word can associate with up to MAX_DOCS documents, the additional space complexity is O(n * MAX_DOCS).
Hash Table:

The size of the hash table is capacity, occupying O(c), where c is the capacity of the hash table.
Overall:

The total space complexity is O(n + c), where in the worst case, n and c may be very close.
4. Conclusion
The implemented inverted index effectively supports word retrieval in documents. By using a hash table to store words and their associated documents, the program can achieve fast query and addition operations on average. However, in extreme cases, hash collisions may lead to performance degradation. Therefore, in practical applications, improving the hash function or adopting other data structures may further enhance efficiency.
