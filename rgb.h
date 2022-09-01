//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%                                                                                                                %%
//%%  LIBRERIA PROPIA: RGB                                                                                          %%
//%%                                                                                                                %%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%                                                        %%
//%%    - pin 11 , 16 RGB's %%
//%%                                                        %%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Este ejemplo muestra varias formas de configurar y usar 'paletas' de colores
// con FastLED.
//
// Estas paletas compactas brindan una manera fácil de volver a colorear su
// animación sobre la marcha, rápida, fácil y con poca sobrecarga.
//
// USAR paletas es MUCHO más simple en la práctica que en teoría, así que primero
// ejecute este boceto y observe las bonitas luces mientras lee
// el código. Aunque este boceto tiene ocho (o más) esquemas de color diferentes,
// todo el boceto se compila hasta aproximadamente 6.5K en AVR.
//
// FastLED proporciona algunas paletas de colores preconfiguradas y lo hace
// extremadamente fácil de crear sus propios esquemas de color con paletas.
//
// Algunas notas sobre la 'teoría y práctica' más abstracta de
// Las paletas compactas FastLED se encuentran al final de este archivo.
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include <FastLED.h>

#define LED_PIN     11
#define NUM_LEDS    16
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

unsigned long Rgb_TiempoInicioTemporizador = 0;
unsigned int Rgb_NumeroCiclos=0;
bool Rgb_Encendido=true;

//====================================================================================================================
// Esta función configura una paleta de rayas moradas y verdes.
//====================================================================================================================
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


//====================================================================================================================
// This function fills the palette with totally random colors.
//====================================================================================================================
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

//====================================================================================================================
// Esta función configura una paleta de rayas blancas y negras,
// usando código. Dado que la paleta es efectivamente una matriz de
// dieciséis colores CRGB, se pueden utilizar las diversas funciones fill_*
// para configurarlos.
//====================================================================================================================
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

//====================================================================================================================
// Aquí se muestran varias paletas de colores diferentes.
//
// FastLED proporciona varias paletas 'preestablecidas': RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p y PartyColors_p.
//
// Además, puede definir manualmente sus propias paletas de colores, o puede escribir
// código que crea paletas de colores sobre la marcha. Todos se muestran aquí.
//====================================================================================================================
//void CambiarPaletaPeriodicamente()
//{
//    uint8_t secondHand = (millis() / 1000) % 60;
//    static uint8_t lastSecond = 99;
//    
//    if( lastSecond != secondHand) {
//        lastSecond = secondHand;
//        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
//        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
//        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
//        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
//        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
//        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
//        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
//        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
//        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
//        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
//        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
//    }
//}

//====================================================================================================================
//====================================================================================================================
void RellenarLEDSDePaletaColores( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}




//====================================================================================================================
// Este ejemplo muestra cómo configurar una paleta de colores estática
// que se almacena en PROGMEM (flash), que casi siempre es más
// abundante que RAM. Una paleta PROGMEM estática como esta
// ocupa 64 bytes de flash.
//====================================================================================================================
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};

//============================================================================
// Espera no bliqueante del loop
//============================================================================
bool Rgb_Esperar()
{
  
    bool bTemp_RGB=false;

      if (millis() >= (Rgb_TiempoInicioTemporizador+10))
      {
        Rgb_NumeroCiclos++;
        // Duracion del efecto = 200 ciclos
        if (Rgb_NumeroCiclos<400)
        {
          // Fin periodo
          Rgb_TiempoInicioTemporizador=millis();
          bTemp_RGB=true; // Fin de espera
        }
        else
        {
          // Apagar el rgb
          Rgb_Encendido=false;
          FastLED.clear();  // clear all pixel data
          FastLED.show();
        }
      }

    return bTemp_RGB;
}

//====================================================================================================================
//====================================================================================================================
void Rgb_Start()
{
  Rgb_NumeroCiclos=0;
  Rgb_TiempoInicioTemporizador=millis();
  Rgb_Encendido=true;
}

//====================================================================================================================
// Esto es lo que va en el loop
//====================================================================================================================
void Pinta_Rgb()
{

  if (Rgb_Encendido)
  {
    bool Rgb_bFin = Rgb_Esperar();
    if (Rgb_bFin)
    {
      static uint8_t Rgb_startIndex = 0;
      Rgb_startIndex = Rgb_startIndex + 1; /* motion speed */
      RellenarLEDSDePaletaColores( Rgb_startIndex);
      FastLED.show();
    } 
   
  }
}

//====================================================================================================================
// Efecto Rainbow (el inicial)
//====================================================================================================================
void Rgb_Rainbow()
{
    Rgb_Start(); currentPalette = RainbowColors_p;  currentBlending = LINEARBLEND; // standard rainbow animation
}

void Rgb_Cloud()
{
  Rgb_Start(); currentPalette = CloudColors_p;  currentBlending = LINEARBLEND;         // blue and white
}

void Rgb_Party()
{
  Rgb_Start(); currentPalette = PartyColors_p;  currentBlending = LINEARBLEND;        // red, yellow, orange, purple and blue
}

void Rgb_Heat()
{
  Rgb_Start(); currentPalette = HeatColors_p;  currentBlending = LINEARBLEND;         // red, orange, yellow and white
}
        
void Rgb_Rojo()
{
    Rgb_Start();  currentPalette = LavaColors_p; currentBlending = LINEARBLEND;// orange, red, black and yellow)
}

void Rgb_Azul()
{
    Rgb_Start();     currentPalette = OceanColors_p; currentBlending = LINEARBLEND;// blue, cyan and white
}

void Rgb_Verde()
{
    Rgb_Start();      currentPalette = ForestColors_p; currentBlending = LINEARBLEND;// greens and blues
}

void Rgb_BlancoNegro()
{
Rgb_Start();  SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; 
}

void Rgb_Aleatorio()
{
Rgb_Start();  SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; 
}

void Rgb_VioletaVerde()
{
 Rgb_Start();  SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND;
}


//====================================================================================================================
// INICIALIZACION
//====================================================================================================================
void Rgb_Inicializa()
{
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    Rgb_Rainbow();
    
    Serie_Envia('I', "Rgb");
}

//====================================================================================================================
// Notas adicionales sobre las paletas compactas FastLED:
//====================================================================================================================
//
// Normalmente, en gráficos por computadora, la paleta (o "tabla de búsqueda de colores")
// tiene 256 entradas, cada una de las cuales contiene un color RGB específico de 24 bits. entonces puedes
// indexar en la paleta de colores utilizando un valor simple de 8 bits (un byte).
// Una paleta de colores de 256 entradas ocupa 768 bytes de RAM, que en Arduino
// es muy posiblemente "demasiados" bytes.
//
// FastLED ofrece paletas tradicionales de 256 elementos, para configuraciones que
// puede pagar el costo de 768 bytes en RAM.
//
// Sin embargo, FastLED también ofrece una alternativa compacta. Ofertas FastLED
// paletas que almacenan 16 entradas distintas, pero se puede acceder COMO SI
// en realidad tienen 256 entradas; esto se logra interpolando
// entre las 16 entradas explícitas para crear quince paletas intermedias
// entradas entre cada par.
//
// Entonces, por ejemplo, si establece las dos primeras entradas explícitas de un compacto
// paleta a verde (0,255,0) y azul (0,0,255), y luego recuperada
// las primeras dieciséis entradas de la paleta virtual (de 256), obtendrías
// Verde, seguido de un degradado suave de verde a azul y luego Azul.
//====================================================================================================================
