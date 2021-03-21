#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "string.h"

void test_equivalence(string* s, char* value) {
	printf("\nTesting string has value of: %s\n", value);
	assert(strcmp(s->string, value) == 0);
	printf("PASSED\n");
}

int main() {
	string s1, s2, s3, s4, s5, s6, s7;
	string_initialise(&s1);
	string_initialise(&s2);
	string_initialise(&s3);
	string_initialise(&s4);
	string_initialise(&s5);
	string_initialise(&s6);
	string_initialise(&s7);

	string_set(&s1, "Hello World.");
	test_equivalence(&s1, "Hello World.");

	string_set(&s2, "  This is a second string.");
	test_equivalence(&s2, "  This is a second string.");

	string_appendStrings(&s3, &s1);
	test_equivalence(&s3, "Hello World.");
	string_appendStrings(&s3, &s2);
	test_equivalence(&s3, "Hello World.  This is a second string.");

	string_duplicate(&s3, &s4);
	test_equivalence(&s4, "Hello World.  This is a second string.");

	string_setCharAt(&s4, '@', 5);
	test_equivalence(&s4, "Hello@World.  This is a second string.");

	string_insertStringAt(&s1, "  You are looking good today.", strlen(s1.string));
	test_equivalence(&s1, "Hello World.  You are looking good today.");

	string_set(&s5, "  And it is a good sentence.");
	test_equivalence(&s5, "  And it is a good sentence.");

	string_insertStringsAt(&s2, &s5, strlen(s2.string));
	test_equivalence(&s2, "  This is a second string.  And it is a good sentence.");

	string_set(&s6, "Dover");
	string_set(&s7, "White cliffs of ");
	string_insertStringsAt(&s6, &s7, 0);
	test_equivalence(&s6, "White cliffs of Dover");

	string_erase(&s7, 12, 4);
	test_equivalence(&s7, "White cliffs");

	string_free(&s1);
	string_free(&s2);
	string_free(&s3);
	string_free(&s4);
	string_free(&s5);

	return 0;
}
