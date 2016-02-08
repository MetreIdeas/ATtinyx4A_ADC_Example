# ATtinyx4A_ADC_Example
Example program for using ADC on Atmel ATtiny24A/44A/84A

Created: 02/08/2016
Author: Will Sakran
Metre Ideas and Design

This program reads an analog input on ADC0 (pin PA0). An external interrupt
(INT0 on pin PB2) is also provided which can be used to trigger the chip to
store the current analog input value to a reference variable. Current samples
can then be compared to this reference value.

Developed for use with Atmel ATtiny24A / ATtiny44A / ATtiny84A
For ATtiny24A/44A/84A Prototyping boards go to
www.metreideas.com
