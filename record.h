/*
 * record.h
 * Author: Jared Spaulding
 * Date: Mar 13, 2020
 *
 * COSC 3750, Homework 5
 *
 * Header file for records
 *
 */

#ifndef RECORD_H
#define RECORD_H

struct record {
  char name[100];
  char mode[8];
  char uid[8];
  char gid[8];
  char size[12];
  char mtime[12];
  char chksum[12];
  char typeflag;
  char linkname[100];
  char magic[6];
  char version[2];
  char uname[32];
  char gname[32];
  char devmajor[8];
  char devminor[8];
  char prefix[155];
  char pad[12];
};

extern struct record *createRecord(void);

#endif