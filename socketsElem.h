/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 06/02/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>

typedef int socketRaw;
typedef struct ifreq interfaces;
typedef unsigned char datos;
typedef char name;
typedef int dato;
typedef short tinyDato;
typedef struct sockaddr_ll tramas;
/*Uso de variables globales para trabajar más adelante con cada libreria de la
api creada*/
socketRaw packet_socket;

datos macOrigen[6],ipO[4],mascara[4],
macBroadcast[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},ethertype[2]={0x0C,0x0C},
tramaEnv[1514];

dato mtuO,metricO,des;

name *cpHardware,binFlags[100];

tinyDato flags;

interfaces nic;
/*Función que realiza algo similar a strlen*/
int sizeChar(char *arr){
  return (arr)?(0):(1+sizeChar(arr+1));
}

/*Muestra los valores que recibe la interfaz de red para encender o apagar*/
void showModos(){
  printf("Valores que toma la interfaz si esta encendida: %d Hex: %X\n"
  "Valores que toma la interfaz si esta apagada: %d Hex: %X\n"
  ,IFF_UP,IFF_UP,~IFF_UP,~IFF_UP);
}

/*Función que analiza si el resultado es un numero*/
dato isNumber(name x){
  return (x>='0'||x<='9')?(1):(0);
}
