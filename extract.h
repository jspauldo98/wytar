/*
 * extract.h
 * Author: Jared Spaulding
 * Date: Mar 13, 2020
 *
 * COSC 3750, Homework 5
 *
 * Header file for extraction
 *
 */

#ifndef EXTRACT_H
#define EXTRACT_H

#include "record.h"

extern void createFile(struct record*, char*);
extern void extractContent(char*);

// Cant get the conversion back from decimal right
// extern char *asciiToString(char*);

#endif