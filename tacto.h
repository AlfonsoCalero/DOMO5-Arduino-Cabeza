//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%																												%%
//%%  LIBRERIA PROPIA: SENSOR DEL TACTO																				%%
//%%																												%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%																												%%
//%%    - 1 sensor de tacto derecha         Pin 3   (posteriormente sera una interrupcion)							%%
//%%    - 1 sensor de tacto izquierda       Pin 2   (posteriormente sera una interrupcion)							%%
//%%																												%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  #define IPINTACTODERECHA  3
  #define IPINTACTOIZQUIERDA  2

    unsigned long tAntesTacto = 0;
    
//====================================================================================================================
// INICIALIZA
//====================================================================================================================
void Tacto_Inicializa()
{
    pinMode(IPINTACTODERECHA, INPUT);     // Led Test Tacto: Pin=2
    pinMode(IPINTACTOIZQUIERDA, INPUT);     // Led Test Tacto: Pin=3
    Serie_Envia('I',"Tacto");
}

void TactoDerecha_Gira()
{
       Serie_Envia('T',"D");                             // Envia al PC
     tAntesTacto = millis();
     Cara_Establece(iCaraSorpresa);
     GiraCuelloDerecha(iVelocidadRapidoX);          // Giramos el cuello
    //if (!bParo) GiraCuelloArriba(iVelocidadRapidoY);                // Mira arriba
    //if (!bParo) Espera(500);                     // Espera medio segundo
    //if (!bParo) GiraCuelloAbajo(iVelocidadRapidoY);// Mira abajo
    //if (!bParo) Espera(500);                     // Espera medio segundo
    //if (!bParo) GiraCuelloCentroY(iVelocidadRapidoY);// Mira centro
}
//====================================================================================================================
// Si lee del tacto, envia cuello a la derecha(directo desde arduino como si fuera un acto reflejo)
// Otra opcion seria enviar cadena serial y que fuera el procesador el que decidiera.
//====================================================================================================================
bool Tacto_Derecha_Lee()
{
  bool bDetectado=false;
  
  int estado = digitalRead(IPINTACTODERECHA);

   if (estado == HIGH && ( millis() - tAntesTacto > 3000))
   {
    TactoDerecha_Gira();
    bDetectado=true;
  }
  return bDetectado;
}



void TactoIzquierda_Gira()
{
       Serie_Envia('T',"I");                             // Envia al PC
     tAntesTacto = millis();
     Cara_Establece(iCaraSorpresa);
     GiraCuelloIzquierda(iVelocidadRapidoX);       // Giramos el cuello
    //if (!bParo) GiraCuelloArriba(iVelocidadRapidoY);                // Mira arriba
    //if (!bParo) Espera(500);                     // Espera medio segundo
    //if (!bParo) GiraCuelloAbajo(iVelocidadRapidoY);// Mira abajo
    //if (!bParo) Espera(500);                     // Espera medio segundo
    //if (!bParo) GiraCuelloCentroY(iVelocidadRapidoY);// Mira centro

}

//====================================================================================================================
// Si lee del tacto, envia cuello a la derecha(directo desde arduino como si fuera un acto reflejo)
// Otra opcion seria enviar cadena serial y que fuera el procesador el que decidiera.
//====================================================================================================================
bool Tacto_Izquierda_Lee()
{
    bool bDetectado=false;
    
  int estado = digitalRead(IPINTACTOIZQUIERDA);
 
   if (estado == HIGH && ( millis() - tAntesTacto > 3000))
   {
    TactoIzquierda_Gira();
    bDetectado=true;
  }
  return bDetectado;
}
