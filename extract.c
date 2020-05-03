/*
 * extract.x
 * Author: Jared Spaulding
 * Date: Mar 13, 2020
 *
 * COSC 3750, Homework 5
 *
 * Source code for extraction
 *
 */

#include "extract.h"
#include "record.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

// Could not get converting back to letters working right

// char *asciiToString(char*s){
//   // Why does this not convert back to letters?
//   char *str = (char*)malloc(sizeof(s));
//   int i;
//   for (i=0; i<strlen(s)-1; i++) {
//     char temp[3];
//     temp[0] = s[i];
//     temp[1] = s[i+1];
//     char test = atoi(temp)+56;  // Decimals are way off?
//     strncat(str, &test, 1);  // Core dump
//   }
//   return str;
// }

void createFile(struct record *r, char *content) {
  FILE *fp;
  // Need to convert back to letters but can't get that to work
  // fp = fopen(asciiToString(r->name), "w");
  fp = fopen(r->name, "w");

  if (fp != NULL) {
    // get size of file from header
    int size;
    sscanf(r->size, "%d", &size);

    // write to new file all content
    const size_t out = fwrite(content, size , 1, fp);
    if (out == 0)
      perror("wytar");
    fclose(fp);
  } else {
    perror("wytar1");
  }

  // Need to use stat to set permissions and such
  // but filename is too long, according to stat.
  // Not being able to convert back has become a major issue!
}

void extractContent(char *filename) {
  FILE *fp;
  fp = fopen(filename, "r");
  int i = 0;

  if (fp != NULL) {
    while (!feof(fp)) {
      // Read header into record
      struct record *r = createRecord();
      const size_t name = fread(r->name, sizeof(r->name),1, fp);
      if (name == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t mode = fread(r->mode, sizeof(r->mode),1 , fp);
      if (mode == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t uid = fread(r->uid, sizeof(r->uid), 1, fp);
      if (uid == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t gid = fread(r->gid, sizeof(r->gid),1 , fp);
      if (gid == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t size = fread(r->size, sizeof(r->size),1 , fp);
      if (size == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t mtime = fread(r->mtime, sizeof(r->mtime),1 , fp);
      if (mtime == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t chksum = fread(r->chksum, sizeof(r->chksum),1 , fp);
      if (chksum == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t typeflag = fread(&r->typeflag, sizeof(&r->typeflag), 1, fp);
      if (typeflag == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t linkname = fread(r->linkname, sizeof(r->linkname),1 , fp);
      if (linkname == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t magic = fread(r->magic, sizeof(r->magic),1 , fp);
      if (magic == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t version = fread(r->version, sizeof(r->version),1 , fp);
      if (version == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t uname = fread(r->uname, sizeof(r->uname),1 , fp);
      if (uname == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t gname = fread(r->gname, sizeof(r->gname),1 , fp);
      if (gname == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t devminor = fread(r->devmajor, sizeof(r->devmajor),1 , fp);
      if (devminor == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t devmajor = fread(r->devminor, sizeof(r->devminor),1 , fp);
      if (devmajor == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t prefix = fread(r->prefix, sizeof(r->prefix),1 , fp);
      if (prefix == 0)
        printf("%s\n", "wytar: Header read error");
      const size_t pad = fread(r->pad, sizeof(r->pad),1 , fp);
      if (pad == 0)
        printf("%s\n", "wytar: Header read error");

      int fsize;
      sscanf(r->size, "%d", &fsize);

      char buffer[512];
      char c[fsize];
      // check if buffer is big enough
      if (fsize / sizeof buffer < 1){
        const size_t buf = fread(buffer, 1, sizeof buffer, fp);
        if (buf == 0){
          // Ignore this
        }
        strcpy(c, buffer);
      } else {
        int j;
        long size2 = fsize / sizeof buffer;

        for (j = 0; j < size2 +1; j++) {
          const size_t buf2 = fread(buffer, 1, sizeof buffer, fp);
          if (buf2 == 0) {
            // Ignore this
          }
          if (j==0)
            strcpy(c, buffer);
          else
            strcat(c, buffer);
        }
      }
      if (strcmp(r->name, "") != 0) {
        createFile(r,c);
      }
      i++;
    }

    fclose(fp);
  } else 
    perror("wytar");
}