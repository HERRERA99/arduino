const int LED_VERDE = 2;
const int LED_AMARILLO = 3; 
const int LED_ROJO = 4;
const int TRIG = 12;
const int ECHO = 13;
const int BUZZER = 8;

const float RANGO_ROJO = 10.0;
const float RANGO_AMARILLO = 40.0;

void setup() {
  Serial.begin(9600);
  Serial.println("Hola");

  // Leds
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AMARILLO, LOW);
  digitalWrite(LED_ROJO, LOW);

  pinMode(TRIG, OUTPUT); // Arduino envía la orden.
  pinMode(ECHO, INPUT);  // Arduino recibe el pulso.

  // Buzzer
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  digitalWrite(TRIG, LOW);
}

void loop() {
  // Suena buzzer
  // digitalWrite(BUZZER, HIGH);
  // delay(500);

  // digitalWrite(BUZZER, LOW);
  // delay(500);

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  // Solicito medicion
  Serial.println("Solicito medicion");
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // Recibo señal
  unsigned long tiempo = pulseIn(ECHO, HIGH, 30000UL);

  // Comprobamos que llega
  if (tiempo == 0) {
    Serial.println("Sin medicion");
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_AMARILLO, LOW);
    digitalWrite(LED_ROJO, LOW);

    actualizarBuzzer(-1); // Añadido: apagar si no hay medición.
  } else {
    // 5. Convertir el tiempo en distancia.
    float distancia = tiempo * 0.0343 / 2.0;

    Serial.print("Tiempo: ");
    Serial.print(tiempo);
    Serial.print(" us | Distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");

    if (distancia <= RANGO_ROJO) {
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AMARILLO, LOW);
      digitalWrite(LED_ROJO, HIGH);
    } else if (distancia <= RANGO_AMARILLO) {
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AMARILLO, HIGH);
      digitalWrite(LED_ROJO, LOW);
    } else {
      digitalWrite(LED_VERDE, HIGH);
      digitalWrite(LED_AMARILLO, LOW);
      digitalWrite(LED_ROJO, LOW);
    }

    actualizarBuzzer(distancia); // Añadido.
  }

  // Esperar antes de hacer otra medición.
  delay(100);

}

// Función añadida: controla los pitidos sin añadir esperas.
void actualizarBuzzer(float distancia) {
  static bool encendido = false;
  static unsigned long ultimoCambio = 0;

  unsigned long ahora = millis();

  if (distancia < 0 || distancia > RANGO_AMARILLO) {
    // Sin medición o más de 40 cm: silencio.
    encendido = false;
    ultimoCambio = ahora;

  } else if (distancia <= RANGO_ROJO) {
    // A 10 cm o menos: sonido continuo.
    encendido = true;
    ultimoCambio = ahora;

  } else {
    // Cuanto más cerca, menor pausa entre pitidos.
    unsigned long pausa = map(
      (long)distancia,
      (long)RANGO_ROJO,
      (long)RANGO_AMARILLO,
      80,
      700
    );

    unsigned long espera = encendido ? 60 : pausa;

    if (ahora - ultimoCambio >= espera) {
      encendido = !encendido;
      ultimoCambio = ahora;
    }
  }

  digitalWrite(BUZZER, encendido ? HIGH : LOW);
}