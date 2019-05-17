#ifndef STDLIBC_H
#define STDLIBC_H

#include "stdint.h"
#include "stdbool.h"
#include "arg.h"

#define RAND_INIT 99
#define _UINT_MAX 4294967295

//stdlib.c
extern uint32_t __last_rand;
uint32_t sprintf(char *s, char *format, ...);
uint32_t to_dec_asc(char *buf, int n);
uint32_t to_hex_asc(char *buf, int n, bool capital);
uint32_t ndigit(uint32_t n, uint32_t base);
uint32_t upow(uint32_t x, uint32_t n);
bool iscapital(char c);
int atoi(char *s);
bool isdigit(char c);
char *memcpy(char *buf1, char *buf2, int n);
int memset(char *buf, char byte, int n);
int strcmp(char *s1, char *s2);
int strncmp(char *s1, char *s2, uint32_t n);
int strlen(char *str);
int strtol(char *s, char **endp, int base);

void rand_seed(uint32_t x);
uint32_t rand();

uint32_t roundup(uint32_t x, uint32_t y);

#endif
