lis $1 ; input address
.word 0xffff
lis $2 ; output address
.word 0xffff
lis $7 ; eof constant
.word -1
lis $9 ; 1 constant
.word 1
lis $4 ; 4 constant
.word 4
sub $3, $0, $0 ; counter
add $29, $0, $30 ; stack pointer 2

f:
    lw $6, 0($1)
    beq $6, $7, g ; exit

    sw $6,-4($29) ; store $6
    sub $29, $29, $4 ; increment SP2

    beq $0, $0, f
g:
    beq $29, $30, exit ; end of stack

    lw $6, 0($29)
    sw $6, 0($2)
    add $3, $3, $9
    add $29, $29, $4

    beq $0, $0, g

exit:
    jr $31
    