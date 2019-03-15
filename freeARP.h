/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 10/03/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/

/*Función que permite analizar si la MAC que tiene la trama esta en la
  estructura de ARP*/
ARPStruct initSearchMAC(ARPStruct arp,datos *ma){
  int i;
  ARPStruct arpCen=arp;
  while(!isEmpty(arpCen)){
    if(!memcmp(ma+0,getMAC(arpCen),6)){
      return arpCen;
    }
    arpCen=nextHost(arpCen);
  }
  return NULL;
}

/*Función que realiza la busqueda de una IP en la trama de datos*/
ARPStruct initSearchIP(ARPStruct arp,datos *IP){
  int i;
  ARPStruct arpCen=arp;
  while(!isEmpty(arpCen)){
    if(!memcmp(IP+0,getIP(arpCen),4)){
      return arpCen;
    }
    arpCen=nextHost(arpCen);
  }
  return NULL;
}

/*Función que envia la trama que espera el Infractor*/
void estructuraTramaARPFree(datos *trama){
  memcpy(trama+0,macInfractor+0,6);
  memcpy(trama+6,macDefensor+0,6);
  memcpy(trama+12,etherARP+0,2);
  memcpy(trama+14,typeHar+0,2);
  memcpy(trama+16,IPP+0,2);
  memcpy(trama+18,lonP+0,2);
  memcpy(trama+20,arpFreeOp+0,2);
  memcpy(trama+22,macDefensor+0,6);
  memcpy(trama+28,ipDefensor+0,4);
  memcpy(trama+32,macInfractor+0,6);
  memcpy(trama+38,ipDefensor+0,6);
}

/*Función que envia una solicitud de ARP gratuito a nivel de broadcast*/
void estructuraTramaARPFreeTwo(datos *trama){
  memcpy(trama+0,macBroadcast+0,6);
  memcpy(trama+6,macDefensor+0,6);
  memcpy(trama+12,etherARP+0,2);
  memcpy(trama+14,typeHar+0,2);
  memcpy(trama+16,IPP+0,2);
  memcpy(trama+18,lonP+0,2);
  memcpy(trama+20,arpFreeOp+0,2);
  memcpy(trama+22,macDefensor+0,6);
  memcpy(trama+28,ipDefensor+0,4);
  memcpy(trama+32,macComodin+0,6);
  memcpy(trama+38,ipDefensor+0,4);

}

/*Función que escucha las tramas en la red*/
void initListenerARPFree(socketRaw sock, datos *trama,ARPStruct arp){
  int tam;
  ARPStruct arp1,arp2;
  arp1=arp2=NULL;
  while(1){
    tam=recvfrom(sock,trama,1514,MSG_DONTWAIT,NULL,0);
    if(!isAnError(tam)){
      if(isBroadcastMac(trama)&&isARP(trama)&&isIPFree(trama)){
        arp1=initSearchMAC(arp,trama+6);
        printf("%sTrama recibida\n%sIP Destino: %d.%d.%d.%d%s\n",
          BBLU,BMAG,*(trama+38),*(trama+39),*(trama+40),*(trama+41),KNRM);
        imprimeTramaARP(trama,tam);
        if(isEmpty(arp1)){
          arp2=initSearchIP(arp,trama+38);
          if(isEmpty(arp2)){
            printf("%sIP Not Match: No hay nada que hacer%s\n",KRED,KNRM);
          }else{
            printf("%sEstructurando defensa%s\n",KGRN,KNRM);
            macDefensor=getMAC(arp2);
            ipDefensor=getIP(arp2);
            *arpFreeOp=0x00;
            *(arpFreeOp+1)=0x02;
            memcpy(macInfractor+0,trama+6,6);
            estructuraTramaARPFree(tramaEnv);
            enviaTrama(packet_socket,tramaEnv,indice);
            imprimeTramaARP(tramaEnv,60);
            printf("%sEnviando solicitud ARP Gratuito%s\n",BYEL,KNRM);
            *arpFreeOp=0x00;
            *(arpFreeOp+1)=0x01;
            estructuraTramaARPFreeTwo(tramaEnv);
            enviaTrama(packet_socket,tramaEnv,indice);
            imprimeTramaARP(tramaEnv,60);
          }
        }else{
          printf("%sMisma MAC en la base de datos%s\n",KYEL,KNRM);
          if(memcmp(trama+38,getIP(arp1),6)){
            arp2=initSearchIP(arp,trama+38);
            if(isEmpty(arp2)){
              printf("%sIP Not Match: No hay nada que hacer%s\n",KRED,KNRM);
            }else{
              printf("%sEstructurando defensa%s\n",KGRN,KNRM);
              macDefensor=getMAC(arp2);
              ipDefensor=getIP(arp2);
              *arpFreeOp=0x00;
              *(arpFreeOp+1)=0x02;
              memcpy(macInfractor+0,trama+6,6);
              estructuraTramaARPFree(tramaEnv);
              enviaTrama(packet_socket,tramaEnv,indice);
              imprimeTramaARP(tramaEnv,60);
              printf("%sEnviando solicitud ARP Gratuito%s\n",BYEL,KNRM);
              *arpFreeOp=0x00;
              *(arpFreeOp+1)=0x01;
              estructuraTramaARPFreeTwo(tramaEnv);
              enviaTrama(packet_socket,tramaEnv,indice);
              imprimeTramaARP(tramaEnv,60);
            }
          }
        }
        arp1=arp2=NULL;
        printf("\n\n");
      }
    }
  }
}

/*Función que inicializa el servidor de arpFree*/
void initServerARPFree(ARPStruct arp){
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    obtainHardwareData();
    if(!memcmp(ipO+0,ipARPFree+0,4)){exit(1);}
    printf("%sServer iniciado%s\n",BYEL,KNRM);
    initListenerARPFree(packet_socket,tramaGet,arp);
    closeSocket();
  }else{
    printf("Error al abrir la aplicacion en las funciones de arpFree\n");
    free(database);
    free(user);
    free(password);
    freeStruct(arp);
    exit(1);
  }
}

/*Función que permite crear la instancia a la BD y crea la estructura de ARP*/
void getSQLStruct(){
  ARPStruct arp=empty(),arpGet;
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
  nameSSID=(char *)malloc(sizeof(char)*200);
  printf("%sIngresa el nombre de la red: %s",BRED,KNRM);
  fflush(stdin);
  fgets(nameSSID,200,stdin);
  *(nameSSID+sizeChar(nameSSID))='\0';
  getDataAndConsARPStruct(&arp,nameSSID);
  if(isEmpty(arp)){
    printf("Esta vacio >:v\n");
    free(database);
    free(password);
    free(user);
    exit(1);
  }else{
    initServerARPFree(arp);
  }
  freeStruct(arp);
}
