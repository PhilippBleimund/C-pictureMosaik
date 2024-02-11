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

img_t *addImage(char *str) {
  if (numberImages % STEPS == 0) {
    selectedImages =
        realloc(selectedImages, sizeof(img_t) * (numberImages + STEPS));
  }

  img_t new_image;
  strcpy(new_image.path, str);
  char *type_str = strrchr(new_image.path, '.');
  if (strcmp(type_str, "png") == 0 || strcmp(type_str, "PNG") == 0) {
    new_image.type = PNG;
  } else if (strcmp(type_str, "jpg") == 0 || strcmp(type_str, "JPG") == 0) {
    new_image.type = JPG;
  }

  selectedImages[numberImages] = new_image;
  numberImages++;

  return &selectedImages[numberImages - 1];
}

/* core user interface functions
 */

int handle_a() {

  bool exitMenu = false;
  char lastAction_str[6][50] = {"",
                                "added database",
                                "added image folder",
                                "added multiple folders",
                                "added single image",
                                "added multiple images"};
  int lastAction = 0;

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
    printf("last action: %s\n", lastAction_str[lastAction]);
    printf("\nyour selection: ");

    char c = getch();
    if (c == EOF) {
      printf("error try again\npress any key to continue...");
      getch();
    }

    printf("\n\n");

    char path[PATH_MAX];
    char r;
    bool recursive;
    bool continue_loop;

    switch (c) {
    case 'd':
      printf("insert path to database:\n$ ");
      getstr(path);
      addDatabase(path);
      lastAction = 1;
      break;

    case 'f':
      printf("insert path to image folder:\n$ ");
      getstr(path);
      printf("include sub directorys?\nn(no)/y(yes): ");
      r = getch();

      recursive = false;
      if (r == 'y')
        recursive = true;

      addFolder(path, recursive);
      lastAction = 2;
      break;

    case 'F':
      continue_loop = true;
      while (continue_loop) {
        printf("insert path to image folder:\n$ ");
        getstr(path);
        printf("include sub directorys?\nn(no)/y(yes): ");
        r = getch();

        recursive = false;
        if (r == 'y')
          recursive = true;

        folder_t *folder = addFolder(path, recursive);

        printf("\nadded folder %s", folder->name);
        printf("add another folder?\nn(no)/y(yes): ");
        char a = getch();
        if (a != 'y')
          continue_loop = false;
      }
      lastAction = 3;
      break;

    case 'i':
      printf("insert path to single image:\n$ ");
      getstr(path);
      addImage(path);
      lastAction = 4;

      break;

    case 'I':
      continue_loop = true;
      while (continue_loop) {
        printf("insert path to single image:\n$ ");
        getstr(path);

        img_t *folder = addImage(path);

        printf("\nadded %s image\n", (folder->type == PNG) ? "png" : "jpg");
        printf("add another imgage?\nn(no)/y(yes): ");
        char a = getch();
        if (a != 'y')
          continue_loop = false;
      }
      lastAction = 5;
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

void handle_e() {

  bool exitMenu = false;

  while (exitMenu == false) {
    printf("=======edit========");
    printf("current number of Databases: %zu\n", numberDatabases);
    printf("current number of Image Folders %zu\n", numberImageFolders);
    printf("current number of Images %zu\n", numberImages);
    printf("~~~~~~options~~~~~~\n");
    printf("d -> show all databases\n");
    printf("f -> show all folders\n");
    printf("i -> show all images\n");
    printf("q -> quit menu\n");
    printf("\nyour selection: ");

    char c = getch();
    switch (c) {
    case 'd':

      break;

    case 'f':

      break;

    case 'i':

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
    printf("r -> create new render job\n");
    printf("c -> create new Database\n");

    char c = getch();
    if (c == EOF) {
      printf("error try again\npress any key to continue...");
      getch();
    }

    switch (c) {
    case 'a':
      handle_a();
      break;
    case 'e':
      handle_e();
      break;
    case 'q':
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
