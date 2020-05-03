/*
 * archive.c
 * Author: Jared Spaulding
 * Date: Mar 13, 2020
 *
 * COSC 3750, Homework 5
 *
 * Source code for archiving.
 *
 */

#include "archive.h"
#include "record.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <tar.h>
#include <pwd.h>
#include <grp.h>

char * getCurPath() {
  size_t size;
  char *buf;
  char *curPath;

  size = pathconf(".", _PC_PATH_MAX);

  if ((buf = (char *)malloc(size)) != NULL)
    curPath = getcwd(buf, size);

  curPath = strcat(curPath, "/");

  return curPath;
}

char *getOctalAsChar(char *out, int macro) {
  int re = sprintf(out, "%o", TUREAD);
  if (re < 0) {
    perror("wytar");
    return "00000";
  } else
    return out;
}

void assignString(char* str, char *s) {
  int i;
  for (i=0; i<strlen(str)+1; i++) {
    if (str[i] == '\0') {
      s[i] = '\0';
      break;
    }
    else {
      sprintf(&s[i], "%d", (int)str[i]);
    }
  }
}

char *octalConv(int macro) {
  size_t size = 8;
  char *str = malloc(sizeof(char)*size);

  sprintf(str, "%1o", macro);

  return str;
}

void assignMode(struct stat sb, char *s) {
  int perm = 0;
  int other = 0;

  // Owner permissions
  if (sb.st_mode & S_IRUSR)
    perm += TUREAD;
  if (sb.st_mode & S_IWUSR)
    perm += TUWRITE;
  if (sb.st_mode & S_IXUSR)
    perm += TUEXEC;

  // Group permissions
  if (sb.st_mode & S_IRGRP) 
    perm += TGREAD;
  if (sb.st_mode & S_IWGRP)
    perm += TGWRITE;
  if (sb.st_mode & S_IXGRP)
    perm += TGEXEC;

  // Others permissions
  if (sb.st_mode & S_IROTH) 
    perm += TOREAD;
  if (sb.st_mode & S_IWOTH)
    perm += TOWRITE;
  if (sb.st_mode & S_IXOTH)
    perm += TOEXEC;

  // Set Uid, Set Gid, and Set Text
  other += TSUID;
  other += TSGID;
  other += S_ISVTX;

  strcpy(s, octalConv(perm));
  strcat(s, octalConv(other));
  strcat(s, "\0");
}

void calculateChksum(struct record *r) {
  unsigned int sum = 0;
  memset(r->chksum, ' ', sizeof(r->chksum));

  sum += strlen(r->name);
  sum += strlen(r->mode);
  sum += strlen(r->uid);
  sum += strlen(r->gid);
  sum += strlen(r->size);
  sum += strlen(r->mtime);
  sum += strlen(r->chksum);
  sum += 1; //typeflag
  sum += strlen(r->linkname);
  sum += strlen(r->magic);
  sum += strlen(r->version);
  sum += strlen(r->uname);
  sum += strlen(r->gname);
  sum += strlen(r->devmajor);
  sum += strlen(r->devminor);
  sum += strlen(r->prefix);
  sum += strlen(r->pad);

  memset(r->chksum, '\0', sizeof(r->chksum));
  strcpy(r->chksum, strcat(strcat(octalConv(sum), "\0"), " "));
}

struct record *addContent(char *filename) {
  struct record *r = createRecord();

  struct stat sb;

  int st = stat(filename, &sb);
  struct passwd *pas = getpwuid(sb.st_uid);
  struct group *gr = getgrgid(sb.st_gid);

  if (st == 0) { 
    // Name and prefix
    memset(r->name, '\0', sizeof(r->name));
    memset(r->prefix, '\0', sizeof(r->prefix));
    char *name = strcat(getCurPath(), filename);
    if (strlen(name) < 100) {
      assignString(name, r->name);
      memset(r->prefix, '\0', sizeof(r->prefix));
    }
    else {
      if (strlen(name) > 255) {
        printf("%s\n", "wytar: filename path is too long (255 char max)");
        return NULL;
      } else {
        if (strlen(filename) < 100) {
          assignString(filename, r->name);
          assignString(getCurPath(), r->prefix);
        } else {
          printf("%s\n", "wytar: filename is too long (100 char max)");
          return NULL;
        }
      }
    }

    // Mode
    assignMode(sb, r->mode);

    // Uid
    memset(r->uid, '\0', sizeof(r->uid));
    strcpy(r->uid, octalConv(sb.st_uid));

    // Uid
    memset(r->gid, '\0', sizeof(r->gid));
    strcpy(r->gid, octalConv(sb.st_gid));

    // Size, Typeflag, and Linkname
    memset(r->size, '\0', sizeof(r->size));
    if ((sb.st_mode & S_IFMT) == S_IFLNK) {
      memset(r->size, '\0', sizeof(r->size));
      r->typeflag = SYMTYPE;
      if (strlen(strcat(name, "\0")) < 100)
        strcpy(r->linkname,strcat(name, "\0"));
    } else if ((sb.st_mode & S_IFMT) == S_IFDIR)
      r->typeflag = DIRTYPE;
    else {
      sprintf(r->size, "%ld", sb.st_size);
      r->typeflag = REGTYPE;
      memset(r->linkname, '\0', sizeof(r->linkname));
    }
    strcat(r->size, "\0");

    // Mtime
    sprintf(r->mtime, "%lo", sb.st_mtime);
    strcpy(r->mtime, "\0");

    // Magic
    memset(r->magic, '\0', sizeof(r->magic));
    strcpy(r->magic, strcat(TMAGIC, "\0"));

    // Version
    r->version[0] = (int)48;
    r->version[1] = (int)48;

    // Uname
    memset(r->uname, '\0', sizeof(r->uname));
    if (strlen(pas->pw_name) < 32) {
      assignString(pas->pw_name, r->uname);
    } 

    // Gname
    memset(r->gname, '\0', sizeof(r->gname));
    if (strlen(gr->gr_name) < 32) {
      assignString(gr->gr_name, r->gname);
    } 

    // Devmajor
    memset(r->devminor, '\0', sizeof(r->devminor));

    // Devminor
    memset(r->devmajor, '\0', sizeof(r->devmajor));

     // Pad
    memset(r->pad, '\0', sizeof(r->pad));

    // Chksum
    calculateChksum(r);

  } else {
    if (errno == EACCES || errno == EBADF || errno == EFAULT || 
      errno == ELOOP || errno == ENAMETOOLONG || errno == ENOENT ||
      errno == ENOMEM || errno == ENOTDIR || errno == EOVERFLOW) {
      perror("wytar: Couldn't open the file");
    }
  }
  return r;
}

void writeContents(struct record *r, char *filename, char *outFile) {
  FILE *fp, *fp2;
  fp = fopen(filename, "r");
  fp2 = fopen(outFile, "a");
  char buffer[512];

  if (fp != NULL) {
    // Get num bytes of file
    fseek(fp, 0L, SEEK_END);
    long numbytes = ftell(fp);

    // reset file indicator to start of file
    fseek(fp, 0L, SEEK_SET);

    // write header, had trouble writing as a 512byte block
    // didnt process null bytes
    const size_t name = fwrite(r->name, sizeof(r->name),1, fp2);
    if (name == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t mode = fwrite(r->mode, sizeof(r->mode),1 , fp2);
    if (mode == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t uid = fwrite(r->uid, sizeof(r->uid), 1, fp2);
    if (uid == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t gid = fwrite(r->gid, sizeof(r->gid),1 , fp2);
    if (gid == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t size = fwrite(r->size, sizeof(r->size),1 , fp2);
    if (size == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t mtime = fwrite(r->mtime, sizeof(r->mtime),1 , fp2);
    if (mtime == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t chksum = fwrite(r->chksum, sizeof(r->chksum),1 , fp2);
    if (chksum == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t typeflag = fwrite(&r->typeflag, sizeof(&r->typeflag), 1, fp2);
    if (typeflag == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t linkname = fwrite(r->linkname, sizeof(r->linkname),1 , fp2);
    if (linkname == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t magic = fwrite(r->magic, sizeof(r->magic),1 , fp2);
    if (magic == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t version = fwrite(r->version, sizeof(r->version),1 , fp2);
    if (version == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t uname = fwrite(r->uname, sizeof(r->uname),1 , fp2);
    if (uname == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t gname = fwrite(r->gname, sizeof(r->gname),1 , fp2);
    if (gname == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t devmajor = fwrite(r->devmajor, sizeof(r->devmajor),1 , fp2);
    if (devmajor == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t devminor = fwrite(r->devminor, sizeof(r->devminor),1 , fp2);
    if (devminor == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t prefix = fwrite(r->prefix, sizeof(r->prefix),1 , fp2);
    if (prefix == 0)
        printf("%s\n", "wytar: Header write error");
    const size_t pad = fwrite(r->pad, sizeof(r->pad),1 , fp2);
    if (pad == 0)
        printf("%s\n", "wytar: Header write error");
   
    if (r->typeflag != SYMTYPE && r->typeflag != DIRTYPE) {
      // write content
      if (numbytes / sizeof(buffer) < 1) {
        const size_t bufR = fread(buffer, sizeof(buffer), 1, fp);
        if (bufR == 0) {
          // Ignore this
        }
        const size_t bufW = fwrite(buffer, sizeof(buffer), 1, fp2);
        if (bufW == 0) {
          // Ignore this
        }
      } else {
        int i;
        long size = numbytes / sizeof(buffer);

        for (i=0; i<size+1; i++){
          const size_t bufR2 = fread(buffer, sizeof(buffer), 1, fp);
          if (bufR2 == 0){
            // Ignore this
          }
          const size_t bufW2 = fwrite(buffer, sizeof(buffer), 1, fp2);
          if (bufW2 != 0) {
            // Ignore this
          }
          memset(buffer, '\0', sizeof(buffer));
        }
      }
    }
    fclose(fp);
    fclose(fp2);
  } else {
    perror("wytar");
  }
}

void writeTermBytes(char *outFile) {
  FILE *fp;
  fp = fopen(outFile, "a");
  char buffer[512];
  memset(buffer, '\0', sizeof(buffer));

  if (fp != NULL) {
    const size_t out = fwrite(buffer, sizeof(buffer), 2, fp);
    if (out == 0)
      perror("wytar");
    fclose(fp);
  } else {
    perror("wytar");
  }
}