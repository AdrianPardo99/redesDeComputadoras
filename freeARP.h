/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 06/03/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/

/*Función que permite analizar si la MAC que tiene la trama esta en la
  estructura de ARP*/
ARPStruct initSearch(ARPStruct arp,datos *ma){
  int i;
  ARPStruct arpCen=arp;
  while(!isEmpty(arpCen)){
    if(!memcmp(ma+0,getMAC(arpCen),6)){
      return arpCen;
    }
    arpCen=nextHost(arpCen);
  }
  freeStruct(arpCen);
  return NULL;
}

/*Función que inicializa el servidor de arpFree*/
void initServerARPFree(ARPStruct arp){
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    obtainHardwareData();
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
  printf("Ingresa el nombre de la red: ");
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
    printARPStruct(arp);
    initServerARPFree(arp);
  }
  freeStruct(arp);
}
