#ifndef FUSA_CONFIG_H
#define FUSA_CONFIG_H

/* Copies the value of [section] key from filename into buffer, or def if absent.
   Returns the length of the value found, 0 if it fell back to def. */
int ini_gets(const char *section, const char *key, const char *def,
             char *buffer, int size, const char *filename);

#endif
