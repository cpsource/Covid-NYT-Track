#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

struct state {
  struct state *next;
  char *state;
  int check_cnt;
  unsigned int population;
};

struct results {
  struct results *next;
  char *key;
  int cases;
  int dead;
  unsigned int population;
};

struct state *list = NULL;

struct results*last_result = NULL;
struct results *results_list = NULL;

void place_results_at_end ( struct results *r );

struct results *find_results ( char *key )
{
  struct results *n = results_list;
  char *tmp = strdup(key);
  char *c;

  c = strchr(tmp,','); if ( c ) *c = 0;
  
  while ( n ) {
    if ( !strcmp(tmp,n->key) ) {
      free(tmp);
      return n;
    }
    
  nxt:;
    n = n->next;
  }

  n = malloc(sizeof(struct results));
  n->key = strdup(tmp);
  place_results_at_end(n);
  
  free(tmp);
  return n;
}

void place_results_at_end ( struct results *r )
{
  if ( last_result ) {
    last_result->next = r;
    last_result = r;
  } else {
    results_list = last_result = r;
  }
  r->next = NULL;
  r->cases = 0;
  r->dead = 0;
  r->population = 0;
}

unsigned int getpop(char *state)
{
  static char *buf = NULL;
  char searcher[128];
  char worker[128];
  char *src,*dst;
  char *c;
  int sts;
  
  if ( !buf ) {
    int fd = open("PopulationsByState.txt",O_RDONLY);
    buf = malloc(8000);
    memset(buf,0,8000);
    sts = read(fd,buf,7999);
    close(fd);
  }

  sprintf(searcher,",%s,",state);

  c = strstr(buf,searcher);
  if ( !c ) {
    fprintf(stderr,"state population not found: %s\n",state);
    return 0;
  }
  
  src = strchr(c,"\""[0]);
  if ( !src ) { fprintf(stderr,"no quote found\n"); return 0; }
  src += 1;
  dst = worker;
  while ( *src && *src != '"' ) {
    if ( *src == ',' ) { src += 1; continue; };
    *dst++ = *src++;
  }
  *dst = 0;
  return (unsigned int)atoi(worker);
}

void get_state_list(char *states_file)
{
  FILE *inf = fopen(states_file,"r");
  char wbuf[132];
  char *c;
  struct state *s;
  
  if ( NULL == inf ) {
    fprintf(stderr,"get_state_list: can't find %s\n",states_file);
    exit(0);
  }

  while ( fgets(wbuf,132,inf) ) {

    c = strchr(wbuf,'\n'); if ( c ) *c = 0;
    if ( 0 == strlen(wbuf) ) continue;
    if ( wbuf[0] == '#' ) continue; // skip commenbts
    s = malloc(sizeof(struct state));
    s->state = strdup(wbuf);
    s->check_cnt = 0;
    s->population = getpop(wbuf);
    s->next = list;
    list = s;
  }
  
  fclose(inf);
}

int find_state(char *s, struct state **r_n )
{
  struct state *n = list;
  char *c1;
  
  while ( n != NULL ) {
    
    c1 = strchr(s,','); if ( !c1 ) goto nxt;
    c1 += 1;

    //printf("c1 = <%s>\n",c1);
    //printf("state = <%s>\n",n->state);
    
    if ( !strncmp(c1,n->state, strlen(n->state) ) ) { n->check_cnt += 1; *r_n = n; return 1;}
    
  nxt:;
    n = n->next;
  }

  return 0;
}

void check_check_cnt()
{
  struct state *n = list;

  while ( n ) {
    if ( !n->check_cnt ) fprintf(stderr,"State %s never found\n",n->state);
    n = n->next;
  }
}

int main(int argc, char *argv[])
{
  char wbuf[132];
  FILE *inf = fopen ("covid-19-data/us-states.csv","r");
  char *c;
  char *c0,*c1,*c2,*c3,*c4;
  struct results *r;
  char states_file[32] = "dstates.txt";
  struct state *r_n;
  
  if ( !inf ) {printf("can't open us-states.csv\n"); exit(0); }

  if ( argc > 1 ) {
    states_file[0] = argv[1][1];
  }

  if ( 0 ) {
    if ( argc > 1 && argv[1][0] == '-' && argv[1][1] == 'r' ) states_file[0] = 'r';
    if ( argc > 1 && argv[1][0] == '-' && argv[1][1] == 'g' ) states_file[0] = 'g';
    if ( argc > 1 && argv[1][0] == '-' && argv[1][1] == 'm' ) states_file[0] = 'm';
  }
  
  get_state_list(states_file);
  
  while ( fgets(wbuf,132,inf) ) {
    c = strchr(wbuf,'\n'); if ( c ) *c = 0;

    // is it our state ???
    if ( find_state(wbuf,&r_n) ) {
      
      //printf("found %s\n",wbuf);

      c0 = wbuf;
      c1 = strchr(wbuf,',');
      c2 = strchr(c1+1,',');
      c3 = strchr(c2+1,',');
      c4 = strchr(c3+1,',');

      *c1 = *c2 = *c3 = *c4 = 0;

      r = find_results(c0);
      r->cases += atoi(c3+1);
      r->dead += atoi(c4+1);
      r->population += r_n->population;
      
    }

  } // while

  // show results
  r = results_list;
  while ( r ) {

    printf("%s,%d,%d,%d\n",r->key,r->population,r->cases,r->dead);
    r = r->next;
  }
  
  check_check_cnt();

  fclose(inf);
  return 0;
}
