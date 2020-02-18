void LCD_init(void);                        //I/O init 
void LCD_writeCmd(char x);              //display one byte 
void LCD_writeChar(char x);             //LCD display setting.
void LCD_write(char string[],char len);
void LCD_clear(void);
void LCD_firstLine(void);                   //Switches to line 1
void LCD_secondLine(void);                  //Switches to line 2
void LCD_thirdLine(void);                   //Switches to line 3
void LCD_fourthLine(void);                  //Switches to line 4


