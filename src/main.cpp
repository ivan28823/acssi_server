/**
 * This is a server based in ascii characters,
 * to use you have to define the comands and each command
 * must to have a response funtion
 * 
 * Author: Ivan Moreno
 * December 2018
 * License - https://www.gnu.org/licenses/gpl-3.0.en.html
 * 
 * This program is sharing multiple variables to multiple clients.
 * The server run in a process and another process is changing the value of these variable
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include "AsciiServer.h"
#include "serverModel.h"

using namespace std;

#define PORT_SERVER     8292

struct response_stream *rspn;

int main(void) {  
  // for shared memory btw child and parent
  rspn = (response_stream *) mmap(NULL,sizeof(response_stream),PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,0,0);

  char * (*ptrFunc[3])(char *) = {unknownCommand,CCResponse,RTSResponse};

  AsciiServer server(PORT_SERVER);
  server.setFunctions(ptrFunc);
  server.setComands(cmdArr);
  

  pid_t childPID = fork();
  if(childPID == (pid_t)0){
    /*Child process*/
    if(server.initServer()){
      printf("Connection cannot be established!!\n");
      exit(0);
    }
    server.startServer();
  }else {
    /*Parent process*/
    printf("Init Child process pid = %d\n",(int)childPID);
    rspn->name = (char *)"TST";

    for(;;){
      rspn->temp    = (float)(rand()%10);
      rspn->pres    = (float)(rand()%10);
      rspn->hum     = (float)(rand()%10);
      rspn->ppm_co  = (float)(rand()%10);
      rspn->ppm_no2 = (float)(rand()%10);
      rspn->ppm_so2 = (float)(rand()%10);
      rspn->ppm_o3  =  (float)(rand()%10);
      sleep(1);
    }
  }

  server.closeServer();
  return 0;
}
