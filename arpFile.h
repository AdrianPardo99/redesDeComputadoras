/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 06/03/2019
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
void recibeOfRequestOfARP(socketRaw sock,datos *trama,int des){
  int tam,i;
  ban=0;
  mtime = 0;
  useconds = 0;
  seconds = 0;
  gettimeofday(&start,NULL);
  while(mtime<200){
    tam=recvfrom(sock,trama,1514,MSG_DONTWAIT,NULL,0);
    if(isAnError(tam)){
    }else{
      if(isFilterArp(trama,tam)){
        puts("Trama de respuesta:");
        imprimeTramaARP(trama,tam);
        gettimeofday(&end,NULL);
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = ((seconds)*1000 + useconds/1000.0)+0.5;
        ban=1;
        break;
      }
    }
    gettimeofday(&end,NULL);
    seconds = end.tv_sec - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds)*1000 + useconds/1000.0)+0.5;
  }

  if(!ban){
    printf("Tiempo de espera %ld milliseconds agotado UnU\n",mtime);
  }else{
    printf("Respuesta esperada recibida correctamente en %ld milliseconds\n"
    "%sMAC de la respuesta: ",mtime,KYEL);
    memcpy(macD+0,trama+6,6);
    for(i=6;i!=12;i++){
      printf("%.2X",*(trama+i));
      if(i!=11){
        printf(":");
      }
    }
    printf("\n%sIP de la respuesta: ",KGRN);
    for(i=28;i!=32;i++){
      printf("%d",*(trama+i));
      if(i!=31){
        printf(".");
      }
    }
    printf("%s\n",KNRM);
    if(des){
      createStoredProcedure(macD,ipD);
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
      printf("Ingresa la ip del destino: ");
      scanf("%s",ipPrueba);
      charToIPHex(ipPrueba);
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
    recibeOfRequestOfARP(packet_socket,tramaGet,0);
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
    enviaTrama(packet_socket,tramaEnv,indice);
    imprimeTramaARP(tramaEnv,60);
    printf("Comunicación exitosa cerrando\n");
    closeSocket();
  }else{
    perror("Error al abrir la aplicación en función ARP Waiting For Trama\n");
    exit(1);
  }
}

/*Función que escanea toda la red para enviar tramas arp, a nivel de id de red*/
void initScanARP(int des){
  int i;
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    obtainHardwareData();
    if(isAnError(indice)){exit(1);}
    *(opARP)=0x00;
    *(opARP+1)=0x01;
    red=(char*)malloc(sizeof(char)*4);
    sub=(char*)malloc(sizeof(char)*4);
    printf("Id de red: ");
    for(i=0;i!=4;i++){*(red+i)=0;*(sub+i)=0;}
    for(i=0;i!=4;i++){
      *(red+i)=*(mascara+i)&*(ipO+i);
      *(sub+i)=*(ipO+i)|~*(mascara+i);
      printf("%d",*(red+i));
      if(i!=3){
        printf(".");
      }
    }
    memcpy(ipD+0,red+0,4);
    *(ipD+3)=*(ipD+3)+1;
    printf("\nPrimer ip: ");
    for(i=0;i!=4;i++){
      printf("%d",*(ipD+i));
      if(i!=3){
        printf(".");
      }
    }
    printf("\nUltima ip: ");
    for(i=0;i!=4;i++){
      printf("%d",*(sub+i));
      if(i!=3){
        printf(".");
      }
    }
    printf("\n");

    if(des){
      nameSSID=(char *)malloc(sizeof(char)*200);
      printf("Ingresa un nombre para clasificar la red en la Base de datos: ");
      fflush(stdin);
      fgets(nameSSID,200,stdin);
      *(nameSSID+sizeChar(nameSSID))='\0';
      nameNetwork(nameSSID);
    }
    fflush(stdin);
    printf("%sPor favor presiona enter para iniciar el escaneo de la red%s\n",
      KBLU,KNRM);
    getchar();
    while(memcmp(sub+0,ipD+0,4)){
      printf("%sIP: %d.%d.%d.%d%s\n",KMAG,*(ipD),*(ipD+1),*(ipD+2),*(ipD+3),KNRM);
      for(i=0;i!=6;i++){*(macD+i)=0x00;}
      estructuraARP(tramaEnv);
      enviaTrama(packet_socket,tramaEnv,indice);
      imprimeTramaARP(tramaEnv,60);
      recibeOfRequestOfARP(packet_socket,tramaGet,des);
      if(*(ipD+3)==0xff){
        *(ipD+3)=0x01;
        if(*(ipD+2)==0xff){
          *(ipD+2)=0x01;
          if(*(ipD+1)==0xff){
            *(ipD+1)=0x01;
            *(ipD+0)=*(ipD+0)+1;
          }else{
            *(ipD+1)=*(ipD+1)+1;
          }
        }else{
          *(ipD+2)=*(ipD+2)+1;
        }
      }else{
        *(ipD+3)=*(ipD+3)+1;
      }
      printf("\n");
    }
    closeSocket();
  }else{
    printf("Error al abrir la aplicación en la función initScanARP\n");
  }
  free(nameSSID);
  free(red);
  free(sub);
}
