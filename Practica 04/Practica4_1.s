.data
y: .word 0


.text
.global main

main:
@primer metodo
ldr r0, =y
ldr r1, =0xaaaaaaaa
str r1, [r0]

@segundo metodo
ldr r2, =0xbbbbbbbb
str r2, [r0],#4
@ldr r0, =y	@damos el valor a r0 de y otra vez

@tercer metodo
mov r4, r0
sub r0, r4, #
8
ldr r3, =0xcccccccc
str r3, [r0, #4]
