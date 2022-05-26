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
 
.Ldone:
    sub     sp, fp, FP_OFF
    pop     {r4, r5, fp, lr}
    bx      lr

    .size decrypt, (. - decrypt)
    .section .note.GNU-stack,"",%progbits
.end
#endif
