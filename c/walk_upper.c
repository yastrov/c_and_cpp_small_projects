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
/* Next two include for support large files
http://www.ualberta.ca/dept/chemeng/AIX-43/share/man/info/C/a_doc_lib/aixprggd/genprogc/prg_lrg_files.htm
OR
http://www-01.ibm.com/support/knowledgecenter/ssw_aix_53/com.ibm.aix.genprogc/doc/genprogc/prg_lrg_files.htm%23a288fa5d972soni
*/
#include <sys/types.h>
#include <fcntl.h>
#include <ftw.h> /* dirent.h is a lats year for Linux:). As and sys/dir.h*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // for isupper and others.

#define MAX_PATH_SIZE 255
#define PATH_SEPARATOR '/'
#define NULL_STR_END '\0'
#define CONTROL_EXT ".fb2"
#define HOW_MANY_CHAR_AFTER_UNDERL_UPPER 2
#define DEPTH_FOR_WALKING 20
char buf[MAX_PATH_SIZE];

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
  ptr++;
  for(; *ptr != NULL_STR_END && counter < HOW_MANY_CHAR_AFTER_UNDERL_UPPER; ptr++)
  {
    if( *(ptr-1) == '_')
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
    size_t index;
    char * pch;

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
          return -1;
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

  if (nftw64(argv[1], process_file, DEPTH_FOR_WALKING, flags)
                     == -1) {
                 perror("Attention: nftw error");
                 exit(EXIT_FAILURE);
            }
  exit(EXIT_SUCCESS);
}