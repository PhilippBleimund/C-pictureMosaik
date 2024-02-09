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

/* core user interface functions
 */

int handle_a() {
  clear();
  printf("====add Database, Images or Folder====\n");
  printf("current number of Databases: %d\n", numberDatabases);
  printf("current number of Image Folders %d\n", numberImageFolders);
  printf("current number of Images %d\n", numberImages);
  printf("\n~~~~~options~~~~~\n");
  printf("d -> add Database\n");
  printf("f -> add Image Folder\n");
  printf("F -> add multiple Folders\n");
  printf("i -> add single Image\n");
  printf("I -> add multiple Images\n");
  printf("\n your selection: ");
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
    printf("\n~~~~~~options~~~~~~\n");
    printf("a -> add a Database, Images or Folders\n");
    printf("e -> edit your selected Databases, Images or Folders\n");
    printf("r -> create new render job");
    printf("c -> create new Database");

    char c = getch();
    if (c == EOF) {
      printf("error try again\npress any key to continue...");
      getch();
    }

    switch (c) {
    case 'a':
      handle_a();
    case 'd':
      keepRunning = false;
    }

    getch();
  }

  endwin();

  return EXIT_SUCCESS;
}
