/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 06/03/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/

/*Estructura de datos que permite almacenar los datos que arroja el reporte*/
typedef struct ARP{
  datos MAC[6],IP[4];
  struct ARP *sig;
}*ARPStruct;

/*Función que permite saber si la estructura de datos esta vacia*/
int isEmpty(ARPStruct arp){return arp==NULL;}

/*Función que permite inicializar la estructura de datos*/
ARPStruct empty(){return NULL;}

/*Función que permite añadir mas datos a la interfaz de datos*/
ARPStruct addHostStruct(ARPStruct ar,datos *M,datos *I){
  int i;
  ARPStruct arp=(ARPStruct)malloc(sizeof(struct ARP));
  for(i=0;i!=6;i++){
    arp->MAC[i]=M[i];
    if(i<4){
      arp->IP[i]=I[i];
    }
  }
  arp->sig=ar;
  return arp;
}

/*Función que permite avanzar al siguiente apuntador de la estructura*/
ARPStruct nextHost(ARPStruct arp){return arp->sig;}

/*Función que optiene los datos de la MAC de la estructura*/
datos *getMAC(ARPStruct arp){return arp->MAC;}

/*Función que permite obtener los datos de la IP de la estructura*/
datos *getIP(ARPStruct arp){return arp->IP;}

/*Función que imprime los datos de la estructura*/
void printARPStruct(ARPStruct arp){
  if(!isEmpty(arp)){
    printARPStruct(nextHost(arp));
    datos *M=getMAC(arp),*I=getIP(arp);
    int i;
    for(i=0;i!=6;i++){
      printf("%.2x",M[i]);
      if(i!=5){printf(":");}
    }
    printf("  :  ");
    for(i=0;i!=4;i++){
      printf("%d",I[i]);
      if(i!=3){printf(".");}
    }
    printf("\n");
  }
}

/*Función que permite liberar la memoria de la estructura*/
void freeStruct(ARPStruct arp){
  if(!isEmpty(arp)){
    freeStruct(nextHost(arp));
    free(arp);
    free(arp->sig);
  }else{
    printf("Limpieza correcta\n");
  }
}

/*Función que solo imprime una estructuraARP*/
void onlyMatchStruct(ARPStruct arp){
  if(!isEmpty(arp)){
    datos *M=getMAC(arp),*I=getIP(arp);
    int i;
    for(i=0;i!=6;i++){
      printf("%.2x",M[i]);
      if(i!=5){printf(":");}
    }
    printf("  :  ");
    for(i=0;i!=4;i++){
      printf("%d",I[i]);
      if(i!=3){printf(".");}
    }
    printf("\n");
  }else{
    printf("No match mac\n");
  }
}
