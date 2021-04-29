void semiAuto(){


 

  btPedal = LOW;
  btAlimentador = LOW;
  ligaPrensa = LOW;
  ligaLuz = LOW;
  ligaAlimentador = LOW;
  intSensor = LOW;



 
   //chave seletora 2/4 manual com alimentador A0 = 14 LOW A1=15 HIGH semiAuto
   while (!TstBit(PINC, 0) && TstBit(PINC, 1)) {

    //      chave seletora de posição no modo 3/4 manual para controle de batida uma por vez
    //        -Liga o motor se estiver desligado aguarda X segundo para atingir velocidade de trabalho
    if (!ligaMotor) {
      ligaMotor = HIGH;
      SetBit(PORTB, ReleMotor);
      delay(1000);
    }//end if liga motor
    //        -Acionamento por pedal ou bimanual - bimanual com sistema individual
    if ((bimanual() || ( !btPedal && ! TstBit(PIND, 7))) && millis() - timerPrensa >= pot1) {
      btPedal = HIGH;
      ligaPrensa = HIGH;
    }
    if (btPedal && TstBit(PIND, 7))btPedal = LOW;
    if (intSensor && ligaPrensa) {
      contar();//conta as peças produzidas
      timerPrensa = millis();
      intSensor = LOW;
      ligaPrensa = LOW;
          //Alimentador ligar quando prensa chegar no sensor e ficar ligado pelo periodo de tempo do potenciometro
    if ( millis() - timerAlimentador >= pot2 * 2) {
      ligaAlimentador = HIGH;
      btAlimentador = HIGH;
      timerAlimentador = millis();
    }

    }

    //Alimentador ligar quando prensa chegar no sensor e ficar ligado pelo periodo de tempo do potenciometro
    if (!btAlimentador && !TstBit(PIND, BtAlimentador) && millis() - timerAlimentador >= pot2 * 2) {
      ligaAlimentador = HIGH;
      btAlimentador = HIGH;
      timerAlimentador = millis();
    }
    if (btAlimentador && TstBit(PIND, BtAlimentador))btAlimentador = LOW;
    if (millis() - timerAlimentador >= pot2)ligaAlimentador = LOW;
    atualizar();//atualiza reles

  }//end while

}//end semiAuto
