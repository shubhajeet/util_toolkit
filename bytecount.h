/**
 * @authors shubhajeet creation
 * 
 * @file bytecount.h
 * 
 * @brief A header file for bytecount
 *
 */
 typedef uint8_t  BYTE;
 typedef struct BYTE_COUNT {
  BYTE b; // b represents the byte
  int count; // count represents the no of ocurrence of that byte
} BYTE_COUNT;
void initBYTE_COUNT(BYTE_COUNT filedata[]);
int addByteCount(BYTE data, BYTE_COUNT filedata[]);
int mergesortBYTECOUNT(BYTE_COUNT filedata[]);
