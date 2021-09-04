        .ORIG x800
        ; (1) Initialize interrupt vector table.
        LD R0, VEC
        LD R1, ISR
        STR R1, R0, #0;x1000 store in x0180

        ; (2) Set bit 14 of KBSR.
        LDI R0, KBSR
        LD R1, MASK
        NOT R1, R1
        AND R0, R0, R1
        NOT R1, R1
        ADD R0, R0, R1
        STI R0, KBSR
 
        ; (3) Set up system stack to enter user space.
        LD R0, PSR
        ADD R6, R6, #-1
        STR R0, R6, #0
        LD R0, PC
        ADD R6, R6, #-1
        STR R0, R6, #0
        ; Enter user space.     
        RTI
        
VEC     .FILL x0180
ISR     .FILL x1000
KBSR    .FILL xFE00
MASK    .FILL x4000
PSR     .FILL x8002
PC      .FILL x3000
        .END

        
        
        
        .ORIG x3000
Loop    LEA R0,ICS
        TRAP x22
DELAY   ST R1, SaveR1
        LD R1, COUNT  
REP     ADD R1, R1, #-1
        BRp REP
        LD R1, SaveR1
        BRnzp Loop
        RET
COUNT   .FILL x7FFF
SaveR1  .BLKW #1
ICS     .STRINGZ	"ICS2020  "
        .END




        .ORIG x1000
        TRAP x20
        LD R4,digit
        ADD R3,R3,#10
check   ADD R5,R4,R0
        BRz isdigit
        ADD R4,R4,#-1; to do add operation
        ADD R3,R3,#-1; serve as a counter
        BRp check

        TRAP x21
        LEA R0,no
        TRAP x22
        BRnzp end

isdigit TRAP x21
        LEA R0,is
        TRAP x22
end     RTI

KBDR    .FILL xFE02
NEW     .FILL   x000A
digit   .FILL x-0030
is      .STRINGZ	" is a decimal digit. "
no      .STRINGZ	" is not a decimal digit. "
        .END