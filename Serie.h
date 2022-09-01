//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%%																												%%
//%%  LIBRERIA PROPIA: PUERTO SERIE																					%%
//%%																												%%
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool bDebug=true;

//====================================================================================================================
//====================================================================================================================
void Serie_Envia(char Parametro, String Texto)
{
  Serial.print(Parametro);
  Serial.print(F(":"));
  Serial.print(Texto);
  Serial.println(F("#"));
}

void Serie_Debug(String Texto)
{
  if (bDebug)
  {
     Serial.println(Texto);
  }
}


//====================================================================================================================
// INICIALIZACION
//====================================================================================================================
void Serie_Inicializa()
{
  //Serial.begin(9600);
  Serial.begin(57600);
  Serie_Envia('I', "serie");
  //Serial.println("Puerto serie inicializado a 9600 bps");
}

//====================================================================================================================
//====================================================================================================================
void Serie_Ayuda()
{
  Serial.println(F("Comandos disponibles:"));
  Serial.println(F(""));
  Serial.println(F("          BAJO NIVEL:"));
  Serial.println(F(""));
  Serial.println(F("              - 'u_'  : Arriba"));
  Serial.println(F("              - 'd_'  : Abajo"));
  Serial.println(F("              - 'l_'  : Izquierda"));
  Serial.println(F("              - 'r_'  : Derecha"));
  Serial.println(F("              - 'c_'  : centro"));
  Serial.println(F("              - 's_'  : Stop"));
  Serial.println(F("              - 'k_'  : Reset"));
  Serial.println(F("              - 't_'  : test (arriba/abajo/derecha/izquierda)"));
  Serial.println(F("              - 'g[grados]' : grados X: [g] + [grados]"));
  Serial.println(F("              - 'h[grados]' : grados Y: [h] + [grados]"));
  Serial.println(F("              - 'b[grados]' : incrementar grados X: [b] + [grados]"));
  Serial.println(F("              - 'e[grados]' : incrementar grados Y: [e] + [grados]"));
  Serial.println(F("              - 'f[grados]' : decrementar grados X: [f] + [grados]"));
  Serial.println(F("              - 'x[grados]' : decrementar grados Y: [x] + [grados]"));
  Serial.println(F("              - 'v_'  : Posicion actual X (grados)"));
  Serial.println(F("              - 'w_'  : Posicion actual Y (grados)"));
  Serial.println(F("              - '0_'  : Velocidad lenta"));
  Serial.println(F("              - '1_'  : Velocidad normal"));
  Serial.println(F("              - '2_'  : Velocidad rapida"));
  Serial.println(F(""));
  Serial.println(F("          ALTO NIVEL:"));
  Serial.println(F(""));
  Serial.println(F("              - 'n_'  : NO"));
  Serial.println(F("              - 'm_'  : SI"));
  Serial.println(F("              - 'o_'  : Ojear X"));
  Serial.println(F("              - 'p_'  : Ojear Y"));
  Serial.println(F("              - 'a_'  : Aleatorio"));
  Serial.println(F("              - 'i_'  : Triste"));
  Serial.println(F("              - 'j_'  : Susto"));
  Serial.println(F("              - 'q_'  : Sorpresa"));

}
