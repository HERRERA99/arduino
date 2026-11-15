# SOS con un LED

[Volver al índice](../README.md) · [Ver programa](./prueba_1.ino) · [Siguiente: fundido del LED](../led_analogico/README.md)

## Qué hace

El programa repite la secuencia **S–O–S** mediante un LED conectado al pin digital **8**:

```text
S             O             S
· · ·         — — —         · · ·
tres cortos   tres largos   tres cortos
```

Permite practicar salidas digitales, funciones, parámetros y bucles `for`. Las esperas con `delay()` son apropiadas para observar esta primera secuencia, aunque impiden ejecutar otras tareas del `loop()` durante cada espera.

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

D8 ─────────── [R1: 220 Ω] ─────────── A  LED  K ───────┐
                                       +     -        │
GND ──────────────────────────────────────────────────┘
```

`A` es el ánodo y `K` el cátodo. El pin 5V no se conecta al LED en este proyecto: la salida D8 proporciona la tensión cuando el programa escribe `HIGH`.

| Desde | Hasta |
|---|---|
| Arduino D8 | Un extremo de R1 de 220 Ω |
| Otro extremo de R1 | Ánodo del LED |
| Cátodo del LED | Arduino GND |

### Montarlo paso a paso

1. Desconecta el USB.
2. Coloca las patas del LED en filas distintas de la protoboard.
3. Conecta la fila del cátodo a GND.
4. Conecta un extremo de la resistencia a la fila del ánodo y el otro a una fila libre.
5. Lleva un cable desde D8 a esa fila libre.
6. Revisa la polaridad, conecta el USB y carga [prueba_1.ino](./prueba_1.ino).

Para identificar filas y carriles, consulta la [guía de protoboard del índice](../README.md#leer-los-esquemas-y-utilizar-la-protoboard).

## Cómo está organizado el código

### Constantes

```cpp
const int PIN_LED = 8;
const int UNIDAD = 200;
```

`PIN_LED` identifica el conector utilizado. `UNIDAD` establece la unidad temporal del ejercicio en **200 milisegundos**.

### `setup()`

Configura D8 como salida mediante `pinMode(PIN_LED, OUTPUT)`. Esta preparación se ejecuta una vez al arrancar o reiniciar la placa.

### `senal(duracion)`

Recibe cuánto debe durar un destello. Enciende el LED, espera ese tiempo, lo apaga y añade una pausa de una unidad.

| Función | Destello encendido | Pausa que añade `senal()` |
|---|---:|---:|
| `punto()` | 200 ms | 200 ms |
| `raya()` | 600 ms | 200 ms |

### `letraS()` y `letraO()`

`letraS()` llama tres veces a `punto()` y `letraO()` llama tres veces a `raya()`. El bucle `for` permite expresar esas repeticiones sin escribir tres llamadas iguales.

Al finalizar cada función se añaden dos unidades de pausa, 400 ms. Como la última señal ya añadió 200 ms de silencio, la separación entre las letras S–O y O–S resulta de **600 ms**.

### `loop()`

Ejecuta `letraS()`, `letraO()` y `letraS()`, añade otra espera y vuelve a comenzar.

## Tiempos reales del programa actual

La pausa final requiere tener en cuenta las esperas que se acumulan:

```text
Fin del último destello de la S
   200 ms de senal()
+  400 ms de letraS()
+ 1400 ms de delay(UNIDAD * 7) en loop()
= 2000 ms de silencio antes de comenzar el siguiente SOS
```

Por tanto, el comentario de «pausa entre palabras» describe la intención, pero el silencio final total es de **10 unidades**, no de 7. Con los valores actuales, el comienzo de un SOS y el comienzo del siguiente están separados por aproximadamente **7,4 segundos**, sin contar el pequeño tiempo de ejecución de las instrucciones.

Esta guía describe ese comportamiento tal como está escrito; no cambia los tiempos del sketch.

## Cómo comprobarlo

1. Observa tres destellos cortos.
2. Después deben aparecer tres largos, de aproximadamente el triple de duración.
3. Vuelven los tres cortos.
4. Tras unos dos segundos apagado comienza otra secuencia.

Este programa no utiliza `Serial`, por lo que no imprime nada en el monitor serie. El LED integrado de la placa está asociado a otro pin; el que debe mostrar SOS es el LED externo conectado a D8.

## Qué puedes experimentar

- Reducir `UNIDAD` a 100 acelera toda la secuencia a la mitad del tiempo.
- Aumentar `UNIDAD` a 400 duplica su duración.
- Cambiar `PIN_LED` requiere mover también el cable al nuevo pin.
- Puedes crear otras letras combinando `punto()` y `raya()` y calculando sus pausas.

## Si no funciona

| Síntoma | Qué revisar |
|---|---|
| Nunca enciende | Ánodo y cátodo, conexión de GND y que el cable esté en D8. |
| Está siempre encendido | Comprueba que el circuito comienza en D8 y que no has conectado el ánodo directamente a 5V. |
| No se distingue la secuencia | Confirma que se ha cargado este sketch y que `UNIDAD` sigue valiendo 200. |
| Falla la carga | Sigue el [diagnóstico común](../README.md#diagnóstico-común) con la placa desconectada del montaje. |

[Volver al índice](../README.md) · [Continuar con el LED de brillo variable](../led_analogico/README.md)
