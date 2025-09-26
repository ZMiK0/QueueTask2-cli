#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "linkedlib.h"

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

int createDB(sqlite3 *db, char* dbpath)
{
  char* stmt;
  char* error;
  int rc;

  rc = sqlite3_open(dbpath, &db);
  if (rc)
  {
    fprintf(stderr, "Can't open database :( - %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
  }
  error = "ERROR";
  stmt = "CREATE TABLE IF NOT EXISTS tasklist (id INTEGER PRIMARY KEY AUTOINCREMENT, task TEXT);";
  rc = sqlite3_exec(db, stmt,NULL, 0, &error);
  if (rc)
  {
    fprintf(stderr, "ERROR EXECUTING - %s\n", error);
    sqlite3_free(error);
  }
  sqlite3_close(db);
  return 1;
}

int removeDB(sqlite3 *db, char* dbpath)
{
  char* stmt;
  char* error;
  int rc;

  rc = sqlite3_open(dbpath, &db);
  if (rc)
  {
    fprintf(stderr, "Can't open database :( - %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
  }
  error = "ERROR";
  stmt = "DROP TABLE IF EXISTS tasklist;";
  rc = sqlite3_exec(db, stmt,NULL, 0, &error);
  if (rc)
  {
    fprintf(stderr, "ERROR EXECUTING - %s\n", error);
    sqlite3_free(error);
  }
  sqlite3_close(db);
  return 1;
}

int makeTaskList(void* list, int count, char** data, char** column)
{
  struct LinkedList *head = list;
  if (data[1] != NULL)
  {
    char* taskcpy = strdup(data[1]);
    push(head->head, taskcpy);
  }
  
  return 0;
}

int pushTask(sqlite3 *db, char* dbpath, char* task)
{
  char stmt[512];
  char* error;
  int rc;

  rc = sqlite3_open(dbpath, &db);
  if (rc)
  {
    fprintf(stderr, "Can't open database :( - %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
  }
  error = "ERROR";
  snprintf(stmt, sizeof(stmt), "INSERT INTO tasklist (task) VALUES (\"%s\");", task);
  rc = sqlite3_exec(db, stmt,NULL, 0, &error);
  if (rc)
  {
    fprintf(stderr, "ERROR INSERTING - %s\n", error);
    sqlite3_free(error);
  }
  sqlite3_close(db);
  return 1;
}

int pushToDB(node* head, sqlite3 *db, char* dbpath)
{
    node* current = head;
    while(current != NULL)
    {
        pushTask(db, dbpath, current->data);
        current = current->next;
    }
    printf("\n");
    return 1;
}

int remadeDB(node* head, sqlite3 *db, char* dbpath)
{
  if (removeDB(db, dbpath))
    if (createDB(db, dbpath))
      if (pushToDB(head, db, dbpath))
        return 1;
  return 0;
}