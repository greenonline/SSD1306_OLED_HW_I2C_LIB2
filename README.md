# SSD1306_OLED_HW_I2C_LIB2
Enhanced version of [SSD1306_OLED_HW_I2C_LIB](https://github.com/greenonline/SSD1306_OLED_HW_I2C_LIB) with 32 line display support

**Note:**

- SSD1306_OLED_HW_I2C_LIB is not required
- SSD1306_OLED_HW_I2C_LIB2 is a base class

There are a number of derived classes, from this base class, which add compatability with the print() function (derived from Arduino Print class), larger 8x16 characters, and additional dashed vertical line drawing. These classes are:

 - [SSD1306_OLED_HW_I2C_PRINT_LIB](https://github.com/greenonline/SSD1306_OLED_HW_I2C_PRINT_LIB)
 - [SSD1306_OLED_HW_I2C_PRINT_8X16_LIB](https://github.com/greenonline/SSD1306_OLED_HW_I2C_PRINT_8X16_LIB)
 - [SSD1306_OLED_HW_I2C_PRINT_8X16_DASH_LIB](https://github.com/greenonline/SSD1306_OLED_HW_I2C_PRINT_8X16_DASH_LIB)
 
 In addition, there are "wrapper" classes, to some of the above derived classes, which provide standard Arduino begin(), clear() and other functions. These should be used in Arduino projects as they aim to provide plug in capabilities, as a display driver for the SSD1306, for existing Arduino projects:
 
  - [Small_SSD1306_Print_8x16_Dash_lib](https://github.com/greenonline/Small_SSD1306_Print_8x16_Dash_lib)
  - [Small_SSD1306_Print_8x16_lib](https://github.com/greenonline/Small_SSD1306_Print_8x16_lib)

**SSD1306_OLED_HW_I2C and derived classes**

Here is a list of classes which are derived from the original [SSD1306_OLED_HW_I2C](https://bitbucket.org/paraglider/ssd1306_oled_hw_i2c/) code (only those preceeded by an asterisk (`*`) are available on Github):

      SSD1306_OLED_HW_I2C                - original, no class
    * SSD1306_OLED_HW_I2C_LIB            - Arduino library object class version of SSD1306_OLED_HW_I2C
    * SSD1306_OLED_HW_I2C_LIB2           - Arduino library object class version of SSD1306_OLED_HW_I2C, with updated character set, PRINT_STR(char *s, size_t size) and line drawing fixes
                                         - Has config.h

    * SSD1306_OLED_HW_I2C_PRINT_LIB      - Arduino library object class derived from both Print and SSD1306_OLED_HW_I2C_LIB/SSD1306_OLED_HW_I2C_LIB2 classes
    * SSD1306_OLED_HW_I2C_PRINT_8X16_LIB - Derived from SSD1306_OLED_HW_I2C_PRINT_LIB contains 8x16 PRINT() functions
    * SSD1306_OLED_HW_I2C_PRINT_8X16_DASH_LIB   
                                         - Arduino library object class derived from SSD1306_OLED_HW_I2C_PRINT_8X16_LIB 
                                         - Adds D_DRAW_VERT_DASHED. 

      SSD1306_OLED_HW_I2C_LIB_PRINT
                                         - Arduino library object class of SSD1306_OLED_HW_I2C_LIB derived from Print class
      SSD1306_OLED_HW_I2C_LIB_PRINT_8X16
                                         - Derived from SSD1306_OLED_HW_I2C_LIB_PRINT contains 8x16 PRINT() functions

      SSD1306_OLED_HW_I2C_LIB_MGJ        - Test modifications of SSD1306_OLED_HW_I2C for 128x32, no class
      SSD1306_OLED_HW_I2C_LIB_FULL       - Arduino library object class version of SSD1306_OLED_HW_I2C_LIB, contains begin(), clear(), setCursor(), setCursorMode()

      Small_SSD1306                      - Class derived from SSD1306_OLED_HW_I2C_LIB, contains begin(), clear(), setCursor(), setCursorMode()
      Small_SSD1306_Print                - Class derived from SSD1306_OLED_HW_I2C_LIB_PRINT class, contains begin(), clear(), setCursor(), setCursorMode()
      Small_SSD1306_Print_8x16           - Class derived from SSD1306_OLED_HW_I2C_LIB_PRINT_8X16 class, contains begin(), clear(), setCursor(), setCursorMode(), setPrint8x16CharacterMode() functions

      Small_SSD1306_Print_lib            -  (not created - "missing link") Class derived from SSD1306_OLED_HW_I2C_PRINT_LIB class, contains begin(), clear(), setCursor(), setCursorMode()
    * Small_SSD1306_Print_8x16_lib       - Class derived from SSD1306_OLED_HW_I2C_PRINT_8X16_LIB class, contains begin(), clear(), setCursor(), setCursorMode(), setPrint8x16CharacterMode() functions
    * Small_SSD1306_Print_8x16_Dash_lib  - Class derived from SSD1306_OLED_HW_I2C_PRINT_8X16_DASH_LIB class, contains begin(), clear(), setCursor(), setCursorMode(), setPrint8x16CharacterMode() functions

    16 class libraries



    Red herring
    -----------
    SSD1306_Extended_Char - Derived from U8g2 library, U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C
