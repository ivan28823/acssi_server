/**
 * This is a server based in ascii characters,
 * to use you have to define the comands and each command
 * must to have a response funtion
 * 
 * Author: Ivan Moreno
 * December 2018
 * License - https://www.gnu.org/licenses/gpl-3.0.en.html
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include "AsciiServer.h"

using namespace std;

#define PORT_SERVER     8292
#define NUM_OF_COMANDS  3

struct response_stream *rspn;
/**
 * Struct from stream server response
*/
struct response_stream{
  char* name;
  float temp;
  float pres;
  float hum;
  float ppm_co;
  float ppm_no2;
  float ppm_so2;
  float ppm_o3;
};
/**
 * Pointer to char array that sets the commands that the server will respond
 * the last comand must be the exit comand
*/
const char *cmdArr[] = {"[CC]","[RTS]","[END]"};
char * CCResponse(char *buff){
  printf("%s\n",buff);
  return (char *)"OK";
}
/**
 * Response functions
*/
char * RTSResponse(char *buff){
  sprintf(buff,"R:[%s|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f]",rspn->name,rspn->temp,rspn->pres,rspn->hum,rspn->ppm_co,rspn->ppm_no2,rspn->ppm_so2,rspn->ppm_o3);
  return buff;
}
char * (*ptrFunc[2])(char *) = {CCResponse,RTSResponse};

int main(int argc, char const *argv[]) {  
  // for shared memory btw child and parent
  rspn = (response_stream *) mmap(NULL,sizeof(response_stream),PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,0,0);

  AsciiServer server(PORT_SERVER);
  server.setFunctions(ptrFunc);
  server.setComands(cmdArr,NUM_OF_COMANDS);
  

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
    for(;;){
      rspn->name = (char *)"TST";
      rspn->temp = (float)(rand()%10);
      rspn->pres = (float)(rand()%10);
      rspn->hum = (float)(rand()%10);
      rspn->ppm_co = (float)(rand()%10);
      rspn->ppm_no2 = (float)(rand()%10);
      rspn->ppm_so2 = (float)(rand()%10);
      rspn->ppm_o3 =  (float)(rand()%10);
      sleep(1);
    }
  }

  server.closeServer();
  return 0;
}
