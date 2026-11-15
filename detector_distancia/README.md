# Detector de distancia y avisador de aparcamiento

[Volver al índice](../README.md) · [Ver programa](./detector_distancia.ino) · [Anterior: fundido del LED](../led_analogico/README.md)

## Qué hace

El programa utiliza un sensor ultrasónico **HC-SR04** para medir distancia, tres LED para indicar un rango y un buzzer activo **TMB12A05** para producir avisos. Cuanto más se acerca el objeto dentro del rango amarillo, más cortas son las pausas entre pitidos. Al llegar al rango rojo, el sonido es continuo.

Se cambia la **frecuencia de repetición de los pitidos**, no el tono grave o agudo del buzzer. Los LED llevan resistencias individuales de **220 Ω**.

## Comportamiento del programa actual

| Resultado de la medición | LED verde | LED amarillo | LED rojo | Buzzer |
|---|---|---|---|---|
| Más de 40 cm | Encendido | Apagado | Apagado | Silencio |
| Más de 10 cm y hasta 40 cm | Apagado | Encendido | Apagado | Pitidos con pausas variables |
| 10 cm o menos, con pulso recibido | Apagado | Apagado | Encendido | Continuo |
| No llega un pulso completo a tiempo | Encendido | Apagado | Apagado | Silencio |

Los umbrales proceden de `RANGO_ROJO = 10.0` y `RANGO_AMARILLO = 40.0`. En la igualdad, 10 cm pertenece al rango rojo y 40 cm al amarillo.

> **Limitación del código actual:** el caso `Sin medicion` deja el verde encendido. Ese estado indica falta de un dato válido y no demuestra que el camino esté libre. Esta documentación conserva y explica lo que hace el sketch; no cambia esa decisión.

## Material

### Sensor y luces

- Arduino Uno R3 con cable USB de datos.
- HC-SR04 de cuatro patillas.
- LED verde, amarillo y rojo.
- **Tres resistencias de 220 Ω**, una por cada LED, preferiblemente de ¼ W.
- Protoboard, cables macho a macho y cuatro cables macho a hembra para el sensor.

### Etapa recomendada para el buzzer

- Buzzer activo **TMB12A05** de 5 V.
- Transistor NPN adecuado, por ejemplo **BC337**.
- **Cuatro resistencias adicionales de 220 Ω en serie**, que suman 880 Ω, para la base del transistor.
- Diodo de protección **1N4007**.

Este montaje recomendado utiliza únicamente resistencias individuales de 220 Ω: tres para los LED y cuatro para formar la resistencia de base. La etapa de transistor no se puede deducir del `.ino` y su presencia en el montaje físico no se ha confirmado. Comprueba la referencia y el patillaje del transistor antes de montarlo; las posiciones de base, colector y emisor varían entre modelos y encapsulados.

## Mapa de pines

| Constante | Pin Arduino | Conexión |
|---|---:|---|
| `LED_VERDE` | D2 | Resistencia de 220 Ω y ánodo del LED verde |
| `LED_AMARILLO` | D3 | Resistencia de 220 Ω y ánodo del LED amarillo |
| `LED_ROJO` | D4 | Resistencia de 220 Ω y ánodo del LED rojo |
| `BUZZER` | D8 | Resistencia equivalente de 880 Ω y base del NPN |
| `TRIG` | D12 | Patilla TRIG del HC-SR04 |
| `ECHO` | D13 | Patilla ECHO del HC-SR04 |
| Alimentación | 5V | VCC del HC-SR04 y positivo del buzzer |
| Referencia | GND | GND del HC-SR04, cátodos de los LED y emisor del NPN |

Todos los GND del circuito deben estar unidos. Los números son los escritos junto a los conectores del Arduino. En particular, este sketch utiliza **TRIG en 12 y ECHO en 13**.

## Esquema de conexión del sensor y los LED

```text
ARDUINO UNO R3                           HC-SR04

5V ──────────────────────────────────── VCC
D12 ─────────────────────────────────── TRIG
D13 ◀────────────────────────────────── ECHO
GND ─────────────────────────────────── GND


D2 ────── [R1: 220 Ω] ───── A LED VERDE K ───────┐
D3 ────── [R2: 220 Ω] ───── A LED AMARILLO K ────┤
D4 ────── [R3: 220 Ω] ───── A LED ROJO K ────────┤
                                                │
GND ────────────────────────────────────────────┘
```

Cada LED tiene su propio recorrido pin → resistencia → ánodo → cátodo → GND. No unas los ánodos entre sí.

El HC-SR04 ya es un módulo de 5 V: su alimentación **no lleva una resistencia de 220 Ω en serie**. Sigue las etiquetas impresas en el sensor, porque al mirarlo por detrás cambia el orden visual de las patillas. El extremo hembra del jumper entra en la patilla del sensor y el extremo macho en el Arduino.

## Esquema recomendado para el TMB12A05

La [ficha del TMB12A05](https://cdck-file-uploads-global.s3.dualstack.us-west-2.amazonaws.com/digikey/original/3X/0/6/06ce1a41fadf8ce314498ca1cd3938833cb5a855.pdf) lo identifica como un buzzer electromagnético activo, de 5 V y hasta 30 mA. El pin D8 debe controlar una etapa de conmutación, porque ese consumo supera los 20 mA por pin especificados para el Uno R3. La alimentación del buzzer procede del conector **5V**.

```text
                     +5 V
                       │
                       ├──────────────────────────────┐
                       │                              │
                 (+) TMB12A05                  K, cátodo con raya
                 (−) buzzer                       D1: 1N4007
                       │                         A, ánodo sin raya
                       ├──────────────────────────────┘
                       │
                    C, colector
                     Q1: NPN
D8 ─ [220 Ω] ─ [220 Ω] ─ [220 Ω] ─ [220 Ω] ─ B, base
                    E, emisor
                       │
GND ───────────────────┘
```

La colocación del transistor en el dibujo es funcional: **no representa el orden físico de sus patas**. Los tres terminales pertenecen al mismo Q1.

| Elemento | Conexiones exactas |
|---|---|
| Buzzer positivo, marcado `+` | Arduino 5V |
| Buzzer negativo | Colector del NPN |
| Emisor del NPN | Arduino GND |
| Base del NPN | D8 a través de las cuatro resistencias de 220 Ω en serie |
| Diodo, cátodo con raya | Positivo del buzzer / 5V |
| Diodo, ánodo sin raya | Negativo del buzzer / colector |

Las cuatro resistencias suman `220 + 220 + 220 + 220 = 880 Ω`. Su función es limitar la corriente de base; no están en serie con la alimentación del buzzer. Con una caída base-emisor aproximada de 0,7 V, la corriente de base se estima en `(5 - 0,7) / 880 ≈ 4,9 mA`.

El transistor actúa como interruptor: `digitalWrite(BUZZER, HIGH)` permite que circule corriente por el buzzer y `LOW` lo desconecta. El diodo se coloca en paralelo como protección frente a transitorios al cortar una carga inductiva. La raya del diodo debe quedar hacia **5V**.

## Montaje por etapas

1. **Desconecta el USB** y monta únicamente el sensor: VCC a 5V, GND a GND, TRIG a D12 y ECHO a D13.
2. Conecta el USB, carga el programa y abre el monitor serie a **9600 baudios**. Puedes comprobar la distancia con los LED y el buzzer todavía desconectados.
3. Coloca un libro de frente al sensor, primero a 20–30 cm. Busca una distancia numérica en el monitor.
4. Desconecta de nuevo el USB y añade los tres LED con sus resistencias independientes de 220 Ω. Repite la prueba con objetos a unos 50, 20 y 8 cm.
5. Con el USB desconectado, monta la etapa NPN y su diodo, verificando el patillaje de Q1 en su ficha. Añade el TMB12A05 y prueba los pitidos.

No conectes este buzzer directamente entre D8 y GND para intentar reproducir el esquema. Si solo dispones del buzzer, los cables y la protoboard, puedes completar la prueba de sensor y luces; la etapa recomendada requiere el transistor, el diodo y las resistencias de base.

## Cómo calcula la distancia

El sensor emite ultrasonidos y recibe el eco reflejado en un objeto. El tiempo representado por ECHO incluye el recorrido de ida y vuelta. El [fabricante del HC-SR04](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) especifica alimentación de 5 V, un disparo de al menos 10 µs y un alcance nominal de 2–400 cm.

El programa prepara TRIG en `LOW`, lo mantiene en `HIGH` durante 10 µs y lo devuelve a `LOW`. Después ejecuta:

```cpp
unsigned long tiempo = pulseIn(ECHO, HIGH, 30000UL);
```

`pulseIn()` mide la duración de un pulso alto en microsegundos. El límite de espera es de 30 000 µs, es decir, 30 ms. Si no obtiene un pulso completo, devuelve cero: significa falta de una medida válida, no distancia cero. Consulta la [referencia de pulseIn()](https://docs.arduino.cc/language-reference/en/functions/advanced-io/pulseIn/).

La conversión utilizada es:

```cpp
float distancia = tiempo * 0.0343 / 2.0;
```

`0.0343` aproxima la velocidad del sonido a temperatura ambiente en centímetros por microsegundo. La división entre dos elimina la vuelta del eco. Por ejemplo, un pulso de 1000 µs produce aproximadamente 17,15 cm.

El alcance nominal no garantiza una lectura en todas las superficies. Telas, ángulos oblicuos, objetos estrechos y distancias fuera del rango pueden dificultar la recepción del eco.

## Cómo cambian los pitidos

`actualizarBuzzer(distancia)` se llama una vez por medición, o recibe `-1` cuando esta falla. Dos variables `static` conservan su contenido entre llamadas:

- `encendido`: si el buzzer debe estar sonando.
- `ultimoCambio`: el instante del último cambio de estado, obtenido con `millis()`.

Entre 10 y 40 cm se calcula la pausa mediante:

```cpp
unsigned long pausa = map(
  (long)distancia,
  (long)RANGO_ROJO,
  (long)RANGO_AMARILLO,
  80,
  700
);
```

`map()` asigna pausas menores a distancias menores. La conversión a `long` elimina los decimales para ese cálculo; no modifica la distancia original que se imprime en el monitor.

| Distancia de ejemplo | Pausa solicitada | Comportamiento |
|---|---:|---|
| 40 cm | 700 ms | Pitidos separados |
| 25 cm | 390 ms | Pitidos más frecuentes |
| 11 cm | 100 ms | Pitidos rápidos |
| 10 cm o menos | No se usa la pausa | Sonido continuo |

El valor de 80 ms es el extremo matemático asignado a 10 cm, pero esa distancia entra en la rama de sonido continuo. El pitido solicita una duración de 60 ms y el silencio la pausa calculada.

La condición `ahora - ultimoCambio >= espera` comprueba si ya toca alternar el estado. Si no toca, la función termina sin añadir un `delay()`.

## Limitaciones temporales del código actual

El programa conserva `delay(100)` al final del `loop()`, y `pulseIn()` también espera el eco. Por eso:

- La función del buzzer no añade esperas, pero el programa completo sigue siendo bloqueante durante esas llamadas.
- Los cambios de sonido solo se revisan una vez por vuelta, separadas por al menos unos 100 ms más el tiempo de medición y de ejecución.
- Un pitido solicitado de 60 ms durará hasta la siguiente llamada que detecte el vencimiento; con este bucle suele durar al menos unos 100 ms.
- Las pausas de la tabla son las solicitadas al temporizador, no tiempos acústicos exactos medidos.

Cambiar la medición a una planificación con `millis()` permitiría revisar el buzzer más a menudo. Eliminar la espera de `pulseIn()` requeriría además otra forma de capturar los cambios de ECHO. Esas mejoras no se han aplicado al sketch actual.

Referencia del patrón de temporización: [Blink Without Delay](https://docs.arduino.cc/built-in-examples/digital/BlinkWithoutDelay/).

## Mensajes del monitor serie

Al arrancar se envía `Hola`. En cada vuelta se imprime `Solicito medicion`, seguido de un resultado como:

```text
Tiempo: 1166 us | Distancia: 20.00 cm
```

Los números son un ejemplo aproximado de la conversión. Si no se recibe el pulso completo, aparece `Sin medicion`. El mensaje `Solicito medicion` confirma que el programa ejecutó esa línea, pero no demuestra que la orden haya llegado físicamente al módulo.

## Diagnóstico del montaje

### Se repite `Sin medicion`

1. Confirma que el sketch cargado utiliza TRIG en **12** y ECHO en **13**.
2. Comprueba las etiquetas del sensor y cada extremo de los cables, con el USB desconectado al moverlos.
3. Verifica que VCC va directamente a 5V, sin una resistencia de 220 Ω en medio.
4. Prueba un libro de frente a 20–30 cm.
5. Si tienes multímetro, mide aproximadamente 5 V **en las patillas VCC y GND del sensor**, en modo tensión continua y con el cable rojo en V/Ω.
6. Si la alimentación llega y no hay medida, prueba los cables de señal uno por uno. Una foto del cableado o un sensor de sustitución ayudan a separar un contacto defectuoso de un fallo del módulo.

### No carga el programa al conectar el buzzer

Desconecta el USB, retira el buzzer y vuelve a probar la carga. Si sin él carga, revisa su montaje antes de reconectarlo. El hecho de utilizar D8 no impide por sí mismo la comunicación USB; un error de conexión o un problema eléctrico puede estar afectando a la placa. Comprueba el NPN, su patillaje, la polaridad del diodo y los carriles de la protoboard.

### El buzzer suena siempre o nunca

- Comprueba que es el activo TMB12A05 y que se ha montado la etapa de control de esta guía.
- Conectado permanentemente entre 5V y GND sonará continuamente; para controlarlo, su negativo debe pasar por el colector del NPN.
- Verifica GND común y que la base recibe D8 a través de la resistencia equivalente de 880 Ω.
- No confundas la resistencia de base con las resistencias de 220 Ω de los LED.

## Parámetros que puedes estudiar

| Parámetro | Valor actual | Efecto |
|---|---|---|
| `RANGO_ROJO` | 10 cm | Umbral de rojo y sonido continuo |
| `RANGO_AMARILLO` | 40 cm | Límite superior del amarillo y de los pitidos |
| Extremos de `map()` | 80 y 700 ms | Pausas mínima matemática y máxima solicitadas |
| Duración de pitido | 60 ms | Duración solicitada mientras `encendido` es verdadero |
| Timeout de `pulseIn()` | 30 000 µs | Espera máxima para obtener el pulso completo |
| `delay(100)` | 100 ms | Espera añadida entre vueltas |

Mantén el rango rojo menor que el amarillo para que el mapeo tenga un intervalo válido. Las pruebas de este montaje son educativas; la falta de eco y el indicador verde del código actual impiden interpretarlo como una garantía de espacio libre.

[Volver al índice](../README.md) · [Repasar el fundido del LED](../led_analogico/README.md)
