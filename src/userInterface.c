#include <linux/limits.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define printf printw

/* Struct used for storing multiple file links
 */
typedef struct {
  FILE fileLink;
  char path[PATH_MAX];
  char name[PATH_MAX];
} file_t;

/* Global Variables
 */

file_t currentDatabaseFile[20];
int numberDatabases = 0;

char default_str[17] = "nothing selected";

/* Helper functions
 */

char *getDatabaseName(int index) {

  if (numberDatabases == 0) {
    return default_str;
  } else {
    return currentDatabaseFile[index].name;
  }
}

/* Main method
 */

int main(void) {

  printf("test1\n");

  initscr();
  scrollok(stdscr, TRUE);

  bool keeprunning = true;

  while (keeprunning) {
    clear();
    printf("======status=======\n");
    printf("current selected database: %s\n", getDatabaseName(0));

    char c = getch();
    if (c == EOF) {
      printf("error try again\npress any key to continue...");
      getch();
    }

    switch (c) {
    case 'a':
      printf("Youve entered the char a");
    case 'd':
      keeprunning = false;
    }

    getch();
  }

  endwin();

  return EXIT_SUCCESS;
}
