//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.01 05/27/2022
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
    //place pointer to first character in arg 1
    mov     r1, r0
    //place output file (stderr) in arg 0
    ldr     r0, =stderr
    ldr     r0, [r0]
    //call fprintf
    bl      fprintf
    .Lexit:
    sub     sp, fp, FP_OFF
    pop     {fp, lr}
    bx      lr

    .size errmsg, (. - errmsg)
    .section .note.GNU-stack,"",%progbits
.end
#endif
