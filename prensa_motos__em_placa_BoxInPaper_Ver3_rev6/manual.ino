void manual() {

  btPedal = LOW;
  btAlimentador = LOW;
  ligaPrensa = LOW;
  ligaLuz = LOW;
  ligaAlimentador = LOW;
  intSensor = LOW;

  bool direito, esquerdo, parada, alimentar;
  //while (TstBit(PINC, 0) && !TstBit(PINC, 1)&&TstBit(PIND,4)) {
  //atualizar();
  //
  //
  //}



  //chave seletora 3/4 manual sem alimentador A0 = 14 HIGH A1=15 LOW
  while (TstBit(PINC, 0) && !TstBit(PINC, 1)) {

    //      chave seletora de posição no modo 3/4 manual para controle de batida uma por vez

    //        -Acionamento por pedal ou bimanual - bimanual com sistema individual
    if (bimanual()  && millis() - timerAlimentador >= pot1 * 2) {
      ligaPrensa = HIGH;
    }

    if (intSensor && ligaPrensa) {
      contar();//conta as peças produzidas

      intSensor = LOW;
      ligaPrensa = LOW;

    }

    //Alimentador ligar e ficar ligado pelo periodo de tempo do potenciometro
    while (!TstBit(PIND, BtAlimentador)) {
      ligaAlimentador = LOW;
      atualizar();//atualiza reles
    }
    ligaAlimentador = HIGH;
    atualizar();//atualiza reles

  }//end while
}//end manual
