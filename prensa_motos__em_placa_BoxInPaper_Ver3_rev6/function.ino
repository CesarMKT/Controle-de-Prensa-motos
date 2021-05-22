// contar peças
void contar() {
  contadorAuto++;
  if (contadorAuto > 32000)contadorAuto = 30000;

  contador++; //soma no contador
  if (contador > 99999999) contador = 0;
  if (contador != EEPROMReadLong(marcador))EEPROMWriteLong(marcador, contador); // verifica se o valor é diferente no endereço, se for atualiza
  displayN(contador); //mostra o valor do contador no display

}//end contar
//_________________________________________________________________________//

void atualizar() {
  if ( ligaPrensa)SetBit(PORTB, RelePrensa);
  else ClrBit(PORTB, RelePrensa);
  if ( ligaAlimentador)SetBit(PORTB, ReleAlimentador);
  else ClrBit(PORTB, ReleAlimentador);
  if (ligaMotor)SetBit(PORTB, ReleMotor);
  else ClrBit(PORTB, ReleMotor);
  if ( ligaLuz)SetBit(PORTB, ReleSinaliza);
  else ClrBit(PORTB, ReleSinaliza);


  if (TstBit(PIND, BtBimanualDireito) && !btDireito)btDireito = HIGH;
  if (TstBit(PIND, BtBimanualEsquerdo) && !btEsquerdo )btEsquerdo = HIGH;
  if (pot1 != EEPROMReadInt(996))EEPROMWriteInt(996, pot1);
}//end atualizar
//_________________________________________________________________________//

bool bimanual() {
  bool ibimanual = LOW;// recebe LOW
  if (!TstBit(PIND, BtBimanualDireito) &&
      btDireito &&
      !TstBit(PIND, BtBimanualEsquerdo) &&
      btEsquerdo ) {
    btDireito = LOW;
    btEsquerdo = LOW;
    ibimanual = HIGH;
  }

  return ibimanual;
}//end bimanual


//_________________________________________________________________________//
void ai0CHANGE() { // interrupção 0

  if (!TstBit(PIND, 2))intSensor = HIGH;
  else offSensor = HIGH;

}//end ai0

//_________________________________________________________________________//
