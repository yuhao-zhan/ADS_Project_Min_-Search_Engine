Our stemming code implements a variation of the Porter stemming algoritm, which processed English words to reduce them to their base or root forms.
## Rule Syntax and Handling
#### 1.Consonant and Vowel Recognition:
The algorithm distinguishes between consonants and vowels using the `isConsonant` function. This function handles special cases, such as 'y', which can act as both a consonant and a vowel depending on its position in the word.
#### 2.Measure Calculation:
The `getMeasure` function counts the number of vowel-consonant sequences (VC) in the word, crucial for determining if certain suffixes can be removed based on their length and structure.
#### 3.Suffix Handling:
Various suffixed are processed through functions like `ends` and `setTo` . These functions check if the word ends with specific patterns (e.g., "ing","ed") and apply transformations(like "ies" to "i"). The algorithm also uses specific conditions, such as checking for vowel presence or ensuring the measure is above a certain threshold before applying these rules.
## Special Cases
#### 1.Handling Capitalization: 
The algorithm treats words case-insensitively. For example, "White" and "white" are processed the same way, as the stemming logic doesn't depend on case.
#### 2.Punctuation Removal:
The `removePunctuation` function iterates through the input string and retains only valid alphabetic characters, dots(if not at the end). This cleaning ensures that punctuation doesn't interfere with the stemming process.
#### 3.CVC Pattern:
The `cvc` function identifies patterns that fit the consonant-vowel-consonant structure, which is critical for rules that alter the ending of words based on this pattern (e.g., "hop" can become "hop", but "hopped" might trigger a transformation).
## Additional Considerations:
1.The algorithm employs a series of steps(1a, 1b, 1c, etc.) to handle different classes of suffixes systematically, allowing for extensively coverage of English morphology.
2.Special characters and word endings are managed carefully, with specific checks to prevent inappropriate transformations (e.g., ensuring that "running" doesn't become "run").

Here’s a summary of all the morphological rules applied to word processing in the provided stemming algorithm:

### 1. Vowel and Consonant Identification
- **Vowel/Consonant Check**: The `isConsonant` function determines whether a character is a consonant, with special handling for 'y' based on its position.

### 2. Measure Calculation
- **Measure Calculation**: The `getMeasure` function counts the number of vowel-consonant (VC) sequences in the word to assess its structure.

### 3. Suffix Handling
- **Suffix Matching**: The `ends` function checks if the word ends with specific suffixes and applies transformations accordingly.
- **Suffix Replacement**: The `setTo` function is used to replace matched suffixes with new ones.

### 4. Specific Rules
#### Step 1: Basic Suffix Processing
- **1a**: Remove "sses", convert "ies" to "i", or remove the final "s" if it is not preceded by another "s".
- **1b**: For "eed", remove the final "e" if the measure is greater than 0.
- **1c**: Change "y" to "i" if it is at the end and there is at least one vowel in the stem.

#### Step 2: Complex Suffix Processing
- Handles suffixes like "ational", "tional", "enci", "anci", replacing them with simpler forms like "ate" or "ence".

#### Step 3: Suffix Removal
- Remove suffixes such as "icate", "ative", "alize".

#### Step 4: Final Checks
- Checks the final character for "e" and "l", removing them based on specific conditions.

### 5. CVC Pattern Check
- **CVC Check**: The `cvc` function identifies if the last three characters form a consonant-vowel-consonant pattern, applying certain rules based on this structure.

### 6. Punctuation Handling
- **Punctuation Removal**: The `removePunctuation` function strips out punctuation, retaining only valid alphabetic characters, periods (if not at the end), and apostrophes (if not at the end).

### 7. Case Insensitivity
- The algorithm treats words like "White" and "white" uniformly, processing them without regard to case.

These rules collectively enable the algorithm to effectively stem English words, addressing a variety of morphological variations and special cases.