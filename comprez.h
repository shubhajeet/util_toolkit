/**
 * @authors shubhajeet creation
 * 
 * @file comprez.h
 * 
 * @briefs A header file for comprez
 *
 */
#define ASCIIMODE 0
#define WORDMODE 1
#define ALGOLEASTBIT 0
#define ALGOPROB 1
extern int comprez ( char *fileToCompress , char *compressedFileName, char *compressedFileHeader, int mode, int algo  );
extern int decomprez ( char *fileToBeDecomprez, char *decomprezFileHeader, int mode );

