#INCLUDE<P16F877A.INC>
    __CONFIG _DEBUG_OFF&_CP_ALL&_WRT_HALF&_CPD_ON&_LVP_OFF&_BODEN_OFF&_PWRTE_ON&_WDT_OFF&_HS_OSC
	ORG	00H		; Program entry point 
        BSF	STATUS,RP0	; select BANK1 
	
	MOVLW	000H		; sets inputs and outputs on Port D
        MOVWF	TRISD		
	
	MOVLW   000H		
	MOVWF   TRISA		; Sets inputs and outputs on Port A
	
	MOVLW   007H		; DATA SHEET PAGE 130. To make all pins digital you enter 011x (7)
	MOVWF   ADCON1		; Move to register
	
	
	BCF	STATUS,RP0	; Select BANK 0
	
	;MOVLW   0F9H		; Select which pins you want to display. 0 being ON
	;MOVWF   PORTD
	
	;MOVLW   000H		; Select which 7-seg to use. 0 Being ON.
	;MOVWF   PORTA
    
	LOOP
	
	;Display 1
	MOVLW   0F9H		; 1111 1001   (1)
	MOVWF   PORTD
	
	MOVLW   0FEH		; 1111 1110    Turn on 7_seg 1
	MOVWF   PORTA
	
	MOVLW   0FFH
	MOVWF   PORTA
	
	;Display 2
	
	MOVLW   0A4H		; 1010 0100   (2)
	MOVWF   PORTD
	
	MOVLW   0FDH		; 1111 1101
	MOVWF   PORTA
	
	MOVLW   0FFH
	MOVWF   PORTA
	
	;Display 2
	
	MOVLW   0B0H		; 1011 0000   (3)
	MOVWF   PORTD
	
	MOVLW   0FBH		; 1111 1011
	MOVWF   PORTA
	
	MOVLW   0FFH
	MOVWF   PORTA
	
	GOTO LOOP		; Enter an infinite loop
	END			;
    





