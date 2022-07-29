# db3pls
simple c++ classes to interface with db3pls files

/* dBASE technical specifications
    *Simpson, dBASE III+ programmers reference guide

   Database file limitations
     1 billion records
     2 billion bytes per file
     128 fields
     4000 bytes per record max
     10 character fieldname max length

   Data limitations
     Character  0-254 characters
     Numeric    19 bytes max length
     Date       8 bytes always
     Logical    1 byte in file
     Memo       5000 bytes or capacity of external word processor

   Numeric Accuracy
     Computational accuracy: 15.9 digits
     Accuracy of non-zero comparisons: 13 digits
     largest number: 10 ^ 308
     smallest positive number: 10 ^ -307
*/

// valid dBASE tombstone values
#define  db3EOF         0x1A
#define  db3EOFD        0x0D  /* end of field descriptors */
#define  tsDELETE       '*'
#define  tsLOCK         '!'
#define  tsCLEAR        ' '

// Structure to hold the standard DBASE dates
typedef struct DATE_tag
  {
    byte year  ;
    byte month ;
    byte day   ;
  } DATE ;

// Parameters of a standard DBASE III field
typedef struct dBASEHEADER_tag
  {
    byte      version    ; // dBase that made file
    DATE      date       ; // last update
    dword     nRecs      ; // #Records in structure
    word      headLength ; // length of header
    word      recLength  ; // length of record
    byte      hold1 [20] ; // unused - usually 0's
  } dBASEHEADER ;

typedef struct dBASEFIELD_tag
  {
    char      name [11]     ; // 10 characters w/ 0 end
    char      type          ;
    dword     offset        ; // Offset from record top
    byte      length        ; // Length of field < 256
    byte      deccnt        ;
    byte      hold2 [14]    ; // unused - usually 0's
  } dBASEFIELD ;
