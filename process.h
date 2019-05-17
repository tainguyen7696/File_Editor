/*
* Name: Tai Nguyen, Ryan Stobbe, Branden Wallace
* Account: cssc1218
* Class: CS570, Summer 2018
* Assignment Info: Assignment #2, File Management
* Filename: process.h
*/
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

//for printing the filepath
struct dirent *ent;

void get_file_name(char*);
void read_file_to_file(char*,FILE*);
void read_file_to_string(char*,char *);
void write_file(char*);
int cmp(const void *,const void*);
int cmp_reverse(const void *, const void *);
void flush();
int create_file();
int create_directory();
void dir_listing(char *);
int print_file_status();
void sort_string_contents(char *, int);
void insert_file(char *);
void append_file(char *);
void overwrite_file(char *);
#ifndef process
#define process


FILE *file;
FILE *p_file;
DIR *dir;
char w_str[256];
char s_cpy[256];
char file_name[256];
int c;
#endif
