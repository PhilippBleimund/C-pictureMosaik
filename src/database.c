#include "database.h"
#include "image.h"
#include "lib/progressbar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define STEPS 128

Data_all_t **allDatabases;
int *Databases_lengths;
int totalDatabases = 0;

void increaseDatabases() {
  if (totalDatabases == 0) {
    allDatabases = malloc(sizeof(Data_all_t *));
    Databases_lengths = malloc(sizeof(int *));
  } else {
    allDatabases = realloc(allDatabases, totalDatabases + 1);
    Databases_lengths = realloc(Databases_lengths, totalDatabases + 1);
  }
  totalDatabases++;
}

void unloadDatabase(unsigned int index) {
  if (index >= totalDatabases)
    return;

  free(allDatabases[index]);
  for (int i = index; i < totalDatabases - 1; i++) {
    Databases_lengths[index] = Databases_lengths[index + 1];
  }
  Databases_lengths = realloc(Databases_lengths, totalDatabases - 1);
  totalDatabases--;
}

int loadDatabase(char *path) {
  FILE *fp;
  fp = fopen(path, "rb");
  if (fp == NULL) {
    puts("Error, File Could Not Be Opened.");
    return 1;
  }

  int total_length = 0;
  int current_length = 0;
  Data_all_t *buffer = malloc(sizeof(Data_all_t) * STEPS);
  while ((current_length = fread(buffer, sizeof(Data_all_t), STEPS, fp)) != 0) {
    total_length += current_length;
    if (current_length == STEPS) {
      buffer = realloc(buffer, sizeof(Data_all_t) * (total_length + STEPS));
    }
  }

  increaseDatabases();
  int index = totalDatabases - 1;
  allDatabases[index] = buffer;
  Databases_lengths[index] = total_length;
  return index;
}

void saveDatabase(unsigned int index, char *path) {
  if (index >= totalDatabases)
    return;

  FILE *fp;
  fp = fopen(path, "wb");
  if (fp == NULL) { // Note: only w
    puts("Error, File Could Not Be Opened.");
  } else {
    fwrite(allDatabases[index], sizeof(Data_all_t), Databases_lengths[index],
           fp);
    fclose(fp);
  }
}

int generateNewDatabase(char **paths, int numPaths) {
  increaseDatabases();
  int index = totalDatabases - 1;
  allDatabases[index] = malloc(sizeof(Data_all_t) * numPaths);

  // init statusbar
  progressbar *prog = progressbar_new("iterating all Images", numPaths);

  for (int i = 0; i < numPaths; i++) {
    Image img;
    Image_load(&img, paths[i]);
    strcpy(allDatabases[index][i].img_path, paths[i]);
    Image_to_Data_t(&img, &allDatabases[index][i]);
    Databases_lengths[index]++;
    progressbar_inc(prog);
  }
  progressbar_finish(prog);

  return index;
}
