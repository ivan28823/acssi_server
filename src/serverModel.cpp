#include "serverModel.h"
#include <stdio.h>


char * UnknowComand(char *buff) {
  sprintf(buff,"Unknow Command, try again");
  return buff;
}
char * CCResponse(char *buff){
  sprintf(buff,"OK");
  return buff;
}
char * RTSResponse(char *buff){
  sprintf(buff,"R:[%s|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f|%.4f]",rspn->name,rspn->temp,rspn->pres,rspn->hum,rspn->ppm_co,rspn->ppm_no2,rspn->ppm_so2,rspn->ppm_o3);
  return buff;
}
