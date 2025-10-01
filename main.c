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

  if (!createDB(db, dbpath))
    return 0;


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

void printMenu(LinkedList list)
{
  char* startstr = "┃ 1. Start             ┃\n";
  if (get(list.head, 0) == NULL)
    startstr =     "┃ 1. Start (EMPTY)     ┃\n";

  printf("\e[1;1H\e[2J");
  printf("┏━━━━━━━━━━━━━━━━━━━━━━┓\n");
  printf("┃ WELCOME TO QUEUETASK ┃\n");
  printf("┣━━━━━━━━━━━━━━━━━━━━━━┫\n");
  printf("%s", startstr);
  printf("┃ 2. Edit queue        ┃\n");
  printf("┃ 0. End program.      ┃\n");
  printf("┣━━━━━━━━━━━━━━━━━━━━━━┛\n");
  printf("┗━▶ Option: ");
}

void printEditMenu(LinkedList list)
{
  printf("\e[1;1H\e[2J");
  printf("┏━━━━━━━━━━━━━━━━━━━━━━┓\n");
  printf("┃   QUEUE EDIT MENU    ┃\n");
  printf("┗━━━━━━━━━━━━━━━━━━━━━━┛\n");
  traverseList(list.head);
  printf("┏━━━━━━━━━━━━━━━━━━━━━━┓\n");
  printf("┃ 1. ADD TASK          ┃\n");
  printf("┃ 2. INSERT TASK BY ID ┃\n");
  printf("┃ 3. REMOVE TASK BY ID ┃\n");
  printf("┃ 4. PURGE LIST        ┃\n");
  printf("┃                      ┃\n");
  printf("┃ 0. EXIT              ┃\n");
  printf("┣━━━━━━━━━━━━━━━━━━━━━━┛\n");
  printf("┗━▶ Option: ");
}

void printPlayMenu(LinkedList list)
{
  printf("\e[1;1H\e[2J");
  printf("┏━━━━━━━━━━━━━━━━━━━━━━┓\n");
  printf("┃     CURRENT TASK     ┃\n");
  printf("┗━━━━━━━━━━━━━━━━━━━━━━┛\n");
  printf("%s\n",get(list.head, 0));
  printf("┏━━━━━━━━━━━━━━━━━━━━━━┓\n");
  printf("┃ 1. COMPLETE TASK     ┃\n");
  printf("┃                      ┃\n");
  printf("┃ 0. EXIT              ┃\n");
  printf("┣━━━━━━━━━━━━━━━━━━━━━━┛\n");
  printf("┗━▶ Option: ");
}


int main()
{
  sqlite3 *db;
  char* dbpath;
  int isOver;
  int isEditionOver;
  int playing;
  char option;
  char editOption;
  char playOption;
  char task[1024];
  
  LinkedList list = {};
  prepend(&list, NULL);

  dbpath = getdbPath();
  if (!start(db, dbpath, list))
    return 1;
  
  if (!isDataBaseEmpty(list, db, dbpath))
    list.head = removeHead(list.head);

  isOver = 0;
  while (!isOver)
  {
    printMenu(list);
    scanf("%c", &option);
    if (option == '0')
      isOver = 1;
    else if (option == '1')
    {
      if (get(list.head, 0) != NULL)
      {
        playing = 1;
        while (playing)
        {
          if (get(list.head, 0) != NULL)
          {
            printPlayMenu(list);
            scanf("%c", &playOption);
            if (playOption == '0')
              playing = 0;
            else if (playOption == '1')
              list.head = removeHead(list.head);
          }
          else
            playing = 0;
        }
      }
    }
    else if (option == '2')
    {
      isEditionOver = 0;
      while (!isEditionOver)
      {
        printEditMenu(list);
        scanf("%c", &editOption);
        if(editOption == '0')
          isEditionOver = 1;
        else if (editOption == '1')
        {
          printf("━━▶ Task: ");
          scanf("%s", task);
          char* taskcpy = strdup(task);
          push(list.head, taskcpy);
          if (get(list.head, 0) == NULL)
            list.head = removeHead(list.head);
        }
        else if (editOption == '2')
        {
          int n;
          printf("━━▶ Task: ");
          scanf("%s", task);
          printf("━━▶ Desired Index: ");
          scanf("%d", &n);
          char* taskcpy = strdup(task);
          list.head = pushByIndex(list.head, taskcpy, n);
        }
        else if (editOption == '3')
        {
          int n;
          printf("━━▶ Desired Index: ");
          scanf("%d", &n);
          if (n != 0)
            removeByIndex(list.head, n);
          else
            list.head = removeHead(list.head);
        }
        else if (editOption == '4')
        {
          list.head = purge(list.head);
          prepend(&list, NULL);
        }
      }
    }
  }
  
  if (get(list.head, 0) == NULL)
    list.head = removeHead(list.head);
  if (!logout(&list, db, dbpath))
    return 1;
  free(dbpath);
  sqlite3_close(db);
  return 0;
}