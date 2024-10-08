# include <string.h>
#include <ctype.h>
# include "../header_files/AllFunctions.h"

static char *b;
static int k;
static int k0;
static int j;

static int isConsonant(int index)
{
	switch (b[index])
	{
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
		   return FALSE;
		case 'y':
		   return (index == k0) ? TRUE : !isConsonant(index - 1);
		default:
		   return TRUE;
	}
}

static int getMeasure()
{
	int position;
	int index;
	
	position = 0;
	index = k0;
	
	while (TRUE)
	{
		if (index > j)
		{
			return position;
		}
		
		if (!isConsonant(index))
		{
			break;
		}
		
		index++;
	}
	
	index++;
	
	while (TRUE)
	{
		while(TRUE)
		{
			if (index > j)
			{
				return position;
			}
			
			if(isConsonant(index))
			{
				break;
			}
			
			index++;
		}
		index++;
		position++;
		
		while (TRUE)
		{
			if (index > j)
			{
				return position;
			}
			
			if(!isConsonant(index))
			{
				break;
			}
			
			index++;
		}
		
		index++;
	}
}

static int vowelInStem()
{
	int index;
	index = k0 - 1;
	
	while (++index <= j)
	{
		if (!isConsonant(index))
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

static int isDoubleConsonant(int index)
{
	if (b[index] != b[index - 1])
	{
		return FALSE;
	}
	
	return isConsonant(index);
}

static int cvc(int index)
{
	int character;
	
	if (index < k0 + 2 || !isConsonant(index) || isConsonant(index - 1) || !isConsonant(index - 2))
	{
		return FALSE;
	}
	
	character = b[index];
	
	if (character == 'w' || character == 'x' || character == 'y')
	{
		return FALSE;
	}
	
	return TRUE;
}

static int ends(const char *value)
{
	int length = value[0];
	
	if (value[length] != b[k])
	{
		return FALSE;
	}
	
	if (length > k - k0 + 1)
	{
		return FALSE;
	}
	
	if (memcmp(b + k - length + 1, value + 1, length) != 0)
	{
		return FALSE;
	}
	
	j = k - length;
	
	return TRUE;
}

static void setTo(const char *value)
{
	int length = value[0];
	
	memmove(b + j + 1, value + 1, length);
	
	k = j + length;
}

static void replace(const char *value)
{
	if (getMeasure() > 0)
	{
		setTo(value);
	}
}

static void step1ab()
{
	int character;
	
	if (b[k] == 's')
	{
		if (ends("\04" "sses"))
		{
			k -= 2;
		}else if (ends("\03" "ies"))
		{
			setTo("\01" "i");
		}else if (b[k - 1] != 's')
		{
			k--;
		}
	}
	
	if (ends("\03" "eed"))
	{
		if (getMeasure() > 0)
		{
			k--;
		}
	}else if ((ends("\02" "ed") || ends("\03" "ing")) && vowelInStem())
	{
		k = j;
		
		if (ends("\02" "at"))
		{
			setTo("\03" "ate");
		}else if (ends("\02" "bl"))
		{
			setTo("\03" "ble");
		}else if(ends("\02" "iz"))
		{
			setTo("\03" "ize");
		}else if (isDoubleConsonant(k))
		{
			k--;
			
			character = b[k];
			
			if (character == 'l' || character == 's' || character == 'z')
			{
				k++;
			}
		}else if (getMeasure() == 1 && cvc(k))
		{
			setTo("\01" "e");
		}
	}
}

static void step1c()
{
	if (ends("\01" "y") && vowelInStem())
	{
		b[k] = 'i';
	}
}

static void step2()
{
	switch (b[k - 1])
	{
		case 'a':
		   if (ends("\07" "ational"))
		   {
		   	replace("\03" "ate");
			break;
		   }
		   
		   if (ends("\06" "tional"))
		   {
		   	replace("\04" "tion");
			break;
		   }
		   
		   break;
		   
		case 'c':
		   if (ends("\04" "enci"))
		   {
		   	replace("\04" "ence");
			break;
		   }
		   
		   if (ends("\04" "anci"))
		   {
		   	replace("\04" "ance");
			break;
		   }
		   
		   break;
		   
		case 'e':
		   if (ends("\04" "izer"))
		   {
		   	replace("\03" "ize");
			break;
		   }
		   
		   break;
		   
		case 'l':
		   if (ends("\03" "bli"))
		   {
		   	replace("\03" "ble");
			break;
		   }
		   
		   if (ends("\04" "alli"))
		   {
		   	replace("\02" "al");
			break;
		   }
		   
		   if (ends("\05" "entli"))
		   {
		   	replace("\03" "ent");
			break;
		   }
		   
		   if (ends("\03" "eli"))
		   {
		   	replace("\01" "e");
			break;
		   }
		   
		   if (ends("\05" "ousli"))
		   {
		   	replace("\03" "ous");
			break;
		   }
		   
		   break;
		   
		case 'o':
		   if (ends("\07" "ization"))
		   {
		   	replace("\03" "ize");
			break;
		   }
		   
		   if (ends("\05" "ation"))
		   {
		   	replace("\03" "ate");
			break;
		   }
		   
		   if (ends("\04" "ator"))
		   {
		   	replace("\03" "ate");
			break;
		   }
		   
		   break;
		   
		case 's':
		   if (ends("\05" "alism"))
		   {
		   	replace("\02" "al");
			break;
		   }
		   
		   if (ends("\07" "iveness"))
		   {
		   	replace("\03" "ive");
			break;
		   }
		   
		   if (ends("\07" "fulness"))
		   {
		   	replace("\03" "ful");
			break;
		   }
		   
		   if (ends("\07" "ousness"))
		   {
		   	replace("\03" "ous");
			break;
		   }
		   
		   break;
		   
		case 't':
		   if (ends("\05" "aliti"))
		   {
		   	replace("\02" "al");
			break;
		   }
		   
		   if (ends("05" "iviti"))
		   {
		   	replace("\03" "ive");
			break;
		   }
		   
		   if (ends("\06" "biliti"))
		   {
		   	replace("\03" "ble");
			break;
		   }
		   
		   break;
		   
		case 'g':
		   if (ends("\04" "logi"))
		   {
		   	replace("\03" "log");
			break;
		   }
	}
}

static void step3()
{
	switch (b[k])
	{
		case 'e':
		   if (ends("\05" "icate"))
		   {
		   	replace("\02" "oc");
			break;
		   }
		   
		   if (ends("\05" "ative"))
		   {
		   	replace("\00" "");
			break;
		   }
		   
		   if (ends("\05" "alize"))
		   {
		   	replace("\02" "al");
			break;
		   }
		   
		   break;
		   
		case 'i':
		   if (ends("\05" "iciti"))
		   {
		   	replace("\02" "ic");
			break;
		   }
		   
		   break;
		   
		case 'l':
			if (ends("\04" "ical"))
			{
				replace("\02" "ic");
				break;
			}
			
			if (ends("\03" "ful"))
			{
				replace("\00" "");
				break;
			}
			
			break;
			
		case 's':
			if (ends("\04" "ness"))
			{
				replace("\00" "");
				break;
			}
			
			break;
	}
}

static void step4()
{
	switch (b[k - 1])
	{
		case 'a':
		   if (ends("\02" "al"))
		   {
		   	break;
		   }
		   
		   return;
		   
		case 'c':
		   if (ends("\04" "ance"))
		   {
		   	break;
		   }
		   
		   if (ends("\04" "ence"))
		   {
		   	break;
		   }
		   
		   return;
		   
		case 'e':
		   if (ends("\02" "er"))
		   {
		   	break;
		   }
		   
		   return;
		   
		case 'i':
		   if (ends("\02" "ic"))
		   {
		   	break;
		   }
		   
		   return;
		   
		case 'l':
		   if (ends("\04" "able"))
		   {
		   	break;
		   }
		   
		   if (ends("\04" "ible"))
		   {
		   	break;
		   }
		   
		   return;
		   
		case 'n':
		   if (ends("\03" "ant"))
		   {
		   	break;
		   }
		   
		   if (ends("\05" "ement"))
		   {
		   	break;
		   }
		   
		   if (ends("\04" "ment"))
		   {
		   	break;
		   }
		   
		   if (ends("\03" "ent"))
		   {
		   	break;
		   }
		   
		   return;
		   
		case 'o':
		   if (ends("\03" "ion") && j >= k0 && (b[j] == 's' || b[j] == 't'))
		   {
		   	break;
		   }
		   
		   if (ends("\02" "ou"))
		   {
		   	break;
		   }
		   
		   return;
		   
		case 's':
		   if (ends("\03" "ism"))
		   {
		   	break;
		   }
		   
		   return;
		   
		case 't':
		   if (ends("\03" "ate"))
		   {
		   	break;
		   }
		   
		   if (ends("\03" "iti"))
		   {
		   	break;
		   }
		   
		   return;
		   
		case 'u':
		   if (ends("\03" "ous"))
		   {
		   	break;
		   }
		   
		   return;
		   
		case 'v':
		   if (ends("\03" "ive"))
		   {
		   	break;
		   }
		   
		   return;
		   
		case 'z':
		   if (ends("\03" "ize"))
		   {
		   	break;
		   }
		   
		   return;
		   
		default:
		   return;
	}
	
	if (getMeasure() > 1)
	{
		k = j;
	}
}

static void step5()
{
	int a;
	
	j = k;
	
	if (b[k] == 'e')
	{
		a = getMeasure();
		
		if (a > 1 || (a == 1 && !cvc(k - 1)))
		{
			k--;
		}
	}
	
	if (b[k] == 'l' && isDoubleConsonant(k) && getMeasure() > 1)
	{
		k--;
	}
}

static void removePunctuation(char *p, int *start, int *end)
{
	int j = *start;
	for(int i = *start; i <= *end; ++i)
	{
		if (isalpha(p[i]) || (p[i] == '.' && i < *end) || (p[i] == '\'' && i < *end))
		{
			p[j++] = p[i];
		}
	}
	*end = j - 1;
	p[j] = '\0';
}

int stem(char *p, int index, int position)
{
	int start = index;
	int end = position;
	removePunctuation(p, &start, &end);
	
	b = p;
	k =end;
	k0 = start;
	
	if (k <= k0 + 1)
	{
		return k;
	}
	
	step1ab();
	
	if (k > k0)
	{
		step1c();
		step2();
		step3();
		step4();
		step5();
	}
	
	return k;
}