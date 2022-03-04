/* ----------------------------------------------------- */
/* wordfind.c                                            */
/* Liest die Eingabedatei ein,                           */
/* sucht nach den Vorkommen des eingegebenen Wortes      */
/* und gibt die Anzahl in einer Ausgabedatei aus         */
/* ----------------------------------------------------- */

#include <time.h>
#include <fcntl.h>
#include <ctype.h>
#include <conio.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#ifndef byte
   #define byte unsigned char
#endif
#define OR   ||
#define AND  &&
#define NUL  ('\0')

#define EINGABEDATEI argv[1]
#define TEMPDATA "temp.tmp"
char* mystrlwr (char *string);
char* mystrupr (char *string);
long dateilaenge(FILE *infile);
int strstr_count (const char *string, const char *substring);
/*------------------------------*/
/*        MAIN ANFANG           */
/*------------------------------*/
int main(int argc, char **argv)
{
   FILE *infile, *outfile, *tempfile;
   int z;
   double zcount=0;           /* Z�hler f�r die Anzahl der Vorkommen */

   char zpuff[256];        /* Tempor�rer Speicher f�r die Zeilen. */
   char outfilename[32];
   char *ptr,*ptr1;

/* ---------------------------------------------------------------------- */
/* HINWEISS                                                               */
/* ---------------------------------------------------------------------- */

   if(argc < 3)
   {
      fprintf(stderr,"\n\nSyntax: wordfind <zu durchsuchende Datei> <zu suchendes Wort/Zeichen>\n\n");
      exit(1);
   }

/* ---------------------------------------------------------------------- */
/* DATEIEN OEFFNEN                                                        */
/* ---------------------------------------------------------------------- */
	if ( (infile=fopen(EINGABEDATEI,"rb")) == NULL ) 
	{
		printf("ERROR open INPUT-File: %s",EINGABEDATEI);
		exit(1);
	}
	if ( (tempfile=fopen(TEMPDATA,"wb")) == NULL ) 
	{
		printf("ERROR open TEMP-File: %s",TEMPDATA);
		exit(1);
	}
   while (1)
   {
      z = fgetc(infile);
      if(!feof(infile)){
         putc(tolower(z), tempfile);
      }else break;
   }
   fclose(tempfile);
   fclose(infile);  

	strcpy(outfilename, EINGABEDATEI);

   if(strstr(outfilename,".")) {
	   ptr=outfilename+strlen(outfilename)-1;
	   while((*ptr)&&(*ptr!='.')) ptr--;
	   strcpy(ptr, ".txt");
   }
   else {
	   ptr1=outfilename+strlen(outfilename);
	   strcpy(ptr1, ".txt");
   }

	if ( (outfile=fopen(outfilename,"wb")) == NULL ) 
	{
		printf("\nERROR open OUTPUT-File: %s\n", outfilename);
		exit(1);
	}
	if ( (tempfile=fopen(TEMPDATA,"rb")) == NULL ) 
	{
		printf("ERROR open TEMP-File: %s",TEMPDATA);
		exit(1);
	}

/* ---------------------------------------------------------------------- */
/* Zeilenweise aus der Datei einlesen und Ausgabe generieren              */
/* ---------------------------------------------------------------------- */

   while(1)
   {
      if(fgets(zpuff, sizeof(zpuff), tempfile) == 0) break;

      if(strstr(zpuff,mystrlwr(argv[2])))
      {
         if(strstr_count(zpuff,mystrlwr(argv[2])) != 1)
         {
            zcount+=strstr_count(zpuff,mystrlwr(argv[2])); 
         }else zcount++;
      }
   }
   fprintf(outfile,"Die Zeichenfolge\"%s\" %.0lf mal in %s gefunden\r\r",mystrupr(argv[2]), zcount, EINGABEDATEI);
   fprintf(outfile,"Die Dateilaenge von %s betraegt %ld Byte\r", EINGABEDATEI, dateilaenge(tempfile));

	fclose(tempfile);
	fclose(outfile);
        remove(TEMPDATA);
	return(0);
}
/*--------------------------*/
/*        ENDE MAIN         */
/*--------------------------*/
int strstr_count(const char *string, const char *substring) {
   int i, j, k, count=0;
   for(i = 0; string[i]; i++)
      for(j = i, k = 0; string[j] == substring[k]; j++, k++)
         if(!substring[k+1])
            count++;
   return(count);
} 
long dateilaenge(FILE *infile) {
   long curpos, laenge;
   curpos = ftell(infile);
   fseek(infile, 0L, SEEK_END);
   laenge = ftell(infile);
   fseek(infile, curpos, SEEK_SET);
   return laenge;
}
char* mystrlwr (char *string) {
char *p=string;
   for (p=string; *p; p++)
      *p = tolower(*p);
   return string;
}
char* mystrupr (char *string) {
char *p=string;
   for (p=string; *p; p++)
      *p = toupper(*p);
   return string;
}
