#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
typedef struct dict {
  char *word;
  int count;
  struct dict *next;
} dict_t;

char *
make_word( char *word ) {
  return strcpy( malloc( strlen( word )+1 ), word );
}
FILE *infile1;
pthread_mutex_t lock;
 dict_t *wds = NULL;
dict_t *
make_dict(char *word) {
  dict_t *nd = (dict_t *) malloc( sizeof(dict_t) );
  nd->word = make_word( word );
  nd->count = 1;
  nd->next = NULL;
  return nd;
}

dict_t *
insert_word( dict_t *d, char *word ) {
  
  //   Insert word into dict or increment count if already there
  //   return pointer to the updated dict
  
  dict_t *nd;
  dict_t *pd = NULL;		// prior to insertion point 
  dict_t *di = d;		// following insertion point
  // Search down list to find if present or point of insertion
  while(di && ( strcmp(word, di->word ) >= 0) ) { 
    if( strcmp( word, di->word ) == 0 ) { 
      di->count++;		// increment count 
      return d;			// return head 
    }
    pd = di;			// advance ptr pair
    di = di->next;
  }
  nd = make_dict(word);		// not found, make entry 
  nd->next = di;		// entry bigger than word or tail 
  if (pd) {
    pd->next = nd;
    return d;			// insert beond head 
  }
  return nd;
}

void print_dict(dict_t *d) {
  while (d) {
    printf("[%d] %s\n", d->count, d->word);
    d = d->next;
  }
}

int
get_word( char *buf, int n, FILE *infile) {
  int inword = 0;
  int c;  
  while( (c = fgetc(infile)) != EOF ) {
    if (inword && !isalpha(c)) {
      buf[inword] = '\0';	// terminate the word string
      return 1;
    } 
    if (isalpha(c)) {
      buf[inword++] = c;
    }
  }
  return 0;			// no more words
}

#define MAXWORD 1024
void *
words( void *args ) {
 // dict_t *wd = NULL;
  char wordbuf[MAXWORD];
  pthread_mutex_lock(&lock);
    while( get_word( wordbuf, MAXWORD, infile1 ) ) {
    wds = insert_word(wds, wordbuf); // add to dict
  }
  pthread_mutex_unlock(&lock);
    
 // return wd;
}
/*
void * threading(void* arg){
  /*FILE* infile = (FILE*)arg;//
  char wordbuf[MAXWORD];// return wd;//char *f = (char* )arg;// char* e = f;
  while( get_word( wordbuf, MAXWORD, infile ) ) {
    if(dg==NULL){
      dg=make_dict(wordbuf);  
    }else{ 
      dg = insert_word(dg, wordbuf); // add to dict
    }
    }*/
/*
  //pthread_mutex_lock(&lock);
 wds= words(infile1);
 //pthread_mutex_unlock(&lock);
}*/
int
main( int argc, char *argv[] ) {
  dict_t *d = NULL;
 infile1  = stdin;
  if (argc >= 2) {
    infile1 = fopen (argv[1],"r");
  }
  if( !infile1 ) {
    printf("Unable to open %s\n",argv[1]);
    exit( EXIT_FAILURE );
  }
                                                                                     //char y=getc(infile);
                                                                                     //char *g;
  int t;
  pthread_t tid[4];                                                                                  // while(fscanf(infile,"%ms",&g)!

for(t=0;t<4;t++){                                                             //  pthread_attr_t attr;// pthread_attr_init(&attr); // if(strcmp(&g,EOF)!=0){
    pthread_create(&tid[t],NULL,&words,NULL);
}

for(t=0;t<4;t++){    
    pthread_join(tid[t],NULL);  
}    
  print_dict( wds );
  fclose( infile1 );
}

