/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 06/03/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/

#include "socketsElem.h"
#include "colors.h"
#include "initSockets.h"
#include "hardwareInfo.h"
#include "getSend.h"
#include "arpStruct.h"
#include "sqlLib.h"
#include "timer.h"
#include "freeARP.h"
#include "arpFile.h"
#include "freeARPInfractor.h"

/*Función que envia mensajes*/
void estructuraMensaje(datos *trama,datos *message){
  memcpy(trama+0,macProfe+0,6);
  memcpy(trama+6,macOrigen+0,6);
  memcpy(trama+12,ethertype+0,2);
  memcpy(trama+14,message+0,sizeChar(message));
}

/*Función que estructura trama de ethernet*/
void estructuraTrama(datos *trama){
  memcpy(trama+0,macBroadcast+0,6);
  memcpy(trama+6,macOrigen+0,6);
  memcpy(trama+12,ethertype+0,2);
  memcpy(trama+14,"Adrian Gonzalez Pardo",sizeChar("Adrian Gonzalez Pardo"));
}

/*Función que enciende o apaga la interfaz de red*/
void interfazOnOf(socketRaw ds){
  printf("Ingresa un numero para:\n1.-Encender\n2.-Apagar\n");
  scanf("%d",&des);
  if(des==1){
    nic.ifr_flags |= (IFF_UP|IFF_RUNNING);
  }else if(des==2){
    nic.ifr_flags &= ~IFF_UP;
  }
  if(ioctl(ds, SIOCSIFFLAGS, &nic)==-1){
    printf("Error al realizar la operación\n");
  }else{
    printf("Exito al realizar la operación\nValor: %d\n",nic.ifr_flags);
  }
}

/*Función que muestra el número de socket*/
void showSocketNumber(){
  printf("Numero de socket: %d\n",packet_socket);
}


/*Función que sirve para enviar y recibir mensajes por medio de sockets (chat)*/
void chatSocket(){
  while (1) {
    messages=(char*)malloc(sizeof(datos)*400);
    fflush(stdin);
    printf("Escribe un mensaje: ");
    fgets(messages,400,stdin);
    if(!memcmp(messages+0,"exit",4)){
      printf("Salir :3\n");
      estructuraMensaje(tramaEnv,messages);
      enviaTrama(packet_socket,tramaEnv,indice);
      recibeMensaje(packet_socket,tramaGet);
      free(messages);
      break;
    }else{
      estructuraMensaje(tramaEnv,messages);
      enviaTrama(packet_socket,tramaEnv,indice);
      recibeMensaje(packet_socket,tramaGet);
      free(messages);
    }
  }
}

/*Función que recibe tramas de red con filtro de mac*/
void getNTrama(){
  printf("Cuantas tramas quieres capturar?\nIngresa el numero\n");
  scanf("%d", &num);
  recibeTrama(packet_socket,tramaGet,num);
}

/*Función que crea una trama a nivel de broadcast de prueba*/
void createBroadcastTrama(){
  estructuraTrama(tramaEnv);
  enviaTrama(packet_socket,tramaEnv,indice);
}

/*Función que itera las funciones de obtener datos de hardware de red*/
void showInterface(){
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    showSocketNumber();
    obtainHardwareData();
    closeSocket();
  }
}

/*Función que permite ser un escucha de tramas*/
void listenTrama(){
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    obtainHardwareData();
    getNTrama();
    closeSocket();
  }else{
    perror("Error al abrir la aplicación en función listenTrama\n");
  }
}

/*Función que solo crea un socket y lo cierra*/
void showOnlySocket(){
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    showSocketNumber();
    closeSocket();
  }
}

/*Función que intancia el chat con sockets 7u7*/
void createChatSockets(){
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    showSocketNumber();
    obtainHardwareData();
    chatSocket();
    closeSocket();
  }else{
    perror("Error al abrir la función createChatSockets");
  }
}
