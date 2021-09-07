.data

input1: .asciz "Give me the first operand: "
input2: .asciz "Give me the second operand: "
format: .asciz "%d"
num1: .int 0
num2: .int 0
output: .asciz "The result of %d + %d is: %d\n "

.text
.global main
.extern printf
.extern scanf

main:
push {ip, lr}

@Imprimimos input1 y recibimos valro de num1
ldr  r0, =input1
bl   printf
ldr r0, =format
ldr r1, =num1
bl scanf

@Imprimimos input2 y recibimos valor de num2
ldr r0, =input2
bl printf
ldr r0, =format
ldr r1, =num2
bl scanf

@Imprimimimos resultado
ldr r1, =num1
ldr r1, [r1]
ldr r2, =num2
ldr r2, [r2]
add r3, r1, r2

ldr r0, =output
bl printf

pop  {ip, pc}