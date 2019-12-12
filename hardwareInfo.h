/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 12/12/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/

/*Función que construye el binario de las flags*/
void obtenerBinFlags(tinyDato fl){
  if(fl>0){
    obtenerBinFlags(fl/2);
    printf("%s",(fl%2==0)?("0"):("1"));
  }else{
    printf("El binario de la flag es: ");
  }
}

/*Función que obtiene la flag del hardware de red*/
void obtenerDatosFlags(socketRaw ds){
  if(ioctl(ds,SIOCGIFFLAGS,&nic) == -1){
    perror("Error al obtener las FLAGS ");
  }else{
    flags=nic.ifr_flags;
    printf("\nLas FLAGS son: %d\n",flags);
  }
}

/*Función que obtiene la metrica del hardware de red*/
void obternerDatosMetrica(socketRaw ds){
  if(ioctl(ds,SIOCGIFMETRIC,&nic) == -1){
    perror("Error al obtener la Metrica ");
  }else{
    metricO=nic.ifr_metric;
    printf("\nLa Metrica es: %d",metricO);
  }
}

/*Función que obtiene el MTU del hardware de red*/
void obtenerDatosMTU(socketRaw ds){
  if(ioctl(ds,SIOCGIFMTU,&nic) == -1){
    perror("Error al obtener el MTU ");
  }else{
    mtuO=nic.ifr_mtu;
    printf("\nMTU es: %d",mtuO);

  }
}

/*Función que obtiene la mascara de subred del hardware de red*/
void obtenerDatosMask(socketRaw ds){
  int i;
  if(ioctl(ds,SIOCGIFNETMASK,&nic) == -1){
    perror("Error al obtener la Macara de subred ");
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
    perror("Error al obtener la IP ");
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
    perror("Error al obtener la MAC ");
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
  if(ioctl(ds,SIOCGIFINDEX,&nic) == -1){
    perror("Error en el indice ");
    return -1;
  }else{
    return nic.ifr_ifindex;
  }
}


/*Función que enumera las interfaces de red*/
void enumInterface(socketRaw ds){
  confInt conf;
  char buf[16384];
	unsigned i;
  int t=0;
  size_t len;
  conf.ifc_len=sizeof buf;
  conf.ifc_buf=buf;
  if(ioctl(ds, SIOCGIFCONF,&conf)!=0){
    perror("ioctl(SIOCGIFCONF)");
		exit(EXIT_FAILURE);
  }
  nics=conf.ifc_req;
  printf(KYEL"Interfaces de red disponibles: \n"KNRM);
  for(i=0;i<conf.ifc_len;){
    len=sizeof *nics;
    printf(BCYN"\tInterface[%d]: %s\n"KNRM,t,nics->ifr_name);
    strcpy(cpHardware[t]+0,nics->ifr_name);
    nics=(struct ifreq*)((char*)nics+len);
		i+=len;
    t++;
  }
  printf(BGRN"Ingresa el numero de interfaz con la que deseas trabajar: "KNRM);
  scanf("%d",&indice);
  strcpy(nic.ifr_name+0,cpHardware[indice]+0);
}

/*Función que permite obtener los datos del hardware de red*/
void obtainHardwareData(){
  enumInterface(packet_socket);
  indice = obtenerDatosIndex(packet_socket);
  obtenerDatosHardwareOrigen(packet_socket);
  obtenerDatosIp(packet_socket);
  obtenerDatosMask(packet_socket);
  obtenerDatosMTU(packet_socket);
  obternerDatosMetrica(packet_socket);
  obtenerDatosFlags(packet_socket);
  obtenerBinFlags(flags);
  printf("\nEl indice de la interfaz es: %d\n", indice);
}
