#include <dirent.h>
#include <linux/limits.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define printf printw
#define STEPS 128

/* all enums
 */

typedef enum { JPG, PNG } img_type;

/* all strucs used
 */
typedef struct database_s {
  char path[PATH_MAX];
  char *name;
} database_t;

typedef struct folder_s {
  char path[PATH_MAX];
  char *name;
  bool recursive;
} folder_t;

typedef struct img_s {
  char path[PATH_MAX];
  img_type type;
} img_t;

/* Global Variables
 */

database_t *selectedDatabaseFiles;
size_t numberDatabases = 0;

folder_t *selectedImageFolders;
size_t numberImageFolders = 0;

img_t *selectedImages;
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

database_t *addDatabase(char *str) {
  if (numberDatabases % STEPS == 0) {
    selectedDatabaseFiles = realloc(
        selectedDatabaseFiles, sizeof(database_t) * (numberDatabases + STEPS));
  }

  database_t new_database;
  strcpy(new_database.path, str);
  new_database.name = strrchr(new_database.path, '/');

  selectedDatabaseFiles[numberDatabases] = new_database;
  numberDatabases++;

  return &selectedDatabaseFiles[numberDatabases - 1];
}

folder_t *addFolder(char *str, bool rec) {
  if (numberImageFolders % STEPS == 0) {
    selectedImageFolders = realloc(
        selectedImageFolders, sizeof(folder_t) * (numberImageFolders + STEPS));
  }

  folder_t new_folder;
  strcpy(new_folder.path, str);
  new_folder.name = strrchr(new_folder.path, '/');
  new_folder.recursive = rec;

  selectedImageFolders[numberImageFolders] = new_folder;
  numberImageFolders++;

  return &selectedImageFolders[numberImageFolders - 1];
}

/* core user interface functions
 */

int handle_a() {

  bool exitMenu = false;

  while (exitMenu == false) {
    clear();
    printf("====add Database, Images or Folder====\n");
    printf("current number of Databases: %zu\n", numberDatabases);
    printf("current number of Image Folders %zu\n", numberImageFolders);
    printf("current number of Images %zu\n", numberImages);
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

    printf("\n\n");

    switch (c) {
    case 'd':
      printf("insert path to database:\n$ ");
      char path_database[PATH_MAX];
      getstr(path_database);
      addDatabase(path_database);
      break;

    case 'f':
      printf("insert path to Image Folder:\n$ ");
      char path_folder[PATH_MAX];
      getstr(path_folder);
      printf("include sub directorys?\n n(no)/y(yes): ");
      char r = getch();

      bool recursive = false;
      if (r == 'y')
        recursive = true;

      addFolder(path_folder, recursive);
      break;

    case 'F':

      break;
    case 'i':

      break;
    case 'I':

      break;
    case 'q':
      exitMenu = true;
      break;
    default:
      printf("sorry this option is not available. Try again.\npress any key to "
             "continue...");
      getch();
    }
  }

  return EXIT_SUCCESS;
}

/* Main method
 */

int main(void) {

  /* Init all variables */
  selectedDatabaseFiles = malloc(sizeof(database_t) * STEPS);
  selectedImageFolders = malloc(sizeof(folder_t) * STEPS);
  selectedImages = malloc(sizeof(img_t) * STEPS);

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
