#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h> //For access
#define MAX_PATH_LENGTH 4096 //You may check it with strlen

void listdir(char* dirname, int lvl,
            void (*filevisit)(const char *, int),
            void (*foldervisit)(const char *, int));
            
void filevisitor(const char * path, int lvl)
{
    fprintf(stdout, "%s\n", path);
}

void foldervisitor(const char * path, int lvl)
{
    fprintf(stdout, "Folder visit: %s\n", path);
}

int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Incorrect usage!\n");
    exit(-1);
  }
  listdir(argv[1], 0, filevisitor, NULL);
  return 0;
}

void listdir(char* dirname, int lvl,
            void (*filevisit)(const char *, int),
            void (*foldervisit)(const char *, int)
            )
{

  int i;
  DIR* d_fh;
  struct dirent* entry;
  char longest_name[MAX_PATH_LENGTH];
  if(access(dirname, R_OK) == -1)
  {
    fprintf(stderr, "Couldn't open directory: %s\n", dirname);
    return;
  }
  while( (d_fh = opendir(dirname)) == NULL) {
    fprintf(stderr, "Couldn't open directory: %s\n", dirname);
    exit(-1);
  }
  while((entry=readdir(d_fh)) != NULL) {
    if(strncmp(entry->d_name, "..", 2) != 0 &&
       strncmp(entry->d_name, ".", 1) != 0) {
      if (entry->d_type == DT_DIR) {
        strncpy(longest_name, dirname, MAX_PATH_LENGTH);
        strncat(longest_name, "/", MAX_PATH_LENGTH);
        strncat(longest_name, entry->d_name, MAX_PATH_LENGTH);
        if(access(longest_name, R_OK) == -1) {
          fprintf(stderr, "Couldn't open directory: %s\n", dirname);
          return;
        }
        listdir(longest_name, lvl+1, filevisit, foldervisit);
        if (foldervisit != NULL) {
            foldervisit(longest_name, lvl);
        }
      }
      else {
        strncpy(longest_name, dirname, MAX_PATH_LENGTH);
        strncat(longest_name, "/", MAX_PATH_LENGTH);
        strncat(longest_name, entry->d_name, MAX_PATH_LENGTH);
        filevisit(longest_name, lvl);
      }
    }
  }
  closedir(d_fh);
  return;
}
