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

using namespace std;

#define PORT_SERVER     8292
#define NUM_OF_COMANDS  3

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
} *rspn;
/**
 * Pointer to char array that sets the commands that the server will respond
 * the last comand must be the exit comand
 * the server compare commands by the length, if client send {[CC] options}
 * the server match with [CC] because is the same initial command, this way support
 * to add aditional options, the full command that the client have sendend is passed by
 * buff on response function 
*/
const char *cmdArr[] = {"[CC]","[RTS]","[END]"};
/**
 * Response functions
 *
 * Return the array of char that the server will response
 * @param buff - is the buffer of 1024 char array,
 *  in buff the server pass the command that client have sended 
 *  you can see the command if the client have added additional params
*/
char * CCResponse(char *buff){
  buff = (char *)"OK";
  return buff;
}
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
