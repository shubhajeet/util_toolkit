#define HASH_STRUCT_LENGTH 256
#define LENGTH 200
typedef struct hashstruct
{
  struct hashstruct *character[ HASH_STRUCT_LENGTH ];
  struct hashstruct *back;
  char b;
  int *count;
} hashstruct;
int getWord(FILE* outptr, char word[]);
int find(hashstruct *words,char* word, int *count);

int load(hashstruct *wordlist,char* file, int* size, int* uniqueWord);
int unload(hashstruct * wordlist);
int mergesortHashstruct(hashstruct *wordlistArray[],int size);
int getWordlistInArray(hashstruct *wordlist,hashstruct* wordlistArray[], int size);
int getWordBack(hashstruct* wordpt,char word[]);

