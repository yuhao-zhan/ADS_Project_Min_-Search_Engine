# Global Variables
b:Pointer to the input string.
k, k0, j: Integers for positions in the string.
# Function Analysis
## 1. isConsonant(int index)
 _**Time Complexity:**_ O(1) since it involves a fixed number of comparisons.
 _**Space Complexity:**_ O(1) as it uses a constant amount of space.

## 2.getMeasure()
_**Time Complexity:**_ O(n) where n is the length of the substring from k0 to j due to the while loops that traverse the string.
_**Space Complexity:**_ O(1) since it uses constant amount of additional space.

## 3.vowelInStem()
_**Time Complexity:**_ O(n) where n is the length from k0 to j because it iterates through the string.
_**Space Complexity:** _ O(1) since it only uses a few variables.

## 4.isDoubleConsonant(int index)
_**Time Complexity:**_ O(1) as it performs a constant number of checks.
_**Space Complexity:**_ O(1) due to the use of a fixed number of variables.

## 5.cvc(int index)
_**Time Complexity:**_ O(1) since it performs a fixed number of checks.
_**Space Complexity:**_ O(1)

## 6.ends(const char *value)
_**Time Complexity:**_ O(m) where m is the length of the value string being checked. This is due to memcmp which can compare up to m characters.
_**Space Complexity:**_ O(1) as it uses a constant amount of additional space.

## 7.setTo(const char *value)
_**Time Complexity:**_ O(m) where m is the length of the new suffix being set(determined by value[0]).
_**Space Complexity:**_ O(1) since it modifies the string in place.

## 8.replace(const char *value)
_**Time Complexity:**_ O(n) due to the call to getMeasure() which is O(n).
_**Space Complexity:**_ O(1)

## 9.step1ab()
_**Time Complexity:**_ O(n) due to calls to ends() and vowelInStem(), each of which can run in O(n).
_**Space Complexity:**_ O(1)

## 10.step1c()
_**Time Complexity:**_ O(n) due to the call to vowelInStem().
_**Space Complexity:**_ O(1)

## 11.step2()
_**Time Complexity:**_ O(n) due to the calls to ends() which can take O(n).
_**Space Complexity:**_ O(1)

## 12.step3()
_**Time Complexity:**_ O(n) due to the calls to ends().
_**Space Complexity:**_ O(1)

## 13.step4()
_**Time Complexity:**_ O(n) due to the calls to ends().
_**Space Complexity:**_ O(1)

## 14.step5()
_**Time Complexity:**_ O(n) because of the calls to getMeasure() and cvc(), both of which can run in O(n).
_**Space Complexity:**_ O(1)

## 15.removePunctuation(char *p, int *start, int *end)
_**Time Complexity:**_ O(n) where n is the length of the input string due to the loop that processes each character.
_**Space Complexity:**_ O(1) as it uses a constant number of variables.

## 16.stem(char *p, int index, int position)
_**Time Complexity:**_ Overall O(n) due to the calls to removePunctuation() and other steps ehich could potentially reach O(n).
_**Space Complexity:**_ O(1)

# Overall Complexity
_**Time Complexity:**_ The overall time complexity is O(n), where n is the length of the input string. This is because the most time-consuming operations in the stemming process involve iterating through the string and checking for conditions.
_**Space Complexity:**_ The overall space complexity the algorithm operates in place and only requires a constant amount of extra space.