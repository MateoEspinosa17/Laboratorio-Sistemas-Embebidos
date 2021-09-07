.data

input1: .asciz "Give me the first operand: "
input2: .asciz "Give me the operation to be performed (+, -, *, /): "
input3: .asciz "Give me the second operand: "
input4: .asciz "Give me the third operand: "
output: .asciz "The result of %d %c %d "
output2: .asciz "%c %d is: %d\n"

format1: .asciz "%d"
format2: .asciz "%c"

num1: .int 0
num2: .int 0
num3: .int 0
operand: .int 0
resultado: .int 0

.text
.global main
.extern printf
.extern scanf

@Inicio rutinas aritmeticas
sum:
push {ip, lr}
add r3, r4, r5
add r9, r3, r7
pop {ip, pc}

sub:
push {ip, lr}
sub r3, r4, r5
sub r9, r3, r7
pop {ip, pc}

mult:
push {ip, lr}
mul r3, r4, r5
mul r9, r3, r7
pop {ip, pc}

div:
push {ip, lr}
sdiv r3, r4, r5
sdiv r9, r3, r7
pop {ip, pc}

main:
push {ip, lr}

@Imprimimos input1 y recibimos valor de num1
ldr  r0, =input1
bl   printf
ldr r0, =format1
ldr r1, =num1
bl scanf
ldr r4, =num1
ldr r4, [r4]

@Imprimimos input2 y recibimos valor del operando
ldr  r0, =input2
bl   printf
@Por alguna razon si no se usa scanf dos veces no permite ingresar el valor del operando
ldr r0, =format2
ldr r1, =operand
bl scanf
@Fin del primer scanf que no se utiliza
ldr r0, =format2
ldr r1, =operand
bl scanf
ldr r6, =operand
ldr r6, [r6]

@Imprimimos input3 y recibimos valor de num2
ldr r0, =input3
bl printf
ldr r0, =format1
ldr r1, =num2
bl scanf
ldr r5, =num2
ldr r5, [r5]

@Imprimimos input4 y recibimos valor de num3
ldr r0, =input4
bl printf
ldr r0, =format1
ldr r1, =num3
bl scanf
ldr r7, =num3
ldr r7, [r7]

@Comparamos el operando y hacemos branch a la rutina
cmp r6, #43	@ +
bleq sum
cmp r6, #45	@ -		
bleq sub
cmp r6, #42	@ *
bleq mult	
cmp r6, #47	@ /
bleq div

@Imprimimimos resultado
ldr r8, =resultado
str r9, [r8]

ldr r1, =num1
ldr r1, [r1]
ldr r2, =operand
ldr r2, [r2]
ldr r3, =num2
ldr r3, [r3]
ldr r0, =output
bl printf

@Es necesario imprimirlo dos veces, por alguna razon cuando intente imprimirlo todo en una sola linea se imprimira solo la direccion de memoria segun yo
ldr r1, =operand
ldr r1, [r1]
ldr r2, =num3
ldr r2, [r2]
ldr r3, =resultado
ldr r3, [r3]
ldr r0, =output2
bl printf

pop  {ip, pc}