/**
 * @file bytecount.c
 * @brief implements data type bytecount and its functionality
 * 
 * @authors shubhajeet creation
 */
#include <stdlib.h>
#include <stdint.h>
/**
 * @brief represents a byte i.e 8 bit
 **/
typedef uint8_t  BYTE;
/**
 * @brief represents a data type that stores character and its no of occurence
 * 
 * @details b is byte and count is and int whose value is the occurence of that byte in the file 
 **/
typedef struct BYTE_COUNT {
  BYTE b; // b represents the byte
  int count; // count represents the no of ocurrence of that byte
} BYTE_COUNT;
// Declaring global variable for analysis of file data
/**
 * @brief initializes the variable of type BYTE_COUNT[]
 *
 * @details It initializes the BYTE_COUNT[i].c=i and BYTE_COUNT[i].count where
i is value less than the length of array
 * @param BYTE_COUNT filedata[] 
 * @return void
 **/
void initBYTE_COUNT(BYTE_COUNT filedata[])
{
  int i;
  for(i=0;i<255;i++)
    {
      filedata[i].b=i;
      filedata[i].count=0;
    }
}
/**
 * @brief adds count of given byte by 1
 * @details adds count of given byte by 1 i.e BYTE_COUNT[BYTE].count+=1
 *
 * @param BYTE data byte whose occurence should be increased by 1
 * @param BYTE_COUNT filedata[] the variable whose count is to be increased
 *
 * @return 0 sucess
 * @return -1 array out of bound
 **/
int addByteCount(BYTE data, BYTE_COUNT filedata[])
{
  if(data<255)
    {
      filedata[data].count+=1;
      return 0;
    }
  else
    return -1;
}
int rmsort(BYTE_COUNT filedata[], int start, int end)
{
  if ((end-start)==0)
    {
      return 0;
    }
  else
    {
        int returnValue, middle;
	middle=(start+end)/2;
        returnValue=rmsort(filedata,start,middle);
	returnValue=rmsort(filedata,middle+1,end);
    
	// merging two sorted list
	BYTE_COUNT temp[end-start+1];
	int position=0;
	int position1=start, position2=middle+1;
	while ( position1 < (middle+1) || position2 < (end+1) )
	  {
	    if ( position1 > middle )
	      {
		temp[position].b = filedata[position2].b;
		temp[position].count = filedata[position2].count;
		position2++;
	      }
	    else if (position2 > end)
	      {
		temp[position].b = filedata[position1].b;
		temp[position].count = filedata[position1].count;
		position1++;
	      }
	    else
	      {
		if (filedata[position2].count > filedata[position1].count)
		  {
		    temp[position].b = filedata[position2].b;
		    temp[position].count = filedata[position2].count;
		    position2++;
		  }
		else
		  {
		    temp[position].b = filedata[position1].b;
		    temp[position].count = filedata[position1].count;
		    position1++;
		  }
	      }
	    position++;
	  }
	int i;
	for(i=0;i<(end-start+1);i++)
	  {
	    filedata[start+i].count = temp[i].count;
	    filedata[start+i].b = temp[i].b;
	  }
	return 0;
    }
    
}

/**
 * @breif sorts BYTE_COUNT according to BYTE_COUNT.count
 * 
 * @param BYTE_COUNT filedata[] array of BYTE_COUNT
 **/
int mergesortBYTECOUNT(BYTE_COUNT filedata[])
{
  return rmsort(filedata,0,255-1);
}
