    .ORIG x3000
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;
DO      LEA R3,Player1
        JSR isend
        JSR STATE
        JSR check
        LEA R3,Player2
        JSR isend
        JSR STATE
        JSR check
        BRnzp DO

check   ST R7,Addr
Loop    JSR Input
        JSR isvalid
        LD R0,NEWLINE
        TRAP x21
        LD R7,Addr
        RET

isvalid LD R5,Letter
        
        LD R6,ASCIIA
        ADD R6,R5,R6
        BRz numA

        LD R6,ASCIIB
        ADD R6,R6,R5
        BRz numB

        LD R6,ASCIIC
        ADD R6,R5,R6
        BRz numC
        
WRONG   LEA R0,ERROR
        TRAP x22
        LD R0,NEWLINE
        TRAP x21

        BRnzp Loop
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
numA    LD R5,Number
        BRnz WRONG
        LD R6,A
        NOT R5,R5
        ADD R5,R5,#1
        ADD R5,R5,R6
        BRn WRONG
        ST R5,A
        RET

numB    LD R5,Number
        BRnz WRONG
        LD R6,B
        NOT R5,R5
        ADD R5,R5,#1
        ADD R5,R5,R6
        BRn WRONG
        ST R5,B
        RET

numC    LD R5,Number
        BRnz WRONG
        LD R6,C
        NOT R5,R5
        ADD R5,R5,#1
        ADD R5,R5,R6
        BRn WRONG
        ST R5,C
        RET
;;;;;;;;;;;;;;;;;;;;;;;;;;;
Input   ADD R0,R3,#0
        TRAP x22
        LEA R0,CHOOSE
        TRAP x22
        LD R0,NEWLINE
        TRAP x21
        TRAP x20
        ST R0,Letter
        TRAP x21
        TRAP x20
        TRAP x21
        ADD R0,R0,#-16
        ADD R0,R0,#-16
        ADD R0,R0,#-16
        ST R0,Number
        
        LD R0,NEWLINE
        TRAP x21
        RET
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
STATE   ADD R6, R7, #0
        LEA R0, RA
        TRAP x22
        LD R1, A; plot oooooo
        JSR ploto
        LEA R0, RB
        TRAP x22
        LD R1, B
        JSR ploto
        LEA R0, RC
        TRAP x22
        LD R1, C
        JSR ploto
        ADD R7, R6, #0
        RET

ploto   BRz new
        LD R0,o
        TRAP x21
        ADD R1,R1,#-1
        BRnzp ploto

new     LD R0,NEWLINE
        TRAP x21
        RET
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

isend   LD R0,A
        BRnp return 
        LD R0,B
        BRnp return 
        LD R0,C
        BRnp return

        ADD R0,R3,#0
        TRAP x22
        LEA R0,WIN
        TRAP x22
        BRnzp end
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
return RET
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


end     HALT




A   .FILL #3
B   .FILL #5
C   .FILL #8
ALL .FILL #16
o   .FILL #111
NEWLINE .FILL x000A
Letter .FILL x0000
Number .FILL x0000
ASCIIA .FILL #-65
ASCIIB .FILL #-66
ASCIIC .FILL #-67
Addr .FILL x0000
ERROR .STRINGZ "Invalid move.Try again."
Player1 .STRINGZ "Player 1"
Player2 .STRINGZ "Player 2"
CHOOSE .STRINGZ ", choose a row and number of rocks:"
RA .STRINGZ "ROW A:"
RB .STRINGZ "ROW B:"
RC .STRINGZ "ROW C:"
WIN .STRINGZ	" Wins"

    .END