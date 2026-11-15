
const int PIN_LED = 8;
const int UNIDAD  = 200;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  letraS();
  letraO();
  letraS();
  delay(UNIDAD * 7);       // pausa entre palabras
}

// Enciende el LED durante 'duracion' y luego hace la pausa interna
void senal(int duracion) {
  digitalWrite(PIN_LED, HIGH);
  delay(duracion);
  digitalWrite(PIN_LED, LOW);
  delay(UNIDAD);           // pausa entre símbolos
}

void punto() { senal(UNIDAD); }
void raya()  { senal(UNIDAD * 3); }

void letraS() {
  for (int i = 0; i < 3; i++) punto();
  delay(UNIDAD * 2);       // +1 de la última señal = 3 unidades entre letras
}

void letraO() {
  for (int i = 0; i < 3; i++) raya();
  delay(UNIDAD * 2);
}


