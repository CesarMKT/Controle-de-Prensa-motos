void automatico(){



  btPedal = LOW;
  btAlimentador = LOW;
  ligaPrensa = LOW;
  ligaLuz = LOW;
  ligaAlimentador = LOW;
  intSensor = LOW;
  
    //        -Liga o motor se estiver desligado aguarda X segundo para atingir velocidade de trabalho
    if (!ligaMotor) {
      ligaMotor = HIGH;
      SetBit(PORTB, ReleMotor);
      delay(1000);
    }//end if liga motor

  bool modoAuto, direito, esquedo, pedal;
  pedal=LOW;
  modoAuto=LOW;

  //chave seletora 1/4 automático A0 = 14  e A1 = 15  LOW
  while (!TstBit(PINC, 0) && !TstBit(PINC, 1)) {

    

    //        -Acionamento por pedal ou bimanual - bimanual com sistema individual

if(bimanual() || ( !btPedal && ! TstBit(PIND, 7))){
  modoAuto=HIGH;
  
}
    
    while(modoAuto){

      if(!TstBit(PIND,BtParada ))modoAuto=LOW;
    
    if (millis() - timerPrensa >= pot1) {
      btPedal = HIGH;
      ligaPrensa = HIGH;
      timerPrensa = millis();
    }
    if (btPedal && TstBit(PIND, 7))btPedal = LOW;
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

    //Alimentador ligar quando prensa chegar no sensor e ficar ligado pelo periodo de tempo do potenciometro
    if (!btAlimentador && !TstBit(PIND, BtAlimentador) && millis() - timerAlimentador >= pot2 * 2) {
      ligaAlimentador = HIGH;
      btAlimentador = HIGH;
      timerAlimentador = millis();
    }
    if (btAlimentador && TstBit(PIND, BtAlimentador))btAlimentador = LOW;
    if (millis() - timerAlimentador >= pot2)ligaAlimentador = LOW;
    atualizar();//atualiza reles
    }//end while modoAuto


  }//end while seletor
  
}//end automatico
