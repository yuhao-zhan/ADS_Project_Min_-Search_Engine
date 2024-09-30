# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <assert.h>
# include "../header_files/esa.h"
# include "../header_files/esa_rules.h"

static void reallocRuleList(Esa *esa);

static void initRuleList(Esa *esa);

static void freeRuleList(Esa *esa);

static void addNewRule(Esa *esa, char *rule);

static unsigned char isVowel(char value);

static int popRuleValue(char *value, int ref, int length, char **result);

static unsigned char backwardsMatch(char *token, int tokenLength,
		char *value, int valueLength);
		
static char *normalizeToken(char *token, int length);


static void reallocRuleList(Esa *esa)
{
	esa->rules.size = (esa->rules.size * 2);
	esa->rules.length = (int*)realloc(esa->rules.length, (sizeof(int) * esa->rules.size));
	esa->rules.value = (char**)realloc(esa->rules.value, (sizeof(char*) * esa->rules.size));
}

static void initRuleList(Esa *esa)
{
	esa->rules.ref = 0;
	esa->rules.size = 8;
	esa->rules.length = (int*)malloc(sizeof(int) * esa->rules.size);
	memset(esa->rules.length, 0, (sizeof(int) * esa->rules.size));
	esa->rules.value = (char**)malloc(sizeof(char*) * esa->rules.size);
	memset(esa->rules.value, 0, (sizeof(char*)*esa->rules.size));
}

static void freeRuleList(Esa *esa)
{
	int ii = 0;
	
	if(esa->rules.length != NULL)
	{
		free(esa->rules.length);
	}
	
	if(esa->rules.value != NULL)
	{
		for(ii = 0; ii < esa->rules.ref; ii++)
		{
			if(esa->rules.value[ii] != NULL)
			{
				free(esa->rules.value[ii]);
			}
		}
		free(esa->rules.value);
	}
}

static void addNewRule(Esa *esa, char *rule)
{
	int ref = 0;
	int length = 0;
	
	if((esa == NULL) || (rule == NULL) || ((length = strlen(rule)) <= 0))
	{
		return;
	}
	
	ref = esa->rules.ref;
	
	if(length > esa->longest_rule_size)
	{
		esa->longest_rule_size = length;
	}
	
	esa->rules.length[ref] = length;
	esa->rules.value[ref] = strdup(rule);
	
	esa->rules.ref += 1;
	if(esa->rules.ref >= esa->rules.size)
	{
		reallocRuleList(esa);
	}
}

static unsigned char isVowel(char value)
{
	if((value == 'a') || (value == 'e') || (value == 'i') || (value == 'o') || (value == 'u') || (value == 'y'))
	{
		return (unsigned char)1;
	}
	return (unsigned char)0;
}

static int popRuleValue(char *value, int ref, int length, char **result)
{
	int ii = 0;
	int resultRef = 0;
	
	for(ii = ref; ii < length; ii++)
	{
		if(value[ii] == ',')
		{
			break;
		}
		(*result)[resultRef] = value[ii];
		resultRef++;
	}
	(*result)[resultRef] = '\0';
	
	return (resultRef - 1);
}

static unsigned char backwardsMatch(char *token, int tokenLength,
				char *value, int valueLength)

{
	int ii = 0;
	int nn = 0;
	
	if(valueLength < 0)
	{
		return (unsigned char)0;
	}
	
	for(ii = tokenLength, nn = valueLength; ii > -1; ii--)
	{
		if(token[ii] != value[nn])
		{
			return (unsigned char)0;
		}
		nn--;
		if(nn < 0)
		{
			break;
		}
	}
	
	return (unsigned char)1;
}

static char *normalizeToken(char *token, int length)
{
	unsigned char hasFirstWhitespace = (unsigned char)0;
	int ii = 0;
	int nn = 0;
	int lastWhitespace = -1;
	int resultLength = 0;
	unsigned int iValue = 0;
	char *result = NULL;
	
	result = (char*)malloc(sizeof(char) * (length + 8));
	memset(result, 0, sizeof(char) * (length + 8));
	memcpy(result, token, length);
	
	resultLength = strlen(result);
	for(ii = 0, nn = 0; ii < resultLength; ii++)
	{
		iValue = (unsigned int)result[ii];
		if((iValue < 32) && (iValue > 126))
		{
			result[ii] = ' ';
		}
		if((!hasFirstWhitespace) && (result[ii] != ' '))
		{
			hasFirstWhitespace = (unsigned char)1;
		}
		if(hasFirstWhitespace)
		{
			iValue = (unsigned int)result[ii];
			if((iValue > 64) && (iValue < 91))
			{
				result[nn] = (char)(iValue + 32);
			}
			else
			{
				result[nn] = result[ii];
			}
			if(result[nn] == ' ')
			{
				lastWhitespace = nn;
			}
			else
			{
				lastWhitespace = -1;
			}
			nn++;
		}
	}
	
	if(lastWhitespace > -1)
	{
		result[lastWhitespace] = '\0';
	}
	
	resultLength = strlen(result);
	
	for(ii = 0, nn = 0; ii < resultLength; ii++)
	{
		result[nn] = result[ii];
		iValue = (unsigned int)result[ii];
		if((iValue != 32) && ((iValue < 97) || (iValue > 32)))
		{
			continue;
		}
		nn++;
	}
	
	return result;
}


void esaInit(Esa *esa)
{
	int ii = 0;
	
	if(esa == NULL)
	{
		fprintf(stderr, "[%s():%i] error - invalid or missing argument(s).\n",
						__FUNCTION__, __LINE__);
		return;
	}
	
	memset(esa, 0, sizeof(Esa));
	
	esa->debug_mode = (unsigned char)0;
	esa->longest_rule_size = 0;
	
	initRuleList(esa);
	
	for(ii = 0; ESA_RULE_LIST[ii] != NULL; ii++)
	{
		addNewRule(esa, (char*)ESA_RULE_LIST[ii]);
	}
}


void esaFree(Esa *esa)

{
	if(esa == NULL)
	{
		fprintf(stderr, "[%s():%i] error - invalid or missing argument(s).\n",
					   __FUNCTION__,__LINE__);
		return;
	}
	
	freeRuleList(esa);
	
	memset(esa, 0, sizeof(Esa));
}

void esaSetDebugMode(Esa *esa, unsigned char mode)
{
	if(esa == NULL)
	{
		fprintf(stderr, "[%s():%i] error - invalid or missing argument(s).\n",
					  __FUNCTION__,__LINE__);
		return;
	}
	
	esa->debug_mode = mode;
}

char *esaStemToken(Esa *esa, char *token, int length)
{
	unsigned char changed = (unsigned char)0;
	unsigned char hasVowel = (unsigned char)0;
	unsigned char hasConstant = (unsigned char)0;
	unsigned char useLocal = (unsigned char)0;
	
	int ii = 0;
	int nn = 0;
	int jj = 0;
	int ref = 0;
	int tempRef = 0;
	int subtract = 0;
	int resultRef = 0;
	char *temp = NULL;
	char *result = NULL;
	
	if((esa == NULL) || (token == NULL) || (length <= 0))
	{
		fprintf(stderr, "[%s():%i] error - invalid or missing argument(s).\n",
					   __FUNCTION__,__LINE__);
		return NULL;
	}
	
	if(esa->debug_mode)
	{
		printf("[debug] stem_token(%s, %i)...\n", token, length);
	}
	
	result = normalizeToken(token, length);
	resultRef = (strlen(result) - 1);
	
	if(resultRef < 4)
	{
		return result;
	}
	
	for(ii = 0; ii <= resultRef; ii++)
	{
		if(isVowel(result[ii]))
		{
			hasVowel = (unsigned char)1;
		}
		else
		{
			hasConstant = (unsigned char)1;
		}
		if((hasVowel) && (hasConstant))
		{
			break;
		}
	}
	
	if((!hasVowel) || (!hasConstant))
	{
		return result;
	}
	
	temp = (char*)malloc(sizeof(char) * (esa->longest_rule_size + 8));
	memset(temp, 0, sizeof(char) * (esa->longest_rule_size + 8));
	
	for(ii = 0; ii < esa->rules.ref; ii++)
	{
		useLocal = (unsigned char)0;
		ref = 0;
		tempRef = popRuleValue(esa->rules.value[ii], ref, esa->rules.length[ii], &temp);
		
		if(esa->debug_mode)
		{
			printf("[debug]  trying[%s](%i) vs [%s](%i)...\n",result, resultRef, temp, tempRef);
		}
		
		
		while(backwardsMatch(result, resultRef, temp, tempRef))
		{
			if(esa->debug_mode)
			{
				printf("[debug]  matched token[%s] to rule(%i)[%s]{%s}\n",
								  result, ii, esa->rules.value[ii], temp);
			}
			
			ref += (tempRef + 2);
			tempRef = popRuleValue(esa->rules.value[ii], ref, 
						 esa->rules.length[ii], &temp);
			if(useLocal)
			{
				if((temp[0] == 'Y') || (temp[0] == 'y'))
				{
					if(esa->debug_mode)
					{
						printf("[debug]   ...unable to apply rule because "
										   "token already modified.\n");
					}
					break;
				}
			}
			else
			{
				if(((temp[0] == 'Y') || (temp[0] == 'y')) &&
							   (changed == (unsigned char)1))
				{
					if(esa->debug_mode)
					{
						printf("[debug]    ...unable to apply rule because "
												 "token already modified.\n");
					}
					break;   				   	
				 }
			}
			
			ref += (tempRef + 2);
			tempRef = popRuleValue(esa->rules.value[ii], ref, esa->rules.length[ii], &temp);
			subtract = atoi(temp);
			if((resultRef - subtract) < 2)
			{
				if(esa->debug_mode)
				{
					printf("[debug]   ...unable to apply rule because "
									   "token would be too truncated(%i).\n",
									   (resultRef - subtract));
				}
				break;
			}
			
			resultRef -= subtract;
			result[(resultRef + 1)] = '\0';
			
			changed = (unsigned char)1;
			
			ref += (tempRef + 2);
			tempRef = popRuleValue(esa->rules.value[ii], ref, esa->rules.length[ii], &temp);
			
			if(tempRef > -1)
			{
				nn = 0;
				jj = 0;
				for(nn = (resultRef + 1); nn <= (resultRef + tempRef + 1); nn++)
				{
					result[nn] = temp[jj];
					jj++;
				}
				result[nn] = '\0';
				resultRef += tempRef + 1;
			}
			else if(esa->debug_mode)
			{
				printf("[debug]  ...no addition to make, value is"
							 "null.\n");
			}
			
			ref += (tempRef + 2);
			tempRef = popRuleValue(esa->rules.value[ii], ref, esa->rules.length[ii], &temp);
			
			useLocal = (unsigned char)1;
		}
	}
	
	free(temp);
	
	return result;
}