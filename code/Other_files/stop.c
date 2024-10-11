#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
# include "../header_files/AllFunctions.h"
// If you want to use the stemming algorithm, you should move this file to the project `code` directory.

extern int stem(char *p, int index, int position);

// 停用词列表
const char *stopWords[] = {
	"i", "me", "my", "myself", "we", "our", "ours", "ourselves",
	"you", "your", "yours", "yourself", "yourselves", "he", "him",
	"his", "himself", "she", "her", "hers", "herself", "it", "its",
	"itself", "they", "them", "their", "theirs", "themselves", "what",
	"which", "who", "whom", "this", "that", "these", "those", "am",
	"is", "are", "was", "were", "be", "been", "being", "have", "has",
	"had", "having", "do", "does", "did", "doing", "a", "an", "the",
	"and", "but", "if", "or", "because", "as", "until", "while", "of",
	"at", "by", "for", "with", "about", "against", "between", "into",
	"through", "during", "before", "after", "above", "below", "to",
	"from", "up", "down", "in", "out", "on", "off", "over", "under",
	"again", "further", "then", "once", "here", "there", "when", "where",
	"why", "how", "all", "any", "both", "each", "few", "more", "most",
	"other", "some", "such", "no", "nor", "not", "only", "own", "same",
	"so", "than", "too", "very", "s", "t", "can", "will", "just", "don",
	"should", "now", NULL
};


int isStopWord(const char *word) {
	for (int i = 0; stopWords[i] != NULL; i++) {
		if (strcmp(word, stopWords[i]) == 0) {
			return 1; 
		}
	}
	return 0; 
}


int main() {
	FILE *file = fopen("input.txt", "r");
	if (!file) {
		perror("无法打开文件");
		return EXIT_FAILURE;
	}

	char word[100];
	while (fscanf(file, "%99s", word) == 1) {
		
		int len = strlen(word);
		for (int i = 0; i < len; i++) {
			if (!isalpha(word[i])) {
				word[i] = '\0'; 
				break;
			}
		}

		
		int newLength = stem(word, 0, strlen(word) - 1);
		word[newLength] = '\0'; 

		
		if (!isStopWord(word) && newLength > 0) {
			printf("%s\n", word);
		}
	}

	fclose(file);
	return EXIT_SUCCESS;
}
