; -!- gpasm -!- ###########################################################
;
;  System        : 
;  Module        : 
;  Object Name   : $RCSfile$
;  Revision      : $Revision$
;  Date          : $Date$
;  Author        : $Author$
;  Created By    : Robert Heller
;  Created       : Sat Jun 17 13:57:04 2017
;  Last Modified : <170617.1747>
;
;  Description	
;
;  Notes
;
;  History
;	
;############################################################################
; 
;    Copyright (C) 2017  Robert Heller D/B/A Deepwoods Software
;			51 Locke Hill Road
;			Wendell, MA 01379-9728
;
;    This program is free software; you can redistribute it and/or modify
;    it under the terms of the GNU General Public License as published by
;    the Free Software Foundation; either version 2 of the License, or
;    (at your option) any later version.
;
;    This program is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU General Public License for more details.
;
;    You should have received a copy of the GNU General Public License
;    along with this program; if not, write to the Free Software
;    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
;
; 
;############################################################################


        include p16f57.inc
        include PIC16f59_signalHat.inc
        LIST

Start   MOVLW   H'01'
        OPTION

        MOVLW   H'00'
        TRIS    LAMPS
        TRIS    SIGNALS
        MOVWF   SPISTATE
        MOVLW   H'01'
        TRIS    SPI
        CALL    INITSIG

MAINLOOP MOVF   TMR0,W
        BTFSC   STATUS,Z
        CALL    NEXTSIG
        BTFSC   SPISTATE,0
        GOTO    SPICLK1
        BTFSC   SPI,SCLK
        CALL    READBIT
        GOTO    MAINLOOP
SPICLK1 BTFSS   SPI,SCLK
        CALL    WRITEBIT
        GOTO    MAINLOOP





INITSIG MOVLW   8
        MOVWF   SIGINDX

DLOOP   MOVLW   SIGBUF
        ADDWF   SIGINDX,W
        MOVF    FSR,F
        CLRF    INDF
        DECFSZ  SIGINDX,F
        GOTO    DLOOP
        MOVLW   8
        MOVWF   SIGINDX
        MOVLW   H'80'
        MOVWF   SIGNALS
        MOVWF   SIGBITS
        MOVLW   H'00'
        MOVWF   LAMPS
        RETLW   0

NEXTSIG MOVLW   SIGBUF
        ADDWF   SIGINDX,W
        MOVWF   FSR
        MOVF    INDF,W
        MOVWF   LAMPS
        MOVF    SIGBITS,W
        MOVWF   SIGNALS
        RRF     SIGBITS,F
        DECFSZ  SIGINDX,F
        RETLW   0
        MOVLW   8
        MOVWF   SIGINDX
        MOVLW   H'80'
        MOVWF   SIGBITS
        RETLW   0

READBIT BTFSC   SPI,SCLK
        RETLW   0
        BTFSS   SPI,DI
        GOTO    READ0
READ1   BSF     STATUS,C
        GOTO    SHIFTIN
READ0   BCF     STATUS,C
SHIFTIN RRF     SPIB1,F
        RRF     SPIB0,F
        BSF     SPISTATE,0
        RETLW   0
WRITEBIT BTFSS  SPIB1,7
        GOTO    WRITE0
WRITE1  BSF     SPI,DO
        GOTO    CHECKCE
WRITE0  BCF     SPI,DO
CHECKCE BCF     SPISTATE,0
        BTFSC   SPI,CE
        GOTO    PROCESSSPI
        RETLW   0
PROCESSSPI
;; SPI Command Processing: SPIB1 contains a command byte
;;                         SPIB0 contains a data byte
        MOVF    SPIB1,W     ; get command byte
        MOVF    SPICMD,F    ; stash cmd
        RRF     SPICMD,F    ; Shift right 5 bits
        RRF     SPICMD,F
        RRF     SPICMD,F
        RRF     SPICMD,F
        RRF     SPICMD,F
        MOVLW   CMDTAB      ; Jump table
        ADDWF   SPICMD,W    ; Add cmd
        MOVF    PCL,F       ; Jump

CMDTAB  RETLW   0       ; NOP
        GOTO    SETSTDASP
        GOTO    SETSPCASP
        GOTO    DEFASP
        GOTO    CYCLETEST
        GOTO    ENDTEST
        GOTO    NORAC3COL
        GOTO    NORACBICOL

;; Place holders
SETSTDASP
        MOVF    SPIB0,W
        ANDLW   SPIASPM
        MOVF    ASPNO,F
        MOVLW   ASPECTS
        ADDWF   ASPNO,W
        MOVWF   FSR
        MOVF    INDF,W
        MOVWF   ASPBITS
;
SETASP  MOVF    SPIB1,W
        ANDLW   SPISIGM
        MOVF    SPISIGNO,F
        INCF    SPISIGNO,F
        MOVLW   SIGBUF
        ADDWF   SPISIGNO,W
        MOVWF   FSR
        MOVF    ASPBITS,W
        MOVWF   INDF
        RETLW   0

SETSPCASP
        MOVF    SPIB0,W
        MOVWF   ASPBITS
;
        GOTO    SETASP

DEFASP  MOVF    SPIB0,W
        MOVWF   ASPBITS
;
        MOVF    SPIB1,W
        ANDLW   SPIASPM
        MOVF    ASPNO,F
        MOVLW   ASPECTS
        ADDWF   ASPNO,W
        MOVWF   FSR
        MOVF    ASPBITS,W
        MOVWF   INDF
        RETLW   0
CYCLETEST RETLW   1
ENDTEST RETLW   1
NORAC3COL                       ; Bits are GYR GYR YR
        MOVLW   ASPECTS
        MOVWF   FSR
        MOVLW   B'00000000'     ; #0: Dark
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'10000101'     ; #1: Clear: Green over Red over Red
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'01001001'     ; #2: Advance Approach: Yellow over Yellow over Red
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'01010001'     ; #3: Approach Limited: Yellow over Green over Red
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'01010001'     ; #4: Approach Medium: Yellow over Green over Red
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'01000101'     ; #5: Approach: Yellow over Red over Red
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'00101001'     ; #6: Approach Slow / Restricting: Red over Yellow over Red
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'00100101'     ; #7: Stop: Red over Red over Red
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'00000000'     ; #8 through #15: Dark -- user definable.
        MOVWF   INDF
        INCF    FSR,F
        MOVWF   INDF
        INCF    FSR,F
        MOVWF   INDF
        INCF    FSR,F
        MOVWF   INDF
        INCF    FSR,F
        MOVWF   INDF
        INCF    FSR,F
        MOVWF   INDF
        INCF    FSR,F
        MOVWF   INDF
        INCF    FSR,F
        MOVWF   INDF
        INCF    FSR,F
        RETLW   0        
NORACBICOL
                       ; Bits are GR GR GR XX (G+R = Y)
        MOVLW   ASPECTS
        MOVWF   FSR
        MOVLW   B'00000000'     ; #0: Dark
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'10010100'     ; #1: Clear: Green over Red over Red
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'11110100'     ; #2: Advance Approach: Yellow over Yellow over Red
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'11100100'     ; #3: Approach Limited: Yellow over Green over Red
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'1110010'     ; #4: Approach Medium: Yellow over Green over Red
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'11010100'     ; #5: Approach: Yellow over Red over Red
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'01110100'     ; #6: Approach Slow / Restricting: Red over Yellow over Red
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'01010100'     ; #7: Stop: Red over Red over Red
        MOVWF   INDF
        INCF    FSR,F
        MOVLW   B'00000000'     ; #8 through #15: Dark -- user definable.
        MOVWF   INDF
        INCF    FSR,F
        MOVWF   INDF
        INCF    FSR,F
        MOVWF   INDF
        INCF    FSR,F
        MOVWF   INDF
        INCF    FSR,F
        MOVWF   INDF
        INCF    FSR,F
        MOVWF   INDF
        INCF    FSR,F
        MOVWF   INDF
        INCF    FSR,F
        MOVWF   INDF
        INCF    FSR,F
        RETLW   0        
        end
