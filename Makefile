###############################################################################
# Makefile 
# Jared Spaulding
# COSC 3750, SP 2020
# Homework 6
# Mar. 13, 2020
# 
# Makefile to complile wytar
###############################################################################

CC=gcc
CFLAGS=-ggdb -Wall
RM=/bin/rm -f

.PHONY: tidy clean

wytar: wytar.c record.o archive.o extract.o
	$(CC) $(CFLAGS) $? -o wytar -I $(CURDIR)

record: record.c record.h
	$(CC) -c $(CFLAGS) $<

archive: archive.c archive.h
	$(CC) -c $(CFLAGS) $<

extract: extract.c extract.h
	$(CC) -c $(CFLAGS) $<

tidy:
	$(RM) *.o

clean: tidy
	$(RM) wytar
	$(RM) core.*