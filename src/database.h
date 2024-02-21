#include "image.h"

extern Data_all_t **allDatabases;
extern int *Databases_lengths;
extern int totalDatabases;

int loadDatabase(char *path);
void unloadDatabase(unsigned int index);
void saveDatabase(unsigned int index, char *path);
int generateNewDatabase(char **paths, int numPaths);
