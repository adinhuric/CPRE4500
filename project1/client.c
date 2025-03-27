#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <tirpc/rpc/rpc.h>
#include "project.h"

#define MAX_LEN 100
CLIENT *rpc_setup(char *host);
long get_response(void);
void date(CLIENT *clnt, long *option);
void cpu(CLIENT *clnt);
void memory(CLIENT *clnt);
void process(CLIENT *clnt);

int main(int argc, char **argv)
{
  CLIENT *clnt;  
  char *server;  
  long response;

  if(argc != 2)
  {
    fprintf(stderr, "usage:%s hostname\n", argv[0]);
    exit(1);
  }

  server = argv[1];

  if((clnt = rpc_setup(server)) == 0)
    exit(1);	 

  response = get_response();
  while(response!=5)
  {
    switch(response)
    {
      case 1:
        date(clnt, &response);
      break;
      case 2:
        cpu(clnt);
      break;
      case 3:
        memory(clnt);
      break;
      case 4:
        process(clnt);
      break;
    }
    response = get_response();
  }

  clnt_destroy(clnt);
  exit(0);
}

CLIENT *rpc_setup(char *server)
{
  CLIENT *clnt = clnt_create(server,DATE_PROG,DATE_VERS,"udp");
  if(clnt == NULL)
  {
    clnt_pcreateerror(server);
    return(0);
  }
  return(clnt);
}

long get_response()
{
  long choice;
  printf("1. Current system time\n");
  printf("2. CPU usage\n");
  printf("3. Memory usage\n");
  printf("4. Load procs per minute\n");
  printf("5. Exit\n");
  scanf("%ld", &choice);
  return(choice);
}

void date(CLIENT *clnt, long *option)
{


  char** sresult;
  if ((sresult = get_date(option, clnt)) == NULL) 
  { 
    return;
  }
  printf("%s\n\n",*sresult); 
}

void memory(CLIENT *clnt)
{
  double* sresult;
  if ((sresult = get_memory(NULL,clnt)) == NULL) 
  { 
    return;
  }
  printf("RAM usage: %lf %% \n\n",*sresult);
}

void process(CLIENT *clnt)
{
  double* sresult;
  if ((sresult = get_load_procs(NULL,clnt)) == NULL) 
  { 
    return;
  }
  int res = (int)*sresult;
  printf("Load procs per minute: %d\n\n",res);
}

void cpu(CLIENT *clnt)
{
  double* sresult;
  if ((sresult = get_cpu(NULL,clnt)) == NULL) 
  { 
    return;
  }
  printf("CPU usage: %lf %% \n\n",*sresult);
}


