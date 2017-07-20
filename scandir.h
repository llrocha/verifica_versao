#ifndef _SCANDIR_H_
#define _SCANDIR_H_

#ifdef WINDOWS
#include <windows.h>
#else
#include <dirent.h>
#endif

#include <string.h>

#ifdef WINDOWS
int createfilelist(char *dir, char *filter, char **list);
int findfiles (char *dirname, HANDLE * hSearch);
#else
int findfiles (char *dirname, struct dirent ***namelist);
#endif

#endif