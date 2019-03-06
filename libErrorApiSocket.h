/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 06/03/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/

/*Función que permite mostrar las opciones con las que trabaja la aplicación*/
void printErrorParams(){
  printf("%sError por favor ingresa alguna de las siguientes opciones\n"
    "%sarp -> genera una petición ARP enviada a traves de la red\n"
    "%sarpScanner -> genera varias peticiones ARP y escanea toda"
    " red de trabajo\n%sarpScanner sql -> al igual que arpScanner pero"
    " conectado a una BD\n%sinterfaces -> Muestra las interfaces de red"
    " así como su contenido\n%s",KRED,KGRN,KYEL,KCYN,KBLU,KNRM);
}
