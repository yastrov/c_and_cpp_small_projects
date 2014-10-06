/*
Program for renaming files at Linux: Upper first character of filename and
each of two characters after '_' symbol.

Created by Yuri Astrov
05-10-2014. (day, month, year)
Based on ftw.h example.
For help, print in terminal:
man ftw
*/
#define _XOPEN_SOURCE 500
#if __x86_64__ || _WIN64
  #include <ftw64.h>
#else
//__i386__ || _M_IX86 || _WIN32
  #include <ftw.h> /* dirent.h is a lats year for Linux:). As and sys/dir.h*/
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h> // for isupper and others.

#define MAX_PATH_SIZE 255
#define PATH_SEPARATOR '/'
#define NULL_STR_END '\0'
#define CONTROL_EXT ".fb2"
#define HOW_MANY_CHAR_AFTER_UNDERL_UPPER 2
char buf[MAX_PATH_SIZE];

/*
First, old version.
index - position of first filename symbol in fpath;
localbuf is a char array, that must be bigger than fpath.
*/
void
create_new_name(const char *fpath, size_t index, char * localbuf)
{
  size_t i = index;
  int counter = 0;
  strcpy(localbuf, fpath);
  localbuf[i] = toupper(localbuf[i]);
  for(; localbuf[i] != NULL_STR_END; i++)
  {
    if((localbuf[i-1] == '_')&&(counter < HOW_MANY_CHAR_AFTER_UNDERL_UPPER))
    {
      localbuf[i] = toupper(localbuf[i]);
      counter++;
    }
  }
}

/*
Actual version, with using pointers.
index - position of first filename symbol in fpath;
localbuf is a char array, that must be bigger than fpath.
*/
void
create_new_name_ptr(const char *fpath, size_t index, char * localbuf)
{
  int counter = 0;
  strcpy(localbuf, fpath);
  char * ptr = localbuf + index;
  *ptr = toupper(*ptr);
  for(; *ptr != NULL_STR_END; ptr++)
  {
    if((*(ptr-1) == '_')&&(counter < HOW_MANY_CHAR_AFTER_UNDERL_UPPER))
    {
      *ptr = toupper(*ptr);
      //*ptr = 'A' - 'a' + *ptr; //This is analog.
      counter++;
    }
  }
}

static int
process_file(const char *fpath, const struct stat *sb,
            int tflag, struct FTW *ftwbuf)
{
  if (tflag == FTW_F)
  {
    printf("-----------\n");
    size_t index;
    char * pch;
    printf("%s\n", fpath);

    pch = strrchr(fpath, PATH_SEPARATOR);
    if (pch == NULL)
      return 0;
    index = pch-fpath+1;
    #ifdef DEBUG
      printf("Last occurence of '/' found at %d \n", index-1);
    #endif
    if (isupper(fpath[index]))
    {
      return 0;
    }
    else if(strstr(fpath, CONTROL_EXT) != NULL)
    {
      create_new_name_ptr(fpath, index, buf);
      if (strcmp (buf, fpath) != 0)
      {
        printf("'%c' is lower, Renaming file to:\n-> %s\n",
                fpath[index], buf);
        if(rename(fpath, buf) != 0)
        {
          perror( strcat("Error renaming file:", fpath));
          return 1;
        }
      }
    }
  }
  return 0;
}

int
main(int argc, char *argv[])
{
  int flags = 0;
  if(argc == 1)
  {
    printf("Uses: %s your_path\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (argc > 2 && strchr(argv[2], 'd') != NULL)
      flags |= FTW_DEPTH;
  if (argc > 2 && strchr(argv[2], 'p') != NULL)
      flags |= FTW_PHYS;

  if (nftw(argv[1], process_file, 20, flags)
                     == -1) {
                 perror("Attention: nftw error");
                 exit(EXIT_FAILURE);
            }
  exit(EXIT_SUCCESS);
}