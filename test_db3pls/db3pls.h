/*
*/
#pragma once
#ifndef DB3PLS_H
#define DB3PLS_H

#include <stdint.h>
#include <stdio.h>
#include <string>

// valid dBASE tombstone values
#define  db3EOF         0x1A
#define  db3EOFD        0x0D  /* end of field descriptors */
#define  tsDELETE       '*'
#define  tsLOCK         '!'
#define  tsCLEAR        ' '

// Structure to hold the standard DBASE dates
typedef struct DATE_tag
{
  uint8_t year;
  uint8_t month;
  uint8_t day;
} DATE;

// Parameters of a standard DBASE III field
typedef struct dBASEHEADER_tag
{
  uint8_t     version; // dBase that made file
  DATE        date; // last update
  uint32_t    nRecs; // #Records in structure
  uint16_t    headLength; // length of header
  uint16_t    recLength; // length of record
  uint8_t     hold1[20]; // unused - usually 0's
} dBASEHEADER;

typedef struct dBASEFIELD_tag
{
  char        name[11]; // 10 characters w/ 0 end
  char        type;
  uint32_t    offset; // Offset from record top
  uint8_t     length; // Length of field < 256
  uint8_t     deccnt;
  uint8_t     hold2[14]; // unused - usually 0's
} dBASEFIELD;

class DB3pls
{
protected :
  std::string      _fname;
  std::string      _dbname;
  FILE            *_inf;
  errno_t          _err;
  dBASEHEADER      _hdr;
  dBASEFIELD      *_fields;
  uint16_t         _nFields;
  uint32_t         _buf_pos;
  uint16_t         _buf_sz;
  uint16_t         _buf_amt;
  uint8_t         *_buf;
  uint32_t         _recNo; // current record number; 1-based records
  uint32_t         _rec_pos; // current record position in buffer

public    :
  DB3pls();
  ~DB3pls();

  int16_t   open(const char* fname);
  void      close();
  void      display_struct();

  // navigation
  int16_t   goto_rec(uint32_t rec_no);
  int16_t   goto_last();
  int16_t   goto_next();
  int16_t   goto_prev();
  int16_t   goto_top();

  // get field data
  int16_t   get_field_no(const char* name);
  int16_t   get_field_name(int16_t n, char *name);
  int16_t   get_field_type(int16_t n, char &type);
  int16_t   get_field_length(int16_t n, uint8_t &len);

  // get record info
  int16_t   get_field(int16_t n, char* dst); // places current record's field into dst

  // access methods
  bool      is_open() { return (_inf != nullptr); }
  uint16_t  nFields() { return _nFields; }
  uint32_t  nRecords() { return _hdr.nRecs; }
  uint32_t  rec_no() { return _recNo;  } 
  std::string& dbname() { return _dbname; }

}; // class DB3pls

#endif
