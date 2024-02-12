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

enum struct_type { database, folder, img };

/* all strucs used
 */
typedef struct database_s {
  char path[PATH_MAX];
  char name[PATH_MAX];
} database_t;

typedef struct folder_s {
  char path[PATH_MAX];
  char name[PATH_MAX];
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

  strcpy(selectedDatabaseFiles[numberDatabases].path, str);

  char *index = strrchr(selectedDatabaseFiles[numberDatabases].path, '/');

  if (index == NULL)
    strcpy(selectedDatabaseFiles[numberDatabases].name,
           selectedDatabaseFiles[numberDatabases].path);
  else
    strcpy(selectedDatabaseFiles[numberDatabases].name, index + 1);

  numberDatabases++;

  return &selectedDatabaseFiles[numberDatabases - 1];
}

folder_t *addFolder(char *str, bool rec) {
  if (numberImageFolders % STEPS == 0) {
    selectedImageFolders = realloc(
        selectedImageFolders, sizeof(folder_t) * (numberImageFolders + STEPS));
  }

  strcpy(selectedImageFolders[numberImageFolders].path, str);

  char *index = strrchr(selectedImageFolders[numberImageFolders].path, '/');

  if (index == NULL) {
    strcpy(selectedImageFolders[numberImageFolders].name,
           selectedImageFolders[numberImageFolders].path);
  } else {
    strcpy(selectedImageFolders[numberImageFolders].name, index + 1);
  }

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

void selectionMenu_database(database_t *arr, size_t length) {

  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);

  clear();

  // create a window
  WINDOW *menuwin = newwin(yMax - 2, xMax - 2, 1, 1);
  box(menuwin, 0, 0);
  refresh();
  wrefresh(menuwin);

  // create variables
  int choice;
  int curr_pos = 0;
  int top_pos = 0;
  bool exit_menu = false;

  // enable user input
  keypad(menuwin, true);

  // generate display
  bool *user_selection = malloc(sizeof(bool) * length);

  while (exit_menu == false) {

    clear();
    box(menuwin, 0, 0);

    // print menu information
    mvwprintw(menuwin, 0, 2, "d Delete");
    mvwprintw(menuwin, 0, 11, "q Quit");

    for (int i = 0; i < (yMax - 4); i++) {
      if ((i + top_pos) > length - 1)
        break;
      if (i == curr_pos)
        wattron(menuwin, A_REVERSE);
      mvwprintw(menuwin, i + 1, 1, "[%c] %s in path: %s",
                user_selection[i + top_pos] == true ? 'X' : ' ',
                arr[i + top_pos].name, arr[i + top_pos].path);
      wattroff(menuwin, A_REVERSE);
    }
    choice = wgetch(menuwin);

    switch (choice) {
    case KEY_UP:
      curr_pos--;
      if (curr_pos == -1) {
        curr_pos = 0;
        top_pos--;
        if (top_pos == -1) {
          top_pos = 0;
        }
      }
      break;
    case KEY_DOWN:
      curr_pos++;
      if (curr_pos >= (yMax - 4)) {
        curr_pos = yMax - 5;
        top_pos++;
        if (top_pos + (yMax - 4) > length) {
          top_pos--;
        }
      }
      if (curr_pos >= length) {
        curr_pos = length - 1;
      }
      break;
    case 10:
      if (user_selection[curr_pos + top_pos] == false)
        user_selection[curr_pos + top_pos] = true;
      else
        user_selection[curr_pos + top_pos] = false;
      break;
    case 'd':
      for (size_t i = 0; i < length; i++) {
        if (user_selection[i] == true) {
          for (size_t j = i; j < length - 1; j++) {
            arr[j] = arr[j + 1];
          }
          length--;
        }
      }
      break;
    case 'q':
      exit_menu = true;
      break;
    default:
      break;
    }
  }

  numberDatabases = length;

  free(selection_total);
  free(user_selection);
}

void handle_e() {

  bool exitMenu = false;

  do {
    clear();
    printf("=======edit========");
    printf("current number of Databases: %zu\n", numberDatabases);
    printf("current number of Image Folders %zu\n", numberImageFolders);
    printf("current number of Images %zu\n", numberImages);
    printf("~~~~~~options~~~~~~\n");
    printf("d -> edit databases\n");
    printf("f -> edit folders\n");
    printf("i -> edit images\n");
    printf("q -> quit menu\n");
    printf("\nyour selection: ");

    char c = getch();
    switch (c) {
    case 'd':
      selectionMenu_database(selectedDatabaseFiles, numberDatabases);
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
  } while (exitMenu == false);
}

/* Main method
 */

int main(void) {

  /* Init all variables */
  selectedDatabaseFiles = malloc(sizeof(database_t) * STEPS);
  selectedImageFolders = malloc(sizeof(folder_t) * STEPS);
  selectedImages = malloc(sizeof(img_t) * STEPS);

  strcpy(selectedDatabaseFiles[0].name, "test1");
  strcpy(selectedDatabaseFiles[1].name, "test2");
  strcpy(selectedDatabaseFiles[2].name, "test3");
  strcpy(selectedDatabaseFiles[3].name, "test4");
  strcpy(selectedDatabaseFiles[4].name, "test5");
  strcpy(selectedDatabaseFiles[5].name, "test6");
  strcpy(selectedDatabaseFiles[6].name, "test7");
  strcpy(selectedDatabaseFiles[7].name, "test8");
  strcpy(selectedDatabaseFiles[8].name, "test9");
  strcpy(selectedDatabaseFiles[9].name, "test10");
  numberDatabases = 10;

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
    printf("q -> quit programm");

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
