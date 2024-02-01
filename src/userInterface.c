#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include <limits.h>
#include <string.h>

typedef struct {
  FILE fileLink;
  char path[PATH_MAX];
  char name[PATH_MAX];
} file_t;

file_t currentDatabaseFile[20];
int numberDatabases = 0;

char* getDatabaseName(int index){
  char name[PATH_MAX];

  if(numberDatabases == 0){
    strcpy(name, "nothing selected");
  }else{
     strcpy(name, currentDatabaseFile[index].name);
 }

  return name;
}


int main(int argc, char *argv[]){

  initscr();

  while (true){
    clear();
    printw("======status=======");
    printw("current selected database: %s", getDatabaseName(0));
    
  }

  int c = getchar();
  if (c != EOF)
    printf("%c\n", c);

  return EXIT_SUCCESS;
}

