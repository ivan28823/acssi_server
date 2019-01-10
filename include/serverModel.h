#ifndef SERVERMODEL_H_
#define SERVERMODEL_H_
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

extern struct response_stream *rspn;

/**
 * Pointer to char array that sets the commands that the server will respond
 * the last comand must be the exit comand
 * the server compare commands by the length, if client send {[CC] options}
 * the server match with [CC] because is the same initial command, this way support
 * to add aditional options, the full command that the client have sendend is passed by
 * buff on response function, after the end command, you have to  put in the other line 
 * a null or 0, indicate a null pointer
 */
static const char *cmdArr[] = {"[CC]","[RTS]","[END]",0};
/**
 * Response functions
 *
 * Return the array of char that the server will response
 * @param buff - is the buffer of 1024 char array,
 *  in buff the server pass the command that client have sended 
 *  you can see the command if the client have added additional params
 *  The first function must be the Unknown Command function
 */
char * unknownCommand(char *buff);
char * CCResponse(char *buff);
char * RTSResponse(char *buff);

#endif // SERVERMODEL_H_