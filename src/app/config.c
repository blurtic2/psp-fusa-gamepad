/*
 * Minimal INI reader for config.ini.
 * Behaves like minIni's ini_gets() for what FuSa GamePad needs:
 * case-insensitive section/key names, "key=value" or "key:value",
 * surrounding whitespace and optional quotes stripped, ';' / '#' comments.
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "config.h"

static char *trim(char *s)
{
	char *e;
	while (*s && isspace((unsigned char)*s)) s++;
	e = s + strlen(s);
	while (e > s && isspace((unsigned char)e[-1])) *--e = '\0';
	return s;
}

static int streq_nocase(const char *a, const char *b)
{
	while (*a && *b) {
		if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
		a++;
		b++;
	}
	return *a == *b;
}

static void copy_value(char *dst, const char *src, int size)
{
	strncpy(dst, src, size - 1);
	dst[size - 1] = '\0';
}

int ini_gets(const char *section, const char *key, const char *def,
             char *buffer, int size, const char *filename)
{
	char line[256];
	int in_section = 0;
	FILE *fp;

	copy_value(buffer, def, size);

	fp = fopen(filename, "r");
	if (!fp) return 0;

	while (fgets(line, sizeof(line), fp)) {
		char *p = trim(line);
		char *sep, *val;
		int len;

		if (*p == '\0' || *p == ';' || *p == '#') continue;

		if (*p == '[') {
			char *end = strchr(p, ']');
			if (!end) continue;
			*end = '\0';
			in_section = streq_nocase(trim(p + 1), section);
			continue;
		}
		if (!in_section) continue;

		sep = strpbrk(p, "=:");
		if (!sep) continue;
		*sep = '\0';
		if (!streq_nocase(trim(p), key)) continue;

		val = sep + 1;
		sep = strpbrk(val, ";#");
		if (sep) *sep = '\0';
		val = trim(val);
		len = strlen(val);
		if (len >= 2 && val[0] == '"' && val[len - 1] == '"') {
			val[len - 1] = '\0';
			val++;
		}
		copy_value(buffer, val, size);
		fclose(fp);
		return strlen(buffer);
	}

	fclose(fp);
	return 0;
}
