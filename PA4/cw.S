    .arch armv6             // armv6 architecture
    .arm                    // arm 32-bit instruction set
    .fpu vfp                // floating point co-processor
    .syntax unified         // modern sytax

    //  include files
    #include    "cw.h"

    // name imports
    .extern     setup
    .extern     rd
    .extern     result
    
    // name exports
    .global     buffer
    .global     cntchar
    .global     cntword
    .global     cntline

    .data       // segment
    // allocate space for (1) buffer, (2) cntchar (3) cntword
    // (4) cntline; make sure to watch alignments!

    .text       // segment

    //////////////////////////////////////////////////////
    // int main(int argc, char **argv)                  //
    // program usage: cw [file]                         //
    //                                                  //
    // you can use any scratch register r0-r3           //
    // you can use these preserved registers            //
    // r4                                               //
    // r5                                               //
    // r6                                               //
    // r7                                               //
    //////////////////////////////////////////////////////

    .global main                // global for linking to
    .type   main, %function     // define as a function
    .equ    EXIT_SUCCESS, 0
    .equ    EXIT_FAILURE, 1
    .equ    FP_OFF, 20          // fp offset in stack frame

main:
    // function prologue
    // do not edit
    push    {r4-r7,fp, lr}      // save fp and lr on stack
    add     fp, sp, FP_OFF      // set frame pointer to frame base

    bl      setup               // call setup(argc, argv)
    cmp     r0, 0               // returns 0 if ok, -1 otherwise
    beq     .Lok
    mov     r0, EXIT_FAILURE
    b       .Lexit

.Lok:
    // can edit from here


/*
 * in a loop call rd() until end of file
 * add your code to
 * (1)  count the number of characters in the words
 * (not including any spaces, newlines or tabs,
 * (2) the number of words. words are delimited by space, \n or \t
 * (3) the number of \n in the input
 */

// here is how to use rd()
    bl      rd                  // read another buffer
    cmp     r0, 0               // if r0 <= 0 then EOF

// your code here


    // do not edit from here to end of this file
    bl      result              // print the results
    mov     r0, EXIT_SUCCESS    // fall through to .Lexit

.Lexit:
    // function epilogue
    sub     sp, fp, FP_OFF      // restore stack frame top
    pop     {r4-r7,fp,lr}       // remove frame and restore
    bx      lr                  // return to caller

    // function footer
    .size   main, (. - main)    // set size for function
.end
