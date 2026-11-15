const int LED = 9;
const unsigned long PERIODO = 5000; // Un ciclo cada 2 segundos

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  float fase = (millis() % PERIODO) / (float)PERIODO;
  int brillo = round(127.5 * (1 - cos(2 * PI * fase)));

  analogWrite(LED, brillo);
}