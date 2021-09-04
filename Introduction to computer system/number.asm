        .ORIG x3000
        AND R7,R7,#0
        AND R0,R0,#0
        LD R7,ADR1; R0 stores the initial bit pattern
        LDR R0,R7,#0
        AND R7,R7,#0
        AND R1,R1,#0
        LD R7,ADR2; R1 stores the rotate amount, count variable
        LDR R1,R7,#0
        AND R4,R4,#0
        ADD R4,R4,#1
        NOT R4,R4
        ADD R4,R4,#1; R4 contains -1
        AND R5,R5,#0; R5 is the temporary register
        ADD R5,R5,R0
Loop    AND R6,R6,#0; set R6 0
        ADD R5,R5,#0
        BRp Loop1
        ADD R6,R6,#1;cause the number is negative, so after shift, add 1
Loop1   ADD R5,R5,R5
        ADD R5,R5,R6
        ADD R1,R1,R4
        BRp Loop 
        AND R7,R7,#0
        LD R7,ADR3
        STR R5,R7,#0
        HALT
ADR1 .FILL x3100
ADR2 .FILL x3101
ADR3 .FILL x3102
        .END