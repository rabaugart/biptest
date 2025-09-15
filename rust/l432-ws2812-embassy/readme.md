# Versuche ws2812 mit nucleo_l432kc anzusteuern

Stand: 15.9.2025

Aufbau:

- nucleo_l432kc
- 1 Segment aus der alten, noch vorhandenen LED-Kette

Probiere beide Ansätze, PWM und SPI aus den [F4-Beispielen](https://github.com/embassy-rs/embassy/tree/main/examples/stm32f4/src/bin) von embassy.
Dazu muss der Code an den l4 adaptiert.

Es baut und flasht, blinkt aber nicht.

Ich habe in beiden Varianten erfolglos versucht, PB3 in der Loop blinken zu lassen.
Möglicherweise wird die Loop gar nicht ausgeführt.
