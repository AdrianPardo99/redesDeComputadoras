/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 10/03/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/

/*Función que envia tramas como infractor en las tramas ARP gratuito*/
void initInfractorMode(){
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    obtainHardwareData();
    int i;
    *(opARP)=0x00;
    *(opARP+1)=0x01;
    printf("Ingresa la ip del destino: ");
    scanf("%s",ipPrueba);
    charToIPHex(ipPrueba);
    for(i=0;i!=6;i++){
      *(macD+i)=0x00;
    }
    for(i=0;i!=4;i++){
      *(ipO+i)=0;
    }
    estructuraARP(tramaEnv);
    enviaTrama(packet_socket,tramaEnv,indice);
    imprimeTramaARP(tramaEnv,60);
    closeSocket();
  }else{
    printf("Error al abrir la aplicación en la función initInfractorMode\n");
  }
}
