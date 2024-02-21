#include "image.h"
#include <linux/limits.h>

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
