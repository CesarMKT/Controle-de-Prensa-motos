void semiAuto() {




  btPedal = LOW;
  btAlimentador = LOW;
  ligaPrensa = LOW;
  ligaLuz = LOW;
  ligaAlimentador = LOW;
  intSensor = LOW;
  sensor = LOW;
  int potSemiAuto = pot1;
  if (pot1 <= 350)potSemiAuto = 350;
  atualizar();


  //chave seletora 2/4 manual com alimentador A0 = 14 LOW A1=15 HIGH semiAuto
  while (!TstBit(PINC, 0) && TstBit(PINC, 1)) {

    //      chave seletora de posição no modo 3/4 manual para controle de batida uma por vez

    //        -Acionamento por pedal ou bimanual - bimanual com sistema individual
    if (bimanual()  && millis() - timerAlimentador >= potSemiAuto * 2)  {
      ligaAlimentador = HIGH;
      ligaPrensa = HIGH; // só liga prensa  se valor superior ida e volta alimentador
    }
    if (offSensor ){ // desliga se a prensa retornar antes do alimentador *2
      offSensor = LOW;
      ligaPrensa = LOW;
    } // end prensa off por tempo
    if (intSensor && ligaPrensa) {

      intSensor = LOW;
      sensor = HIGH;
      ligaAlimentador = HIGH;
      btAlimentador = HIGH;
      timerAlimentador = millis();


    }//end if liga prensa/alimentador

    //Alimentador ligar quando prensa chegar no sensor e ficar ligado pelo periodo de tempo do potenciometro
    if (!btAlimentador && !TstBit(PIND, BtAlimentador) && millis() - timerAlimentador >= potSemiAuto * 2) {
      ligaAlimentador = HIGH;
      btAlimentador = HIGH;
      timerAlimentador = millis();
    }
    if (btAlimentador && TstBit(PIND, BtAlimentador))btAlimentador = LOW;
    if (millis() - timerAlimentador >= potSemiAuto && sensor) {
      sensor = LOW;
      ligaAlimentador = LOW;
      contar();//conta as peças produzidas
    }
    atualizar();//atualiza reles

  }//end while

}//end semiAuto
