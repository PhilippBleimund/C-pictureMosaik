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

file_t *selectedDatabaseFiles;
size_t numberDatabases = 0;

file_t *selectedImageFolders;
size_t numberImageFolders = 0;

file_t *selectedImages;
size_t numberImages = 0;

char default_str[17] = "nothing selected";

/* Helper functions
 */

char *getDatabaseName(int index) {

  if (numberDatabases == 0) {
    return default_str;
  } else {
    return selectedDatabaseFiles[index].name;
  }
}

char *addFile_t(file_t *arr, size_t size, char *str) {
  if (size % STEPS == 0) {
    arr = realloc(arr, size + STEPS);
  }

  file_t new_File;
  new_File.
}

/* core user interface functions
 */

int handle_a() {

  bool exitMenu = false;

  while (exitMenu == false) {
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
    printf("q -> qiut menu\n");
    printf("\n your selection: ");

    char c = getch();
    if (c == EOF) {
      printf("error try again\npress any key to continue...");
      getch();
    }

    switch (c) {
    case 'd':

      break;
    case 'f':

      break;
    case 'F':

      break;
    case 'i':

      break;
    case 'I':

      break;
    case 'q':

      break;
    default:
      printf("sorry this option is not available. Try again.\npress any key to "
             "continue...");
      getch();
    }
  }
}

/* Main method
 */

int main(void) {

  /* Init all variables */
  selectedDatabaseFiles = malloc(sizeof(file_t) * STEPS);
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

  /* free all dynamic variables */
  free(selectedDatabaseFiles);
  free(selectedImageFolders);
  free(selectedImages);

  return EXIT_SUCCESS;
}
