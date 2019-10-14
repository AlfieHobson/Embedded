#INCLUDE<P16F877A.INC>
 __CONFIG _DEBUG_OFF&_CP_ALL&_WRT_HALF&_CPD_ON&_LVP_OFF&_BODEN_OFF&_PWRTE_ON&_WDT_OFF&_HS_OSC
   ORG	00H		; Program entry point 
    ; Predefined values
   outer_delay   equ   0ffh            
   inner_delay   equ   0ffh           
                  
  main 
   clrf     PORTD   ;Clear Port D (HIGHS AND LOWS)
   banksel  TRISD   ;Select which ever bank TRISC belongs to. (BANK1)
   clrf     TRISD   ;Completely clear TRISD (ALL OUTPUT)
   clrf	    TRISA   ;Turn all 7-segs on
   clrf     STATUS  ;Clear status register (including the bank selection, so back to BANK0)
   
   ;Set 9 registers to our number values
   movlw    0C0h    ;0
   movwf    20h  
   movlw    0F9h    ;1
   movwf    21h
   movlw    0A4h    ;2
   movwf    22h
   movlw    0B0h    ;3
   movwf    23h
   movlw    099h    ;4
   movwf    24h
   movlw    092h    ;5
   movwf    25h
   movlw    082h    ;6
   movwf    26h
   movlw    0f8h    ;7
   movwf    27h
   movlw    080h    ;8
   movwf    28h
   movlw    098h    ;9
   movwf    29h
   
   ;Set first address in 32h
   MOVLW    20h
   MOVWF    FSR
   
   ; Set first 7-segment 
   MOVLW   0FEH		; 1111 1110    Turn on 7_seg 1
   MOVWF   PORTA
   
  loop
   ;Move register address into working register
   MOVF   INDF,0	    
      
   ;Move value of Working register to PORT D
   MOVWF   PORTD 
   
   ;Go to next number register
   incf	    FSR
   call     delay   ;Call Delay
   goto     loop    ;Go round again
   
  ; Delay loops 
  delay 
   movlw    outer_delay  
   movwf    30h 
  outer_loop 
  
   movlw    inner_delay 
   movwf    31h	
   
  inner_loop 
   decfsz   31h,1	    ;Decrease value in this register, if 1, execute next line, if 0 skip.
   goto     inner_loop 
   
   decfsz   30h,1 
   goto    outer_loop 
   return  
   end 



