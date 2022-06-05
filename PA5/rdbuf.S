//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.1 05/22/2022
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified

    // global constants
    #include "encrypter.h"

    .section .rodata
.Lmsg:  .string "rdbuf: Bookfile is too short for message\n"
.Lmsg2: .string "rdbuf: Buffer size error\n"
    .text

    //////////////////////////////////////////////////////
    // int rdbuf(char *IOBUF, char *BOOKBUF, FILE *FPIN
    //              FILE *FPBOOK, int BUFSZ);
    // returns number of bytes read EXIT_FAIL otherwise
    //////////////////////////////////////////////////////

    .type   rdbuf, %function
    .global rdbuf
    .equ    IARG5,  4 
    .equ    FP_OFF, 12
 
    ////////////////////////////////////////
    // preserved register use suggestions //
    // r4 pointer to book buffer          //
    // r5 FILE *FPBOOK  & cnt read saved  //
    ////////////////////////////////////////   
rdbuf:
    push    {r4, r5, fp, lr}
    add     fp, sp, FP_OFF

// your code here
    //store bufsz in r5
    ldr     r5, [fp, IARG5]
    //fail if BUFSZ <= 0
    cmp     r5, 0
    ble     .Lerr1
    b       .Lcont1
    .Lerr1:
    ldr     r0, =.Lmsg2
    bl      errmsg
    mov     r0, EXIT_FAIL
    b       .Lexit
    .Lcont1:
    //read the file
    //fread(IOBUF, 1, bufsz, FPIN)
    //IOBUF is already in r0, so
    //put FPBOOK (currently in r3) in r4
    mov     r4, r3
    //put FPIN (currently in r2) in r3
    mov     r3, r2
    //put bufsz in r2
    mov     r2, r5
    //put FPBOOK (currently in r4) in r5
    mov     r5, r4
    //put BOOKBUF (currently in r1) in r4
    mov     r4, r1
    //put 1 in r1
    mov     r1, 1
    //call fread. result goes in r0
    bl      fread
    //if fread returned 0 return 0
    cmp     r0, 0
    beq     .Lexit
    //otherwise, read from the book
    //fread(BOOKBUF, 1, cnt, FPBOOK)
    //move FPBOOK (currently in r5) to r3
    mov     r3, r5
    //move cnt (currently in r0) to r5
    mov     r5, r0
    //move cnt (currently in r5 and volatile r0) to r2
    mov     r2, r5
    //re-insert 1 in 1
    mov     r1, 1
    //move BOOKBUF (currently in r4) to r0
    mov     r0, r4
    //call fread. result goes in r0
    bl      fread
    //compare fread value to r5
    cmp     r0, r5
    //if the two are not equal, the bookfile is too small
    bne     .Lerr2
    //otherwise, return cnt
    b       .Lexit
    .Lerr2:
    //print error
    ldr     r0, =.Lmsg
    bl      errmsg
    //return failure
    mov     r0, EXIT_FAIL
    .Lexit:
    sub     sp, fp, FP_OFF
    pop     {r4, r5, fp, lr}
    bx      lr

    .size   rdbuf, (. - rdbuf)
    .section .note.GNU-stack,"",%progbits
.end
#endif
