#include <stdlib.h>

#include "string.h"


/*
	As the strsep found in the GNU C Library is not standardised
	this string_strsep function was created
*/
char* string_strsep(char** stringp, char* delim) {
	char* originalString = *stringp;

	if (**stringp == '\0') return NULL;
	while (**stringp != '\0') {
		for (int i = 0; delim[i] != '\0'; i++) {
			if (**stringp == delim[i]) {
				**stringp = '\0';
				(*stringp)++;
				return originalString;
			}
		}
		(*stringp)++;
	}

	return originalString;
}

/*
	Used to shift a substring of a string to another point in that string.
	Useful for when a character in the internal buffer is erased or added
	and the buffer at that / to the right of that character needs to be shifted
	to the left or the right.
*/
void string_shiftString(char* stringToShift, char* shiftPoint) {
	int pointerDifference = shiftPoint - stringToShift;
	char c;

	if (pointerDifference == 0) return;
	if (pointerDifference < 0) {	// shifting to the left (string to shift is to the right of the shift point)
		while ((c = *(stringToShift)) != '\0') {
			*(stringToShift + pointerDifference) = c;
			stringToShift++;
		}
		*(stringToShift + pointerDifference) = '\0';
	}
	else { // shifting to the right (string to shift is to the left of the shift point)
		char* endOfStringIndex = stringToShift;
		while (*endOfStringIndex != '\0') endOfStringIndex++;

		while (endOfStringIndex >= stringToShift) {
			c = *endOfStringIndex;
			*(endOfStringIndex + pointerDifference) = c;
			endOfStringIndex--;
		}
	}
}

void string_free(void* data) {
	free(data);
}
