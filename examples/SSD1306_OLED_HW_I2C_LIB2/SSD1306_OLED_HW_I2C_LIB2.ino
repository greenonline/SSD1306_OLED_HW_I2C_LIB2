// SSD1306_OLED_HW_I2C_LIB2.ino
// Demo sketch - uses built in demonstration function, D_DEMO()
//
// connect display SCL to pin A5, and SDA to pin A4
// connect LED to pin 4 (it will blink in case of I2C error)

#include <SSD1306_OLED_HW_I2C_LIB2.h>
#include <avr/io.h>

SSD1306_OLED_HW_I2C_LIB2 lcd = SSD1306_OLED_HW_I2C_LIB2();

void setup(void) {
    lcd.D_INIT();       // Initialize OLED
}


void loop(void) {
  lcd.D_DEMO();         // Demo mode
}
