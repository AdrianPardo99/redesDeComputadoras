/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 16/02/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/

/*Función que espera la primer trama de ARP*/
void recibeOfAnswersForARP(socketRaw sock,datos *trama){
  int tam;
  while(1){
    tam=recvfrom(sock,trama,1514,0,NULL,0);
    if(isAnError(tam)){
      perror("Error al recibir la trama\n");
      closeSocket();
      exit(1);
    }else{
      if(isARP(trama)&&(isMyIP(trama)||isMyMACArp(trama))&&
      isBroadcastMac(trama)){
        imprimeTramaARP(trama,tam);
        break;
      }
    }
  }
}

/*Función que recibe la trama despues de la petición*/
void recibeOfRequestOfARP(socketRaw sock,datos *trama){
  int tam,i;
  while(1){
    tam=recvfrom(sock,trama,1514,0,NULL,0);
    if(isAnError(tam)){
      perror("Error al recibir la trama\n");
      closeSocket();
      exit(1);
    }else{
      if(isARP(trama)&&isMyIP(trama)&&isMacOrIpDest(trama)&&(tam==60)){
        imprimeTramaARP(trama,tam);
        break;
      }
    }
  }
}

/*Función que estructura la trama de ARP al inicio*/
void estructuraARP(datos *trama){
  memcpy(trama+0,macBroadcast+0,6);
  memcpy(trama+6,macOrigen+0,6);
  memcpy(trama+12,etherARP+0,2);
  memcpy(trama+14,typeHar+0,2);
  memcpy(trama+16,IPP+0,2);
  memcpy(trama+18,lonP+0,2);
  memcpy(trama+20,opARP+0,2);
  memcpy(trama+22,macOrigen+0,6);
  memcpy(trama+28,ipO+0,4);
  memcpy(trama+32,macD+0,6);
  memcpy(trama+38,ipD+0,4);
}

/*Función que crea trama de datos ARP*/
void createARPTrama(){
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    obtainHardwareData();
    printf("Ingresa que solicitud de ARP deseas\n1.- ARP Request\n"
    "2.- ARP Request Inverse\n");
    do{
      scanf("%d",&ARPPetition);
      if(ARPPetition>2){
        printf("Error por favor ingresa alguna de las opciones validas\n");
      }
    }while(ARPPetition>2);
    int i;
    *(opARP)=0x00;
    *(opARP+1)=(ARPPetition==1)?(0x01):(0x08);
    if(ARPPetition==1){
      printf("Ingresa la ip del destino\n1er Octeto: ");scanf("%hhu",ipD);
      printf("2do Octeto: "); scanf("%hhu",(ipD+1));
      printf("3er Octeto: "); scanf("%hhu",(ipD+2));
      printf("4to Octeto: "); scanf("%hhu",(ipD+3));
      printf("%d.%d.%d.%d\n",*(ipD),*(ipD+1),*(ipD+2),*(ipD+3));
      for(i=0;i!=6;i++){
        *(macD+i)=0x00;
      }
    }else{
      printf("Ingresa la MAC del destino por partes\n1: "); scanf("%hhx",(macD));
      printf("2: "); scanf("%hhx",(macD+1));
      printf("3: "); scanf("%hhx",(macD+2));
      printf("4: "); scanf("%hhx",(macD+3));
      printf("5: "); scanf("%hhx",(macD+4));
      printf("6: "); scanf("%hhx",(macD+5));
      for(i=0;i!=4;i++){
        *(ipD+i)=0x00;
      }
    }
    estructuraARP(tramaEnv);
    enviaTrama(packet_socket,tramaEnv,indice);
    imprimeTramaARP(tramaEnv,60);
    puts("Trama de respuesta:");
    recibeOfRequestOfARP(packet_socket,tramaGet);
    closeSocket();
  }else{
    perror("Error al abrir la aplicación en función ARP\n");
    exit(1);
  }
}

/*Función que reconstruye la trama para enviar la respuesta de ARP*/
void estructuraTramaARPTwo(datos *trama){
  memcpy(ipD+0,trama+28,4);
  memcpy(macD+0,trama+6,6);
  memcpy(trama+0,macD,6);
  memcpy(trama+6,macOrigen+0,6);
  memcpy(trama+22,macOrigen+0,6);
  memcpy(trama+28,ipO+0,4);
  memcpy(trama+32,macD+0,6);
  memcpy(trama+38,ipD+0,4);
  if(isRequestARP(trama)==0){
    memcpy(trama+20,ARPAns+0,2);
  }else{
    memcpy(trama+20,ARPAnsI+0,2);

  }
}

/*Función que inicializa la interfaz para recibir la petición ARP*/
void initARPWaitingForTrama(){
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    obtainHardwareData();
    recibeOfAnswersForARP(packet_socket,tramaGet);
    memcpy(tramaEnv+0,tramaGet+0,60);
    estructuraTramaARPTwo(tramaEnv);
    stop(800000);
    enviaTrama(packet_socket,tramaEnv,indice);
    imprimeTramaARP(tramaEnv,60);
    printf("Comunicación exitosa cerrando\n");
    closeSocket();
  }else{
    perror("Error al abrir la aplicación en función ARP Waiting For Trama\n");
    exit(1);
  }
}
