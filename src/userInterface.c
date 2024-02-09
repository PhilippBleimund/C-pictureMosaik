#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>



#define printf printw


typedef struct {
  FILE fileLink;
  char path[PATH_MAX];
  char name[PATH_MAX];
} file_t;

file_t currentDatabaseFile[20];
int numberDatabases = 0;

char default_str[17] = "nothing selected";

char* getDatabaseName(int index){

  if(numberDatabases == 0){
    return default_str;
  }else{
     return currentDatabaseFile[index].name;
 }
}


int main(void){

  printf("test1\n");

  initscr();
  scrollok(stdscr, TRUE);

  while (true){
    clear();
    printf("======status=======\n");
    printf("current selected database: %s", getDatabaseName(0));
    
    char c = getch();
    if (c == EOF)
      printf("error try again");

    switch (c) {
      case 'a':
        printf("Youve entered the char a");
    }

    getch();
  }
  
  endwin();

  return EXIT_SUCCESS;
}

