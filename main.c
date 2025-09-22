#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "linkedlib.h"
#include "dbenginelib.h"

int main()
{
  sqlite3 *db;
  createDB(db);
  /*
  node* head = prepend(head, 2);
  push(head, 4);
  printf("%d\n", get(head, 1));
  */
  
  return 0;
}