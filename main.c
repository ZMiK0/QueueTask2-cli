#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "dbenginelib.h"

char* getdbPath()
{
  FILE *fptr;
  char* dirname;
  char* appdatapath;
  char* homedir;
  char* dbpath;

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
  free(dirname);
  return (dbpath);
}

int logout(LinkedList *list, sqlite3 *db, char* dbpath)
{
  if (!remadeDB(list->head, db, dbpath))
    return 0;
  return 1;
}

int start(sqlite3 *db, char* dbpath, LinkedList list)
{
  char* stmt;
  char* err;
  int rc;

  createDB(db, dbpath);

  stmt = "SELECT * FROM tasklist";
  err = "ERROR";

  rc = sqlite3_open(dbpath, &db);
  if (rc)
  {
    fprintf(stderr, "Can't open database :( - %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 0;
  } else {
    rc = sqlite3_exec(db, stmt, makeTaskList, &list, &err);
    if (rc)
    {
      fprintf(stderr, "Can't execute :( - %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return 0;
    }
  }
  return 1;
}


int main()
{
  sqlite3 *db;
  char* dbpath;
  
  LinkedList list = {};
  prepend(&list, "");

  dbpath = getdbPath();
  if (!start(db, dbpath, list))
    return 1;
  
  list.head = removeHead(list.head);





  traverseList(list.head);

  if (!logout(&list, db, dbpath))
    return 1;
  
  free(dbpath);
  sqlite3_close(db);
  return 0;
}