/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 21/02/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/

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
