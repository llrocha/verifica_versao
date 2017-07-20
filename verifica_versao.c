#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "dirent.h"
#include "strutils.h"
#include "scandir.h"
#include "md5sum.h"
#include "cgic/cgic.h"


/* Change this if the SERVER_NAME environment variable does not report
	the true name of your web server. */
#if 1
#define SERVER_NAME cgiVerificaVersao
#endif



int isFile(const char* name)
{
    DIR* directory = opendir(name);

    if(directory != NULL)
    {
     closedir(directory);
     return 0;
    }

    if(errno == ENOTDIR)
    {
     return 1;
    }

    return -1;
}

char versao(char *filename)
{
  FILE * fp;
  int count = 0;
  char byte, versao = '0';

  fp = fopen (filename, "rb");
  if(fp != NULL){ 
    do {
      byte = fgetc(fp);
      if(byte == '{')
          count++;
      else if(byte == '@' && count > 0)
          count++;
      else if(count == 4)
          count++;
      else if(count == 5) {
          versao = byte;
          break;
      } else
          count = 0;

    } while(!feof(fp));
    
    fclose(fp);
  }

  return versao;
}


int cgiMain() {

  struct _stat buf;
  int result;
  char ctime_buffer [20];
  char mtime_buffer [20];
  struct tm * timeinfo;
  /*scandir*/
  char* filename;
  char *lista;
  char **filelist;
  int i, n;

  char path[256];
  char mask[80];

  if (cgiFormSubmitClicked("d") == cgiFormSuccess)
    cgiFormStringNoNewlines("d", path, 255);
  else
    strcpy(path, ".");

  if (cgiFormSubmitClicked("f") == cgiFormSuccess)
    cgiFormStringNoNewlines("f", mask, 79);
  else
    strcpy(mask, "*");

  cgiHeaderContentType("text/plain");
  
  n = createfilelist(path, mask, &lista);
  parseargs(&n, &filelist, lista, ";");
  for(i=0;i<n;i++) {
    filename = filelist[i];

    result = _stat(filename, &buf);
    // Check if statistics are valid:
    if(result != 0)
    {

      perror( "Problem getting information" );
      switch (errno)
      {
        case ENOENT:
          fprintf(cgiOut, "File %s not found.\n", filename);
          break;
        case EINVAL:
          fprintf(cgiOut, "Invalid parameter to _stat.\n");
          break;
        default:
          /* Should never be reached. */
          fprintf(cgiOut, "Unexpected error in _stat.\n");
      }
    }
    else
    {
      timeinfo = localtime (&buf.st_ctime);
      strftime (ctime_buffer, 20, "%Y-%m-%d.%H:%M:%S",timeinfo);

      timeinfo = localtime (&buf.st_mtime);
      strftime (mtime_buffer, 20, "%Y-%m-%d.%H:%M:%S",timeinfo);

      if(isFile(filename)) {

        fprintf(cgiOut, "%c\t%s\t%c.%04o.%04o.%04o.%s.%s\t%s\n",
          versao(filename),
          MDFile(filename),
          'f',
          buf.st_uid & 07777,
          buf.st_gid & 07777,
          buf.st_mode & 07777,
          mtime_buffer,
          ctime_buffer,
          filename
        );

      } else {

        fprintf(cgiOut, "0\t%032d\t%c.%04o.%04o.%04o.%s.%s\t%s\n",
          0,
          'd',
          buf.st_uid & 07777,
          buf.st_gid & 07777,
          buf.st_mode & 07777,
          mtime_buffer,
          ctime_buffer,
          filename
        );

      }

    }
  }
  freeparseargs(n, filelist);
  free(lista);

  return 0;
}
