#include <sqlite3.h>

char* getOSlocalPath();
char *join_path(const char* path1, const char* path2);
int createDB(sqlite3 *db, char* dbpath);
int makeTaskList(void* list, int count, char** data, char** column);