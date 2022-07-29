/*
*/
#include <stdio.h>
#include "db3pls.h"
#include <algorithm>

void db_dump( const char *name )
{
  DB3pls   db;
  char     buf[256];
  char     b1[256];
  char     b2[256];
  char     b3[256];
  char     b4[256];

  sprintf_s(buf, 255, "../data/%s.dbf", name);
  if (db.open( buf ) != 0)
  {
    printf("error opening product database\n");
    return ;
  }

  db.display_struct();

  db.goto_top();

//  int16_t  iStock = db.get_field_no("STOCKNO");
//  int16_t  iDesc = db.get_field_no("DESC");
//  int16_t  iWholesale = db.get_field_no("WHOLESALE");

  uint32_t  n_displayed = 25;
  uint16_t  sz = 0;
  n_displayed = std::min(n_displayed, db.nRecords());

  for (int i = 0; i < n_displayed; i++)
  {
    printf("%6d.  ", i);
    sz = 0;
    for (int j = 0; j < db.nFields(); j++)
    {
      db.get_field(j, b1);
      sz += strlen(b1);
      if (sz > 200) 
        break;
      printf("%s ", b1);
    }
    printf("\n");

    db.goto_next();
  }

  db.close();
} // :: db_dump

int main()
{
  db_dump("idl");
  db_dump("product");
  db_dump("zipcodes");
  return 0;
} // :: main
