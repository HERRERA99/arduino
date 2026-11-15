# Prácticas con Arduino Uno

Repositorio de proyectos para aprender conexiones eléctricas y programación con Arduino. Cada carpeta contiene su programa `.ino` y una guía con materiales, esquema, explicación del código y pruebas.

Esta documentación toma como referencia el **Arduino Uno R3 de lógica de 5 V**. Las resistencias utilizadas con los LED son de **220 Ω**, una por cada LED. Si la placa es otra revisión, comprueba sus límites eléctricos antes de reutilizar las conexiones.

## Índice de proyectos

| Proyecto | Qué hace | Conceptos | Guía | Programa |
|---|---|---|---|---|
| `prueba_1` | Repite SOS mediante destellos de un LED en D8 | Salidas digitales, funciones, bucles y tiempos | [SOS con LED](./prueba_1/README.md) | [prueba_1.ino](./prueba_1/prueba_1.ino) |
| `led_analogico` | Enciende y apaga progresivamente un LED en D9, en ciclos de 5 segundos | PWM, coseno, `millis()` y conversión de rangos | [Fundido del LED](./led_analogico/README.md) | [led_analogico.ino](./led_analogico/led_analogico.ino) |
| `detector_distancia` | Mide distancia, selecciona un LED y controla la repetición de pitidos | Ultrasonidos, monitor serie, condiciones y temporización | [Avisador de aparcamiento](./detector_distancia/README.md) | [detector_distancia.ino](./detector_distancia/detector_distancia.ino) |

El orden de la tabla es también el orden sugerido de aprendizaje. Cada proyecto se carga por separado: por ejemplo, D8 controla un LED en `prueba_1` y el buzzer en `detector_distancia`.

## Material común

- Arduino Uno R3 y cable USB de datos.
- Protoboard.
- Cables macho a macho para la protoboard y macho a hembra para las patillas del HC-SR04.
- LED indicadores de dos patas, normalmente de 3 o 5 mm.
- Resistencias de **220 Ω**, preferiblemente de ¼ W.
- Para el detector: HC-SR04, buzzer activo TMB12A05 y la etapa de control descrita en su guía.

## Conectar un LED con 220 Ω

```text
Pin digital Arduino ── [220 Ω] ── A  LED  K ── GND
                                  ánodo cátodo
```

La pata larga suele identificar el ánodo; el lado plano del encapsulado suele identificar el cátodo. Si las patas están cortadas, consulta las marcas o la ficha del LED.

La resistencia limita la corriente. A 5 V, suponiendo un LED rojo con una caída de 2 V:

```text
Tensión en la resistencia = 5 V - 2 V = 3 V
Corriente = 3 V / 220 Ω ≈ 0,0136 A = 13,6 mA
Potencia en la resistencia = 3 V × 0,0136 A ≈ 0,041 W
```

Una resistencia de ¼ W admite holgadamente esa potencia. El valor real depende del LED y de la tensión efectiva del pin. El color orienta, pero su ficha técnica determina qué corriente admite. Para los tres colores del detector se utiliza **una resistencia independiente de 220 Ω por LED**; no se comparte una sola resistencia entre ellos.

Arduino especifica 20 mA por pin para el Uno R3 y también existen límites conjuntos del microcontrolador. No se debe interpretar ese dato como una corriente fija que sale siempre del pin. Consulta las [especificaciones del Uno R3](https://store.arduino.cc/products/arduino-uno-rev3) y la [explicación de LED y resistencias de Adafruit](https://learn.adafruit.com/lets-put-leds-in-things?view=all).

## Leer los esquemas y utilizar la protoboard

- `D8`, por ejemplo, significa el conector digital marcado **8**, no la octava posición contada desde un extremo.
- `GND` es la referencia común de todos los componentes del montaje.
- Los esquemas representan conexiones eléctricas; los colores de los cables son una ayuda visual y no cambian su función.
- Una resistencia normal no tiene polaridad: cualquiera de sus extremos puede ir al pin.
- La resistencia puede situarse antes o después del LED, siempre que esté en serie.

En una protoboard habitual, los cinco agujeros de cada fila de una mitad están unidos entre sí. La ranura central separa ambas mitades. Los carriles laterales suelen usarse para alimentación, pero pueden estar partidos a mitad de recorrido: compruébalos con continuidad y sin alimentación.

Para cada LED:

1. Desconecta el USB antes de mover cables.
2. Coloca sus patas en filas eléctricamente distintas.
3. Conecta el pin del programa a una fila libre.
4. Lleva una resistencia de 220 Ω desde esa fila a la fila del ánodo.
5. Conecta la fila del cátodo al carril GND.
6. Une ese carril con un GND del Arduino.

Las dos patas de una resistencia o un LED no deben quedar conectadas a la misma fila: el componente quedaría puenteado.

## Abrir y cargar los programas

1. Abre el `.ino` del proyecto en Arduino IDE. La carpeta y el archivo tienen el mismo nombre.
2. Selecciona **Arduino Uno**, dentro de **Arduino AVR Boards**.
3. Selecciona el puerto de la placa; en Windows suele llamarse `COM3`, `COM5`, etc.
4. Monta el circuito indicado con el USB desconectado y revisa las conexiones.
5. Conecta el USB y pulsa **Subir**, la flecha hacia la derecha.
6. Espera a que la carga termine correctamente. El botón con una marca de verificación solo compila.

Si hay dudas sobre el circuito, comprueba primero que puedes cargar un programa con la placa sin componentes externos. No hace falta instalar bibliotecas externas para estos tres sketches.

## Monitor serie

El detector utiliza `Serial.begin(9600)`. Abre **Herramientas / Tools → Monitor serie / Serial Monitor**, o pulsa **Ctrl + Mayús + M**, y selecciona **9600 baudios**.

`Output` muestra mensajes de compilación y carga; `Serial Monitor` muestra lo enviado mediante `Serial.print()` y `Serial.println()`. Los proyectos de SOS y fundido no imprimen mensajes, por lo que es normal que su monitor esté vacío.

Consulta la [guía del monitor serie](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-serial-monitor).

## Diagnóstico común

| Síntoma | Comprobación inicial |
|---|---|
| El LED no enciende | Polaridad, resistencia en serie, GND común y coincidencia entre el pin físico y la constante del programa. |
| El LED brilla al máximo pero no se regula | En el fundido, utiliza D9, que admite PWM en el Uno R3. |
| El monitor está vacío | Proyecto que imprime mensajes, carga terminada, puerto correcto y 9600 baudios. |
| Aparecen caracteres ilegibles | Comprueba que la velocidad del monitor coincide con `Serial.begin()`. |
| `programmer is not responding` / `not in sync` | Desconecta el USB, retira periféricos, vuelve a conectar y verifica placa, puerto y cable USB de datos. Deja libres D0/RX y D1/TX durante la prueba. |
| La carga falla al conectar el TMB12A05 | Revisa su alimentación y la etapa con transistor del detector. No alimentes ese buzzer directamente desde el pin digital. |
| El detector repite `Sin medicion` | Revisa sus cuatro conexiones, alimentación en el propio módulo, cables y un objeto plano a 20–30 cm. |

Guía oficial para [errores de carga con avrdude](https://support.arduino.cc/hc/en-us/articles/4401874331410--Error-avrdude-when-uploading).

## Alcance de esta documentación

Las explicaciones se han contrastado con los `.ino` presentes en el repositorio. Los esquemas de LED y sensor corresponden a los pines de esos archivos. La etapa de transistor del buzzer es un **montaje recomendado**, porque el código no permite conocer su conexión física y esta no se ha verificado con una foto o una medición.

Se documentan también las limitaciones del código actual: el comentario del periodo del fundido no coincide con su constante, la pausa final del SOS suma más tiempo del indicado y el detector sigue utilizando esperas. Estos README no modifican los programas ni certifican una prueba física del montaje.
