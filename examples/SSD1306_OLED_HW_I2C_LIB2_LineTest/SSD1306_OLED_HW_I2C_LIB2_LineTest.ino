// Small_SSD1306_Print_8x16_lib_LineTest.ino
// Tests line drawing
// Can be used to demonstrate the inverted end-of-line vertical line drawing
//     See config_SSD1306_OLED_HW_I2C_LIB2.h - 
//       #define __Using_inverted_vertical_line_fix__
//
// connect display SCL to pin A5, and SDA to pin A4
// connect LED to pin 4 (it will blink in case of I2C error)


#include <SSD1306_OLED_HW_I2C_LIB2.h>

SSD1306_OLED_HW_I2C_LIB2 lcd = SSD1306_OLED_HW_I2C_LIB2();

void setup() {
    lcd.D_INIT();       // Initialize OLED
}

void loop() {

  lcd.D_CLEAR();

  lcd.D_DRAW_VERT(10, 20, 1);
  lcd.D_DRAW_VERT(20, 20, 2);
  lcd.D_DRAW_VERT(30, 20, 3);
  lcd.D_DRAW_VERT(40, 20, 4);
  lcd.D_DRAW_VERT(50, 20, 5);
  lcd.D_DRAW_VERT(60, 20, 6);
  lcd.D_DRAW_VERT(70, 20, 7);
  lcd.D_DRAW_VERT(80, 20, 8);
  lcd.D_DRAW_VERT(90, 20, 9);
  delay (2000);
  
  lcd.D_DRAW_VERT(20, 20, 1);
  lcd.D_DRAW_VERT(21, 20, 2);
  lcd.D_DRAW_VERT(22, 20, 3);
  lcd.D_DRAW_VERT(23, 20, 4);
  lcd.D_DRAW_VERT(24, 20, 5);
  lcd.D_DRAW_VERT(25, 20, 6);
  lcd.D_DRAW_VERT(26, 20, 7);
  lcd.D_DRAW_VERT(27, 20, 8);
  lcd.D_DRAW_VERT(28, 20, 9);
  lcd.D_DRAW_VERT(29, 20, 10);
  lcd.D_DRAW_VERT(30, 20, 11);
  lcd.D_DRAW_VERT(31, 20, 12);
  lcd.D_DRAW_VERT(32, 20, 13);
  lcd.D_DRAW_VERT(33, 20, 14);
  lcd.D_DRAW_VERT(34, 20, 15);
  lcd.D_DRAW_VERT(35, 20, 16);
  lcd.D_DRAW_VERT(36, 20, 17);
  lcd.D_DRAW_VERT(37, 20, 18);

  delay (2000);

  lcd.D_CLEAR();
  
  lcd.D_DRAW_VERT(1, 0, 10);
  lcd.D_DRAW_VERT(2, 0, 11);
  lcd.D_DRAW_VERT(3, 0, 12);
  lcd.D_DRAW_VERT(4, 0, 13);
  lcd.D_DRAW_VERT(5, 0, 14);
  lcd.D_DRAW_VERT(6, 0, 15);
  lcd.D_DRAW_VERT(7, 0, 16);
  lcd.D_DRAW_VERT(8, 0, 17);

  lcd.D_DRAW_VERT(9, 0, 18);
  lcd.D_DRAW_VERT(10, 0, 19);
  lcd.D_DRAW_VERT(11, 0, 20);
  lcd.D_DRAW_VERT(12, 0, 21);
  lcd.D_DRAW_VERT(13, 0, 22);
  lcd.D_DRAW_VERT(14, 0, 23);
  lcd.D_DRAW_VERT(15, 0, 24);
  lcd.D_DRAW_VERT(16, 0, 25);

  delay (2000);

  lcd.D_CLEAR();
  
  for (int n=0; n<32;n++)
    lcd.D_DRAW_VERT(n, 0, n+1);

  delay (2000);
    
  lcd.D_CLEAR();
  
  lcd.D_DRAW_VERT(0, 0, 1);
  lcd.D_DRAW_VERT(1, 0, 2);
  lcd.D_DRAW_VERT(2, 0, 3);
  lcd.D_DRAW_VERT(3, 0, 4);
  lcd.D_DRAW_VERT(4, 0, 5);
  lcd.D_DRAW_VERT(5, 0, 14);
  lcd.D_DRAW_VERT(6, 0, 15);
  lcd.D_DRAW_VERT(7, 0, 16);
  lcd.D_DRAW_VERT(8, 0, 17);

  lcd.D_DRAW_VERT(9, 0, 18);
  lcd.D_DRAW_VERT(10, 0, 19);
  lcd.D_DRAW_VERT(11, 0, 20);
  lcd.D_DRAW_VERT(12, 0, 21);
  lcd.D_DRAW_VERT(13, 0, 22);
  lcd.D_DRAW_VERT(14, 0, 23);
  lcd.D_DRAW_VERT(15, 0, 24);
  lcd.D_DRAW_VERT(16, 0, 25);

  delay (2000);
    
  lcd.D_CLEAR();
/*      
      lcd.D_DRAW_VERT(0, 6, 10);
      lcd.D_DRAW_VERT(0, 7, 11);
      lcd.D_DRAW_VERT(0, 8, 12);
      lcd.D_DRAW_VERT(0, 9, 13);
      lcd.D_DRAW_VERT(0, 10, 14);
      lcd.D_DRAW_VERT(0, 11, 15);
*/
  delay (1000);

}
