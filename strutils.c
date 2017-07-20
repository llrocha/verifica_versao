/**
 * Rotinas de manipulacao de string
 * strutils.h
 */

#include "strutils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*******************************/
/* LEFT TRIM                   */
/*******************************/

char *
ltrim (char *str)
{
	char *p, *q;

	p = str;
	while (isspace (*p))
		p++;

	q = str;
	while (*p) {
		*q = *p;
		p++;
		q++;
	}
	*q = '\0';

	return str;
}

/*******************************/
/* RIGHT TRIM                  */
/*******************************/

char *
rtrim (char *str)
{
	char *p;

	p = str;
	while (*p)
		p++;

	p--;
	while (isspace (*p))
		*(p--) = 0;

	return str;
}

/*******************************/
/* ALL TRIM                    */
/*******************************/

char *
trim (char *str)
{
	return ltrim (rtrim (str));
}

/*******************************/
/* UPPER CASE STRING           */
/*******************************/

char *
upper (char *str)
{
	char *p;

	for (p = str; *p; p++)
		*p = toupper (*p);

	return str;
}

/*******************************/
/* PARSE ARGUMENTS FROM ARRAY  */
/*******************************/

int
parseargs (int *rargc, char ***rargv, char *str, char *sep)
{
	char *tok;
	char *item;
	char **p;
	int i = 0;

	*rargv = NULL;
	if (str) {
		tok = strtok (str, sep);
		while (tok) {
			*rargv = (char **)realloc ((void *) *rargv, sizeof (char *) * (i + 1));
			p = *rargv + i;
			item = (char *)malloc (strlen (tok) + 1);
			strcpy (item, trim (tok));
			*p = item;
			tok = strtok (NULL, sep);
			i++;
		}
	}
	*rargc = i;

	return 0;
}

/*******************************/
/* FREE ARGUMENTS MEMORY       */
/*******************************/

int
freeparseargs (int argc, char **argv)
{
	char **p = argv;
	int i;

	for (i = 0; i < argc; i++) {
		free (*p);
		p++;
	}
	free (argv);

	return 0;
}

/*******************************/
/* INT TO ASCII WITH SIX POS   */
/*******************************/

void
itoa6 (int no, char *NO)
{
	int i, aux;

	for (i = 0; i < 6; i++) {
		aux = (int) (no / (int) pow (10.0, 5 - i));
		NO[i] = '0' + aux;
		no -= (int) (aux * (int) pow (10.0, 5 - i));
	}

	NO[6] = '\0';
}

/*******************************/
/* INT TO ASCII                */
/*******************************/
#ifndef WINDOWS
int
itoa (char *buffer, int bsize, int no)
{
  long int i, aux, num;

  memset(buffer, 0, bsize);
  
  num=1;
  aux=10;
  for (i=0; num; i++) {
    num=no/aux;    
    aux *= 10;
  }
  if(i<bsize)
    sprintf(buffer, "%d", no);
  else
    return i;

  return 0;
}
#endif

/*******************************/
/* HEX TO CHAR                 */
/*******************************/

char
x2c (char *what)
{
	register char digit;

	digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A') + 10 : (what[0] - '0'));
	digit *= 16;
	digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A') + 10 : (what[1] - '0'));

	return (digit);
}

/*******************************/
/* UNESCAPE URL                */
/*******************************/

void
unescape_url (char *url)
{
	register int x, y, len;

	len = strlen (url);

	for (x = 0, y = 0; url[y]; ++x, ++y) {
		if (url[y] == '+')
			url[y] = ' ';
		if ((url[x] = url[y]) == '%' && y < len - 2) {
			url[x] = x2c (&url[y + 1]);
			y += 2;
		}
	}

	url[x] = '\0';
}

/*******************************/
/* ESCAPE URL                  */
/*******************************/

void
escape_url(char* to, int tosize, char* from)
{
	int tolen;

	for ( tolen = 0; *from != '\0' && tolen + 4 < tosize; ++from ) {
  	if ( isalnum(*from) || strchr( "/_.-~", *from ) != (char*) 0 ) {
      *to = *from;
      ++to;
      ++tolen;
    } else {
      (void) sprintf( to, "%%%02x", (int) *from & 0xff );
      to += 3;
      tolen += 3;
  	}
  }
    *to = '\0';
}


/*******************************/
/* ASCII TO BASE 64 (UUENCODE) */
/*******************************/

char t_64[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

/*********************************************************/
/* convert a three character string to four char encoded */
/*********************************************************/

int
a_to_b (char *a, int a_len, char *b)
{																/* a[3], b[4] */
	int len;
	int i;
	long b32;
	char c[4];

	/*****************************************/
	/* convert string in "a" to 32 bit value */
	/*****************************************/

	if (a_len > 3)
		a_len = 3;

	if (a_len < 1)
		return (0);

	b32 = 0;

	for (i = 0; i < a_len; ++i) {
		b32 <<= 8;
		b32 |= (a[i] & 0xff);
	}

	while (i < 3) {
		b32 <<= 8;
		++i;
	}

	/****************************************/
	/* fill in "c" with 6 bit chunks of "a" */
	/****************************************/

	c[3] = b32 & 0x3f;
	b32 >>= 6;
	c[2] = b32 & 0x3f;
	b32 >>= 6;
	c[1] = b32 & 0x3f;
	b32 >>= 6;
	c[0] = b32 & 0x3f;


	/********************************************/
	/* fill in "b" with encoded values from "c" */
	/********************************************/

	len = 2;

	b[0] = t_64[c[0]];						/* one character encoding format */
	b[1] = t_64[c[1]];
	b[2] = '=';
	b[3] = '=';
	if (a_len > 1) {							/* update to two character format */
		b[2] = t_64[c[2]];
		++len;
	}
	if (a_len > 2) {							/* update to three character format */
		b[3] = t_64[c[3]];
		++len;
	}
	return (len);
}

/******************************************/
/* endcode a string in base 64 (UUENCODE) */
/******************************************/

int
ascii_to_base64 (char *asc, int asc_len, char *b64)
{
	int i;
	int len;
	int b_len;
	char b[4];

	b_len = 0;

	for (i = 0; i < asc_len; i += 3) {
		len = a_to_b (&asc[i], asc_len - i, b);
		if (len > 0) {
			memcpy (&b64[b_len], b, 4);
			b_len += 4;
		}
		if (len < 4)
			break;
	}
	return (len);
}

/*******************************/
/* REPLACE                     */
/*******************************/

int
replace(char _old, char _new, char *_buffer)
{
  char *p = _buffer;

  while(*p) {
    if(*p == _old)
      *p = _new;
    p++;
  }

  return 0;
}

/*******************************/
/* REMOVE_CHAR                 */
/*******************************/

int
remove_char(char c, char *buffer)
{
  char *p, *q, *b;

  p = q = buffer;

  while(*p) {
    if(*p == c){      
      b = q = p;
      while(*q) {
        *b++ = *++q;
      }      
    }
    p++;
  }

  return 0;
}

/*******************************/
/* CONVERT DATE                */
/* DE DDMMAAAA                 */
/* PARA AAAA-MM-DD             */
/*******************************/
void
convert_dateAAAAMMDD(char *in, char *out)
{
  char *p;
  char buffer[11];

  p = in + 4;
  buffer[0] = *p++;
  buffer[1] = *p++;
  buffer[2] = *p++;
  buffer[3] = *p++;
  buffer[4] = '-';
  p = in + 2;
  buffer[5] = *p++;
  buffer[6] = *p++;
  buffer[7] = '-';
  p = in;
  buffer[8] = *p++;
  buffer[9] = *p++;
  buffer[10] = '\0';

  strncpy(out, buffer, 10);
}

/*******************************/
/* CONVERT DATE                */
/* DE DDMMAAAA                 */
/* PARA DD/MM/AAAA             */
/*******************************/
void
convert_dateDDMMAAAA(char *in, char *out)
{
  char *p;
  char buffer[11];

  p = in;
  buffer[0] = *p++;
  buffer[1] = *p++;
  buffer[2] = '/';
  p = in + 2;
  buffer[3] = *p++;
  buffer[4] = *p++;
  buffer[5] = '/';
  p = in + 4;
  buffer[6] = *p++;
  buffer[7] = *p++;
  buffer[8] = *p++;
  buffer[9] = *p++;
  buffer[10] = '\0';

  strncpy(out, buffer, 10);
}

#ifdef STRUTILS_AS_EXE
int 
main (int argc, char *argv[])
{
		
  return 0;
}
#endif
