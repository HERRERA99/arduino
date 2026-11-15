# Encendido y apagado progresivo de un LED

[Volver al índice](../README.md) · [Ver programa](./led_analogico.ino) · [Anterior: SOS](../prueba_1/README.md) · [Siguiente: detector de distancia](../detector_distancia/README.md)

## Qué hace

Un LED conectado a **D9** aumenta y reduce progresivamente su brillo siguiendo una curva de coseno. La secuencia se repite:

```text
Apagado → brillo intermedio → máximo → brillo intermedio → apagado
```

El valor de control recorre aproximadamente `0 → 255 → 0`. El ciclo completo dura **5 segundos**, porque la constante del programa es `PERIODO = 5000`.

> El comentario que acompaña a esa constante dice «Un ciclo cada 2 segundos», pero el valor ejecutado es 5000 ms. Para entender el programa actual, utiliza 5 segundos. El `.ino` conserva su comentario original.

## Material

| Cantidad | Componente |
|---|---|
| 1 | Arduino Uno R3 y cable USB de datos |
| 1 | LED indicador de dos patas |
| 1 | Resistencia de **220 Ω**, preferiblemente de ¼ W |
| 1 | Protoboard y cables jumper |

## Esquema de conexión

```text
ARDUINO UNO R3

D9 (~PWM) ─────── [R1: 220 Ω] ─────── A  LED  K ───────┐
                                       +     -        │
GND ──────────────────────────────────────────────────┘
```

| Desde | Hasta |
|---|---|
| Arduino D9 | Un extremo de R1 de 220 Ω |
| Otro extremo de R1 | Ánodo del LED |
| Cátodo del LED | Arduino GND |

### Montarlo paso a paso

1. Desconecta el USB.
2. Coloca el LED con las patas en filas eléctricamente distintas.
3. Une el cátodo con GND.
4. Coloca la resistencia en serie entre D9 y el ánodo.
5. Revisa que el cable esté en **D9**, marcado con `~` en el Uno R3.
6. Conecta el USB y carga [led_analogico.ino](./led_analogico.ino).

Si vienes de `prueba_1`, mueve la conexión del pin 8 al **9**; conserva la resistencia de 220 Ω. Consulta el [montaje de un LED](../README.md#conectar-un-led-con-220-ω).

## Cómo se obtiene el brillo

### Posición dentro del ciclo

```cpp
float fase = (millis() % PERIODO) / (float)PERIODO;
```

- `millis()` devuelve los milisegundos transcurridos desde que arrancó el programa.
- `% PERIODO` obtiene el resto de dividir ese tiempo entre 5000: permite repetir el recorrido temporal.
- Dividir por `(float)PERIODO` lo convierte en una fase entre 0 y un valor ligeramente inferior a 1.

La conversión a `float` permite obtener decimales. Por ejemplo, a los 1250 ms la fase vale `1250 / 5000 = 0,25`.

### Transformación del coseno

```cpp
int brillo = round(127.5 * (1 - cos(2 * PI * fase)));
```

`2 * PI * fase` convierte la fase en un ángulo en radianes. El coseno varía entre -1 y 1. Al calcular `1 - cos(...)` el rango pasa a 0–2, y multiplicarlo por 127,5 lo lleva a 0–255. `round()` redondea al entero más próximo.

El coseno hace que el cambio sea más lento cerca del mínimo y del máximo y más rápido en las partes intermedias. Así el LED parece «respirar».

| Tiempo de referencia dentro del ciclo | Fase | Valor de brillo aproximado |
|---|---:|---:|
| 0 ms | 0 | 0 |
| 1250 ms | 0,25 | 128 |
| 2500 ms | 0,50 | 255 |
| 3750 ms | 0,75 | 128 |
| 5000 ms | Reinicia en 0 | 0 |

La subida dura aproximadamente 2,5 segundos y la bajada otros 2,5 segundos. El brillo percibido no es estrictamente proporcional al número: también influyen el LED y la respuesta del ojo.

### Aplicación al pin

```cpp
analogWrite(LED, brillo);
```

En D9 del Uno R3, esta función genera **PWM**: conmuta rápidamente la salida entre un nivel cercano a 0 V y otro cercano a 5 V, cambiando la proporción de tiempo encendido.

| Valor | Comportamiento de la salida |
|---|---|
| `0` | Siempre apagada |
| `128` | Encendida aproximadamente la mitad del tiempo |
| `255` | Siempre encendida |

El nombre `analogWrite()` no significa que D9 produzca una tensión analógica constante intermedia. La resistencia sigue siendo necesaria: durante cada pulso encendido limita la corriente del LED. Consulta la [explicación oficial de PWM](https://docs.arduino.cc/tutorials/generic/secrets-of-arduino-pwm/).

## Temporización

Este sketch no utiliza `delay()` para realizar el fundido. En cada vuelta calcula qué brillo corresponde al tiempo actual y lo aplica. Esa estructura permite añadir otras tareas al `loop()` sin una espera deliberada entre niveles de brillo.

No requiere bibliotecas externas ni necesita abrir el monitor serie. Las funciones matemáticas utilizadas forman parte del entorno habitual de Arduino.

## Cómo comprobarlo

1. Tras cargar el programa, el LED debe ir aumentando su brillo gradualmente.
2. Cerca de la mitad del ciclo, unos 2,5 segundos, llega al máximo.
3. Después disminuye hasta apagarse y comienza de nuevo.
4. Comprueba varios ciclos con un cronómetro: cada uno debe durar aproximadamente 5 segundos.

## Qué puedes experimentar

- `PERIODO = 2000`: ciclo completo de 2 segundos.
- `PERIODO = 10000`: ciclo completo de 10 segundos.
- Para cambiar de pin, selecciona otro que admita PWM en el Uno R3: D3, D5, D6, D9, D10 o D11, y mueve también el cable.
- Mantén `PERIODO` mayor que cero: el cálculo utiliza ese valor como divisor y en el resto `%`.

## Si no funciona

| Síntoma | Qué revisar |
|---|---|
| Nunca enciende | Polaridad, GND, resistencia en serie y conexión real a D9. |
| Solo cambia entre encendido y apagado | Comprueba que usas un pin PWM y que has cargado este programa. |
| Tarda más de lo que dice el comentario | La constante actual es 5000, así que el periodo es de 5 segundos. |
| El monitor serie no muestra nada | Es normal: este sketch no utiliza `Serial.print()`. |

[Volver al índice](../README.md) · [Continuar con el detector de distancia](../detector_distancia/README.md)
