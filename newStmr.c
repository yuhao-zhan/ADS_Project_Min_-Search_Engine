#include <string.h>
#include "stmr.h"

// Global variables for stemmer state
static char *b;  // Input string to stem
static int k;    // Current position in the string
static int k0;   // Start of the word
static int j;    // End of the word

// Role to check if the character at the given index is a consonant
static int isConsonant(int index)
{
	switch (b[index])
	{
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
		   return FALSE;  // Vowel
		case 'y':
		   return (index == k0) ? TRUE : !isConsonant(index - 1); // Check 'y' conditionally
		default:
		   return TRUE; // Consonant
	}
}

// Role to calculate the measure of the word (number of VC sequences)
static int getMeasure()
{
	int position = 0;  // Measure position count
	int index = k0;    // Start at the beginning of the stem

	while (TRUE)
	{
		if (index > j)
		{
			return position; // Return measure if out of bounds
		}
		
		if (!isConsonant(index))
		{
			break; // Found a vowel, break the loop
		}
		
		index++; // Move to next character
	}
	
	index++; // Move to the next character after the vowel
	
	while (TRUE)
	{
		while(TRUE)
		{
			if (index > j)
			{
				return position; // Return measure if out of bounds
			}
			
			if(isConsonant(index))
			{
				break; // Found a consonant, break the loop
			}
			
			index++; // Move to next character
		}
		index++;
		position++; // Increment position (found a VC sequence)
		
		while (TRUE)
		{
			if (index > j)
			{
				return position; // Return measure if out of bounds
			}
			
			if(!isConsonant(index))
			{
				break; // Found a vowel, break the loop
			}
			
			index++; // Move to next character
		}
		
		index++; // Move to next character after the vowel
	}
}

// Check if-there is any vowel in the stem
static int vowelInStem()
{
	int index = k0 - 1; // Start at the beginning of the stem
	
	while (++index <= j)
	{
		if (!isConsonant(index))
		{
			return TRUE; // Found a vowel
		}
	}
	
	return FALSE; // No vowels found
}

// Check if the current character and the earlier one are the same consonant
static int isDoubleConsonant(int index)
{
	if (b[index] != b[index - 1])
	{
		return FALSE; // Not a double consonant
	}
	
	return isConsonant(index); // Check if it's a consonant
}

// Check if the substring is a CVC (consonant-vowel-consonant) pattern
static int cvc(int index)
{
	int character;
	
	if (index < k0 + 2 || !isConsonant(index) || isConsonant(index - 1) || !isConsonant(index - 2))
	{
		return FALSE; // Not a CVC pattern
	}
	
	character = b[index];
	
	if (character == 'w' || character == 'x' || character == 'y')
	{
		return FALSE; // Disallowed characters
	}
	
	return TRUE; // Valid CVC
}

// Check if the stem ends with the specified value
static int ends(const char *value)
{
	int length = value[0]; // Length of the suffix
	
	if (value[length] != b[k])
	{
		return FALSE; // Does not match the end character
	}
	
	if (length > k - k0 + 1)
	{
		return FALSE; // Suffix is too long
	}
	
	if (memcmp(b + k - length + 1, value + 1, length) != 0)
	{
		return FALSE; // Suffix does not match
	}
	
	j = k - length; // Set new end position
	return TRUE; // Suffix matches
}

// Replace the end of the stem with the specified value
static void setTo(const char *value)
{
	int length = value[0]; // Length of the new suffix
	
	memmove(b + j + 1, value + 1, length); // Move to set new suffix
	k = j + length; // Update the position
}

// Replace the suffix if the measure is greater than zero
static void replace(const char *value)
{
	if (getMeasure() > 0)
	{
		setTo(value); // Do replacement
	}
}

// Step 1a of the stemming process
static void step1ab()
{
	int character;
	
	if (b[k] == 's')
	{
		if (ends("\04" "sses"))
		{
			k -= 2; // Remove 'sses'
		}else if (ends("\03" "ies"))
		{
			setTo("\01" "i"); // Change 'ies' to 'i'
		}else if (b[k - 1] != 's')
		{
			k--; // Remove 's'
		}
	}
	
	if (ends("\03" "eed"))
	{
		if (getMeasure() > 0)
		{
			k--; // Remove 'e' if measure > 0
		}
	}else if ((ends("\02" "ed") || ends("\03" "ing")) && vowelInStem())
	{
		k = j; // Set position to end
		
		if (ends("\02" "at"))
		{
			setTo("\03" "ate"); // Change 'at' to 'ate'
		}else if (ends("\02" "bl"))
		{
			setTo("\03" "ble"); // Change 'bl' to 'ble'
		}else if(ends("\02" "iz"))
		{
			setTo("\03" "ize"); // Change 'iz' to 'ize'
		}else if (isDoubleConsonant(k))
		{
			k--; // Remove double consonant

			character = b[k];
			
			if (character == 'l' || character == 's' || character == 'z')
			{
				k++; // Keep the character if it's 'l', 's', or 'z'
			}
		}else if (getMeasure() == 1 && cvc(k))
		{
			setTo("\01" "e"); // Change to 'e' if CVC
		}
	}
}

// Step 1c of the stemming process
static void step1c()
{
	if (ends("\01" "y") && vowelInStem())
	{
		b[k] = 'i'; // Change 'y' to 'i'
	}
}

// Step 2 of the stemming process
static void step2()
{
	switch (b[k - 1]) // Check the last character before the stem
	{
		case 'a':
		   if (ends("\07" "ational")) // Check for 'ational'
		   {
		   	replace("\03" "ate"); // Replace with 'ate'
			break;
		   }
		   
		   if (ends("\06" "tional")) // Check for 'tional'
		   {
		   	replace("\04" "tion"); // Replace with 'tion'
			break;
		   }
		   
		   break; // No replacements made
		   
		case 'c':
		   if (ends("\04" "enci")) // Check for 'enci'
		   {
		   	replace("\04" "ence"); // Replace with 'ence'
			break;
		   }
		   
		   if (ends("\04" "anci")) // Check for 'anci'
		   {
		   	replace("\04" "ance"); // Replace with 'ance'
			break;
		   }
		   
		   break; // No replacements made
		   
		case 'e':
		   if (ends("\04" "izer")) // Check for 'izer'
		   {
		   	replace("\03" "ize"); // Replace with 'ize'
			break;
		   }
		   
		   break; // No replacements made
		   
		case 'l':
		   if (ends("\03" "bli")) // Check for 'bli'
		   {
		   	replace("\03" "ble"); // Replace with 'ble'
			break;
		   }
		   
		   if (ends("\04" "alli")) // Check for 'alli'
		   {
		   	replace("\02" "al"); // Replace with 'al'
			break;
		   }
		   
		   if (ends("\05" "entli")) // Check for 'entli'
		   {
		   	replace("\03" "ent"); // Replace with 'ent'
			break;
		   }
		   
		   if (ends("\03" "eli")) // Check for 'eli'
		   {
		   	replace("\01" "e"); // Replace with 'e'
			break;
		   }
		   
		   if (ends("\05" "ousli")) // Check for 'ousli'
		   {
		   	replace("\03" "ous"); // Replace with 'ous'
			break;
		   }
		   
		   break; // No replacements made
		   
		case 'o':
		   if (ends("\07" "ization")) // Check for 'ization'
		   {
		   	replace("\03" "ize"); // Replace with 'ize'
			break;
		   }
		   
		   if (ends("\05" "ation")) // Check for 'ation'
		   {
		   	replace("\03" "ate"); // Replace with 'ate'
			break;
		   }
		   
		   if (ends("\04" "ator")) // Check for 'ator'
		   {
		   	replace("\03" "ate"); // Replace with 'ate'
			break;
		   }
		   
		   break; // No replacements made
		   
		case 's':
		   if (ends("\05" "alism")) // Check for 'alism'
		   {
		   	replace("\02" "al"); // Replace with 'al'
			break;
		   }
		   
		   if (ends("\07" "iveness")) // Check for 'iveness'
		   {
		   	replace("\03" "ive"); // Replace with 'ive'
			break;
		   }
		   
		   if (ends("\07" "fulness")) // Check for 'fulness'
		   {
		   	replace("\03" "ful"); // Replace with 'ful'
			break;
		   }
		   
		   if (ends("\07" "ousness")) // Check for 'ousness'
		   {
		   	replace("\03" "ous"); // Replace with 'ous'
			break;
		   }
		   
		   break; // No replacements made
		   
		case 't':
		   if (ends("\05" "aliti")) // Check for 'aliti'
		   {
		   	replace("\02" "al"); // Replace with 'al'
			break;
		   }
		   
		   if (ends("\05" "iviti")) // Check for 'iviti'
		   {
		   	replace("\03" "ive"); // Replace with 'ive'
			break;
		   }
		   
		   if (ends("\06" "biliti")) // Check for 'biliti'
		   {
		   	replace("\03" "ble"); // Replace with 'ble'
			break;
		   }
		   
		   break; // No replacements made
		   
		case 'g':
		   if (ends("\04" "logi")) // Check for 'logi'
		   {
		   	replace("\03" "log"); // Replace with 'log'
			break;
		   }
	}
}

// Step 3 of the stemming process
static void step3()
{
	switch (b[k]) // Check the last character of the stem
	{
		case 'e':
		   if (ends("\05" "icate")) // Check for 'icate'
		   {
		   	replace("\02" "oc"); // Replace with 'oc'
			break;
		   }
		   
		   if (ends("\05" "ative")) // Check for 'ative'
		   {
		   	replace("\00" ""); // Remove suffix
			break;
		   }
		   
		   if (ends("\05" "alize")) // Check for 'alize'
		   {
		   	replace("\02" "al"); // Replace with 'al'
			break;
		   }
		   
		   break; // No replacements made
		   
		case 'i':
		   if (ends("\05" "iciti")) // Check for 'iciti'
		   {
		   	replace("\02" "ic"); // Replace with 'ic'
			break;
		   }
		   
		   break; // No replacements made
		   
		case 'l':
			if (ends("\04" "ical")) // Check for 'ical'
			{
				replace("\02" "ic"); // Replace with 'ic'
				break;
			}
			
			if (ends("\03" "ful")) // Check for 'ful'
			{
				replace("\00" ""); // Remove suffix
				break;
			}
			
			break; // No replacements made
			
		case 's':
			if (ends("\04" "ness")) // Check for 'ness'
			{
				replace("\00" ""); // Remove suffix
				break;
			}
			
			break; // No replacements made
	}
}

// Step 4 of the stemming process
static void step4()
{
	switch (b[k - 1]) // Check the last character before the stem
	{
		case 'a':
		   if (ends("\02" "al")) // Check for 'al'
		   {
		   	break; // No replacement needed
		   }
		   
		   return; // No change made
		   
		case 'c':
		   if (ends("\04" "ance")) // Check for 'ance'
		   {
		   	break; // No replacement needed
		   }
		   
		   if (ends("\04" "ence")) // Check for 'ence'
		   {
		   	break; // No replacement needed
		   }
		   
		   return; // No change made
		   
		case 'e':
		   if (ends("\02" "er")) // Check for 'er'
		   {
		   	break; // No replacement needed
		   }
		   
		   return; // No change made
		   
		case 'i':
		   if (ends("\02" "ic")) // Check for 'ic'
		   {
		   	break; // No replacement needed
		   }
		   
		   return; // No change made
		   
		case 'l':
		   if (ends("\04" "able")) // Check for 'able'
		   {
		   	break; // No replacement needed
		   }
		   
		   if (ends("\04" "ible")) // Check for 'ible'
		   {
		   	break; // No replacement needed
		   }
		   
		   return; // No change made
		   
		case 'n':
		   if (ends("\03" "ant")) // Check for 'ant'
		   {
		   	break; // No replacement needed
		   }
		   
		   if (ends("\05" "ement")) // Check for 'ement'
		   {
		   	break; // No replacement needed
		   }
		   
		   if (ends("\04" "ment")) // Check for 'ment'
		   {
		   	break; // No replacement needed
		   }
		   
		   if (ends("\03" "ent")) // Check for 'ent'
		   {
		   	break; // No replacement needed
		   }
		   
		   return; // No change made
		   
		case 'o':
		   if (ends("\03" "ion") && j >= k0 && (b[j] == 's' || b[j] == 't')) // Check for 'ion'
		   {
		   	break; // No replacement needed
		   }
		   
		   if (ends("\02" "ou")) // Check for 'ou'
		   {
		   	break; // No replacement needed
		   }
		   
		   return; // No change made
		   
		case 's':
		   if (ends("\03" "ism")) // Check for 'ism'
		   {
		   	break; // No replacement needed
		   }
		   
		   return; // No change made
		   
		case 't':
		   if (ends("\03" "ate")) // Check for 'ate'
		   {
		   	break; // No replacement needed
		   }
		   
		   if (ends("\03" "iti")) // Check for 'iti'
		   {
		   	break; // No replacement needed
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

// Step 5 of the stemming process
static void step5()
{
	int a; // Variable to hold the measure of the word
	
	j = k; // Set j to the current position (end of the word)
	
	// Check if the last character is 'e'
	if (b[k] == 'e')
	{
		a = getMeasure(); // Get the measure of the word
		
		// Remove 'e' if conditions are-met: measure > 1 or measure == 1 and the last consonant is not a CVC
		if (a > 1 || (a == 1 && !cvc(k - 1)))
		{
			k--; // Remove the 'e' by decrementing the position
		}
	}
	
	// Check for the case where the last character is 'l'
	if (b[k] == 'l' && isDoubleConsonant(k) && getMeasure() > 1)
	{
		k--; // Remove the 'l' if it's a double consonant and measure > 1
	}
}

// Role to remove punctuation from the input string
static void removePunctuation(char *p, int *start, int *end)
{
	int j = *start; // Index to track the new position in the cleaned string
	for(int i = *start; i <= *end; ++i)
	{
		// Check for valid alphabetic characters, '.' (if not the last char), or '\'' (if not the last char)
		if (isalpha(p[i]) || (p[i] == '.' && i < *end) || (p[i] == '\'' && i < *end))
		{
			p[j++] = p[i]; // Copy valid characters to the new position
		}
	}
	*end = j - 1; // Update the end index to show the new string length
	p[j] = '\0'; // Null-terminate the string to properly end it
}

// Main role to stem the input string
int stem(char *p, int index, int position)
{
	int start = index; // Start position of the word
	int end = position; // End position of the word
	removePunctuation(p, &start, &end); // Clean up punctuation from the input string
	
	b = p; // Assign the cleaned input string to the global variable
	k = end; // Set k to the end position of the word
	k0 = start; // Set k0 to the start position of the word
	
	// If the word length is too short, return without processing
	if (k <= k0 + 1)
	{
		return k; // Return the current length of the word
	}
	
	step1ab(); // Do the first steps of stemming (1a and 1b)
	
	// Only try to further steps if-there are characters to process
	if (k > k0)
	{
		step1c(); // Do step 1c
		step2(); // Do step 2
		step3(); // Do step 3
		step4(); // Do step 4
		step5(); // Do step 5
	}
	
	return k; // Return the new length of the stemmed word
}
