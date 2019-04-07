/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 07/04/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/

/*Funcion que obtiene la información del control del LLC*/
char *getControl(int crtl){
  switch (crtl) {
    case 0:case 2:return "Trama de información\n";break;
    case 1:return "Trama de supervisión\n";break;
    case 3:return "Trama no numerada\n";break;
  }
}

/*Funcion que obtiene la información del protocolo que usa el LLC*/
void getProtocol(datos tram){
  switch ((tram&0xFE)) {
    case 0x04:
      printf("IBM SNA");
      break;
    case 0x06:
      printf("IP");
      break;
    case 0x80:
      printf("3COM");
      break;
    case 0xAA:
      printf("SNAP");
      break;
    case 0xBC:
      printf("BANYAN");
      break;
    case 0xE0:
      printf("NOVELL");
      break;
    case 0xFA:
      printf("LAN MANAGER FE-CLNS");
      break;
    case 0x42:
      printf("SPANNING TREE BPDU");
      break;
    case 0xF0:
      printf("NETBIOS");
      break;
  }
  printf("\n");
}

/*Funcion que analiza la trama LLC con todos las operaciones que salen*/
void analiceLCC(datos *trama){
  int i,crtl=trama[16]&0x03;
  printf("%sMAC Destino: ",KGRN);
  for(i=0;i!=6;i++){
    printf("%.2x%s",trama[i],(i!=5)?(":"):("\n"));
  }
  printf("%sMAC Origen: ",KYEL);
  for(i=6;i!=12;i++){
    printf("%.2x%s",trama[i],(i!=11)?(":"):("\n"));
  }
  printf("%sLongitud: %d\n",KBLU,trama[12]*256+trama[13]);
  printf("%sDSAP: %s\t\t",BRED,(((trama[14]&0x01))==1)?("Trama de grupo"):
    ("Trama individual"));
  printf("Protocolo: ");
  getProtocol(trama[14]);
  printf("%sSSAP: %s\t\t",BYEL,(((trama[15]&0x01))==1)?("Trama de respuesta"):
    ("Trama de comando"));
    printf("Protocolo: ");
    getProtocol(trama[15]);
  printf("%sControl: %s%s",KRED,getControl(crtl),KYEL);
  if(crtl==0 || crtl==2){
    printf("Extendida\nNúmero de secuencia de envio: %d\n",
      (trama[16]&0xFE)>>1);
    printf("Número de secuencia que se espera recibir: %d\n",
      (trama[17]&0xFE)>>1);
    printf("POLL/FINAL: %d ",trama[17]&0x01);
    if(((trama[16]&0x10)>>4)==1){
      if((trama[15]&0x01)==1){
        printf("FINAL ");
      }else{
        printf("PULL ");
      }
      printf("Requiere respuesta inmediata\n");
    }else{
      printf("P/F Trama de informacion\n");
    }
  }else if(crtl==1){
    printf("Extendida\nNúmero de secuencia que se espera recibir: %d\n",
      (trama[17]&0xFE)>>1);
      //Determinamos que tipo de Trama de Supervision es
      switch((trama[16]&0x0C)){
        case 0:printf("Receiver ready (RR)");break;
        case 4:printf("Receiver not ready (RNR)");break;
        case 8:printf("Retransmicion (REJ)");break;
        case 12:printf("Retransmicion selectiva (SREJ)");break;
      }
      printf("POLL/FINAL: %d ",trama[17]&0x01);
      //Vemos el estado del POLL/FINAL
      if(((trama[16]&0x10)>>4)==1){
        if((trama[15]&0x01)==1){
          printf("FINAL ");
        }else{
          printf("PULL ");
        }
        printf("Requiere respuesta inmediata\n");
      }else{
        printf("P/F Trama de informacion\n");
      }
  }
  if(crtl==3){
    printf("Normal/Extendida\n");
    printf("POLL/FINAL: %d ",(trama[16]&0x10)>>4);
    if(((trama[16]&0x10)>>4)==1){
      if((trama[15]&0x01)==1){
        printf("FINAL Requiere respuesta inmediata\n"
          "\n\tNombre\t\t\t    Descripcion\t\t\tInformacion\n");
        switch((trama[16]&0xEC)){
          case 0x60:
            printf("|Unnumbered Acknowledgment UA|\t"
              "|Reconoce la conexion|\t"
              "|De uno de los comandos de modo de configuracion|\t");
          break;
          case 0x0C:
            printf("|Disconnect Mode DM|\t"
              "|Responder en modo de desconexión|\t"
              "|Modo de configuración requerido|\t");
          break;
          case 0x40:
            printf("|Request Disconnect RD|\t"
              "|Solicitud por el comando DISC|\t"
              "\t");
          break;
          case 0x04:
            printf("|Request Inialization Mode RIM|\t"
              "|Inicialización necesaria| \t"
              "|Solicitud de comando SIM|\t");
          break;
          case 0x84:
            printf("|Frame Reject FRMR|\t"
              "|Informe de Recepción|\t"
              "|De trama inaceptable|\t");
          break;
          default:
            printf("MMM MM INCORRECTO");
          break;
        }
      }
    }else{
        printf("PULL Requiere respuesta inmediata\n"
          "\n\tNombre\t\t\t    Descripcion\t\t\tInformacion\n");
        switch((trama[16]&0xEC)){
          case 0x80:
            printf("|Set normal response SNRM|\t"
              "|Modo de Configuracion|\t"
              "|Usa 3 bits para números de secuencia|\t");
          break;
          case 0xCC:
            printf("|Set normal response extended mode SNRME|\t"
              "|Modo de Configuracion Extendido|\t"
              "|Usa 7 bits para números de secuencia|\t");
          break;
          case 0x0C:
            printf("|Set asynchronous response SARM|\t"
              "|Modo de Configuracion Asincrono|\t"
              "|Usa 3 bits para números de secuencia|\t");
          break;
          case 0x4C:
            printf("|Set asynchronous response extended mode SARME|\t"
              "|Modo de Configuracion Asincrono Extendido|\t"
              "|Usa 7 bits para números de secuencia|\t");
          break;
          case 0x2C:
            printf("|Set asynchronous balanced mode SABM|\t"
              "|Modo de Configuracion Asincrono Balanceadp|\t"
              "|Usa 3 bits para números de secuencia|\t");
          break;
          case 0x6C:
            printf("|Set asynchronous balanced extended mode SABME|\t"
              "|Modo de Configuracion Asincrono Balanceado Extendido|\t"
              "|Usa 7 bits para números de secuencia|\t");
          break;
          case 0x04:
            printf("|Set inicialitation mode SIM|\t"
              "|Iniciar funcion de control de enlace|\t"
              "|Dirigido a la estacion|\t");
          break;
          case 0x40:
            printf("|Disconnect DISC|\t"
              "|Terminar la conexion de enlace lógico|\t"
              "|Para futuras tramas de I y S regresa DM|\t");
          break;
          case 0x20:
            printf("|Unnumbered Poll UP|\t"
              "|Se utiliza para solicitar|\t"
              "|Información de control|\t");
          break;
          case 0x8C:
            printf("|Reset RSET|\t"
              "|Utilizado para la recuperación|\t"
              "|Reestablece N(R), pero no N(S)|\t");
          break;
          default:
            printf("MMM MM INCORRECTO");
          break;
        }
    }
  }else{
    printf("P/F Trama de informacion\n"
      "\n\tNombre\t\t\t    Descripcion\t\t\tInformacion\n");
    switch((trama[16]&0xEC)){
      case 0x00:
        printf("|Unnumbered Information UI|\t"
          "|Datos no reconocidos|\t"
          "|Tiene una carga util|\t");
      break;
      case 0xAC:
        printf("|Exchange Identification XID|\t"
          "|Se utiliza para solicitar|\t"
          "|Información de control|\t");
      break;
      case 0xE0:
        printf("|Test TEST|\t"
          "|Intercambio de información idéntica|\t"
          "|Campos para la realización de pruebas|\t");
      break;
      default:
        printf("MMM MM INCORRECTO");
      break;
    }
  }
  printf("\n%s",KNRM);
}

/*Funcion que escucha si hay una trama LLC en el buffer*/
void recibeLLC(socketRaw sock,datos *trama){
  int tam;
  while(1){
    tam=recvfrom(sock,trama,1514,MSG_DONTWAIT,NULL,0);
    if(!isAnError(tam)){
      if((trama[12]*256+trama[13])<=1500){
        printf("\t\t%sTrama LLC\n\t   %sTamaño de trama: %d%s\n",
          BRED,BBLU,tam,KNRM);
        imprimeTramaLLC(trama,tam);
        analiceLCC(trama);
      }
    }
  }
}

/*Funcion que inicializa la interfaz de red para escuchar si hay una trama
  LLC*/
void initLLC(){
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    obtainHardwareData();
    if(isAnError(indice)){exit(1);}
    printf("%sPor favor preciona enter para continuar...%s",BGRN,KNRM);
    getchar();
    printf("%sInit scanner LLC%s\n",BBLU,KNRM);
    recibeLLC(packet_socket,tramaGet);
    closeSocket();
  }else{
    perror("Error al abrir la aplicación en funcion initLLC: ");
    exit(1);
  }
}

/*Funcion inicializa el analizador LLC apartir de un archivo de texto*/
void initFileLLC(char *nameFile){
  FILE *fl;
  fl=fopen(nameFile,"r");
  if(fl==NULL){
    perror("Error al leer archivo: ");
    exit(1);
  }
  clearScreen();
  datos buffer[500],*trama;
  char hexbyte[3];
  int linea=1,val=0,i;
  while(fgets(buffer,sizeof(buffer),fl)){
    i=0;
    memset(&trama, 0, sizeof(trama));
    trama=malloc((int)(strlen(buffer)-2)/2);
    for(int a = 0; a < strlen(buffer); a+=2){
      hexbyte[0]=buffer[a];
      hexbyte[1]=buffer[a+1];
      sscanf(hexbyte, "%x", &val);
      trama[i]=(unsigned char)val;
      i++;
    }
    printf("\t%sTrama LLC %d\t%s   Tamaño de trama: %d%s\n",
      BRED,linea,BBLU,(int)(strlen(buffer)-2)/2,KNRM);
    imprimeTramaLLC(trama,(int)(strlen(buffer)-2)/2);
    analiceLCC(trama);
    stop(6000000);
    clearScreen();
    linea++;
  }
  fclose(fl);
}
