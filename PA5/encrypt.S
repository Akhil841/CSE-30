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
    // void encrypt(char *IOBUF, char *BOOKBUF, int cnt)
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

.Ldone:
    sub     sp, fp, FP_OFF
    pop     {r4, r5, fp, lr}
    bx      lr
    .size encrypt, (. - encrypt)
    .section .note.GNU-stack,"",%progbits
.end
#endif
