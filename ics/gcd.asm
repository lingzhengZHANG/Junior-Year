        .ORIG x3000
        NOT R2,R0
        ADD R2,R2,#1; R2=-R0
        ADD R3,R1,R2; R3=R1-R0
        BRz DONE
        BRp posi
        BRn nega
posi    ADD R3,R3,R2
        BRz DONE
        BRp jump 
        AND R2,R2,#0
        ADD R2,R2,R3
        AND R3,R3,#0
        ADD R3,R3,R0
        NOT R0,R2
        ADD R0,R0,#1
jump    BRnzp posi
nega    AND R4,R4,#0
        ADD R4,R4,R0
        AND R0,R0,#0
        ADD R0,R0,R1
        AND R1,R1,#0
        ADD R1,R1,R4
        NOT R2,R0
        ADD R2,R2,#1
        ADD R3,R1,R2
        BRnzp posi
DONE    HALT
        .END