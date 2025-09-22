#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

char* getOSlocalPath()
{
  #ifdef _WIN32
    return "%appdata%"; //TODO
  #elif __APPLE__
    return "Library/Application Support";
  #elif __unix__
    return ".local/share";
  #endif
}

char *join_path(const char* path1, const char* path2)
{
  char* result = malloc(strlen(path1) + strlen(path2) + 2);
  strcpy(result, path1);
  strcat(result, "/");
  strcat(result, path2);
  return result;
}

int createDB(sqlite3 *db)
{
  FILE *fptr;
  char* dirname;
  char* appdatapath;
  char* homedir;
  char* dbpath;
  int rc;

  homedir = getenv("HOME");
  appdatapath = getOSlocalPath();
  appdatapath = join_path(homedir, appdatapath);
  dirname = join_path(appdatapath, "queuetask2-cli");
  if (mkdir(dirname, 0777) == -1)
  {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "mkdir -p \"%s\"", dirname);
    system(cmd);
  }
  dbpath = join_path(dirname, "task.db");
  char cmd[512];
  snprintf(cmd, sizeof(cmd), "touch \"%s\"",dbpath);
  system(cmd);
  rc = sqlite3_open(dbpath, &db);
  if (rc)
  {
    fprintf(stderr, "Can't open database :( - %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
  }

  free(dirname);
  free(dbpath);
  sqlite3_close(db);
  return 1;
}