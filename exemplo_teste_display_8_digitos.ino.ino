/**
  testes para escrever numeros  e letras
  

*/

#include "LedController.hpp" //incluir a biblioteca do dsiplay de 8 digitos

#define CS 10 //O pin clock = 13 e o DIN = 11  CS pode ser alterado...

#define Segments 1 // define apenas 1 linha

#define digitsPerSegment 8 // define que tem 8 caracteres

//offset to switch which Digits will be used for output
//play around with this value
#define positionOffset 0

//delay before incrementing the counter
#define delayTime 500

//the uninitilized controller object
LedController<Segments, 1> lc = LedController<Segments, 1>();

//This function calculates the largest number that can be displayed
unsigned long getLargestNumber()
{
  return (unsigned long)pow(10, Segments * digitsPerSegment);
}

//this function sets the Segments to a given number
void setLEDs(unsigned long number)
{
  //the loop is used to split the given number and set the right digit on the Segments
  for (unsigned int i = 0; i < Segments * digitsPerSegment; i++)
  {
    unsigned long divisor = 2;
    for (unsigned int j = 0; j < i; j++)
    {
      divisor *= 10;
    }

    byte num = number / divisor % 10;
    if (i % digitsPerSegment + positionOffset == 3)
      lc.setDigit(Segments - i / digitsPerSegment - 1, i % digitsPerSegment + positionOffset, num, true);
    else if (i % digitsPerSegment + positionOffset == 6)
      lc.setDigit(Segments - i / digitsPerSegment - 1, i % digitsPerSegment + positionOffset, num, true);
    else
      lc.setDigit(Segments - i / digitsPerSegment - 1, i % digitsPerSegment + positionOffset, num, false);
  }
}

void setup()
{

  //just make sure that the config is valid
  static_assert(positionOffset + digitsPerSegment < 9, "invalid configuration");

  //initilize a ledcontroller with a hardware spi and one row
  lc.init(CS);

  //disables all Digits by default
  for (unsigned int i = 0; i < Segments; i++)
  {
    for (unsigned int j = 0; j < 8; j++)
    {
      lc.setRow(i, j, 0x00);
    }
  }

  //Set them to the lowest brightness
  lc.setIntensity(5);
}

void loop()
{

  //clear the matrix just to be sure there is nothing on it
  lc.clearMatrix();
  //calculate the highest possible number and set it as end condition for the loop
  auto limit = getLargestNumber();

  //count up and display the number
  for (unsigned long i = 99990000; i < limit; i++)
  {
    setLEDs(i);
    delay(delayTime);
  }
}
