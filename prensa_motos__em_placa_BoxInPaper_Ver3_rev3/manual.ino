void manual() {

  btPedal = LOW;
  btAlimentador = LOW;
  ligaPrensa = LOW;
  ligaLuz = LOW;
  ligaAlimentador = LOW;
  intSensor = LOW;

  bool direito, esquerdo, parada, alimentar;

  //chave seletora 3/4 manual sem alimentador A0 = 14 HIGH A1=15 LOW
  while (TstBit(PINC, 0) && !TstBit(PINC, 1)) {

    //      chave seletora de posição no modo 3/4 manual para controle de batida uma por vez
   
    //        -Acionamento por pedal ou bimanual - bimanual com sistema individual
    if (bimanual()  && millis() - timerAlimentador >= pot1*2) {
       ligaPrensa = HIGH;
    }
 
    if (intSensor && ligaPrensa) {
      contar();//conta as peças produzidas
      
      intSensor = LOW;
      ligaPrensa = LOW;

    }

    //Alimentador ligar e ficar ligado pelo periodo de tempo do potenciometro
    if (!btAlimentador && !TstBit(PIND, BtAlimentador) && millis() - timerAlimentador >= pot1 * 2) {
      ligaAlimentador = HIGH;
      btAlimentador = HIGH;
      timerAlimentador = millis();
    }
    if (btAlimentador && TstBit(PIND, BtAlimentador))btAlimentador = LOW;
    if (millis() - timerAlimentador >= pot1)ligaAlimentador = LOW;
    atualizar();//atualiza reles

  }//end while
}//end manual
