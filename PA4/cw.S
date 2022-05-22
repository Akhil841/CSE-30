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
    cntchar:    .space 4
    cntword:    .space 4
    cntline:    .space 4
    buffer:     .space BUFSZ

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
    //bl      rd                  // read another buffer
    //cmp     r0, 0               // if r0 <= 0 then EOF

// your code here
    //beginning of while loop
    .Lwhile:
        //call rd and store value in r0
        bl      rd
        //if r0 <= 0, end the loop
        cmp     r0, 0
        ble     .Lendwhile
        //store value of r0 in preserved register
        mov     r4, r0
        //r1 is for loop iterator
        mov     r1, 0
        //r2 is current state
        //0 non-word
        //1 word
        mov     r2, 0 
        //r5 is first character pointer
        ldr     r5, =buffer
        .Lfor:
            //r3 is whether character count updated
            //compare r1 to r4 for for loop
            cmp     r1, r4
            //if r1 > r4, then loop ends and we go to 
            //the while loop
            bgt     .Lwhile
            //get current character
            ldrb    r6, [r5, r1]
            ldr     r3, =cntchar
            ldr     r3, [r3]
            mov     r7, r3
            //compare current character to \n, \t and ' ' to determine
            //if it is a valid character
            cmp     r6, '\n'
            beq     .Lendif1
            b       .Lcif1
            .Lcif1:	
                    cmp     r6, '\t'
                    beq     .Lendif1
                    b       .Lcif2
                    .Lcif2:
                            cmp    r6, ' '
                            beq    .Lendif1
                            b      .Lischar
                            .Lischar:
                                add    r7, 1
                                //use r3 to store cntchar ptr
                                ldr    r3, =cntchar 
                                str    r7, [r3]
                                //r3 stores whether we add to the character count
                                //or not
                                b      .Lendif1
            .Lendif1:
            //perform different actions depending on whether or not
            //the current state is word or not
            cmp    r2, 0
            beq    .Lnotword
            //if we were previously in word mode, go to the word mode
            cmp    r2, 1      
            beq    .Lwordmode
            //otherwise, if r2 is somehow not 0 or 1, go to endfor
            b      .Lendfor
            .Lnotword:
            //check if cntchar was updated
            cmp    r3, r7
            //if it is, change state and update word count
            bne    .Lcurword
            //otherwise check if current character is a newline
            cmp    r6, '\n'
            beq    .Lfnewline
            //otherwise go to the end of the for loop
            b      .Lendfor
            .Lcurword:
                   //update state
                   mov    r2, 1
                   //increment word count by 1
                   ldr    r7, =cntword
                   ldr    r7, [r7]
                   add    r7, 1
                   //use r3 to store cntword ptr
                   ldr    r3, =cntword
                   str    r7, [r3]
                   //go to end of for loop since current character can't be
                   //a word and a '\n'
                   b      .Lendfor
            .Lfnewline:
                   //update line count
                   ldr    r7, =cntline
                   ldr    r7, [r7]
                   add    r7, 1
                   //use r3 to store cntline ptr
                   ldr    r3, =cntline
                   str    r7, [r3]
                   //go to end of for loop
                   b      .Lendfor
            //if the previous character was part of a word
            .Lwordmode:
            //check if the character count was not updated
            //if so, the word has ended.
            cmp    r3, r7
            beq    .Lwordend
            //otherwise go to end of for loop
            b      .Lendfor
            .Lwordend:
                   //update state
                   mov    r2, 0
                   //check if current character is newline and update line
                   //counter if so
                   cmp    r6, '\n'
                   beq    .Lfnewline
                   //otherwise go to endfor
                   b      .Lendfor
            .Lendfor:
                   //increment iterator
                   add    r1, 1
                   //repeat for loop
                   b      .Lfor      
    .Lendwhile:
    ldr     r3, =cntchar
    ldr     r3, [r3]
    sub     r3, 1
    ldr     r7, =cntchar
    str     r3, [r7]
    ldr     r3, =cntword
    ldr     r3, [r3]
    cmp     r3, 1
    bgt     .Lfixword
    b       .Lfinish
    .Lfixword:
    sub     r3, 1
    ldr     r7, =cntword
    str     r3, [r7]
    .Lfinish:
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
