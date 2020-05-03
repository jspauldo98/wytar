# wytar

wytar is a utility program that mimics the Linux tar utility. It is designed to archive files. This is a project for Linux Programming (COSC 3750).

<br>

Note - Current version has some issues extracting.

## Prerequisites

GNU make is required.

## Installation

Use the GNU make utility to compile.

```bash
make
```

## Synopsis

```bash
./wytar [OPTION]... [FILE]...
```

## Description

List information about the FILEs (the current directory by default). Listing is automatically formatted as long listing.

### Options

```
-c, create an archive
-x, extract the files from an archive
-f, next argument is the name of the archive to process
```

## Examples
```bash
./wytar -cf foo.tar f g 
    # Archive f an g into foo.tar

./wytar -xf foo.tar 
    # Extract files from foo.tar
```

## Author

Jared Spaulding
