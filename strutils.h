#ifndef _STRUTILS_H_
#define _STRUTILS_H_

/**
 * Nunix Consultoria Ltda
 * Leonardo Larsen Rocha
 * leonardo@nunix.com.br
 *
 * $Revision: 1.2 $
 *
 * Functions declarations
 */

/*
 * Clear all blank spaces on left side 
 */
char *ltrim (char *str);

/*
 * Clear all blank spaces on right side 
 */
char *rtrim (char *str);

/*
 * Clear all blank spaces on both sides 
 */
char *trim (char *str);

/*
 * Upper Case string 
 */
char *upper (char *str);

/*
 * Parse string with separator and return a list of elements 
 */
int parseargs (int *rargc, char ***rargv, char *str, char *sep);

/*
 * Free allocated memory by parseargs 
 */
int freeparseargs (int argc, char **argv);

/*
 * Unescapes "%"-escaped characters in a query: 
 */
void unescape_url (char *url);

/*
 * Escapes to "%" characters in a query: 
 */
void escape_url(char* to, int tosize, char* url);

/*
 * Convert a number to string with size 6 
 */
void itoa6 (int no, char *NO);

/*
 * Convert char hexadecimal to char 
 */
char x2c (char *what);

/*
 * Convert a three character string to four char encoded 
 */
int a_to_b (char *a, int a_len, char *b);

/*
 * Endcode a string in base 64 (UUENCODE) 
 */
int ascii_to_base64 (char *asc, int asc_len, char *b64);

/*
 * Remove specified character
 */
int
remove_char(char c, char *buffer);

/*
 * Convert date from DDMMAAAA to AAAA-MM-DD
 */
void
convert_dateAAAAMMDD(char *in, char *out);

/*
 * Convert date from DDMMAAAA to DD/MM/AAAA
 */
void
convert_dateDDMMAAAA(char *in, char *out);

#ifndef WINDOWS
/*
 * Convert an INT to ASCII
 */
int itoa (char *buffer, int bsize, int no);
#endif

#endif /*_STRUTILS_H_*/

