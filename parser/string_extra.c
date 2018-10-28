/*
 *  OBJ Loader from taylormt@kixor.net
 *  Original source files can be found here:
 *      http://kixor.net/dev/objloader/
 *
 * Some modifications has been made by the ACU team at EPITA, 2018
 */
#include "string_extra.h"
#include <string.h>

char strequal(const char *s1, const char *s2)
{
	if (strcmp(s1, s2) == 0)
		return 1;
	return 0;
}

char contains(const char *haystack, const char *needle)
{
	if (strstr(haystack, needle) == NULL)
		return 0;
	return 1;
}
