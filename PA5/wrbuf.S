//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.0 05/27/2022
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified
    
    // import constants
    #include "encrypter.h"

    .section .rodata
.Lmsg: .string "wrbuf: Write failed on output\n"
.Lmsg2: .string "wrbuf: Buffer size error\n"

    .text

    ///////////////////////////////////////////////////
    // int wrbuf(char *IOBUF, int cnt, FILE *FPOUT); //
    // return cnt on success EXIT_FAIL otherwise     //
    ///////////////////////////////////////////////////
    .type   wrbuf, %function
    .global wrbuf
    .equ    FP_OFF, 12
 
wrbuf:
    push    {r4, r5, fp, lr}
    add     fp, sp, FP_OFF
    
// your code here
    //put cnt in preserved register r4
    mov     r4, r1
    //check if cnt > 0. If not, return failure.
    cmp     r4, 0
    ble     .Lerr1
    b       .Lcont1
    .Lerr1:
    //store error message in r0
    ldr     r0, =.Lmsg2
    //call errmsg
    bl      errmsg
    //return failure
    mov     r0, EXIT_FAILURE
    //end function
    b       .Ldone
    .Lcont1:
    //write to file
    //fwrite(IOBUF, 1, cnt, FPOUT)
    //store FPOUT (currently in r2) in r3
    mov     r3, r2
    //store cnt (currently in r4) in r2
    mov     r2, r4
    //store 1 in r1
    mov     r1, 1
    //IOBUF is already in r0
    //call fwrite result is in r0
    bl      fwrite
    //return failure if returned value r0 does not equal parameter cnt r4
    cmp     r0, r4
    bne     .Lerr2
    //otherwise, the function has completed
    b       .Ldone
    .Lerr2:
    //store error message in r0
    ldr     r0, =.Lmsg
    //call errmsg
    bl      errmsg
    //return failure
    mov     r0, EXIT_FAILURE
    //end function
    b       .Ldone
.Ldone:
    sub     sp, fp, FP_OFF
    pop     {r4, r5, fp, lr}
    bx      lr

    .size   wrbuf, (. - wrbuf)
    .section .note.GNU-stack,"",%progbits
.end
#endif
