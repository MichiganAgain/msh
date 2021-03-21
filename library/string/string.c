#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

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
	to the left or the right.  The null character is also shifted.
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

void string_initialise(struct string* s) {
	assert(STRING_BUFFER_SIZE > 0);
	s->currentBufferSize = STRING_BUFFER_SIZE;
	s->freeIndex = 0;
	s->string = (char*) malloc(s->currentBufferSize * sizeof(char));
	s->string[0] = '\0';
}

void string_set(struct string* s, char* newString) {
	int newStringLength = strlen(newString);
	if (newStringLength + 1 > s->currentBufferSize) {
		s->currentBufferSize = newStringLength + 1;	// + 1 for null char
		s->string = realloc(s->string, s->currentBufferSize);
	}
	strcpy(s->string, newString);
	s->freeIndex += newStringLength;
	s->string[s->freeIndex] = '\0';
}

void string_appendStrings(struct string* s1, struct string* s2) {
	if (s1->freeIndex + s2->freeIndex + 1 > s1->currentBufferSize) {
		s1->currentBufferSize = s1->freeIndex + s2->freeIndex + 1;
		s1->string = realloc(s1->string, s1->currentBufferSize);
	}
	strcpy(s1->string + s1->freeIndex, s2->string);
	s1->freeIndex += s2->freeIndex;
}

void string_appendString(struct string* s1, char* s2) {
	int newStringLength = strlen(s2);
	if (s1->freeIndex + newStringLength + 1 > s1->currentBufferSize) {
		s1->currentBufferSize = s1->freeIndex + newStringLength + 1;
		s1->string = realloc(s1->string, s1->currentBufferSize);
	}
	strcpy(s1->string + s1->freeIndex, s2);
	s1->freeIndex += newStringLength;
}

void string_appendChar(struct string* s, char c) {
	if (s->freeIndex + 2 > s->currentBufferSize) {
		s->currentBufferSize *= 2;
		s->string = realloc(s->string, s->currentBufferSize);
	}
	s->string[s->freeIndex++] = c;
	s->string[s->freeIndex] = '\0';
}

void string_setCharAt(struct string* s, char c, int index) {
	if (index < 0 || index >= s->freeIndex) return;	// do not allow negative access or access to null char onwards
	s->string[index] = c;
}

void string_insertCharAt(struct string* s, char c, int index) {
	if (index < 0 || index > s->freeIndex) return;
	if (s->freeIndex + 2 > s->currentBufferSize) {
		s->currentBufferSize *= 2;
		s->string = realloc(s->string, s->currentBufferSize);
	}
	string_shiftString(&(s->string[index]), &(s->string[index + 1]));
	s->string[index] = c;
	s->freeIndex++;
	s->string[s->freeIndex] = '\0';
}

void string_insertStringsAt(struct string* s1, struct string* s2, int index) {
	if (index < 0 || index > s1->freeIndex) return;

	int newStringLength = strlen(s2->string);
	if (s1->freeIndex + s2->freeIndex + 1 > s1->currentBufferSize) {
		s1->currentBufferSize = s1->freeIndex + s2->freeIndex + 1;
		s1->string = realloc(s1->string, s1->currentBufferSize);
	}
	string_shiftString(&(s1->string[index]), &(s1->string[index + newStringLength]));
	strncpy(&(s1->string[index]), s2->string, newStringLength);
	s1->freeIndex += newStringLength;
	s1->string[s1->freeIndex] = '\0';
}

void string_insertStringAt(struct string* s1, char* s2, int index) {
	if (index < 0 || index > s1->freeIndex) return;

	int newStringLength = strlen(s2);
	if (s1->freeIndex + newStringLength + 1 > s1->currentBufferSize) {
		s1->currentBufferSize = s1->freeIndex + newStringLength + 1;
		s1->string = realloc(s1->string, s1->currentBufferSize);
	}
	string_shiftString(&(s1->string[index]), &(s1->string[index + newStringLength]));
	strncpy(&(s1->string[index]), s2, newStringLength);
	s1->freeIndex += newStringLength;
	s1->string[s1->freeIndex] = '\0';
}

void string_duplicate(struct string* source, struct string* destination) {
	if (destination->currentBufferSize < source->freeIndex + 1) {
		destination->currentBufferSize = source->freeIndex + 1;
		destination->string = realloc(destination->string, destination->currentBufferSize);
	}
	strcpy(destination->string, source->string);
	destination->freeIndex = source->freeIndex;
}

char* string_copyToBuffer(struct string* source) {
	char* buff = (char*) malloc(source->freeIndex * sizeof(char));
	strcpy(buff, source->string);
	return buff;
}

void string_erase(struct string* s, int index, int characters) {
	if (index < 0 || index >= s->freeIndex || characters <= 0) return;

	characters = (characters > s->freeIndex - index) ? s->freeIndex - index: characters;
	string_shiftString(&(s->string[index + characters]), &(s->string[index]));
	s->freeIndex -= characters;
}

void string_free(struct string* s1) {
	free(s1->string);
}






