    // version 1.0 05/22/2022 //
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
    //.equ    FPBOOK,     ?    // FILE * to book file
    //.equ    FPIN,       ?    // FILE * to input file
    //.equ    FPOUT,      ?      // FILE * to output file
    //.equ    MODE,       ?     // decrypt or encrypt mode
    //.equ    IOBUF,      ?  // buffer for input file
    //.equ    BOOKBUF,   ? // buffer for book file
    // add local variables here: adjust PAD size
    //.equ    PAD,        ?   // Stack frame PAD 
    //.equ    OARG6,      ?       // output arg6
    //.equ    OARG5,      ?     // output arg5 must be at bottom
    //.equ    FRAMESZ,    OARG5-FP_OFF// total space for frame
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
    // r9 output FILE *                  //
    ////////////////////////////////////////

main:
    push    {r4-r9, fp, lr}
    add     fp, sp, FP_OFF          // set frame pointer to frame base
// uncomment this! ldr     r3, =FRAMESZ            // frame size is big, use pseudo ldr
    sub     sp, sp, r3              // allocate space for locals and passed args 
     
    // call int setup(argc, argv, *mode, *FPBOOK, *FPIN, *FPOUT)

    // set up for main loop

.Lloop:

    // int rdbuf(char *IOBUF, char *BOOKBUF, FILE *FPIN
    //              FILE *FPBOOK, int BUFSZ);
    // returns number of bytes read EXIT_FAIL otherwise

    // next:
    // int XXcrypt(char *IOBUF, char *BOOKBUF, int cnt)
    // encrypt or decrypt updating the buffer c 
    // returns the number of chars processed or EXIT_FAIL

    // next:
    // XXcrypted the buffer, write it out
    // int wrbuf(char *IOBUF, int cnt, FILE *FPOUT);
    // return cnt on success EXIT_FAIL otherwise 

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
