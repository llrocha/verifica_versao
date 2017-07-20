#include "scandir.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_FILES 1024

#ifdef WINDOWS
int
createfilelist(char *path, char *mask, char **list)
{
  WIN32_FIND_DATA FindFileData;
  HANDLE hFind;
  int n = 0, size = 1, count = 0;
  char *path_mask = NULL;

  /* ".EXT" */
  size = strlen(path) + strlen(mask) + 2; // 2-> '\\' + '\0' 
  path_mask = (char *)realloc(path_mask, size); 
  strncpy(path_mask, path, size);
  strncat(path_mask, "\\", size - strlen(path_mask));
  strncat(path_mask, mask, size - strlen(path_mask));
  *list = NULL;

  hFind = FindFirstFile ((LPCWSTR)path_mask, &FindFileData);
  if (hFind != INVALID_HANDLE_VALUE) {
  	*list = (char *)realloc ((void *) *list, size);
  	**list = '\0';
  	do {
  		size += 
			strlen(path) + 
			strlen((const char *)FindFileData.cFileName) + 
			2;

  		*list = (char *)realloc ((void *) *list, size);
		strcat(*list, (const char *)path);
		strcat(*list, (const char *)"\\");
  		strcat(*list, (const char *)FindFileData.cFileName);
  		strcat(*list, ";");
  		n++;
  		count++;
  	} while (FindNextFile (hFind, &FindFileData)); 
  	FindClose (hFind);
  } else
  	n = -1;

  free (path_mask);

  return n;
}

#else

int g_filter_size;
char g_filter[32];

int
cppfilter (const struct dirent *dir)
{
	char *s = (char *) dir->d_name;
	int extlen = strlen (s) - g_filter_size;

	if (extlen >= 0) {
		if (strncasecmp (s + extlen, g_filter, g_filter_size) == 0) {
			return 1;
		}
	}
	return 0;
}

int
findfiles (char *dirname, struct dirent ***namelist)
{
	int res; 

	res = scandir (dirname, namelist, cppfilter, alphasort);
	if (res < 0) {
		perror ("findfiles");
		logmsg ("findfiles: %s\n", dirname);
	}

	return res;
}

int
createfilelist(char *dir, char *filter, char **list)
{
  struct dirent **namelist;
  int i, n, size, count = 0;

  *list = NULL;
  size = 1;

  strncpy(g_filter, filter, 32);
  g_filter_size = strlen(g_filter);

  n = findfiles (dir, &namelist);
  *list = realloc ((void *) *list, size);
  **list = '\0';
  for (i = 0; i < n; i++) {
		if (count < MAX_FILES) {
			size += strlen(namelist[i]->d_name) + 1;
			*list = realloc ((void *) *list, size);
			strcat(*list, namelist[i]->d_name);
			strcat(*list, ";");
			count++;
		}
		free (namelist[i]);
	}
	free (namelist);

	return count;
}

#endif

#ifdef SCANDIR_AS_EXE
#include "strutils.h"

int 
main (int argc, char *argv[])
{
  char *lista;
  char **filelist;
  int i, n;

	if (argv[1] == NULL) {
		printf("vc precisa passar o diretorio\n");
		exit(1);
	}

  n = createfilelist(argv[1], "*", &lista);
  printf("%s -> %d\n", lista, n);

  parseargs(&n, &filelist, lista, ";");

  printf("\nArquivos encontrados: %d\n\n", n);

  for(i=0;i<n;i++)
    printf("[%s]\t", filelist[i]);

	printf("\nlimpando lista\n");
  freeparseargs(n, filelist);
  free(lista);

  return 0;
}
#endif
