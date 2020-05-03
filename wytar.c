/*
 * wytar.c
 * Author: Jared Spaulding
 * Date: Mar 13, 2020
 *
 * COSC 3750, Homework 5
 *
 * This is a simple version of the tar utility. 
 *
 */

#include <stdio.h>
#include <record.h>
#include <extract.h>
#include <archive.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char **argv)
{
  char *filename;
  bool fFlag = false;
  bool cFlag = false;
  bool xFlag = false;

  // Check number of arguments
  if (argc > 1) {
    // Loop through arguments
    int i;
    for (i=0; i<argc; i++) {
      // Check if argument is 3rd argument or less
      if (i < 4 && (!fFlag && (!cFlag || !xFlag))) {
        // Check if argument is an option
        if (argv[i][0] == '-') {
          // Loop through option char by char
          int j = 0;
          for (j=0; j<strlen(argv[i]); j++) {
            switch (argv[i][j]) {
              case 'f':
                if (i + 1 == argc) {
                  printf("%s\n", "wytar: Filename must follow '-f' option");
                  break;
                } else {
                  fFlag = true;
                  filename = argv[i+1];
                }
                break;
              case 'c':
                cFlag = true;
                break;
              case 'x':
                xFlag = true;
                break;
            }
          }
        }
      } else {
        // Check option requirements
        if (!fFlag) {
          printf("%s\n", "wytar: You must specify the '-f' option");
          break;
        }
        if (cFlag && xFlag) {
          printf("%s\n", 
            "wytar: You cannot specify both '-c' and '-x' options");
          break;
        } else if (cFlag) {
          if (argv[i] != filename) {
            // Archive content
            struct record *r = addContent(argv[i]);
            writeContents(r, argv[i], filename);
          }
        } else if (xFlag) {
            // Extract content
            extractContent(filename);
        } else {
          printf("%s\n", 
            "wytar: You must specify one of the '-c' or '-x' options");
          break;
        }
      }
    }
  } else
    printf("%s\n", 
      "wytar: You must specify '-f' and one of the '-c' or '-x' options");
  if (cFlag)
    writeTermBytes(filename);

  return 0;
}