/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 03/03/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
#include <mysql/mysql.h>

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
char *server = "localhost";
char *user;
char *password;
char *database;
int status,idNetwork;
MYSQL_STMT *stmt;

/*Función que crea el enlace entre la BD y el programa*/
void initConnection(){
  conn = mysql_init(NULL);
  if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
    printf("Error al intentar hacer la conexión de la BD\n");
    exit(1);
  }
}

/*Función que cierra la conexión*/
void closeConnection(){
  mysql_close(conn);
}

/*Función que limpia el buffer de los resultados*/
void freeResult(){
  mysql_free_result(res);
}

/*Función que añade el nombre del SSID de la red para trabajar el protocolo ARP*/
void nameNetwork(char *name){
  int bandera=0,i;
  initConnection();
  char *sql;
  sql=(char*)malloc(sizeof(char)*100);
  memcpy(sql+0,"SELECT idRed FROM SSID where nameOrDescription='",
    sizeChar("SELECT idRed FROM SSID where nameOrDescription='")+1);
  memcpy(sql+1+sizeChar(sql),name+0,sizeChar(name)+1);
  memcpy(sql+1+sizeChar(sql),"';",2);
  if(!mysql_query(conn,sql)){
    res=mysql_store_result(conn);
    int rows,fields;
    if(res){
      rows=mysql_num_rows(res);
      fields=mysql_num_fields(res);
      while(row= mysql_fetch_row(res)){
        ban=1;
      }
      if(!ban){
        free(sql);
        sql=(char*)malloc(sizeof(char)*100);
        memcpy(sql+0,"INSERT INTO SSID VALUES(null,'",sizeChar("INSERT INTO "
          "SSID VALUES(null,'")+1);
        memcpy(sql+1+sizeChar(sql),name,sizeChar(name)+1);
        memcpy(sql+1+sizeChar(sql),"');",sizeChar("');")+1);
        status=mysql_query(conn,sql);
        if(!status){
          free(sql);
          sql=(char*)malloc(sizeof(char)*100);
          memcpy(sql+0,"SELECT idRed FROM SSID where nameOrDescription='",
            sizeChar("SELECT idRed FROM SSID where nameOrDescription='")+1);
          memcpy(sql+1+sizeChar(sql),name+0,sizeChar(name)+1);
          memcpy(sql+1+sizeChar(sql),"';",2);
          if(!mysql_query(conn,sql)){
            res=mysql_store_result(conn);
            if(res){
              rows=mysql_num_rows(res);
              fields=mysql_num_fields(res);
              while(row= mysql_fetch_row(res)){
                sscanf(row[0],"%d",&idNetwork);
                ban=1;
              }
              printf("ID del SSID: %d\n",idNetwork);
            }
          }
        }else{
          perror("Error al hacer la inserción del SSID ");
          exit(1);
        }
      }else{
        perror("Error al crear este nombre de red (repetitivo en la BD) ");
        exit(1);
      }
    }else{
      perror("Error de conexión  ");
      exit(1);
    }
  }else{
    printf("Error sql :'v\n%s",sql);
    exit(1);
  }
  free(sql);
  freeResult();
  closeConnection();
}

/*Función que añade los registros positivos del arp*/
void createStoredProcedure(datos *MAC,datos *IP){
  initConnection();
  char sql[300]="INSERT INTO Host VALUES(null,";
  sprintf(sql+1+sizeChar(sql),"%d",idNetwork);
  memcpy(sql+1+sizeChar(sql),",",1);
  sprintf(sql+1+sizeChar(sql),"%d",*IP);
  memcpy(sql+1+sizeChar(sql),",",1);
  sprintf(sql+1+sizeChar(sql),"%d",*(IP+1));
  memcpy(sql+1+sizeChar(sql),",",1);
  sprintf(sql+1+sizeChar(sql),"%d",*(IP+2));
  memcpy(sql+1+sizeChar(sql),",",1);
  sprintf(sql+1+sizeChar(sql),"%d",*(IP+3));
  memcpy(sql+1+sizeChar(sql),",",1);
  sprintf(sql+1+sizeChar(sql),"%d",*MAC);
  memcpy(sql+1+sizeChar(sql),",",1);
  sprintf(sql+1+sizeChar(sql),"%d",*(MAC+1));
  memcpy(sql+1+sizeChar(sql),",",1);
  sprintf(sql+1+sizeChar(sql),"%d",*(MAC+2));
  memcpy(sql+1+sizeChar(sql),",",1);
  sprintf(sql+1+sizeChar(sql),"%d",*(MAC+3));
  memcpy(sql+1+sizeChar(sql),",",1);
  sprintf(sql+1+sizeChar(sql),"%d",*(MAC+4));
  memcpy(sql+1+sizeChar(sql),",",1);
  sprintf(sql+1+sizeChar(sql),"%d",*(MAC+5));
  memcpy(sql+1+sizeChar(sql),",CURDATE(),CURTIME());",
    sizeChar(",CURDATE(),CURTIME());")+1);
  status=mysql_query(conn,sql);
  if(status){
    perror("Error al insertar los datos de la red ");
    exit(1);
  }
  closeConnection();
}
