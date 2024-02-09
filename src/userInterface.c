#include <linux/limits.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define printf printw
#define STEPS 128

/* Struct used for storing multiple file links
 */
struct file_s {
  FILE fileLink;
  char path[PATH_MAX];
  char name[PATH_MAX];
  bool recusive;
} file_default = {NULL, "/nothing/", "nothing selected", false};
typedef struct file_s file_t;

/* Global Variables
 */

file_t currentDatabaseFile[STEPS];
int numberDatabases = 0;

file_t *selectedImageFolders;
int numberImageFolders = 0;

file_t *selectedImages;
int numberImages = 0;

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

  /* Init all variables */
  selectedImageFolders = malloc(sizeof(file_t) * STEPS);
  selectedImages = malloc(sizeof(file_t) * STEPS);

  initscr();
  scrollok(stdscr, TRUE);

  bool keepRunning = true;

  while (keepRunning) {
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
      keepRunning = false;
    }

    getch();
  }

  endwin();

  return EXIT_SUCCESS;
}
