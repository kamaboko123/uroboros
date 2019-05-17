#ifndef STDLIBC_H
#define STDLIBC_H

#include "arg.h"

#define TRUE 1
#define FALSE 0

#define RAND_INIT 99
#define _UINT_MAX 4294967295

//stdlib.c
extern unsigned int __last_rand;
unsigned int sprintf(char *s, char *format, ...);
unsigned int to_dec_asc(char *buf, int n);
unsigned int to_hex_asc(char *buf, int n, int capital);
unsigned int ndigit(unsigned int n, unsigned int base);
unsigned int upow(unsigned int x, unsigned int n);
int iscapital(char c);
int atoi(char *s);
int isdigit(char c);
char *memcpy(char *buf1, char *buf2, int n);
int memset(char *buf, char byte, int n);
int strcmp(char *s1, char *s2);
int strncmp(char *s1, char *s2, unsigned int n);
int strlen(char *str);
int strtol(char *s, char **endp, int base);

void rand_seed(unsigned int x);
unsigned int rand();

unsigned int roundup(unsigned int x, unsigned int y);

#endif
