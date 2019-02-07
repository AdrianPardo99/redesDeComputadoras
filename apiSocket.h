/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 06/02/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
#include "socketsElem.h"

/*Función que analiza si el procedimiento (socket) contiene algun error*/
socketRaw isAnError(socketRaw sock){
  return sock == -1;
}

/*Función que crea un socket crudo*/
void createSocket(){
  //socket(AF_PACKET, SOCK_RAW,htons(tipo de protocolo));
  packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
}

/*Función que analiza si el socket esta abierto*/
void openSocket(){
  if(isAnError(packet_socket)){
    perror("Error al abrir socket 7^7\n");
    exit(1);
  }else{
    printf("Exito socket abierto correctamente 7u7\n");
  }
}

/*Función que cierra el socket de comunicación*/
void closeSocket(){
  close(packet_socket);
  if(isAnError(packet_socket)){
    perror("Ya valio madres :'v el socket no se cerro\n\n");
  }else{
    printf("Socket cerrado correctamente uwu\n\n");
  }
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

/*Función que estructura trama de ethernet*/
void estructuraTrama(datos *trama){
  memcpy(trama+0,macBroadcast+0,6);
  memcpy(trama+6,macOrigen+0,6);
  memcpy(trama+12,ethertype+0,2);
  memcpy(trama+14,"Adrian Gonzalez Pardo",sizeof("Adrian Gonzalez Pardo"));
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

/*Función que obtiene la flag del hardware de red*/
void obtenerDatosFlags(socketRaw ds){
  if(ioctl(ds,SIOCGIFFLAGS,&nic) == -1){
    perror("Error al obtener las FLAGS\n");
  }else{
    flags=nic.ifr_flags;
    printf("\nLas FLAGS son: %d\n",flags);
  }
}

/*Función que obtiene la metrica del hardware de red*/
void obternerDatosMetrica(socketRaw ds){
  if(ioctl(ds,SIOCGIFMETRIC,&nic) == -1){
    perror("Error al obtener la Metrica\n");
  }else{
    metricO=nic.ifr_metric;
    printf("\nLa Metrica es: %d",metricO);
  }
}

/*Función que obtiene el MTU del hardware de red*/
void obtenerDatosMTU(socketRaw ds){
  if(ioctl(ds,SIOCGIFMTU,&nic) == -1){
    perror("Error al obtener el MTU\n");
  }else{
    mtuO=nic.ifr_mtu;
    printf("\nMTU es: %d",mtuO);

  }
}

/*Función que obtiene la mascara de subred del hardware de red*/
void obtenerDatosMask(socketRaw ds){
  int i;
  if(ioctl(ds,SIOCGIFNETMASK,&nic) == -1){
    perror("Error al obtener la Macara de subred\n");
  }else{
    memcpy(mascara,nic.ifr_hwaddr.sa_data+2,4);
    printf("\nMascara de subred es: ");
    for(i=0;i!=4;i++){
      printf("%d",mascara[i]);
      if(i!=3){
        printf(".");
      }
    }
  }
}

/*Función que obtiene la ip del hardware de red*/
void obtenerDatosIp(socketRaw ds){
  int i;
  if(ioctl(ds,SIOCGIFADDR,&nic) == -1){
    perror("Error al obtener la IP\n");
  }else{
    memcpy(ipO,nic.ifr_hwaddr.sa_data+2,4);
    printf("\nIP Origen: ");
    for(i=0;i!=4;i++){
      printf("%d",ipO[i]);
      if(i!=3){
        printf(".");
      }
    }
  }
}

/*Función que obtiene el MAC Address del hardware de red*/
void obtenerDatosHardwareOrigen(socketRaw ds){
  int i;
  if(ioctl(ds,SIOCGIFHWADDR,&nic) == -1){
    perror("Error al obtener la MAC\n");
  }else{
    memcpy(macOrigen,nic.ifr_hwaddr.sa_data+0,6);
    printf("La MAC es: ");
    for(i=0;i!=6;i++){
      printf("%.2x",macOrigen[i]);
      if(i!=5){
        printf(":");
      }
    }
  }
}

/*Función que obtiene el indice del hardware de red*/
socketRaw obtenerDatosIndex(socketRaw ds){
  printf("Inserta el nombre de la interfaz de red: ");
  fflush(stdin);
  gets(nic.ifr_name);
  cpHardware=(char*)malloc(sizeof(char)*(sizeChar(nic.ifr_name)));
  memcpy(cpHardware,nic.ifr_name+0,sizeChar(nic.ifr_name));
  if(ioctl(ds,SIOCGIFINDEX,&nic) == -1){
    perror("Error en el indice\n");
    return -1;
  }else{
    return nic.ifr_ifindex;
  }
}

/*Función que muestra el número de socket*/
void showSocketNumber(){
  printf("Numero de socket: %d\n",packet_socket);
}

/*Función que construye el binario de las flags*/
void obtenerBinFlags(tinyDato fl){
  int i=0;
  tinyDato cent=fl;
  while(cent>0){
    i++;cent/=2;
  }
  cent=fl;
  *(binFlags+i)='\0';
  i--;
  while(cent>0){
    *(binFlags+i)=(cent%2==0)?('0'):('1');
    i--;
    cent/=2;
  }
  printf("El binario de la flag es: %s\n",binFlags);
}


/*Función que itera las funciones de obtener datos de hardware de red*/
void showInterface(){
  int indice;
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    showSocketNumber();
    indice = obtenerDatosIndex(packet_socket);
    obtenerDatosHardwareOrigen(packet_socket);
    obtenerDatosIp(packet_socket);
    obtenerDatosMask(packet_socket);
    obtenerDatosMTU(packet_socket);
    obternerDatosMetrica(packet_socket);
    obtenerDatosFlags(packet_socket);
    obtenerBinFlags(flags);
    interfazOnOf(packet_socket);
    printf("El indice de la interfaz es: %d\n", indice);
    estructuraTrama(tramaEnv);
    enviaTrama(packet_socket,tramaEnv,indice);
    closeSocket();
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
