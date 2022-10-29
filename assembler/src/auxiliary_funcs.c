#include "interpreter.h"

/* 
    Checks if two strings are the same 
    and returns a boolean true/false 
*/
bool strsame(char* str1, char* str2)
{
    if (strcmp(str1, str2) == 0){
        return true;
    }
    return false;
}

/* Function taken from Prof. Neill Campbell */
void* nfopen(char* fname, char* mode)
{
   FILE* fp;
   fp = fopen(fname, mode);
   if(!fp){
      on_error("Cannot open file");
   }
   return fp;
}

/* Function taken from Prof. Neill Campbell */
void* ncalloc(int n, size_t size)
{
   void* v = calloc(n, size);
   if(v==NULL){
      on_error("Cannot calloc() space");
   }
   return v;
}

/* Function taken from Prof. Neill Campbell */
char** n2dcalloc(int h, int w, size_t szelem)
{

   int i;
   char** p;
   p = calloc(h, sizeof(void*));
   if(p==NULL){
      on_error("Cannot calloc() space");
   }
   for(i=0; i<h; i++){
      p[i] = calloc(w, szelem);
      if(p[i]==NULL){
         on_error("Cannot calloc() space");
      }
   }
   return p;
}

/* Function taken from Prof. Neill Campbell */
void n2dfree(char**p, int h)
{
   int i;
   for(i=0; i<h; i++){
      free(p[i]);
   }
   free(p);
}

/* Function taken from Prof. Neill Campbell */
void on_error(const char* s)
{
   fprintf(stderr, "%s\n", s);
   exit(EXIT_FAILURE);
}

/* Initialises program structure to hold tokens */
void init_program(program* prog)
{
    prog->tokens = (char**) n2dcalloc(MAXLINES, MAXCHARS, sizeof(char));
    prog->total_lines = 0;
    prog->curr_word = 0;
}

/* 
    A tokenizer that imports a *.ttl file, 
    throwing an error if file is invalid or is too big 
*/
void import_ttl(program* prog, char* filename, int n_args)
{
    int j;
    FILE* fp;

    if (!prog){
        on_error("Invalid program pointer");
    }
    
    if (n_args != REQ_ARGS){
        on_error("Usage: ./extension filepath");
    }
    
    fp = nfopen(filename, "r");

    j = 0;
    while (fscanf(fp, "%s", prog->tokens[j++]) == VALID_SCAN){
        if (j > MAXLINES){
            on_error("Maximum filesize exceeded");
        }
        prog->total_lines++;
    }
    fclose(fp);
}

/* Free program structure and calloc'd 2D array */
void free_program(program* prog)
{
    if (!prog){
        on_error("Invalid program pointer");
    }
    
    n2dfree(prog->tokens, MAXLINES);
    free(prog);
}
