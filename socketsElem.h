/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 21/02/2019
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
#include <sys/time.h>
#include <netinet/in.h>

typedef int socketRaw;
typedef struct ifreq interfaces;
typedef unsigned char datos;
typedef char name;
typedef int dato;
typedef short tinyDato;
typedef struct sockaddr_ll tramas;
typedef struct timeval timer;
/*Uso de variables globales para trabajar más adelante con cada libreria de la
api creada*/
socketRaw packet_socket;

datos macOrigen[6],
ipO[4],
mascara[4],
macBroadcast[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
ethertype[2]={0x0C,0x0C},
tramaEnv[1514],tramaGet[1514],
macCompa[6]={0xD8,0xCB,0x8A,0xD2,0x85,0x03},
macProfe[6]={0xd8,0xcb,0x8a,0xd5,0x7f,0x2f},
*messages,
etherARP[2]={0x08,0x06},
ipD[4],
macD[6],
ARP_ETH[2]={0x08,0x06},
ARPAns[2]={0x00,0x02},
ARPAnsI[2]={0x00,0x09},
ARPReq[2]={0x00,0x01},
ARPReqI[2]={0x00,0x09},
opARP[2],
IPP[2]={0x08,0x00},
typeHar[2]={0x00,0x01},
lonP[2]={0x06,0x04},
ipPrueba[15],
*red,
*sub;

dato mtuO,
metricO,
des,
num,
indice,
ARPPetition,
ban=0,
contador=0;

name *cpHardware,
binFlags[100],*nameSSID;

tinyDato flags;

interfaces nic;

timer start,end;

long mtime=0,seconds=0,useconds=0;

/*Función que realiza algo similar a strlen*/
int sizeChar(char *arr){
  return (*arr)?(1+sizeChar(arr+1)):(-1);
}

/*Función que analiza si el resultado es un numero*/
dato isNumber(name x){
  return (x>='0'||x<='9')?(1):(0);
}

/*Función que analiza si el procedimiento (socket) contiene algun error*/
socketRaw isAnError(socketRaw sock){
  return sock == -1;
}

/*Función que compara sí es mi mac*/
int isMyMAC(datos *trama){
  return !(memcmp(trama+6,macOrigen+0,6));
}

/*Función que compara si es el ethertype es ARP*/
int isARP(datos *trama){
  return !(memcmp(trama+12,ARP_ETH+0,2));
}

/*Función que verifica si es Respuesta de ARP*/
int isAnswerARP(datos *trama){
  if(!(memcmp(trama+20,ARPAns+0,2))){
    return 0;
  }else if(!(memcmp(trama+20,ARPAnsI+0,2))){
    return 1;
  }else{
    return -1;
  }
}

/*Función que verifica si es la IP */
int isTheIP(datos *trama){
  return !(memcmp(trama+28,ipD+0,4));
}

/*Función que verifica si es mi IP*/
int isMyIP(datos *trama){
  return !(memcmp(trama+38,ipO+0,4));
}

/*Función que verifica si es mi ip*/
int isMyMACArp(datos *trama){
  return !(memcmp(trama+32,macOrigen+0,6));
}


/*Función que verifica si es una solicitud ARP*/
int isRequestARP(datos *trama){
  if(!(memcmp(trama+20,ARPReq+0,2))){
    return 0;
  }else if(!(memcmp(trama+20,ARPReqI+0,2))){
    return 1;
  }else{
    return -1;
  }
}

/*Función que verifica si son los destinos*/
int isMacOrIpDest(datos *trama){
  return (!(memcmp(trama+22,macD+0,6)))||(!(memcmp(trama+28,ipD+0,4)));
}

/*Función que verifica si la mac destino es ARP*/
int isBroadcastMac(datos *trama){
  return !(memcmp(trama+0,macBroadcast+0,6));
}

/*Función que converte una cadena de caracteres al valor hexadecimal que necesita la IP*/
void charToIPHex(char *cad){
  struct in_addr addr;
  int i,j,k=0,n=0,ipC;
  char cen[3];
  if (inet_aton(cad, &addr) == 0) {
    fprintf(stderr, "Invalid address\n");
    close(packet_socket);
    exit(EXIT_FAILURE);
  }

  j=sizeChar(inet_ntoa(addr));
  for(i=0;i!=j+1;i++){
    if(*(inet_ntoa(addr)+i)==0x2e){
      sscanf(cen,"%d",&ipC);
      *(ipD+n)=ipC;
      n++;
      *(cen)=0;
      *(cen+1)=0;
      *(cen+2)=0;
      k=0;
    }else{
      *(cen+k)=*(inet_ntoa(addr)+i);
      k++;
    }
  }
  sscanf(cen,"%d",&ipC);
  *(ipD+n)=ipC;
}

int isFilterArp(datos *trama,int tam){
  return isARP(trama)&&isMyIP(trama)&&isMacOrIpDest(trama)&&tam>=42;
}
