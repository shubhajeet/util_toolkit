/**
 * @author shubhajeet creation
 * @author  IOE (Institute of Enginnering), Pulchowk Campus
 * @author Department of Electronics and Computer Science
 * @author 2070 BEX 1st year 1st semester
 * @author Project on C
 * 
 * @file comprez.c
 * 
 * @brief A Library fucntionality of comprez program
  **/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "bytecount.h"
#include "hashstruct.h" 

#define ASCIIMODE 0
#define WORDMODE 1
#define ALGOLEASTBIT 0
#define ALGOPROB 1

 
/**
 * @brief analyzes the data of file to create
 *
 * @details It goes through a file and counts the occurence of a BYTE 
 * 
 * @param char *fileToCompress the name of the file that is analyzed for compression
 * @param BYTE_COUNT filedata[] variable that will have the information about the analyses of that file
 * @param totalByte the total no of byte that is to be returned 
 *
 * @return -2 file does not exist or could not be open
 * @return 0 sucess
 **/
int analyzeAscii(char *fileToCompress, BYTE_COUNT filedata[], unsigned int* totalByte)
{
  int returnValue = 0;
  FILE *inptr = fopen( fileToCompress, "r");
  if (inptr != NULL)
    {
      BYTE data;
      while ( fread(&data,sizeof(BYTE),1,inptr) )
	{
	     returnValue=addByteCount(data,filedata);
	     (*totalByte)++;
	}
    }
  else
    {
      return -2;
    }
  fclose(inptr);
  return returnValue;
}
/**
 * @brief returns bit on specified positon
 * 
 * @param no number in primary whose bit is to be found
 * @param pos postion whose bit is to be found
 * 

 * @return 0 if bit is 0
 * @return 1 if bit is 1
 */
int getbit(int no, int pos)
{
	return ((no & (1<<(pos-1)))==0)?0x00:0x01;
}
/**
 * @brief writes header of the comprezfile
 *
 * @parma outprt file in which we have to write header
 */
void writeheader(FILE *outptr)
{  
BYTE header=0x0;
  fwrite(&header,sizeof(BYTE),1,outptr);
  fwrite(&header,sizeof(BYTE),1,outptr);
} 
/**
 * @breif writes information about comprezfile for ascii analysis
 * @detail it writes sequentially bytes as per the occurence in the analyzed file in file pointed by outptr
 *
 * @param outptr file in which data is to be written
 * @param filedata sorted list of analyzed data
 * @param cdata the temporary data for fast grabbing of the position of ascii as per occurence in the file
 *
 * @return total no of unique byte in the file 
 */
int writeFileInfoForAscii(FILE *outptr,BYTE_COUNT filedata[],BYTE_COUNT cdata[])
{
  int i = 0, pos = 0;
  
  while (filedata[i].count!=0)
    {
      fwrite( &(filedata[i].b), sizeof(BYTE), 1, outptr);
      cdata[filedata[i].b].b = filedata[i].b;
      cdata[filedata[i].b].count = pos;
      pos++;
      i++;
    }
  return pos;
}
/**
 * @brief prepare BYTE and Write in the file 
 * @detail it prepares a BYTE of data to be written and writes in the given file
 * 
 * @param outptr file pointer where data is to be written
 * @param no the no which should be written into the file
 * @param noofbit the no of bit which each no is to be represented in file by if 0 it is just the no of 1 in the file   
 */
int prepareBYTEandWrite(FILE *outptr, int no, int noOfBit, int final)
{

  static BYTE wdata = 0x00;
  static int written = 7;
  // done for special case i.e last byte
  if ( final == 1)
    {
      if ( written != 7 )
	{
	  fwrite( &wdata, sizeof(BYTE), 1, outptr);
	}
      return 0;
    }
    
  int i;
  // printf("\ngot no:%d\n",no);
  if (noOfBit == 0)
    {
      for (i = 0; i < no; i++)
	{
	    if(written > -1 && written < 8)
	      {
		wdata += (1<<written);
		written--;
	      }
	    else if (written == -1)
	      {
	      	fwrite( &wdata, sizeof(BYTE), 1, outptr);
		// printf("  %x",wdata);
 		written = 7;
		wdata = 0x00;
		wdata += (1<<written);
		written--;
	      }
	    else
	      {
		return -3;
	      }
	    
	}
      if (written == -1)
	      {
	      	fwrite(&wdata,sizeof(BYTE),1,outptr);
		// printf("  %x",wdata);
		written = 7;
		wdata = 0x00;
	      }
	wdata += (0<<written);
		written--;	      
    }
  else
    {
      for (i = 0; i < noOfBit; i++)
	{
	  int bit = getbit(no, noOfBit-i);
	  // printf("%d",bit);
	  if(written > -1 && written < 8)
	    {
	      wdata += ( bit << written );
	      written--;
	    }
	 else if (written == -1)
	    {
	      fwrite(&wdata,sizeof(BYTE),1,outptr);
	      //printf("  %x  ",wdata);
	      written = 7;
	      wdata = 0x00;
	      wdata += (bit << written);
	      written--;
	    }
	  else
	    {
	      return -3;
	    }
	    

	}
    }
  return 0;
}

/**
 * @breif writes in file in compressed way
 * @detail it writes the compressed file as per the analyzed data of the file 
 * 
 * @param filedata sorted list of analyzed data
 * @param fileToBeRead name of the file that is to be compressed
 * @param fileToBeWritten name of the file that is to be created
 * @param algo 0 or 1 to specify algorithm to be used
 * @param totalByte the total byte of the file that is compressed
 *
 * @return 0 nomral execution
 * @return -1 cannot read file
 * @return -2 cannot create file
 * @return 1 compressed file is larger than original file   
 **/
int writeInFile(BYTE_COUNT filedata[], char *fileToBeRead, char *fileToBeWritten,char *compressedFileHeader, int algo, int totalByte)
{
  // opening files
  FILE *inptr,*outptr;
  inptr = fopen( fileToBeRead, "r");
  outptr = fopen( compressedFileHeader, "w");

  // checking for sucess
  if (inptr == NULL) return -1;
  if (outptr == NULL) return -2;
  
  // writing inital header
  {
  BYTE header = 0x00;
  fwrite( &header, sizeof(BYTE), 1, outptr);
  fwrite( &algo, sizeof(BYTE), 1, outptr);
  }
	
  // declaring data for quick compression
  BYTE_COUNT cdata[255];  
  
  // writing the initial data into file
  int pos = writeFileInfoForAscii(outptr,filedata,cdata);
  writeheader(outptr);
  
  // checking if the header information and the compressed file is same
  if (strcmp(fileToBeWritten,compressedFileHeader)!=0)
    {
      // closing and opening requried files
      fclose(outptr);
      outptr = fopen(fileToBeWritten,"w");
      if (outptr == NULL) return -3;
    }
  
  int noOfBit = 0;
	
  if (algo == 0)
    {
      
      // calcuating least no of bit that can be used for to represent all data in the file 
      noOfBit = ceil(log2(pos));
      
      // printf("\nNo of bit used is %d",noOfBit);
      fwrite( &(noOfBit), sizeof(int), 1, outptr);
      writeheader(outptr);
    
    }

  BYTE rdata = 0x00;
  while ( fread( &rdata, sizeof(BYTE), 1, inptr) != 0)
    {
      	prepareBYTEandWrite( outptr, cdata[rdata].count, noOfBit, 0);
    }
  
  // closing all the files
  // for any non written byte
  prepareBYTEandWrite( outptr, 0, noOfBit, 1);
  fclose(inptr);
  fclose(outptr);
  return 0;
}


/**
 * @breif writes information about comprezfile for word analysis
 * @detail it writes sequentially words as per the occurence in the analyzed file in file pointed by outptr
 *
 * @param outptr file in which data is to be written
 * @param wordlistInArray sorted list of word 
 * @param uniqueWord the total no of unique word in the compressed file
 *
 * @return total no of unique byte in the file 
 */
int writeFileInfoForWord( FILE *outptr, hashstruct* wordlistInArray[], int uniqueWord)
{
  int pos = 0;
  for( pos = 0; pos < uniqueWord; pos++)
    {
      char word[LENGTH];
      int noofchar = getWordBack( wordlistInArray[pos], word);
      int i = 0;
      while (word[i] != '\0')
	fputc(word[i++],outptr);
      char c = ' ';
      fputc( c,outptr);
      if ( wordlistInArray[pos] != NULL)
	{
	  (*(*wordlistInArray[pos]).count) = pos;
	  // printf("\ngot word %s and its position is %d",word,(*(*wordlistInArray[pos]).count));
	}
    }
  return pos;
}

/**
 * @breif writes in file in compressed way mode word
 * @detail it writes the compressed file as per the analyzed data of the file 
 * 
 * @param wordlist sorted list of word in the file to be compressed
 * @param wordlistInArray array of sorted list of word from the file to be compressed
 * @param fileToBeRead name of the file that is to be compressed
 * @param fileToBeWritten name of the file that is to be created
 * @param compressedFileHeader file name where header information is to be stored
 * @param algo 0 or 1 to specify algorithm to be used
 * @param uniqueWord the no of the unique word in the compressed file
 *
 * @return 0 nomral execution
 * @return -1 cannot read file
 * @return -2 cannot create file
 * @return 1 compressed file is larger than original file   
 **/
int writeInFileModeWord( hashstruct* wordlist, hashstruct* wordlistInArray[], char *fileToBeRead, char *fileToBeWritten, char *compressedFileHeader, int algo, int uniqueWord)
{
  // opening files
  FILE *inptr, *outptr;
  int returnValue;
  inptr = fopen(fileToBeRead,"r");
  outptr = fopen(compressedFileHeader,"w");
  
  // checking for sucess
  if (inptr == NULL) 
    {
      fprintf(stderr,"unable to read inputfile");
      return -1;
    }
  if (outptr == NULL)
    {
      fprintf(stderr,"unable to create outputfile %s\n",compressedFileHeader);
      return -2;
    }
  
  // writing inital header
  {
  BYTE header=0;
  fwrite(&header,sizeof(BYTE),1,outptr);
  fwrite(&algo,sizeof(BYTE),1,outptr);
  }
  
  printf("\nvalue of uniqueWord is %d",uniqueWord);
  fwrite(&uniqueWord, sizeof(int), 1, outptr);	
  
  // writing the initial data into file
  int pos = writeFileInfoForWord( outptr, wordlistInArray, uniqueWord);
  writeheader(outptr);
  
  // checking if the header information and the compressed file is same
  if (strcmp(fileToBeWritten,compressedFileHeader) != 0)
    {
      // closing and opening requried files
      fclose(outptr);
      outptr = fopen(fileToBeWritten,"w");
      if (outptr == NULL) return -3;
    }
  
  int noOfBit = 0;
	
  if ( algo == ALGOLEASTBIT)
    {
      // calcuating least no of bit that can be used for to represent all data in the file 
      noOfBit = ceil(log2(pos));
      printf("\n%d no of digit",noOfBit);
      fwrite( &(noOfBit), sizeof(int), 1, outptr);
      writeheader(outptr);
    }

  //prepareBYTEandWrite()
  char* word = malloc( LENGTH*sizeof(char) );
  while( getWord( inptr, word) != EOF)
    {
      int count = -1;
      returnValue = find(wordlist,word,&count);
      if (returnValue == 0)
	{
	  if (count < uniqueWord && count >= 0)
	    {
	      prepareBYTEandWrite( outptr, count, noOfBit, 0);
	    }
	  else
	    {
	      fprintf(stderr,"unexpected no of words %d\n",count);
	      return -5;
	    }
	}
      else
	{
	  return returnValue;
	}
    }
    prepareBYTEandWrite( outptr, 0, noOfBit, 1);
  free(word);
  fclose(inptr);
  fclose(outptr);
  return 0;

}
/**
 * @brief This function handles the actual task of compression and is to be called by either graphical or terminal program to compreess data.
 * 
 * @details This function takes two char * which are name of existing file to be compressed and newfile to be made and invokes all other function to compress the file
 *
 * @param *fileToCompress : file that is compressed
 * @param *compressedFileName : file that is created by the program
 * @param mode 0 for ascii else for word
 * @param algo 0 for to lower no of bits as per possible and 1 for to use lowest no of bit for highest occurence data
 *
 * @return 0: normal execution of program  
 **/
int comprez ( char *fileToCompress , char *compressedFileName, char*compressedFileHeader, int mode, int algo  )
{
  // setting returnValue to default
  int returnValue = 0;
  if (mode == ASCIIMODE)
  {
    // declaring data type
    int totalByte = 0;
    BYTE_COUNT filedata[255];

    // intialization of the data
    initBYTE_COUNT(filedata);

    // analyzing the file
    returnValue = analyzeAscii(fileToCompress ,filedata, &totalByte);
    if (returnValue != 0)
      {
	fprintf(stderr,"error in analyzeAscii");
	return returnValue;
      }
    printf("\nTOTAL BYTE OF THE FILE %u",totalByte);
    
    // sorting the analyzed data
    mergesortBYTECOUNT(filedata);
    
    // writing in the colmpressed file
    returnValue = writeInFile( filedata, fileToCompress, compressedFileName, compressedFileHeader, algo, totalByte);
    if (returnValue != 0)
      {
	fprintf(stderr,"error in writeInFile");
	return returnValue;
      }
    return returnValue;
  }
  else
  {
    // declaring data type
    hashstruct wordlist;
    int size = 0, uniqueWord = 0;
    
    // loading the data in wordlist
    returnValue = load( &wordlist, fileToCompress, &size, &uniqueWord);
    if (returnValue != 0)
      {
	fprintf(stderr,"error in load\n");
	return returnValue;
      }
    printf("size of the file is %d and uniqueword is %d", size, uniqueWord);

    // declaring data type 
    hashstruct* wordlistArray[uniqueWord];
    returnValue = initWordlistArray( wordlistArray, uniqueWord);
    // getting the array of words
    returnValue = getWordlistInArray( &wordlist, wordlistArray, uniqueWord);
    if (returnValue != 0)
      {
	fprintf(stderr,"error in getWordlistInArray");
	return returnValue;
      }
    // sorting the data
    returnValue = mergesortHashstruct( wordlistArray, uniqueWord);
    if (returnValue != 0)
      {
	fprintf(stderr,"error in mergesortHashstruct");
	return returnValue;
      }
    // writing in the file
    returnValue = writeInFileModeWord(&wordlist , wordlistArray, fileToCompress, compressedFileName, compressedFileHeader, algo, uniqueWord);
    if (returnValue != 0)
      {
	fprintf(stderr,"error in writeInFileModeWord");
	return returnValue;
      }
    return returnValue;
  }

}
/**
 * @brief reads no of digit in the file
 *
 * @param comprezFile file pointer from which noOfBit is to be read
 *
 * @return -1 unable to read noOfBit
 * @return noOfBit
 **/
int readnoOfBit (FILE *comprezFile)
{
  int noOfBit=0;
  if ( fread(&noOfBit,sizeof(int),1,comprezFile) != 0)
    return noOfBit;
  else
    return -1;
}
/**
 * @brief reads initial header of comprez file
 *
 * @param comprezFile file pointer from which initial header is to be read
 *
 * @return -1 error
 * @return algo
 **/
int readInitialHeader(FILE *comprezFile)
{
  BYTE header;  
  fread(&header,sizeof(BYTE),1,comprezFile);
  if(header != 0) return -1;
  BYTE algo=-1;
  fread(&algo,sizeof(BYTE),1,comprezFile);
  return algo;
}
/**
 * @brief escape header
 *
 * @param comprezFile file pointer 
 * @return 0 sucess
 *
 * @return -1 error or incompatible comprezFile 
 **/
int escapeheader(FILE *comprezFile)
{
  BYTE header;  
  fread(&header,sizeof(BYTE),1,comprezFile);
  if(header != 0)
  {
  	fprintf(stderr,"not vaild escape header %d",header);
   	return -1;
   }
  fread(&header,sizeof(BYTE),1,comprezFile);
  if(header != 0)
  {
  	fprintf(stderr,"not vaild escape header %d",header);
  	 return -1;
  }
  return 0; 
}
/**
 * @brief reads header in mode word
 * 
 * @param comprezFile file pointer which should be read
 * @param wordlist to keep all the words
 * @param words array of sorted list of word
 * @param uniqueWord no of unique word in the file
 *
 * @return 0 sucess
 **/
int readHeaderModeWord(FILE* comprezFile, hashstruct *wordlist, hashstruct* words[], int uniqueWord )
{
  // declaring required variable
  int i = 0;
  char word[LENGTH];
  int noOfWord = 0;
  // going through the file
  for (i = 0; i < uniqueWord; i++)
    {
      getWord( comprezFile, word);
      // declaring required variable
      hashstruct* back;
      // putting word in the wordlist
      hash_put( wordlist, word, &noOfWord, &back);
      // printf("got back %xd", (int)back);
      // putting word in array of wordlist
      words[i] = back;
      char word1[LENGTH];
      getWordBack(words[i],word1);
      if(strcmp(word,word1)!=0)
	{
	  fprintf(stderr,"\nword didnot match %s %s",word,word1);
	}
	
    }
  if (noOfWord == uniqueWord)
    {
      return 0;
    }
  else
    {
      fprintf(stderr,"noOfWord is not equal to uniqueWord");
      return -1;
    }
}
/**
 * @brief reads header for ascii analysis file
 *
 * @param comprezFile file pointer which should be read
 * @param cdata array to keep the ascii
 *
 * @return 0 
 **/
int readHeader(FILE *comprezFile, BYTE_COUNT cdata[])
{
  BYTE ascidata,ascidata1=1;

  int i=0;
  while(fread(&ascidata,sizeof(BYTE),1,comprezFile)!=0) 
    {
      if(ascidata==0)
	{
	  fread(&ascidata1,sizeof(BYTE),1,comprezFile);
	  if(ascidata1==0)
		break;
	  else
	    {
	      cdata[i].b=ascidata;
	      cdata[i].count=i;
	      i++;
	      cdata[i].b=ascidata1;
	      cdata[i].count=i;
	      i++;
	    }
	}
      else 
	{
	  cdata[i].b=ascidata;
	  cdata[i].count=i;
	  i++;
	}
		
    }
  while(i<255)
    {
      cdata[i].b=0;
      cdata[i].count=0;
      i++;
    }
  return 0;
}
/**
 * @brief display file for mode ASCII
 *
 * @param comprezFile file pointer whose data is to be read
 * @param noOfBit no of digit in which data is represented in compressed file 0 for probability mode
 * @param cdata array of header data
 * 
 * @return 0 sucess  
 **/
int displayfile(FILE *comprezFile,int noOfBit,BYTE_COUNT cdata[])
{
  // declaration of variable
  BYTE data;
  int no = 0;
  int positionToBeInterpreted = noOfBit;
  // looping through the whole file to decompress the data
  while (fread( &data, sizeof(BYTE), 1, comprezFile)!=0)
    {
      int noOfBitToRead = 8;
      if ( noOfBit > 0)
	{
	  // looping through till all the read data is interpreted
	  while( noOfBitToRead > 0)
	    {
	      // getting bit from the read data
	      int bit = getbit( data,noOfBitToRead );
	      // converting it to the its equivalent no 
	      no += bit * pow(2,positionToBeInterpreted-1);
	      // reducing no of digit read by 1 
	      noOfBitToRead--;
	      positionToBeInterpreted--;
	      // displaying when all required no of digit is read
	      if (positionToBeInterpreted == 0)
		{
		  printf( "%c", cdata[no].b);
		  // resetting no and positionToBeIntepreted
		  no = 0;
		  positionToBeInterpreted = noOfBit;
		}
	    }
	  /*if(no>255 || no<0)
	    {
	    fprintf(stderr,"\n no %d is out of range",no);
	    return -1;
	    }
	  */
	  /*if(cdata[no].b==0 && cdata[no].count==0)
	    {
	    fprintf(stderr,"\n no %d is out of range",no);
	    return -1;
	    }
	  */
	  
	  
	}
      else if (noOfBit == 0)
	{
	  // looping till all the read data is read
	  while(noOfBitToRead > 0)
	    {
	      // getting bit value
	      int bit = getbit(data,noOfBitToRead);
	      // generating required no and dispalying the data
	      if (bit == 1)
		{
		  no += 1;
		}
	      else
		{
		  printf( "%c", cdata[no].b);
		  no = 0;
		}
	      // reducing no of bit read by one
	      noOfBitToRead--;
	      
	    }
	}	
    }
  return 0;
}
/**
 * @brief display file for mode WORD
 *
 * @param comprezFile file pointer whose data is to be read
 * @param wordlist list of all the words in the compressed file 
 * @param words array of all the words in the compressed file
 * @param uniqueWord no of unique word in the compressed file
 * @param noOfBit no of digit in which data is represented in compressed file 0 for probability mode
 * 
 * @return 0 sucess
 **/
int displayfileModeWord(FILE *comprezFile, hashstruct* wordlist, hashstruct* words[],int uniqueWord, int noOfBit)
{
  BYTE data;
  int no = 0;
  int positonToBeInterpreted = noOfBit;
  while(fread(&data,sizeof(BYTE),1,comprezFile)!=0)
    {
      int noOfBitToBeRead = 8;
      if( noOfBit > 0)
	{
	  while (noOfBitToBeRead > 0)
	    {
	      int bit = getbit(data,noOfBitToBeRead);
	      no += bit * pow(2,positonToBeInterpreted-1);
	      noOfBitToBeRead--;
	      positonToBeInterpreted--;
	      if (positonToBeInterpreted == 0)
		{
		  char word[LENGTH];
		  if ( no > uniqueWord )
		    {
		      fprintf(stderr,"no is greater than uniqueWord %d %d",no,uniqueWord);
		      return -1;
		    }
		  if ( words[no] == NULL)
		    {
		      fprintf(stderr,"unexpexcted value of words[no]");
		      return -2;
		    }
		  getWordBack( words[no], word);
		  int i = 0;
		  while(word[i]!='\0')
		  putchar(word[i++]);
		  char c = ' ';
		  putchar(c);
		  no = 0;
		  positonToBeInterpreted = noOfBit;
		}
	    }
	  /*if(no>uniqueWord || no<0)
	    {
	    fprintf(stderr,"\n no %d is out of range",no);
	    return -1;
	    }
	  */
	  /*if(cdata[no].b==0 && cdata[no].count==0)
	    {
	    fprintf(stderr,"\n no %d is out of range",no);
	    return -1;
	    }
	  */
	}
      else if(noOfBit==0)
	{
	  int bit;
	  while (noOfBitToBeRead > 0 )
	    {
	      bit=getbit( data, noOfBitToBeRead);
	      if( bit == 1)
		{
		  no+= 1;
		}
	      else
		{
		  char word[LENGTH];
		  getWordBack(words[no],word);
		  printf("%s ",word);
		  no=0;
		}
	      noOfBitToBeRead--;
	      
	    }
	}	
    }
  return 0;
}
/**
 * @briefs decompresses the file
 * @detail 
 *
 * @param fileToBeDecomprez name of the file to be the decomprez
 * @param decomprezFileHeader name of the file that contains the header information
 * @param mode 0 or 1 signifying Ascii or Word
 *
 * @return 0 sucess 
 **/
int decomprez ( char *fileToBeDecomprez, char *decomprezFileHeader, int mode )
{
  // checking the mode of the decompressor 
  if( mode == ASCIIMODE )
    {

      BYTE_COUNT cdata[255];
      FILE* comprezFile = fopen( decomprezFileHeader, "r");
      if (comprezFile == NULL) return -1;

      int algo = readInitialHeader(comprezFile);
      readHeader(comprezFile, cdata);
      int noOfBit = 0;
      if (algo == ALGOLEASTBIT)
	{
	  
	  noOfBit = readnoOfBit(comprezFile);
	  escapeheader(comprezFile);
	}
      if (strcmp(fileToBeDecomprez,decomprezFileHeader)!=0)
	{
	  fclose(comprezFile);
	  FILE* comprezFile=fopen(fileToBeDecomprez,"r");
	  if (comprezFile==NULL) return -2;
	}
      displayfile(comprezFile,noOfBit,cdata);
      fclose(comprezFile);
      return 0;
    }
  else
    {
	// TODO
      FILE* comprezFile = fopen(decomprezFileHeader,"r");
      if (comprezFile == NULL)
	{
	  fprintf(stderr,"unable to open file %s\n",decomprezFileHeader);
	  return -1;
	 }
      int uniqueWord = 0;
      
      int algo = readInitialHeader(comprezFile);
      fread( &uniqueWord,sizeof(int),1,comprezFile);
      hashstruct wordlist;
      hashstruct_deafult_value( &wordlist, NULL , (char)0);
      hashstruct *words[uniqueWord];
      readHeaderModeWord( comprezFile, &wordlist, words, uniqueWord );
      escapeheader(comprezFile);
      int noOfBit = 0;
      if ( algo == ALGOLEASTBIT)
	{
	  noOfBit = readnoOfBit(comprezFile);
	  if ( noOfBit != ceil(log2(uniqueWord)) )
	    printf("noOfBit is found to be %d",noOfBit);
	  escapeheader(comprezFile);
	}
      
      if(strcmp(fileToBeDecomprez,decomprezFileHeader)!=0)
	{
	  fclose(comprezFile);
	  FILE *comprezFile = fopen(fileToBeDecomprez,"r");
	  if (comprezFile == NULL) return -2;
	}
      displayfileModeWord(comprezFile,&wordlist,words,uniqueWord,noOfBit);
      unload(&wordlist);
      fclose(comprezFile);
      return 0;
    }
}
