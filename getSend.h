/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 16/02/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
/*Función que imprime el mensaje que contiene la trama*/
void printMessage(datos *trama){
  printf("\nEl mensaje es:\n");
  puts(trama+14);
  printf("\n\n");
}

/*Función que permite cambiar el color del buffer de salida (stdout)*/
void setColor(int i,int des){
  if(des==1){
    switch(i){
      case 0:case 1:case 2:case 3:case 4:case 5:
        printf("%s",KGRN);
        break;
      case 6:case 7:case 8:case 9:case 10:case 11:
        printf("%s",KYEL);
        break;
      case 12:case 13:
        printf("%s",KBLU);
        break;
      default:
        printf("%s",KNRM);
    }
  }else if(des==0){
    switch(i){
      case 0:case 1:case 2:case 3:case 4:case 5:case 28:case 29:case 30: case 31:
        printf("%s",KGRN);
        break;
      case 6:case 7:case 8:case 9:case 10:case 11:case 38:case 39: case 40: case 41:
        printf("%s",KYEL);
        break;
      case 12:case 13:case 16:case 17:
        printf("%s",KBLU);
        break;
      case 14:case 15:case 20:case 21:case 32:case 33:case 34:case 35:case 36:case 37:
        printf("%s",KRED);
        break;
      case 18:case 22:case 23: case 24: case 25:case 26:case 27:
        printf("%s",KMAG);
        break;
      default:
        printf("%s",KCYN);
        break;
      }
  }
}

/*Función que imprime trama de ARP*/
void imprimeTramaARP(datos *trama,int tam){
  int i;
  for(i=0;i!=tam;i++){
    setColor(i,0);
    printf("%.2X%s",trama[i],(((i+1)%16==0)?("\n"):(" ")));
  }
  printf("%s\n",KNRM);
  if(!(memcmp(trama+14,"exit",4))){
    closeSocket(packet_socket);
    exit(0);
  }
}


/*Función que imprime trama*/
void imprimeTrama(datos *trama,int tam){
  int i;
  for(i=0;i!=tam;i++){
    setColor(i,1);
    printf("%.2X%s",trama[i],(((i+1)%16==0)?("\n"):(" ")));
  }
  printf("\n");
  if(!(memcmp(trama+14,"exit",4))){
    closeSocket(packet_socket);
    exit(0);
  }
}

/*Función que recibe trama*/
void recibeTrama(socketRaw sock,datos *trama,int lenght){
  int tam,i=0;
  while(lenght>i){
      tam=recvfrom(sock,trama,1514,0,NULL,0);
      if(isAnError(tam)){
        perror("Error al recibir la trama\n");
        closeSocket();
        exit(1);
      }else{
        if(!memcmp(trama+0,macOrigen,6)){
            printf("Trama %d\n",i+1);
            imprimeTrama(trama,tam);
            i++;
        }
      }
  }
}

/*Función que recibe el mensaje del chat de sockets*/
void recibeMensaje(socketRaw sock,datos *trama){
  int tam;
  gettimeofday(&start,NULL);
  while(mtime<=10000){
    tam=recvfrom(sock,trama,1514,MSG_DONTWAIT,NULL,0);
    if(isAnError(tam)){
      //perror("Error al recibir la trama\n");
    }else{
      if(!memcmp(trama+6,macCompa+0,6)&&!memcmp(trama+0,macOrigen+0,6)){
          imprimeTrama(trama,tam);
          printMessage(trama);
          gettimeofday(&end,NULL);
          seconds = end.tv_sec - start.tv_sec;
          useconds = end.tv_usec - start.tv_usec;
          mtime = ((seconds)*1000 + useconds/1000.0)+0.5;
          break;
      }
    }
    gettimeofday(&end,NULL);
    seconds = end.tv_sec - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds)*1000 + useconds/1000.0)+0.5;
  }
  printf("Tiempo de espera %ld milliseconds agotado UnU\n",mtime);
  mtime = 0;
  useconds = 0;
  seconds = 0;
}

/*Función que envia trama ethernet*/
void enviaTrama(socketRaw sock,datos *trama,int index){
  int ban;
  tramas interfa;
  memset(&interfa,0x00,sizeof(interfa));
  interfa.sll_family = AF_PACKET;
  interfa.sll_protocol=htons(ETH_P_ALL);
  interfa.sll_ifindex=index;
  ban=sendto(sock,trama,60,0,(struct sockaddr *)&interfa,sizeof(interfa));
  if(isAnError(ban)){
    perror("Error al enviar\n");
  }else{
    printf("Exito al enviar trama\n");
  }
}
