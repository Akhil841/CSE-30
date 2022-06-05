//uncomment the next line when you want to use your routine
//#define MYCODE
#ifdef MYCODE
// version 1.1 05/27/2022
    .arch   armv6
    .arm
    .fpu    vfp
    .syntax unified

    // function import
    .extern encrypt
    .extern decrypt
    .extern setup
    .extern rdbuf
    .extern cleanup
    .extern wrbuf

    // global constants
    #include "encrypter.h"

    .text

    //////////////////////////////////////////////////////
    // int main(int argc, char **argv)                  //
    // encrypter [-d | -e] -b bookfile encryption_file  //
    //////////////////////////////////////////////////////

    // finish the stack frame first
    .global main
    .type   main, %function
    .equ    BUFSZ,      1024        // buffer size
    .equ    FP_OFF,     28          // fp offset in main stack frame
    .equ    FPBOOK,     4 + FP_OFF    // FILE * to book file
    .equ    FPIN,       4 + FPBOOK    // FILE * to input file
    .equ    FPOUT,      4 + FPIN      // FILE * to output file
    .equ    MODE,       4 + FPOUT     // decrypt or encrypt mode
    .equ    IOBUF,      BUFSZ + MODE  // buffer for input file
    .equ    BOOKBUF,    BUFSZ + IOBUF // buffer for book file
    // add local variables here: adjust PAD size
    .equ    PAD,        0 + BOOKBUF   // Stack frame PAD 
    .equ    OARG6,      4 + PAD      // output arg6
    .equ    OARG5,      4 + OARG6    // output arg5 must be at bottom
    .equ    FRAMESZ,    OARG5-FP_OFF// total space for frame
    ////////////////////////////////////////////////////////////////
    // make sure that FRAMESZ + FP_OFF + 4 divides by 8 EVENLY!
    ////////////////////////////////////////////////////////////////
 
    ////////////////////////////////////////
    // preserved register use             //
    // r4 pointer to input/output buffer  //
    // r5 pointer to book buffer          //
    // r6 mode flag                       //
    // r7 input FILE *                    //
    // r8 book FILE *                     //
    // r9 output FILE *                   //
    ////////////////////////////////////////

main:
    push    {r4-r9, fp, lr}
    add     fp, sp, FP_OFF          // set frame pointer to frame base
    ldr     r3, =FRAMESZ            // frame size is big, use pseudo ldr
    sub     sp, sp, r3              // allocate space for locals and passed args
    ldr     r5, =IOBUF              // load IOBUF ptr
    ldr     r6, =BOOKBUF            // load BOOKBUF ptr

    // call int setup(argc, argv, *mode, *FPBOOK, *FPIN, *FPOUT)
    // prepare to call setup
    // r0 and r1 already contain argc and argv respectively
    // r2 must contain mode flag pointer
    sub     r2, fp, MODE
    // r3 must contain FPBOOKP
    sub     r3, fp, FPBOOK
    // store FPINP in OARG5
    ldr     r7, [fp, -FPIN]
    str     r7, [fp, -OARG5]
    // store FPOUTP in OARG6
    ldr     r9, [fp, -FPOUT]
    str     r9, [fp, -OARG6]
    // call setup, result in r0
    bl      setup
    // end function if setup failed
    cmp     r0, EXIT_FAIL
    beq     .Lfail
    b       .Lcontinue
.Lfail:
    mov     r0, EXIT_FAILURE
    b       .Lexit
.Lcontinue:
    // store IOBUF ptr in r4
    sub     r4, fp, r5
    // store BOOKBUF ptr in r5
    sub     r5, fp, r6
    // store mode flag value in r6
    ldr     r6, [fp, -MODE]
    // store FPIN in r7
    ldr     r7, [r7]
    // store FPBOOK in r8
    sub     r8, fp, FPBOOK
    ldr     r8, [r8]
    // store FPOUT in r9
    ldr     r9, [r9]
.Lloop:
    // int rdbuf(char *IOBUF, char *BOOKBUF, FILE *FPIN
    //              FILE *FPBOOK, int BUFSZ);
    // returns number of bytes read EXIT_FAIL otherwise
    // prepare to call rdbuf
    // store IOBUF ptr in r0
    mov     r0, r4
    // store BOOKBUF ptr in r1
    mov     r1, r5
    // store FPIN in r2
    mov     r2, r7
    // store BUFSZ in OARG5
    mov     r3, BUFSZ
    str     r3, [fp, -OARG5]
    // store FPBOOK in r3
    mov     r3, r8
    // call rdbuf, result cnt in r0
    bl      rdbuf
    cmp     r0, 0
    // if the result leq 0 end the loop
    ble     .Ldone
    
.Lprocinput:
    // int XXcrypt(char *IOBUF, char *BOOKBUF, int cnt)
    // encrypt or decrypt updating the buffer c 
    // returns the number of chars processed or EXIT_FAIL
    // prepare to call XXcrypt
    // store IOBUF ptr (r4) in r0, BOOKBUF ptr (r5) in r1, cnt (r0) in r2
    mov     r2, r0
    mov     r0, r4
    mov     r1, r5
    // go to decrypt if MODE != ENCRYPT_MODE, call XXcrypt and go to writeout
    cmp     r6, ENCRYPT_MODE
    bne     .Ldecrypt
    bl      encrypt
    b       .Lwriteout

.Ldecrypt: 
    // decrypt the input
    bl      decrypt

.Lwriteout:
    // XXcrypted the buffer, write it out

    // int wrbuf(char *IOBUF, int cnt, FILE *FPOUT);
    // return cnt on success EXIT_FAIL otherwise
    // move cnt from r0 to r1
    mov     r1, r0
    // if cnt is EXIT_FAIL clean up
    cmp     r1, EXIT_FAIL
    beq     .Ldone
    // prepare to call wrbuf
    // store IOBUF (r4) in r0, FPOUT (r9) in r2
    mov     r0, r4
    mov     r2, r9
    bl      wrbuf
    // if cnt (r0) is EXIT_FAIL clean up
    cmp     r0, EXIT_FAIL
    beq     .Ldone
    // otherwise go to the beginning of the while loop
    b       .Lloop

.Ldone:
    // int cleanup(int status)
    // r0 should have exit status in it
    bl      cleanup

.Lexit:
    sub     sp, fp, FP_OFF
    pop     {r4-r9, fp, lr}
    bx      lr

    .size   main, (. - main)
    .section .note.GNU-stack,"",%progbits
.end
#endif
