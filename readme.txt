(c) 2024 - 2026 - Jens Tönsing
Kontakt: jens@jens-toensing.de

Von mir verwendete Hardware:
============================
- Steppermotor NEMA23, Motortreiber: DM556 https://de.aliexpress.com/item/4001234168546.html
- Microcontroller Arduino Nano mit ATMEGA328: https://de.aliexpress.com/item/1005006299159261.html
- Stromversorgung für Motortreiber 24V, 6A: https://de.aliexpress.com/store/1102811844.html
- HAL-Sensor für Pedal: KY-024 https://de.aliexpress.com/item/1005005327140435.html
- Pulley
- Keilriemen
- Motorhalter Eigenbau
- Display LCD 16x2 https://www.amazon.de/dp/B07JH6GHPR?psc=1&ref=ppx_yo2ov_dt_b_product_details
- Sensormodule für Nadelposition KY-024 (selbe wie beim Pedal) https://de.aliexpress.com/item/1005005327140435.html
- Neue Stirnabdeckung Eigenbau aus 2mm Acryglas (Toom)

Die Verdrahtung ergibt sich aus der Datei config.h

Anmerkungen:
============
- Ein kleinerer Motor und eine kleinere Stromversorgung reicht für die Pfaff 130 bestimmt auch aus.
  Ich würde auf jeden Fall nächstes Mal einen Motor mit 6mm Welle nehmen, dafür sind leichter kleinere
  Pulleys zu bekommen.
- Wer das nachbaut muss sicher mit den Werten in config.h etwas rum spielen, um für sich die richtigen
  Werte zu ermitteln. Insbesondere für HAL_SENSOR_FULL_THROTTLE_VALUE, da der Wert vom verwendeten 
  Magneten abhängt.
- Wenn der Motor nicht anläuft, einfach mal den Magneten umdrehen. Der Wert, den der HAL-Sensor ausgibt,
  hängt von der Polung des Magneten ab.
- Der Quellcode verwendet die Bibliotheken
  - Continous Stepper: https://github.com/bblanchon/ArduinoContinuousStepper
  - LiquidCrystal_I2C