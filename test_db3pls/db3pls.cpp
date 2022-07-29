/*
*/
#include "db3pls.h"
#include <string.h>

DB3pls::DB3pls()
{
  FILE* _inf = nullptr;
  memset(&_hdr, 0x00, sizeof(dBASEHEADER));
  _fields = nullptr;

  _fname = "";
  _nFields = 0;
  _buf_pos = 0;
  _buf_sz = 0;
  _buf = nullptr ;
  _recNo = 0;
} // DB3pls [ctor]

DB3pls::~DB3pls()
{
  close();
} // DB3pls [dtor]

int16_t DB3pls::open(const char* fname)
{
  if (is_open() && (fname == _fname))
  {
    return 0;
  }
  close();

  if ((fname == NULL) || (fname[0] == '\0'))
  {
    return -1;
  }
  _err = fopen_s(&_inf, fname, "rb");
  if (_err != 0)
  {
    return _err;
  }

  // read header
  int n = fread_s(&_hdr, sizeof(_hdr), 1, sizeof(_hdr), _inf);

  // allocate field buffer
  _fields = (dBASEFIELD*)new uint8_t[_hdr.headLength];
  _nFields = (_hdr.headLength - sizeof(_hdr)) / sizeof(dBASEFIELD);

  // read fields
  int amt = _hdr.headLength - sizeof(dBASEHEADER);
  n = fread_s((void*)_fields, amt+1, 1, amt, _inf);

  _fname = fname;
  size_t  p1 = _fname.rfind('.');
  size_t  p2 = _fname.rfind('/');

  _dbname = _fname.substr(p2 + 1, (p1 - p2 - 1));

  goto_top();

  return 0;
} // DB3pls::open

void DB3pls::close()
{
  if (!is_open())
    return;

  // cleanup
  _fname = "";
  fclose(_inf);
  delete _fields;
  _fields = nullptr;
  _nFields = 0;
  delete _buf;
  _buf = nullptr;
  _buf_sz = 0;
  _buf_pos = 0;
  _buf_amt = 0; 
  _rec_pos = 0;
} // DB3pls::close

void DB3pls::display_struct()
{
  char     name[32];
  uint8_t  len;
  char     type;
  printf("\n");
  printf("db  %s\n", dbname().c_str());
  printf("--  ------------  --  ---\n");
  for (int i = 0; i < nFields(); i++)
  {
    get_field_name(i, name);
    get_field_type(i, type);
    get_field_length(i, len);
    printf("%2d  %-12s   %c  %3d \n", i, name, type, len);
  }
  printf("--\n");
  printf("%d records\n", nRecords());
  printf("\n");
} // DB3pls::display_struct

// navigation
int16_t DB3pls::goto_rec(uint32_t rec_no)
{
  if (!is_open())
    return -1;
  if (rec_no > nRecords())
    return -2;

  uint32_t  pos = _hdr.headLength + (_hdr.recLength * (rec_no - 1));
  uint32_t  amt = _hdr.recLength;

  if (_buf == nullptr)
  {
    _buf_sz = 10 * 1024;
    _buf = new uint8_t[_buf_sz];
    _buf_pos = 0xffffffff;
    _buf_amt = 0;
  }
  if ((pos < _buf_pos) || ((pos + amt) > (_buf_pos + _buf_amt)))
  {
    fseek(_inf, (_buf_pos = pos), SEEK_SET);
    _buf_amt = fread_s(_buf, _buf_sz, 1, _buf_sz, _inf);
  }
  _recNo = rec_no;
  _rec_pos = (pos - _buf_pos);

  return 0;
} // DB3pls::

int16_t DB3pls::goto_last()
{
  return goto_rec(_hdr.nRecs);
} // DB3pls::

int16_t DB3pls::goto_next()
{
  return goto_rec(_recNo+1);
} // DB3pls::

int16_t DB3pls::goto_prev()
{
  return goto_rec(_recNo - 1);
} // DB3pls::

int16_t DB3pls::goto_top()
{
  return goto_rec(1);
} // DB3pls::

int16_t DB3pls::get_field_no(const char* name)
{
  if (!is_open() || (name == NULL) || (name[0] == '\0'))
  {
    return -1;
  }
  for (int i = 0; i < _nFields; i++)
  {
    if (_stricmp(name, _fields[i].name) == 0)
    {
      return i;
    }

  }
  return -2;
} // DB3pls::

int16_t DB3pls::get_field_name(int16_t n, char* name)
{
  if (!is_open() || (name == NULL) || (n < 0) || (n >= _nFields))
  {
    return -1;
  }
  strncpy_s(name, 11, _fields[n].name, 12);
  return 0;
} // DB3pls::

int16_t DB3pls::get_field_type(int16_t n, char& type)
{
  if (!is_open() || (n < 0) || (n >= _nFields))
  {
    type = ' ';
    return -1;
  }
  type = _fields[n].type;
  return 0;
} // DB3pls::

int16_t DB3pls::get_field_length(int16_t n, uint8_t& len)
{
  if (!is_open() || (n < 0) || (n >= _nFields))
  {
    len = 0;
    return -1;
  }
  len = _fields[n].length;
  return 0;
} // DB3pls::

int16_t DB3pls::get_field(int16_t n, char* dst) // places current record's field into dst
{
  if (!is_open() || (n < 0) || (n >= _nFields))
  {
    dst[0] = '\0';
    return -1;
  }
  uint8_t* p = &_buf[_rec_pos];

  memcpy_s(dst, _fields[n].length, (char*) & p[_fields[n].offset], _fields[n].length);

  dst[_fields[n].length] = '\0'; 
  return 0;
} // DB3pls::


