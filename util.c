/**
 * @file util.c
 *
 * IOE (Institute of Enginnering)
 * Pulchowk Campus
 * Department of Electronics and Computer Science
 *
 * 2070 BEX 1st year 1st semester
 * Project on C
 *
 * ABOUT THIS PROJECT
 * Recovers JPEGs from a forensic image.
 * Compresses file using comprez algorithm
 *
 * @author A shubhajeet and CCR creation
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recover.h"
#include "comprez.h"
/**
 * @brief main terminal interface for util toolkits
 *
 * @details it checks whether all the input is supplied or not and then executes command as per users instruction
 *
 * @param -v verbosity
 * @param -i image Filename from which jpeg files are to be recovered
 * @param -p filepath where recovered jpeg files are to be created
 * @param -c file to be compressed
 * @param -f the compressed file that is to be created
 * @param -d the file to be decompressed
 * @param -m mode of analysis i.e word or ascii
 * @param -a algorithm i.e. leastbit (lb) or probability(p)
 *
 * @return 1 not enough command line arguments
 *
 **/
int main ( int argc, char* argv[] )
 {
   int returnValue = 0, counter = 0, verbosity = 0 ;
   int mode = 0, algo = 0; 
   char* fileName = NULL, *filePath=NULL;
   char *fileToCompress = NULL, *compressedFileName = NULL, *decomprezFileName=NULL;
   // loops through the whole set of command line input and checks for appropriate input
   
   if(argc == 1)
     {
       fprintf(stderr,"not a valid syntax");
       return -1;
     }
   
   for ( counter = 1 ; counter < argc; counter++ )
     {

	 if (strcmp( argv[counter] , "-v") == 0) 
	   {
	     // enables verbosity
	     verbosity = 1;
	   }
	else if ( strcmp( argv[counter] , "-p") == 0 )
	  {
	    // assigns filePath for extraction of recovered jpeg files
	    filePath = argv[++counter];
	    if ( verbosity != 0 )
	      {
		printf( "FilePath: %s \n", filePath );
	      }
	  }
	else if ( strcmp( argv[counter] , "-i" ) == 0 || strcmp( argv[counter], "--image" ) == 0)
	   {
	     fileName = argv[++counter];
	     if ( verbosity != 0 )
	       {
		 printf( "Image fileName: %s \n" , fileName );	
		}
	   }
	else if ( strcmp( argv[counter] , "-c") == 0 )
	  {
	    // assigns filePath for extraction of recovered jpeg files
	    fileToCompress = argv[++counter];
	    if ( verbosity != 0 ) 
		printf( "FileToCompress: %s \n", fileToCompress );
	  }
	else if ( strcmp( argv[counter] , "-f" ) == 0 || strcmp( argv[counter], "--file" ) == 0)
	  {
	    compressedFileName = argv[++counter];
	    if ( verbosity != 0 )
	      {
		printf( "Compressed file name: %s \n" , compressedFileName );	
	      }
	
	  }
	else if ( strcmp( argv[counter] , "-d" ) == 0 || strcmp( argv[counter], "--decomprez" ) == 0)
	  {
	    decomprezFileName = argv[++counter];
	    if ( verbosity != 0 )
	      {
		printf( "File to be decomprez: %s \n" , decomprezFileName );	
	      }
	  }
	else if ( strcmp( argv[counter], "-a") == 0 || strcmp( argv[counter], "--algo" ) == 0)
	  {
		counter++;
	    if ( strcmp( argv[counter], "lb") == 0 || strcmp( argv[counter], "leastbit" ) == 0)
	      {
		algo = ALGOLEASTBIT;
		   if ( verbosity != 0 )
		     {
		       printf( "Algorithm used: least Bit \n" );	
		     }
	      }
	    else if ( strcmp( argv[counter], "p") == 0 || strcmp( argv[counter], "prob" ) == 0)
	      {
		algo = ALGOPROB;
		if ( verbosity != 0 )
		  {
		    printf( "Algorithm used: probability \n" );	
		  } 
	      }
	    else
	      {
		fprintf(stderr,"syntax error expected lb or leastbit or p or prob after -a or --algo\n");
	      }
	    
	 
	    
	  }
	 
	else if ( strcmp( argv[counter], "-m") == 0 || strcmp( argv[counter], "--mode" ) == 0)
	  {
		counter++;
		if ( strcmp( argv[counter], "w") == 0 || strcmp( argv[counter], "word" ) == 0)
		  {
		mode = WORDMODE;
		if ( verbosity != 0 )
		  {
		       printf( "Compressing by using word analysis \n" );	
		  }
		  }
		else if ( strcmp( argv[counter], "a") == 0 || strcmp( argv[counter], "ascii" ) == 0)
		  {
		    mode = ASCIIMODE;
		    if ( verbosity != 0 )
		      {
			printf( "Compression by using ascii character analysis \n" );	
		      } 
		  }
		else
		  {
		    fprintf(stderr,"syntax error expected W or WORD or A or ASCII after -M or --MODE\n");
		  }
		
		
	  }
     }
   if ( fileName != NULL )
     {
       if ( filePath != NULL)
	 returnValue = recover( fileName , filePath );
       else
	 {
	   filePath = (char *)malloc(sizeof(char)*5);
	   filePath = "./";
	   returnValue = recover( fileName , filePath );
	   free(filePath);	   
	 }
     }
   else if( fileToCompress != NULL )
     {
       returnValue = comprez( fileToCompress , compressedFileName ,compressedFileName,mode,algo);
     }
   else if (decomprezFileName != NULL)
     {
       returnValue= decomprez(decomprezFileName,decomprezFileName,mode);
     }
   else
     {
       returnValue = -1;
     }
   return returnValue;
 }


