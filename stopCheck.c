#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Define a list of stop words
const char *stopWords[] = {
    "a", "about", "above", "after", "again", "against", "all", "am", 
    "an", "and", "any", "are", "aren't", "as", "at", "be", "because", 
    "been", "before", "being", "below", "between", "both", "but", 
    "by", "can't", "cannot", "could", "couldn't", "did", "didn't", 
    "do", "does", "doesn't", "doing", "don't", "down", "during", 
    "each", "few", "for", "from", "further", "had", "hadn't", 
    "has", "hasn't", "have", "haven't", "he", "he'd", "he'll", 
    "he's", "her", "here", "here's", "hers", "herself", "him", 
    "himself", "his", "how", "how's", "i", "i'd", "i'll", "i'm", 
    "i've", "if", "in", "into", "is", "isn't", "it", "it's", "its", 
    "itself", "just", "ll", "m", "ma", "me", "might", "mightn't", 
    "more", "most", "must", "mustn't", "my", "myself", "need", 
    "needn't", "no", "nor", "not", "of", "off", "on", "once", 
    "only", "or", "other", "our", "ours", "ourselves", "out", 
    "over", "own", "re", "s", "same", "shan't", "she", "she'd", 
    "she'll", "she's", "should", "shouldn't", "so", "some", 
    "such", "t", "than", "that", "that's", "the", "their", 
    "theirs", "them", "themselves", "then", "there", "there's", 
    "these", "they", "they'd", "they'll", "they're", "they've", 
    "this", "those", "through", "to", "too", "under", "until", 
    "up", "ve", "very", "was", "wasn't", "we", "we'd", "we'll", 
    "we're", "we've", "were", "weren't", "what", "what's", 
    "when", "when's", "where", "where's", "which", "while", 
    "who", "who's", "whom", "why", "why's", "will", "with", 
    "won't", "would", "wouldn't", "you", "you'd", "you'll", 
    "you're", "you've", "your", "yours", "yourself", "yourselves",
    NULL // Sentinel value to mark the end of the list
};

// Role to check if a word is a stop word
bool isStopWord(const char *word) {
    for (int i = 0; stopWords[i] != NULL; i++) {
        if (strcmp(word, stopWords[i]) == 0) {
            return true; // Found a stop word
        }
    }
    return false; // Not a stop word
}

// Sample stemming role(simplified for demonstration)
int stem(char *p, int length) {
    // For demonstration, only return the input word (no actual stemming)
    return length; // Return the length of the stemmed word
}

// Main role to check stemmed results against stop words
int main() {
    const char *wordsToCheck[] = {
        "running", "quickly", "fox", "jumps", "over", "lazy", 
        "dog", "and", "example", NULL // Sentinel value
    };

    printf("Checking stemmed results for stop words...\n");

    for (int i = 0; wordsToCheck[i] != NULL; i++) {
        char word[100];
        strncpy(word, wordsToCheck[i], sizeof(word)); // Copy the word to avoid modification

        int length = stem(word, strlen(word)); // Get the stemmed result

        // Assuming stemming modifies the word in place, we would check the result
        if (isStopWord(word)) {
            printf("Found stop word in stemmed result: %s\n", word);
        } else {
            printf("No stop word found in: %s\n", word);
        }
    }

    return 0;
}
