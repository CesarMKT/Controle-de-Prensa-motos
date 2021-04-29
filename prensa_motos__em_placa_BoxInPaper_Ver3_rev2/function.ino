// contar peças
void contar() {
  contadorAuto++; 
  if (contadorAuto>32000)contadorAuto=30000;

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

  pot1 = map(analogRead(A2), 0, 1023, minPrensa, maxPrensa); // le e memoriza pot1
  pot2 = map(analogRead(A3), 0, 1023, minAlimentador, maxAlimentador); // le e memoriza pot1
  if (TstBit(PIND, BtBimanualDireito) && !btDireito)btDireito = HIGH;
  if (TstBit(PIND, BtBimanualEsquerdo) && !btEsquerdo )btEsquerdo = HIGH;
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
void ai0(){ // interrupção 0

if(ligaPrensa)  intSensor = HIGH;
}//end ai0
//_________________________________________________________________________//
void ai1() {// interrupção 1
  intParada = HIGH;
}//end ai1
