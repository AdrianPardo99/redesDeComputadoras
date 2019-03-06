/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 06/03/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
/*AF_PACKET packet(7)
  man 7 packet
  linea para compilar con BD de mysql
  gcc -I/usr/include/mysql/ crudo.c -L/usr/lib/mysql -lmysqlclient -o crudo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "apiSocket.h"
#include "libErrorApiSocket.h"
int main(int argc,char **argv){
  if(argc>1){
    if(argc>2){
      if(!memcmp("arpScanner",argv[1],10)&&!memcmp("sql",argv[2],3)){
        database=(char*)malloc(sizeof(char)*100);
        password=(char*)malloc(sizeof(char)*100);
        user=(char*)malloc(sizeof(char)*100);
        printf("%sIngresa el nombre de la base de datos a conectar:%s ",
          KMAG,KNRM);
        fflush(stdin);
        fgets(database,100,stdin);
        printf("%sIngresa el usuario de la base de datos:%s ",KBLU,KNRM);
        fflush(stdin);
        fgets(user,100,stdin);
        printf("%sPor favor ingresa la contraseña:%s ",KCYN,KNRM);
        fflush(stdin);
        fgets(password,100,stdin);
        *(user+(sizeChar(user)))='\0';
        *(database+(sizeChar(database)))='\0';
        *(password+(sizeChar(password)))='\0';
        initConnection();
        closeConnection();
        initScanARP(1);
        free(database);
        free(password);
        free(user);
      }else{
        printErrorParams();
      }
    }else{
      if(!memcmp("arp",argv[1],3)&&(sizeChar(argv[1])+1)==3){
        createARPTrama();
      }else if(!memcmp("arpScanner",argv[1],10)){
        initScanARP(0);
      }else if(!memcmp("interfaces",argv[1],10)){
        showInterface();
      }else{
        printErrorParams();
      }
    }
  }else{
    printErrorParams();
  }
  /*getSQLStruct();*/
  return 0;
  /*Valores de FLAGS
  IFF_UP=1
  IFF_BROADCAST=2
  IFF_DEBUG=4
  IFF_LOOPBACK=8
  IFF_POINTOPOINT=16
  IFF_RUNNING=64
  IFF_NOARP=128
  IFF_PROMISC=256
  IFF_NOTRAILERS=32
  IFF_ALLMULTI=512
  IFF_MASTER=1024
  IFF_SLAVE=2048
  IFF_MULTICAST=4096
  IFF_PORTSEL=8192
  IFF_AUTOMEDIA=16384
  IFF_DYNAMIC=32768*/
}
