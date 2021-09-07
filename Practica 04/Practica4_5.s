.data

input: .asciz "Give me the value for X: "
output: .asciz "Result: %d\n"

format: .asciz "%d"

x: .int 0
out: .int 0

.text
.global main
.extern printf
.extern scanf

main:
push {ip, lr}

@Leemos el valor de x
ldr r0, =input
bl printf
ldr r0, =format
ldr r1, =x
bl scanf
ldr r2, =x
ldr r2, [r2]

@6x²
mul r3, r2, r2
mov r4, #6
mul r5, r3, r4

@9x
mov r6, #9
mul r7, r2, r6

@6x² + 9x
add r8, r7, r5

@6x² + 9x + 2
add r9, r8, #2

@Imprimimos el resultado
ldr r10, =out
str r9, [r10]

ldr r1, =out
ldr r1, [r1]
ldr r0, =output
bl printf

pop {ip, pc}