##------------Procedimiento que añade el SSID------------##
delimiter **
drop procedure if exists createSSID;
create procedure createSSID(name varchar(150))
  begin
    declare ban int;
    set ban = (select ifnull(count(idRed),0) from SSID where nameOrDescription=name);
    if ban=0 then
      insert into SSID values(null,name);
      select idRed as 'ID de la Red' from SSID where nameOrDescription=name;
    else
      select 0 as 'Error';
    end if;
end **
delimiter ;

##------------Procedimiento que añade el registro de red------------##
drop procedure if exists addHost;
delimiter **
create procedure addHost(red int,ipUno int,ipDos int ,ipTres int,
  ipCuatro int,macUno int,macDos int,macTres int,
  macCuatro int,macCinco int,macSeis int)
  begin
    insert into Host values(null,red,ipUno,ipDos,ipTres,ipCuatro,macUno,macDos,
    macTres,macCuatro,macCinco,macSeis,CURDATE());
end **
delimiter ;

drop view if exists reporte;
create view reporte as
  select concat(if(LENGTH(HEX(macUno))=1,concat('0',HEX(macUno)),HEX(macUno)),
  ':',if(LENGTH(HEX(macDos))=1,concat('0',HEX(macDos)),HEX(macDos)),':',
  if(LENGTH(HEX(macTres))=1,concat('0',HEX(macTres)),HEX(macTres)),':',
  if(LENGTH(HEX(macCuatro))=1,concat('0',HEX(macCuatro)),HEX(macCuatro)),':',
  if(LENGTH(HEX(macCinco))=1,concat('0',HEX(macCinco)),HEX(macCinco)),':',
  if(LENGTH(HEX(macSeis))=1,concat('0',HEX(macSeis)),HEX(macSeis))) as 'MAC',
  concat(ipUno,'.',ipDos,'.',ipTres,'.',ipCuatro) as 'IP',
  s.nameOrDescription as 'RED',
  concat(fecha,' ',hora) as 'Registrado el' from Host h,SSID s where s.idRed=h.idRed;
##call createSSID('REDES');call addHost(1,0,0,0,0,1,2,3,4,5,6);
