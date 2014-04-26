/**
 * @file recover.c
 *
 * @author ccr creation
 * @author IOE (Institute of Enginnering)
 * @author Pulchowk Campus
 * @author Department of Electronics and Computer Science
 * @author 2070 BEX 1st year 1st semester
 * 
 * @brief Recovers JPEGs from a forensic image.
 *
 **/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // for mkdir
#define BLOCK 512
// synoname of byte 
typedef uint8_t  BYTE;
/**
 * @brief header structure for any file 
 */
typedef struct
{
  BYTE a;
  BYTE b;
  //BYTE c;
  //BYTE d;
}__attribute__((__packed__)) HEADER;
/**
 * @brief a data structure that contains information about the file aka extension, starting header, eof header
 */
typedef struct
{
  HEADER starting;
  HEADER eof;
  char extension[5];
}__attribute__((__packed__)) FILETYPE;
/**
 *
 * @brief initializes the jpeg signatures
 *
 * @param file (FILETYPE) info about file
 * @return no of fileinfo
 */
int initFileType(FILETYPE *file)
{ 	
 	(*file).starting.a = 0xff;
 	(*file).starting.b =0xd8;
 	//jpeg.starting.c =0xff;
 	//jpeg.starting.d =0xe0;
 	//header2.a = 0xff;
 	//header2.b = 0xd8;
 	//header2.c = 0xff;
 	//header2.d = 0xe1;
	(*file).eof.a=0xFF;
	(*file).eof.b=0xD9;
	(*file).extension[0]='j';
	(*file).extension[1]='p';
	(*file).extension[2]='e';
	(*file).extension[3]='g';
	(*file).extension[4]='\0';

	return 1;
}
/**
 * @brief gets new file pointer
 * @detail it creates a new file name and returns with 0 if it was sucessfully created
 *
 * @param outptr pointer to a file returned
 * @param outPath (char *) path where file is created
 * @return 0 sucess
 * @return 3 could not create file
 */
int getNewFileptr(FILE **outptr,FILETYPE *file, char *outPath)
{
  static int dirNo=0, fileNo=0;
  char outFile[50];
  if(dirNo==0)
  {
  	 sprintf( outFile, "%s/tmp%02d", outPath, dirNo );
	 mkdir(outFile,0777);
  }
  
  sprintf( outFile, "%s/tmp%02d/%03d.%s", outPath, dirNo, fileNo, (*file).extension );
  // open output file and closes if file is opened
  if (*outptr!=NULL) fclose(*outptr);
  *outptr = fopen(outFile, "w");
  // checks if it was sucessful to create the file
  if (*outptr == NULL)
  {
  	fprintf(stderr, "Could not create %s.\n", outFile);
  	return 3;
  }
  // creates a new dir if fileNo is greater than 100
  if ( fileNo > 100 )
  {
    dirNo++;
    sprintf( outFile, "%s/tmp%02d", outPath, dirNo );
    mkdir(outFile,0777);
    fileNo=0;
  }
  else
  {
    fileNo++;
  }
  return 0;
}
 		
/**
 * @brief writes in file
 * @detail writes in file till it get eof header
 * 
 * @param outptr (FILE *) where data is written
 * @param data[] the data which should be written
 * @param file the information about ther file 
 * @return 0 sucess
 */
int write( FILE **outptr, BYTE data[], FILETYPE *file, int start,char *outpath)
{
  int i;
  int end=BLOCK;
  for( i = start; i < (BLOCK-1) ; i++)
    {
      if (data[i]==(*file).eof.a && data[i+1]==(*file).eof.b)
	{
	  end=i+2;
	  fwrite(&data[start],sizeof(BYTE),end-start,*outptr);
	  *outptr=NULL;
	  return 0;
	}
      //check if the file is actually overwritten
      if (data[i]==(*file).starting.a && data[i+1]==(*file).starting.b && ftell(*outptr)!=0 )
	{
	   end=i;

	  fwrite( &data[start], sizeof(BYTE), end-start, *outptr);
	  *outptr=NULL;
	  if ( getNewFileptr(outptr, file, outpath)==0 )
	    {
	      start= end;
	      end=BLOCK;
	    }
	  else
	    { 
	      *outptr=NULL; 
	      return 3;
	    }
	  
	}
      	
    }
   
   fwrite(&data[start],sizeof(BYTE),end-start,*outptr);
  return 0;
}
/**
 * @brief takes image file of memory card and tries to recover jpeg file in given path
 * @detail It takes an image file of memory card. Go through the memory card byte by byte to see if we can detect header information for jpeg file. If a header information is found then it stats to store it until EOF header is reached
 * 
 * @param inFile (char*) memory card image filename
 * @param outPath (char*) path where jpeg files are to be created
 *  
 * @return 0 sucess
 * @return 1 image file is not inputed
 * @return 2 couldnot read image file or image file doesnot exist
 * @return 3 couldnot create file
 **/
int recover ( char* inFile, char* outPath )
 {
 	// ensure input is given
 	if ( inFile == NULL )
 	{
		fprintf(stderr,"Recover: image file is not inputed \n");
	        return 1;
 	}

	// open input file
	FILE* inptr = fopen(inFile, "r");
	if (inptr == NULL)
	{
        	fprintf(stderr,"Could not open %s.\n", inFile);
        	return 2;
        }
	FILETYPE jpeg;
        initFileType(&jpeg);

        BYTE data[BLOCK];
        FILE* outptr=NULL;
        // loops until all the data is read
        while( fread(&data,sizeof(BYTE),BLOCK,inptr)==BLOCK )
        {
	  if(outptr==NULL)
	    {
	      int i=0;
	      for(i=0;i<(BLOCK-1);i++)
		{
        	// checks if header matches
		  if ( data[i]==jpeg.starting.a && data[i+1]==jpeg.starting.b )
		    {
		      int returnValue=getNewFileptr( &outptr, &jpeg, outPath);
		      if(returnValue!=0)
			{
		      	  return returnValue;
			}
			
			  write(&outptr, data, &jpeg ,i, outPath);//write to the file from where jpeg starts
		   
		      i=BLOCK;
		    }
	 	}
	    }
	     else
	       {
	       
	       write(&outptr,data,&jpeg,0,outPath);// write to a file from the begining
	      	}

        }
        // close file if not closed
        if (outptr!=NULL)
        fclose(outptr);
        fclose(inptr);
        return 0;
 }
