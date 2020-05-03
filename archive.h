/*
 * archive.h
 * Author: Jared Spaulding
 * Date: Mar 13, 2020
 *
 * COSC 3750, Homework 5
 *
 * Header file for archiving.
 *
 */

#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "record.h"
#include <sys/stat.h>

extern struct record *addContent(char*);
extern char *getCurpath(void);
extern char *getOctalAsChar(char *, int);
extern void writeContents(struct record*, char*, char*);
extern char *getHeader(struct record*);
extern void assignString(char*, char*);
extern void assignMode(struct stat, char*);
extern char *octalConv(int);
extern void calculateChksum(struct record *r);
extern void writeTermBytes(char*);

#endif