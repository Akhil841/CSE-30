//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.0 05/22/2022
    .arch armv6
    .arm
    .fpu vfp
    .syntax unified

    #include "encrypter.h"

    .text     // start of text segment

    //////////////////////////////////////////////////
    // int decrypt(char *IOBUF, char *BOOKBUF, int cnt) 
    // decrypts  IOBUF using BOOKBUF; result in IOBUF
    //////////////////////////////////////////////////
    .global decrypt
    .type   decrypt, %function
    .equ    FP_OFF, 12

decrypt:
    push    {r4, r5, fp, lr}
    add     fp, sp, FP_OFF

    // notes for you remove this from your code
    // passed two buffer with the same number of characters in them
    // for (i = 0; i < cnt; i++) {
    //  x = IOBUF[i] 
    //  y = BOOKBUF[i];
    //  x = x EOR y
    //  x = swap_nibbles (in the byte using shifts, or rotates, or masks)
    //  IOBUF[i] = x
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
    // store BOOKBUF[i] in r4
    ldrb   r4, [r1, r3]
    // r2 = r2 EOR r4
    eor    r2, r2, r4
    // swap nibbles of r2
    // r2 = ((r2 AND 0x0F) LSL 4) ORR ((r2 AND 0xF0) LSR 4)
    mov    r4, r2
    and    r2, r2, 0x0F
    lsl    r2, r2, 4
    and    r4, r4, 0xF0
    lsr    r4, r4, 4
    orr    r2, r2, r4
    // store result in IOBUF[i]
    strb   r2, [r0, r3]
    // for loop handling
    add    r3, r3, 1
    b      .Lfor
.Lendfor:
    // store return value cnt in r0
    mov    r0, r3 
.Ldone:
    sub     sp, fp, FP_OFF
    pop     {r4, r5, fp, lr}
    bx      lr

    .size decrypt, (. - decrypt)
    .section .note.GNU-stack,"",%progbits
.end
#endif
