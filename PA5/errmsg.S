 // version 1.0 05/22/2022 //
    .arch armv6
    .arm
    .fpu vfp
    .syntax unified

    // imports
    .extern stderr
    .extern fprintf

    .text

    ////////////////////////////////////////////
    // void errmsg(char *errormsg)            //
    // writes error messages to stderr        //
    ////////////////////////////////////////////
    .type   errmsg, %function
    .global errmsg
    .equ    FP_OFF,  4

errmsg:
    push    {fp, lr}
    add     fp, sp, FP_OFF

//your code here

    sub     sp, fp, FP_OFF
    pop     {fp, lr}
    bx      lr

    .size errmsg, (. - errmsg)
    .section .note.GNU-stack,"",%progbits
.end
