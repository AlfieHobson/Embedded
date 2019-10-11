#INCLUDE<P16F877A.INC>
 __CONFIG _DEBUG_OFF&_CP_ALL&_WRT_HALF&_CPD_ON&_LVP_OFF&_BODEN_OFF&_PWRTE_ON&_WDT_OFF&_HS_OSC
   ORG	00H		; Program entry point 
    ; Predefined values
   outer_delay   equ   0ffh            
   inner_delay   equ   0afh           
                  
  main 
   clrf     PORTC   ;Clear Port C
   banksel  TRISC   ;Select which ever bank TRISC belongs to. (BANK1)
   clrf     TRISC   ;Completely clear it (ALL OUTPUT)
   clrf     STATUS  ;Clear status register (including the bank selection, so back to BANK0)
   
  loop              
   incf     PORTC   ;Increment value in PORTC (This is the counting)
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



