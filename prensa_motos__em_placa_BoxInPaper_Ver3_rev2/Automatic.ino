void automatico() {


  contadorAuto = 0;
  btPedal = LOW;
  btAlimentador = LOW;
  ligaPrensa = LOW;
  ligaLuz = LOW;
  ligaAlimentador = LOW;
  intSensor = LOW;
  atualizar() ;
  //        -Liga o motor se estiver desligado aguarda X segundo para atingir velocidade de trabalho
  if (!ligaMotor) {
    ligaMotor = HIGH;
    SetBit(PORTB, ReleMotor);
    delay(1000);
  }//end if liga motor

  bool modoAuto, direito, esquedo, pedal;
  pedal = LOW;
  modoAuto = LOW;


  //chave seletora 1/4 automático A0 = 14  e A1 = 15  LOW
  while (!TstBit(PINC, 0) && !TstBit(PINC, 1)) {


    //Alimentador ligar quando prensa chegar no sensor e ficar ligado pelo periodo de tempo do potenciometro
    if (!btAlimentador && !TstBit(PIND, BtAlimentador) && millis() - timerAlimentador >= pot2 * 2) {
      ligaAlimentador = HIGH;
      btAlimentador = HIGH;
      timerAlimentador = millis();
    }
    if (btAlimentador && TstBit(PIND, BtAlimentador))btAlimentador = LOW;
    if (millis() - timerAlimentador >= pot2)ligaAlimentador = LOW;
    atualizar() ;

    //        -Acionamento por pedal ou bimanual - bimanual com sistema individual

    if (TstBit(PIND, BtParada ))chaveAuto = LOW;
    if (!TstBit(PIND, BtParada ) && !chaveAuto ) {
      modoAuto = HIGH;
      chaveAuto = HIGH;
    }

    while (modoAuto) {
      if (TstBit(PIND, BtParada ))chaveAuto = LOW;
      if (!chaveAuto && contadorAuto < limiteContadorAuto) {
        modoAuto = LOW;
        return;
      }

      if (!TstBit(PIND, BtParada ) && !chaveAuto) {
        chaveAuto = HIGH;
        modoAuto = LOW;
        return;
      }
      if (TstBit(PINC, 0) || TstBit(PINC, 1)) return; //  sai se a chave seletora for alterada

      if (millis() - timerPrensa >= pot1) {
        btPedal = HIGH;
        ligaPrensa = HIGH;
        timerPrensa = millis();
      }
      if (intSensor && ligaPrensa) {
        contar();//conta as peças produzidas

        intSensor = LOW;
        ligaPrensa = LOW;
        //Alimentador ligar quando prensa chegar no sensor e ficar ligado pelo periodo de tempo do potenciometro
        if ( millis() - timerAlimentador >= pot2 * 2) {
          ligaAlimentador = HIGH;
          btAlimentador = HIGH;
          timerAlimentador = millis();
        }

      }
      if (millis() - timerAlimentador >= pot2)ligaAlimentador = LOW;

      atualizar();//atualiza reles


    if (!TstBit(PIND, 5)) { // exibe o pot1
        pot1 = map(analogRead(A2), 0, 1023, minPrensa, maxPrensa); // le e memoriza pot1
        displayP1(pot1);
        
      }//end if pot1
      if (!TstBit(PIND, 6)) {
        pot2 = map(analogRead(A3), 0, 1023, minAlimentador, maxAlimentador); // le e memoriza pot1
        displayP2(pot2);       
      }//end if pot2

      
    }//end while modoAuto


  }//end while seletor

}//end automatico
