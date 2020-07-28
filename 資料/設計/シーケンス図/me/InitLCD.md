``` mermaid

sequenceDiagram

participant master as I2Cマスター<br>PICマイコン
participant LCD as I2Cスレーブ<br>LCD

activate master

activate master
master ->> master : __delay_ms(40)
deactivate master


master ->> LCD: I2Cx_Write1ByteRegister<br> (address, reg, 0x38)

activate LCD

Note right of LCD : 

LCD -->> master: 

deactivate LCD

deactivate master

```
