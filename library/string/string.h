#ifndef STRING_H
#define STRING_H

#define STRING_BUFFER_SIZE 32

typedef struct string {
	char* string;
	int freeIndex;
	int currentBufferSize;
} string;

char* string_strsep(char** stringp, char* delim);
void string_shiftString(char* stringToShift, char* shiftPoint);

void string_initialise(struct string* s);
void string_set(struct string* s, char* newString);
void string_appendStrings(struct string* s1, struct string* s2);
void string_appendString(struct string* s1, char* s2);
void string_appendChar(struct string* s, char c);
void string_setCharAt(struct string* s, char c, int index);
void string_insertCharAt(struct string* s, char c, int index);
void string_insertStringsAt(struct string* s1, struct string* s2, int index);
void string_insertStringAt(struct string* s1, char* s2, int index);
void string_duplicate(struct string* source, struct string* destination);
char* string_copyToBuffer(struct string* source);
void string_erase(struct string* s, int index, int characters);
void string_free(struct string* s1);

#endif
