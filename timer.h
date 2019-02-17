/*Author: Adrian González Pardo
  Email: gozapaadr@gmail.com
  Nickname: DevCrack
  Fecha de modificación: 16/02/2019
  GitHub: AdrianPardo99
  Licencia Creative Commons CC BY-SA
*/
#include <time.h>

void stop(int a){
  clock_t star=clock();
  while(clock()<star+a);
}
