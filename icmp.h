
void estructuraTramaICMP(datos *trama){
  memcpy(trama+0,MACTarget+0,6);
  memcpy(trama+6,macOrigen+0,6);
  memcpy(trama+12,ICMP_ETH+0,2);
}
