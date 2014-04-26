/**
 * @file hashstruct.c
 * 
 * @authors shubhajeet creation
 * 
 * @brief Implements a hashstruct data structure.
 **/

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define COMMENT
/**
 * @brief HASH_STRUCT_LENGTH is the length of array inside hashstruct
 * @brief LENGTH is the length of buffer word
 **/
#define HASH_STRUCT_LENGTH 256
#define LENGTH 200
/**
 * @brief It implements the data structure hashlength. 
 * @details character pointer to child hashstruct. It is used to search word.
 * @details back pointer to parent hashstruct. It is used to get back the word.
 * @details b character represented by the hashsturct
 * @details count pointer to an int which stores the occurence of the word if the word ends in this hashstruct
 **/
typedef struct hashstruct
{
  struct hashstruct *character[ HASH_STRUCT_LENGTH ];
  struct hashstruct *back;
  char b;
  int *count;
} hashstruct;
/**
 * @brief gets word from a file
 *
 * @param inptr file from which word is to be taken
 * @param word variable where word is stored
 * @return no of char of the word 
 **/
int getWord(FILE* inptr, char word[])
{
  int noofchar = 0;
  int c = 0;
  if ( feof(inptr) )
    {
  	return EOF;
    }
  else
    {
  
      while ( (c = getc(inptr)) != EOF && c != ' ' )
	{
		if ( !(c > 0 && c < 256) )
			fprintf(stderr,"\nnot in char range"); 
		if (noofchar >= LENGTH)
		{
			fprintf(stderr,"\nbuffer memory overflow in getWord");
			break;
		}
    	      	word[noofchar++] = (char)c;
	}
              
      word[noofchar]='\0';
      return noofchar;
    }
}
/**
 * @brief initializes the hashstruct
 *
 * @param structdata pointer to the hashstruct data which is to be initialized
 * @param back pointer to the parent  
 * @param the character it represents
 * 
 * @returns 0 sucess
 * @returns -1 failure
 **/

int hashstruct_deafult_value(hashstruct *structdata,void *back,char b)
{
  // initialising the character array
  int i;
  for(i= 0; i < HASH_STRUCT_LENGTH; i++)
    {
      (*structdata).character[i] = NULL;
      if ( (*structdata).character[i] != NULL) return -1;
    }
  // initialising b
  (*structdata).b = b;
  // initialising back
  (*structdata).back = back;
  // initialising count
  (*structdata).count = NULL;
  return 0;
}

/**
 * @brief adds number of words by 1.
 */
void sizeadd(int *size)
{
      (*size)+=1;
}
/**
 * @brief maps the character to some value within HASH_LENGTH 
 */
int hash(unsigned char c)
{ 
  /*if ( c>= 'a' && c<='z')
    return c-'a';
    else if ( c>= 'A' && c<= 'Z')
    return c-'A';
    else if (c =='`')
    return 27;
    else
    return 28;*/
  if (c < HASH_STRUCT_LENGTH && c >= 0)
    return c;
  else
    {
      fprintf(stderr,"\nhash is not between 0 to 255 %d %d",c,c);
      return c;
    }
}
/**
 * @brief finds the word from the parent hashstruct
 * 
 * @param words pointer to parent hashstruct
 * @param word pointer to the word to be searched in the hashstruct
 * @param count pointer to int which is returned if the word is found
 * 
 * @return -1 word not found
 * @return -2 unable to return count
 * @return 0 sucess
 **/
int find( hashstruct *words, char* word, int *count)
{
  // temp pointer 
  hashstruct *pointer;
  pointer = words;
  // counter to loop through
  int i;
  for ( i = 0; i <= strlen(word) ; i++) // even search the null character at the end
    { 
      // get the HASH_STRUCT postion of the char
      int position = hash( word[i] );
      // check if the word is found and terminate the program if not
      if ((*pointer).character[position] == NULL)
 	{
	  fprintf(stderr,"\n%s doesnot exist \n", word);
	  fprintf(stderr,"character postion is %d and character is %d %c",i,word[i],position);
	  // *count = NULL;
	  return -1;
	}
      else
	{
	  pointer = (*pointer).character[position];
	  // printf("%c",(char)(position));
	}

    }
    if ((*pointer).count != NULL)
      { 
	*count = (*(*pointer).count);
	// printf("\n the count that is inside find is %d",*(*pointer).count);
	return 0;
      } 
      else
	return -2;
}
/**
 * @breif puts the word into the hashstruct 
 * 
 * @param words pointer to parent hashstruct where word is to be stored
 * @param word char array to be stored in the parent hashstruct
 * @param uniqueWord pointer to an int whose value is added by 1 if the word trying to be inserted is not already inserted
 * @param back pointer to the pointer of the children hashstruct of the word just put on the parent hashstruct. Can be used to get back the word 
 **/
int hash_put(hashstruct* words,char word[],int *uniqueWord, hashstruct **back)
{
  // declaring required variable
  hashstruct *pt = words;
  int i;
  if (pt == NULL)
    {
  	fprintf(stderr,"unexpected value of pt");
	return -1;
    }
  // printf("\n%s",word);
  // even put the final null character
  for( i = 0; i <= strlen(word); i++)
    {
      // get HASH_STRUCT LENGTH for the char
      int position = hash(word[i]);
      // create if not created 
      if ( (*pt).character[position] == NULL)
	{
	  // temporary var
	  hashstruct *new=(hashstruct *) malloc(sizeof(hashstruct));
	  // if intialization is sucessful 
	  if ( new != NULL && hashstruct_deafult_value( new, pt, word[i]) == 0)
	    {
	      (*pt).character[position] = new;
	      pt = new;
	    }
	  else
	    {
	      fprintf(stderr,"unable to hash put");
	      return -2;
	    }			
	}
      else
	{
	  pt=(*pt).character[position];
	}
    }
  // create count if not created
  if ( (*pt).count == NULL )
    {
      int *new = (int *)malloc(sizeof(int));
      if (new != NULL)
	{
	  (*pt).count = new;
	  // setting count to 1
	  (*(*pt).count) = 1;
	  // adding no of uniqueWord by 1
	  (*uniqueWord) += 1;
	}
      else
	{
	  fprintf(stderr,"\nunable to malloc count"); 
	}
    }
  else
    {
      // incrementing count by 1
      (*((*pt).count))++;
    }
    // printf(" inserted with back %x\n",pt);
  /*
	char word1[LENGTH];
	getWordBack(pt,word1);
	if(strcmp(word,word1) == 0)
	printf("match");
	else
	{
	printf("word got back is %s",word1);
	return -1;
	}*/
    *back = pt;
  return 0;
 
}
/**
 * @brief Loads all the word of the file into memory.
 * 
 * @param wordlist pointer to parent hashstruct
 * @param file name of the file whose word should be loaded
 * @param size pointer to an int which denotes the no of word in the file
 * @param uniqueWord no of unique word in the file
 * 
 * @return 0 sucess
 * @return -1 failure
 */
int load(hashstruct *wordlist, char* file, int* size, int* uniqueWord)
{
  // initialising wordlist
  hashstruct_deafult_value( wordlist, NULL , (char)0);
  // declaring word
  char word[LENGTH];
  // char* word = (char *) malloc(LENGTH*sizeof(char));
  // opening the file
  FILE* fp = fopen(file,"r");
  // proceding if the file is succesfully opened
  if ( fp != NULL )
    {
      while ( getWord(fp,word) != EOF )//fscanf(fp,"%s",word) != EOF)
	{
	  hashstruct *back;
	  int returnValue = hash_put( wordlist, word, uniqueWord, &back);
	  if (returnValue != 0) return returnValue;
	  sizeadd( size);
	}
    }
  else
    {
      fprintf(stderr,"unable to open file");
      return -1;
    }
  fclose(fp);
  //free(word);
  return 0;
}



/**
 * @brief helper fuction of unload.
 * @details recurrsively frees all the memory allocated for the hashstruct
 * 
 * @param pt pointer to hashstruct
 *
 * @return 0 sucess
 * @return -1 failure
 */
int freehashstruct(hashstruct *pt)
{
  // declaration of var
  int i;
  int returnValue = 0;
  // going through all the child hashstruct
  for ( i = 0; i < HASH_STRUCT_LENGTH; i++ )
    {
      // if child is allocated recurssively dealocating child hashstruct
      if ((*pt).character[i] != NULL)
	{
	  returnValue = freehashstruct((*pt).character[i]);
	  if (returnValue != 0)
	    {
	      fprintf(stderr,"Unable to free hashstruct");
	      return returnValue;
	    }
	}
    }
  // dealocating count if was allocated
  if ( (*pt).count != NULL)
  {
    free((*pt).count);
    (*pt).count = NULL;
  }
  // dealocating parent hashstruct
  free(pt);
  pt = NULL;
  return returnValue;
}
/**
 * @brief frees all the memory allocated by hashstruct
 *
 * @param wordlist parent hashstruct
 **/
int unload(hashstruct * wordlist)
{
  int i, returnvalue;
  for ( i = 0; i < HASH_STRUCT_LENGTH; i++ )
    {
      if((*wordlist).character[i]!=NULL)
	{		
	  returnvalue = freehashstruct((*wordlist).character[i]);
	}
    }
  if ( (*wordlist).count != NULL)
  {
    free((*wordlist).count);
    (*wordlist).count = NULL;
   }
  return returnvalue;
}
/**
 * @breif initializes wordlistarray
 **/
int initWordlistArray(hashstruct **wordlist, int size)
{
  int i = 0;
  for ( i = 0; i < size; i++)
    *(wordlist+i) = NULL;
  return 0;
}

/**
 * @brief creates array of pointer of words in the list
 *
 * @param wordlsit parent wordlist
 * @param wordlistArray array of the child wordlist
 * @param size the upper bound of the wordlist  
 **/
int getWordlistInArray(hashstruct *wordlist, hashstruct* wordlistArray[], int size)
{
  //TODO
  int i, returnvalue = 0;
  static int count = 0;
  for ( i = 0; i < HASH_STRUCT_LENGTH; i++ )
    {
      if ( (*wordlist).character[i] != NULL)
	{		
	  returnvalue = getWordlistInArray((*wordlist).character[i],wordlistArray,size);
	}
    }
  if ( (*wordlist).count != NULL )
    {
      if(count < size)
	{
	  wordlistArray[count++] = wordlist;
	}
      else
	{
	  fprintf(stderr,"wordlistArray out of bound");
	  return -1;
	}
    }
  return returnvalue;
}
/**
 * @brief recurssively sorting helper to wordlist
 *
 * @param wordlistArray the array of child hashstruct to be sorted
 * @param start the initial value of the array
 * @param end the final value of the array
 *
 * @return 0 sucess
 * @return -1 failure 
 **/ 
int rmsortHashstruct(hashstruct* wordlistArray[], int start, int end)
{
  if (end<=start) // base case
    {
      return 0;
    }
  else
    {
      //declaraition of required variable
      int returnValue, middle;
      middle=(start+end)/2;
      // recursively sorting 
      returnValue=rmsortHashstruct(wordlistArray,start,middle);
      returnValue=rmsortHashstruct(wordlistArray,middle+1,end);
	
      // merging two sorted list
      hashstruct *temp[end-start+1];
      int position = 0;
      int position1 = start, position2=middle+1;
      while (position1 < (middle+1) || position2 < (end+1))
	{
	  // checking if the all the value of 1st sorted list has been inserted
	  if (position1 > middle)
	    {
	      temp[position] = wordlistArray[position2];
	      position2++;
	    }
	  // checking if all the value of 2nd sorted list has been inserted
	  else if(position2>end)
	    {
		temp[position] = wordlistArray[position1];
		position1++;
	    }
	  else
	    {
	      // checking which has greater no of count
	      if( *((*wordlistArray[position2]).count) > *((*wordlistArray[position1]).count) )
		{
		  temp[position] = wordlistArray[position2];
		  position2++;
		}
	      else
		{
		  temp[position]=wordlistArray[position1];
		  position1++;
		}
	    }
	  position++;
	}
      // writing all the value back to the initial list
      int i;
      for ( i = 0; i < (end-start+1); i++)
	{
	  wordlistArray[start+i] = temp[i];
	}
      return 0;
    }
  
}
/**
 * @breif sorts hashstruct as pre the occurence of the word
 *
 * @param wordlistArray array of the child word 
 * @param size of the array 
 **/
int mergesortHashstruct(hashstruct *wordlistArray[],int size)
{
  //TODO
  if ( wordlistArray[size-1] == NULL )
    return mergesortHashstruct(wordlistArray,size-1);
  else
     return rmsortHashstruct(wordlistArray,0,size-1);
}
/**
 * @brief recursively get the word back
 * 
 * @param wordpt back pointer to the hashstruct
 * @param word the word which is returned
 * @param wordsize no of char in the word
 *
 * @return 0 sucess
 * @return -1 failure   
 **/
int rgetWordBack(hashstruct* wordpt,char word[],int *wordsize)
{
  //TODO
  if ( (*wordpt).back == NULL ) //base case
    {
      if ((*wordpt).b != '\0')
	word[(*wordsize)++]=(*wordpt).b;
    }
  else
    {
      // recursively get the word
      rgetWordBack((*wordpt).back,word,wordsize);
      word[(*wordsize)++] = (*wordpt).b;
    }
  return 0;
}
/**
 * @breif gets word back by using child hashstruct
 *
 * @param wordpt child hashstruct
 * @param word 
 *
 * @return no of character in the word
 **/
int getWordBack(hashstruct* wordpt,char word[])
{
  int wordsize = 0;
  if (wordpt == NULL)
  {
   fprintf(stderr,"cannot pass null hashword pointer");
   return -1;
  }
  rgetWordBack( wordpt, word, &wordsize);
      word[wordsize] = '\0';
  return wordsize;
}

