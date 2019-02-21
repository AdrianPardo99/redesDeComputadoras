/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 16/02/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
/*AF_PACKET packet(7)
  man 7 packet
  linea para compilar con BD de mysql gcc -I/usr/include/mysql/ crudo.c -L/usr/lib/mysql -lmysqlclient -o crudo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "apiSocket.h"
int main(int argc,char **argv){
  /*printf("Iteracion de solo crear socket, mostrar socket y cerrar socket\n");
  showOnlySocket();
  printf("\nIteracion de solo crear socket, mostrar socket, muestra"
  "interfaz de red y cerrar socket");
  showInterface();*/
  //createARPTrama();
  if(argc>1){
    if(argc>2){
      initScanARP(1);
    }else{
      initScanARP(0);
    }
  }else{
    createARPTrama();
  }
  //initScanARP();
  //createChatSockets();
  //initARPWaitingForTrama();
  //listenTrama();
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
