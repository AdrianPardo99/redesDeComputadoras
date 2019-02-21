drop database if exists arpScanner;
create database if not exists arpScanner;

use arpScanner;

##-------------Creación de tabla SSID-------------##
create table if not exists SSID(idRed int not null primary key auto_increment,
  nameOrDescription varchar(150) not null);

##------------Creación de tabla Hosts-------------##
create table if not exists Host(idHost int not null primary key auto_increment,
  idRed int not null,
  ipUno int not null,
  ipDos int not null,
  ipTres int not null,
  ipCuatro int not null,
  macUno int not null,
  macDos int not null,
  macTres int not null,
  macCuatro int not null,
  macCinco int not null,
  macSeis int not null,
  fecha date not null,
  hora time not null,
  foreign key(idRed) references SSID(idRed)
  on update cascade on delete cascade);
source /home/developercrack/Documentos/redesQuintanilla/baseDeDatos/proceduresRedes.sql
