/*
 * record.c
 * Author: Jared Spaulding
 * Date: Mar 13, 2020
 *
 * COSC 3750, Homework 5
 *
 * Source code for everything dealing with records.
 *
 */

#include "record.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct record *createRecord() {
  struct record *r = (struct record*) malloc(sizeof (struct record));
  return r;
}