/**
 * Author: Ivan Moreno
 * December 2018
 * License - https://www.gnu.org/licenses/gpl-3.0.en.html
 */
#ifndef ASCIISERVER_H_
#define ASCIISERVER_H_

#include <netinet/in.h>

#define BUFF_SER_LEN 1024
#define SERVER_DBG

class AsciiServer{
private:
  int port;             // port
  int sockFd;           // socket File Descriptor
  int clientFd;         // client File Descriptor
  char *buffer;         // buffer 
  int numOfComands;     // number of comands

  struct sockaddr_in serverAddr;    // struct from server addr
  struct sockaddr_in clientAddr;    // struct from client addr

  /**
   * Pointer to char array that sets the commands that the server will respond
   * the last comand must be the exit comand
   * the server compare commands by the length, if client send {[CC] options}
   * the server match with [CC] because is the same initial command, this way support
   * to add aditional options, the full command that the client have sendend is passed by
   * buffer on response function, the number of commands have to be sets in numOfComands
   */
  char **comandsArray;

  /**
   * Response functions
   *
   * Return the array of char that the server will response
   * @param {char *} - is the buffer of 1024 char array,
   *  in the buffer the server pass the command that client have sended 
   *  you can see the command if the client have added additional params
   */
  char *(**responseFunct)(char *);
  
  ////// Private functions
  /**
   * This function set the values of server struct
   */
  void setServerStruct(); 
  /**
   * Handle the connection, this is launched by parent
   * process, take the clientFd from class variable 
   */
  void handleConnection();

public:
  /**
   * First constructor
   */
  AsciiServer();
  /**
   * Second constructor
   * @param p - port to bind the socket
   */
  AsciiServer(int p);
  /**
   * Destroyer - clean dynamic variables 
   */
  ~AsciiServer();
  /**
   * Init the server on port p
   * Return true if is successfully false if has existed an error
   */
  int initServer();
  /**
   * Initialize the server, it's a infinite loop
   */
  void startServer();
  /**
   * Terminate server, close fd's
   */
  void closeServer();
  /**
   * Set the pointer to response function
   * @param ptr - Pointer to function type (char *(**funcname)(char *))
   */ 
  void setFunctions(char * (**ptr)(char *));
  /**
   * Set the port to bind the socket
   * @param p - port to bind
   */
  void setPort(int p);
  /**
   * Set the pointer to comand list
   * @param ptrArr - pinter to array of comands
   * @param numOfCmd - number of comands
   */
  void setComands(const char ** ptrArr,int numOfCmd);
};

#endif