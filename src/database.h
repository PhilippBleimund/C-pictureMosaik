#include "userInterface.c"

extern Data_all_t **allDatabases;
extern int *Databases_lengths;
extern int totalDatabases;

Data_all_t *loadDatabase(char *path);
void saveDatabase(Data_all_t *data);
void generateNewDatabase(char **paths);
