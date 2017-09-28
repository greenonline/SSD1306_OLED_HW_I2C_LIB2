# SSD1306_OLED_HW_I2C_LIB2
Enhanced version of [SSD1306_OLED_HW_I2C_LIB](https://github.com/greenonline/SSD1306_OLED_HW_I2C_LIB) with 32 line display support

**Note:**

- SSD1306_OLED_HW_I2C_LIB is not required
- SSD1306_OLED_HW_I2C_LIB2 is a base class

There are a number of derived classes, from this base class, which add compatability with the print()function (derived from Arduino Print class), larger 8x16 characters, and additional dashed vertical line drawing. These classes are:

 - [SSD1306_OLED_HW_I2C_PRINT_LIB](https://github.com/greenonline/SSD1306_OLED_HW_I2C_PRINT_LIB)
 - [SSD1306_OLED_HW_I2C_PRINT_8X16_LIB](https://github.com/greenonline/SSD1306_OLED_HW_I2C_PRINT_8X16_LIB)
 - [SSD1306_OLED_HW_I2C_PRINT_8X16_DASH_LIB](https://github.com/greenonline/SSD1306_OLED_HW_I2C_PRINT_8X16_DASH_LIB)
 
 In addition, there are "wrapper" classes, to some of the above derived classes, which provide standard Arduino begin(), clear() and other functions. These should be used in Arduino projects as they aim to provide plug in capabilities, as a display driver for the SSD1306, for existing Arduino projects:
 
  - [Small_SSD1306_Print_8x16_Dash_lib](https://github.com/greenonline/Small_SSD1306_Print_8x16_Dash_lib)
  - [Small_SSD1306_Print_8x16_lib](https://github.com/greenonline/Small_SSD1306_Print_8x16_lib)
