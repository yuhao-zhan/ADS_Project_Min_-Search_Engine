# include <stdio.h>
# include <string.h>
# include <stdbool.h>
# include <stdlib.h>
# include "stmr.h"

# define MAX_STOP_WORDS 10

/*Sample stop words*/
const char *stopWords[MAX_STOP_WORDS] = 
{
	"a", "the", "is", "in", "on", "at", "and", "to", "of", "for"
};

/*Role to check if the word is a stop word*/
bool isStopWord(const char *word)
{
	for(int i = 0; i < MAX_STOP_WORDS; ++i)
	{
		if (strcmp(word, stopWords[i]) == 0)
		{
			return true;
		}
	}
	return false;
}

int main()
{
	char query[100];
	printf("Enter a word: ");
	scanf("%s", &query);
	
	if(isStopWord(query))
	{
		printf("\"%s\" is a stop word.\n", query);
	}else
	{
		printf("\"%s\" is not a stop word.\n",query);
	}
	return 0;
}

/*When the query is a stop word: If you enter "the", the output will be:
"the" is a stop word*/