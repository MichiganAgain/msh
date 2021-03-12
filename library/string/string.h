#ifndef STRING_H
#define STRING_H

char* string_strsep(char** stringp, char* delim);
void string_shiftString(char* stringToShift, char* shiftPoint);
void string_free(void* data);

#endif
