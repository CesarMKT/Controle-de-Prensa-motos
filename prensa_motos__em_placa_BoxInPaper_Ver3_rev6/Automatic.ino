void automatico() {

  // zerar variaveis na trocca de chave para nova posição.
  contadorAuto = 0;
  btPedal = LOW;
  btAlimentador = LOW;
  ligaPrensa = LOW;
  ligaLuz = LOW;
  ligaAlimentador = LOW;
  intSensor = LOW;
  sensor = LOW;
  bool direito, esquedo;
  bool pedal = LOW;
  bool modoAuto = LOW;

  atualizar() ;  //atualizar reles  antes de entrar no laço que trava pela chave seletora

  //chave seletora 1/4 automático A0 = 14  e A1 = 15  LOW
  while (!TstBit(PINC, 0) && !TstBit(PINC, 1)) {

    //ajustar tempo de alimentador/////////////////////////////
    while (!TstBit(PIND, 6)) { // soma 1 unidade pot 1
      pot1 = pot1 + 20;
      if (pot1 >= maxAlimentador)pot1 = maxAlimentador;
      displayP1(pot1);
      delay(100);
    }//end while pot1
    while (!TstBit(PIND, 5)) {
      pot1 = pot1 - 20;
      if (pot1 <= minAlimentador)pot1 = minAlimentador;
      displayP1(pot1);
      delay(100);
    }//end While pot1
    if (!TstBit(PIND, 3)) displayN(contador);// volta/atualiza o numerador no display
    //ajustar tempo de alimentador////////////////////////////

    sair = HIGH;
    contadorAuto = 0;

    if (offSensor) {
      offSensor = LOW;
      ligaPrensa = LOW;
    }
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
        sair = LOW;
      }
      if (contadorAuto > limiteContadorAuto) { // confirme numero minimo de batidas para indicar modo auto travado
        ligaLuz = HIGH;
      }

      if (!TstBit(PIND, BtParada ) && !chaveAuto) {
        chaveAuto = HIGH;
        modoAuto = LOW;
        ligaLuz = LOW;
        sair = LOW; // return;
      }
      if (TstBit(PINC, 0) || TstBit(PINC, 1)) return; //  sai se a chave seletora for alterada

      if (millis() -  timerAlimentador  >= pot1 * 2) {
        btPedal = HIGH;
        ligaPrensa = HIGH;
        ligaAlimentador = HIGH;

      }
      if (offSensor) {
        offSensor = LOW;
        ligaPrensa = LOW;
      }
      if (intSensor && ligaPrensa) {

        sensor = HIGH;
        intSensor = LOW;

        //Alimentador ligar quando prensa chegar no sensor e ficar ligado pelo periodo de tempo do potenciometro
        if ( millis() - timerAlimentador >= pot1 * 2 ) {

          btAlimentador = HIGH;
          timerAlimentador = millis();
        }

      }
      if (millis() - timerAlimentador >= pot1 && sensor) {
        contar();//conta as peças produzidas
        sensor = LOW;
        ligaAlimentador = LOW;
        if (!sair)return;
      }

      atualizar();//atualiza reles




    }//end while modoAuto


  }//end while seletor

}//end automatico
