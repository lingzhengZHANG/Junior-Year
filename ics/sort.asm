    .ORIG x3000
    LEA R0,#255; R0代表了链表的head
    LDR R7,R0,#1; R7作为存储最小值的寄存器,初始化为头指针内容
    ADD R6,R0,#0; R6作为存储最小值地址的寄存器，初始化为头的地址
loop1   LDR R1,R0,#0; 用R1开始遍历,R1内存着地址
    BRz end
    LDR R7,R0,#1
    ADD R6,R0,#0
loop2   LDR R2,R1,#1; R2内存着数值
    NOT R5,R2
    ADD R5,R5,#1 ; R5 = -R1
    ADD R5,R5,R7
    BRnz gap
    AND R7,R7,#0
    ADD R7,R2,#0; 更新最小值
    AND R6,R6,#0
    ADD R6,R1,#0; 更新最小值的指针地址
gap LDR R1,R1,#0;存的地址值不是0，那么就临时指针后移
    BRnp loop2
    LDR R4,R0,#1
    STR R7,R0,#1
    STR R4,R6,#1; 交换
    LDR R0,R0,#0 ; 主指针后移
    BRnp loop1
end HALT
    .END