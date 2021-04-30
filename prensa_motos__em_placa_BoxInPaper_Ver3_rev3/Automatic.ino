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
 

  bool modoAuto, direito, esquedo, pedal;
  pedal = LOW;
  modoAuto = LOW;


  //chave seletora 1/4 automático A0 = 14  e A1 = 15  LOW
  while (!TstBit(PINC, 0) && !TstBit(PINC, 1)) {


  
      while (!TstBit(PIND, 5)) { // soma 1 unidade pot 1
        pot1=pot1+5;
        if (pot1>=maxAlimentador)pot1=maxAlimentador;
        displayP1(pot1);
        delay(100);
      }//end while pot1
      while (!TstBit(PIND, 6)) {
        pot1=pot1-5;
        if(pot1<=minAlimentador)pot1=minAlimentador;
        displayP1(pot1);
        delay(100);
      }//end While pot1
      
     if(!TstBit(PIND,3)) displayN(contador);


     
    //Alimentador ligar quando prensa chegar no sensor e ficar ligado pelo periodo de tempo do potenciometro
    if (!btAlimentador && !TstBit(PIND, BtAlimentador) && millis() - timerAlimentador >= pot1 * 2) {
      ligaAlimentador = HIGH;
      btAlimentador = HIGH;
      timerAlimentador = millis();
    }
    if (btAlimentador && TstBit(PIND, BtAlimentador))btAlimentador = LOW;
    if (millis() - timerAlimentador >= pot1)ligaAlimentador = LOW;
    atualizar() ;

    //        -Acionamento por pedal ou bimanual - bimanual com sistema individual

    if (TstBit(PIND, BtParada ))chaveAuto = LOW;
    if (!TstBit(PIND, BtParada ) && !chaveAuto ) {
      modoAuto = HIGH;
      chaveAuto = HIGH;
    }

    while (modoAuto) {
      if (TstBit(PIND, BtParada ))chaveAuto = LOW; //confirme que botão foi solto
      if (!chaveAuto && contadorAuto < limiteContadorAuto) { // confirmar numero minimo de batidas para travar no modo auto
        modoAuto = LOW;
        return;
      }
      if(contadorAuto > limiteContadorAuto){// confirme numero minimo de batidas para indicar modo auto travado
        ligaLuz=HIGH;
      }

      if (!TstBit(PIND, BtParada ) && !chaveAuto) {
        chaveAuto = HIGH;
        modoAuto = LOW;
        ligaLuz=LOW;
        return;
      }
      if (TstBit(PINC, 0) || TstBit(PINC, 1)) return; //  sai se a chave seletora for alterada

      if (millis() -  timerAlimentador  >= pot1*2) {
        btPedal = HIGH;
        ligaPrensa = HIGH;
       
      }
      if (intSensor && ligaPrensa) {
        contar();//conta as peças produzidas

        intSensor = LOW;
       
        //Alimentador ligar quando prensa chegar no sensor e ficar ligado pelo periodo de tempo do potenciometro
        if ( millis() - timerAlimentador >= pot1 * 2) {
          ligaAlimentador = HIGH;
          btAlimentador = HIGH;
          timerAlimentador = millis();
        }

      }
      if (millis() - timerAlimentador >= pot1)ligaAlimentador = LOW;

      atualizar();//atualiza reles

  
   
      
    }//end while modoAuto


  }//end while seletor

}//end automatico
