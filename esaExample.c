# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "esa.h"

static const char *EXAMPLE_TOKENS[] = 
{
	"This",
	"is",
	"a",
	"test",
	"of",
	"the",
	"English",
	"Stemming",
	"Algorithm",
	"(ESA)",
	"(English,",
	"ZLY",
	"S.",
	"2003).",
	"Here",
	"is",
	"a",
	"typical",
	"sentence:",
	"the",
	"quick",
	"brown",
	"fox",
	"jumped",
	"over",
	"the",
	"lazy",
	"dogs.",
	"Theses",
	"here",
	"words",
	"type",
	"things",
	"will",
	"hopefully",
	"be",
	"correctly",
	"stemmed",
	"by",
	"this",
	"heary",
	"algorithmic",
	"and",
	"stemmed",
	"out",
	"to",
	"their",
	"proper",
	"selves",
	NULL
};

int main()
{
	int ii = 0;
	int tokenLength = 0;
	char *token = NULL;
	char *stem = NULL;
	
	Esa esa;
	
	printf("Example output from ESA:\n\n");
	
	esaInit(&esa);
	esaSetDebugMode(&esa, (unsigned char)0);
	
	for(ii = 0; EXAMPLE_TOKENS[ii] != NULL; ii++)
	{
		token = (char*)EXAMPLE_TOKENS[ii];
		tokenLength = strlen(token);
		
		stem = esaStemToken(&esa, token, tokenLength);
		
		printf("\t ...token '%s' => transform => '%s'\n", token, stem );
		
		if(stem != NULL)
		{
			free(stem);
		}
	}
	
	esaFree(&esa);
	
	printf("\n\n...Example completed.\n");
	
	return 0;
}