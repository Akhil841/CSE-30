//uncomment the next line when you want to use your routine
#define MYCODE
#ifdef MYCODE
// version 1.0 05/22/2022
    .arch armv6
    .arm
    .fpu vfp
    .syntax unified

    #include "encrypter.h"

    .text     // start of text segment

    //////////////////////////////////////////////////
    // int encrypt(char *IOBUF, char *BOOKBUF, int cnt)
    // encrypts  IOBUF with BOOKBUF; updating IOBUF
    //////////////////////////////////////////////////
    .global encrypt
    .type   encrypt, %function
    .equ    FP_OFF, 12

encrypt:
    push    {r4, r5, fp, lr}
    add     fp, sp, FP_OFF

    // notes for you remove this from your code
    // passed two buffer with the same number of characters in them
    // for (i = 0; i < cnt; i++) {
    //  x = IOBUF[i]
    //  x = swap_nibbles (in the byte using shifts, or rotates, or masks)
    //  y = BOOKBUF[i]
    //  IOBUF[i] = x EOR y
    // }
    // return cnt
   
// your code here 
    // int cnt = 0;
    mov    r3, 0
    // move max cnt into preserve register r5
    mov    r5, r2
    // for loop
.Lfor:
    cmp    r3, r5
    bge    .Lendfor
    // store IOBUF[i] in r2
    ldrb   r2, [r0, r3]
    // swap nibbles
    // x = ((x AND 0x0F) LSL 4) ORR ((x AND 0xF0) LSR 4)
    mov    r4, r2
    and    r2, r2, 0x0F
    lsl    r2, r2, 4
    and    r4, r4, 0xF0
    lsr    r4, r4, 4
    orr    r2, r2, r4
    // store BOOKBUF[i] in r4
    ldrb   r4, [r1, r3]
    // store x EOR y in r2
    eor    r2, r2, r4
    // store r2 in IOBUF[i]
    strb   r2, [r0, r3]
    add    r3, r3, 1
    b      .Lfor
.Lendfor:
    //put result in r0
    mov    r0, r3
.Ldone:
    sub     sp, fp, FP_OFF
    pop     {r4, r5, fp, lr}
    bx      lr
    .size encrypt, (. - encrypt)
    .section .note.GNU-stack,"",%progbits
.end
#endif
