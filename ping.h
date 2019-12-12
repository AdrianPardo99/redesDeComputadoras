

/*Funcion que obtiene la puerta de enlace la interfaz de red*/
int getGatewayAddr(unsigned char * gatewayIParray){
  int     received_bytes = 0, msg_len = 0, route_attribute_len = 0;
  int     sock = -1, msgseq = 0;
  struct  nlmsghdr *nlh, *nlmsg;
  struct  rtmsg *route_entry;
  struct  rtattr *route_attribute;
  char    gateway_address[INET_ADDRSTRLEN];
  char    msgbuf[BUFFER_SIZE], buffer[BUFFER_SIZE];
  char    *ptr = buffer;
  struct timeval tv;
  if ((sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE)) < 0){
      perror("socket failed");
      return EXIT_FAILURE;
  }
  memset(msgbuf, 0, sizeof(msgbuf));
  memset(gateway_address, 0, sizeof(gateway_address));
  memset(buffer, 0, sizeof(buffer));
  nlmsg = (struct nlmsghdr *)msgbuf;
  nlmsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
  nlmsg->nlmsg_type = RTM_GETROUTE; // Get the routes from kernel routing table .
  nlmsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST; // The message is a request for dump.
  nlmsg->nlmsg_seq = msgseq++; // Sequence of the message packet.
  nlmsg->nlmsg_pid = getpid(); // PID of process sending the request.
  tv.tv_sec = 1;
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));
  /* send msg */
  if (send(sock, nlmsg, nlmsg->nlmsg_len, 0) < 0){
      perror("send failed");
      return EXIT_FAILURE;
  }
  do{
      received_bytes = recv(sock, ptr, sizeof(buffer) - msg_len, 0);
      if (received_bytes < 0){
          perror("Error in recv");
          return EXIT_FAILURE;
      }
      nlh = (struct nlmsghdr *) ptr;
      if((NLMSG_OK(nlmsg, received_bytes) == 0)||(nlmsg->nlmsg_type == NLMSG_ERROR)){
        perror("Error in received packet");
        return EXIT_FAILURE;
      }
      if(nlh->nlmsg_type == NLMSG_DONE){
        break;
      }else{
        ptr += received_bytes;
        msg_len += received_bytes;
      }

      if((nlmsg->nlmsg_flags & NLM_F_MULTI) == 0){
          break;
      }
  }while ((nlmsg->nlmsg_seq != msgseq) || (nlmsg->nlmsg_pid != getpid()));

  for (; NLMSG_OK(nlh, received_bytes); nlh = NLMSG_NEXT(nlh, received_bytes)) {
      route_entry = (struct rtmsg *) NLMSG_DATA(nlh);
      if(route_entry->rtm_table != RT_TABLE_MAIN)
          continue;
      route_attribute = (struct rtattr *) RTM_RTA(route_entry);
      route_attribute_len = RTM_PAYLOAD(nlh);
      for(;RTA_OK(route_attribute,route_attribute_len);
            route_attribute=RTA_NEXT(route_attribute,route_attribute_len)){
          switch(route_attribute->rta_type){
            case RTA_GATEWAY:
              inet_ntop(AF_INET, RTA_DATA(route_attribute),
                        gateway_address, sizeof(gateway_address));
              printf("%sGateway %s%s\n",KCYN,gateway_address,KNRM);
              break;
            default:
              break;
          }
      }

      if ((*gateway_address)) {
          charToIPHex(gateway_address);
          break;
      }
  }
  close(sock);
  return 0;
}


/*Funcion que convierte cualquier Dominio en IP*/
int hostnameToIP(char * hostname , char* ip){
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    if ((he = gethostbyname( hostname ))==NULL){
        return -1;
    }else{
    	addr_list = (struct in_addr **) he->h_addr_list;
	    for(i=0;addr_list[i]!= NULL;i++){
        strcpy(ip , inet_ntoa(*addr_list[i]));
        return 1;
      }
    }
    return 1;
  }

/*Obtiene el nombre de host a partir de una IP*/
char *getHostnamefromIP(char *ip){
  struct sockaddr_in sa;    /* input */
  socklen_t len;         /* input */
  char hbuf[NI_MAXHOST];
  const char * string_to_return;
  memset(&sa, 0, sizeof(struct sockaddr_in));
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = inet_addr(ip);
  len = sizeof(struct sockaddr_in);
  if (getnameinfo((struct sockaddr *) &sa, len, hbuf, sizeof(hbuf),
    NULL, 0, NI_NAMEREQD)){
      strcpy(ip, "\b");
      return ip;
  }else{
    printf("host=%s\n", hbuf);
    string_to_return = (const char *)malloc(sizeof(char) * ((int)strlen(hbuf)+1));
    strcpy((char *)string_to_return, hbuf);
    printf("Strings: %s\n",string_to_return);
    return strdup(string_to_return);
  }
}

/*Revisa si el arreglo de la cadena es de formato IP*/
int isIP(char *x){
  int i,j=strlen(x);
  for(i=0;i!=j;i++){
    if(!((x[i]>='0'&&x[i]<='9')||(x[i]=='.'))){
      return 0;
    }
  }
  return 1;
}

/*Función que analiza si la ip destino esta dentro o fuera de la red*/
int isInLAN(datos *myIP,datos *myMask, datos *externIP){
  int flag1[4],flag2[4],i;
  for(i=0;i!=4;i++){
    flag1[i]=myIP[i]&myMask[i];
    flag2[i]=externIP[i]&myMask[i];
  }
  for(i=0;i!=4;i++){
    if(flag1[i]!=flag2[i]){
      return 0;
    }
  }
  return 1;
}

/*Funcion que inicia la chamba del PING*/
void initPing(char *hostname){
  int i;
  createSocket();
  openSocket();
  if(!isAnError(packet_socket)){
    obtainHardwareData();
    if(isAnError(indice)){exit(1);}
    host=hostname;
    if(isIP(host)){
      charToIPHex(host);
    }else{
      if(isAnError(hostnameToIP(host,ipD))){
        printf("%sError dirección no valida :'v\n%s",BBLU,KNRM),exit(1);
      }
    }
    printf("%sHost: %s\n%sIP destino: %d.%d.%d.%d%s\n",
      KYEL,host,BRED,*(ipD),*(ipD+1),*(ipD+2),*(ipD+3),KNRM);
    if(isInLAN(ipO,mascara,ipD)){
      printf("%sDestino en LAN%s\nEnviando ARP:%s\n",KYEL,BBLU,KNRM);
      *(opARP)=0x00;
      *(opARP+1)=0x01;
      estructuraARP(tramaEnv);
      imprimeTramaARP(tramaEnv,60);
      enviaTrama(packet_socket,tramaEnv,indice);
      recibeOfRequestOfARP(packet_socket,tramaGet,0);
      for(i=6;i!=12;i++){
        *(MACTarget+i-6)=*(tramaGet+i);
      }
      printf("\n");
    }else{
      printf("%sDestino fuera de la LAN%s\n",KBLU,KNRM);
      for(i=0;i!=4;i++){*(ipDFar+i)=*(ipD+i);}
      getGatewayAddr(gatewayIP);
      *(opARP)=0x00;
      *(opARP+1)=0x01;
      estructuraARP(tramaEnv);
      imprimeTramaARP(tramaEnv,60);
      enviaTrama(packet_socket,tramaEnv,indice);
      recibeOfRequestOfARP(packet_socket,tramaGet,0);
      for(i=0;i!=4;i++){
        *(gatewayIP+i)=*(ipD+i);
        *(ipD+i)=*(ipDFar+i);
      }
      for(i=6;i!=12;i++){
        *(MACTarget+i-6)=*(tramaGet+i);
      }
      printf("\n");
    }
    printf("%sPING %s (%d.%d.%d.%d) 56(%d) bytes de datos.\n%s" ,
      BYEL,hostname , *(ipD),*(ipD+1),*(ipD+2),*(ipD+3),icmpdatabytes,KNRM);
    estructuraTramaICMP(tramaEnv);
    imprimeTrama(tramaEnv,60);
    closeSocket();
  }else{
    printf("Error al abrir la aplicación en la función initPing\n");
    exit(1);
  }
}
