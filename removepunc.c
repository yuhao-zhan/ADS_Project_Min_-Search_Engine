# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>

void remove_punctuation(const char *input_file, const char *output_file)
{
	FILE *infile = fopen(input_file, "r");
	FILE *outfile = fopen(output_file, "w");
	
	if (infile == NULL)
	{
		perror("Eorror opening input file");
		return;
	}
	
	if (outfile == NULL)
	{
		perror("Eorror opening output file");
		fclose(infile);
		return;
	}
	
	int ch;
	while ((ch = fgetc(infile)) != EOF)
	{
		if(!ispunct(ch))
		{
			fputc(ch, outfile);
		}
	}
	
	fclose(infile);
	fclose(outfile);
}

int main()
{
	const char *input_file = "input.txt";
	const char *output_file = "output.txt";
	
	remove_punctuation(input_file, output_file);
	
	printf("Punctuation removed and output saved to %s\n",output_file);
	
	return 0;
}