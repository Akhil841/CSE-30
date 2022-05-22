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
    //r7 is current state
    //0 non-word state
    //1 word state
    mov    r7, 0     // int state = 0
    //beginning of while loop
    .Lwhile:
        //call rd and store value in r0
        bl      rd   
        //if r0 <= 0, end the loop
        cmp     r0, 0   // while ((cnt = rd()) > 0) { 
        ble     .Lendwhile
        //store value of r0 in preserved register
        mov     r4, r0
        //r1 is for loop iterator
        mov     r1, 0   // int i = 0
        //r5 is first character pointer
        ldr     r5, =buffer  // char *cur = buffer
        .Lfor:       // for (i = 0; i < cnt; i++) {
            //r3 is whether character count updated
            //compare r1 to r4 for for loop
            mov     r0, 0 // int charUp = 0
            cmp     r1, r4  // if (i >= cnt) break;
            mov     r2, 0
            //if r1 >= r4, then loop ends and we go to 
            //the while loop
            bge     .Lwhile
            //get current character
            ldrb    r6, [r5, r1]
            //compare current character to \n, \t and ' ' to determine
            //if it is a valid character
            cmp     r6, '\n'    // if (*cur != '\n' && *cur != '\t' && *cur != ' ') {
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
                                ldr    r2, =cntchar // cntchar++
                                ldr    r2, [r2]
                                add    r2, 1
                                //use r3 to store cntchar ptr
                                ldr    r3, =cntchar 
                                str    r2, [r3]
                                //r0 stores whether we add to the character count
                                //or not
            .Lendif1:
            //if current character is a newline, increment newline ctr
            cmp     r6, '\n'
            beq     .Lupnln
            b       .Lcont
            .Lupnln:
            ldr     r2, =cntline
            ldr     r2, [r2]
            add     r2, 1
            ldr     r3, =cntline
            str     r2, [r3]
            .Lcont:
            //depending on state, perform different actions
            cmp     r7, 0
            beq     .Lstate0
            b       .Lstate1
            //previous character was not part of a word
            //if current character is a word char, flip state
            //and increment word ctr
            .Lstate0:
            cmp     r6, '\n'
            bne     .Lnext1
            b       .Lendfor
            .Lnext1:
            cmp     r6, '\t'
            bne     .Lnext2
            b       .Lendfor
            .Lnext2:
            cmp     r6, ' '
            bne     .Lwordstart
            b       .Lendfor
            .Lwordstart:
            mov     r7, 1
            ldr     r2, =cntword
            ldr     r2, [r2]
            add     r2, 1
            ldr     r3, =cntword
            str     r2, [r3]
            //go to next loop iteration
            b       .Lendfor
            //previous character was part of a word
            //if current character is a word-ending character,
            //change state and go to next iteration
            .Lstate1:
            cmp     r6, '\n'
            beq     .Lwordend
            b       .Lnext3
            .Lnext3:
            cmp     r6, '\t'
            beq     .Lwordend
            b       .Lnext4
            .Lnext4:
            cmp     r6, ' '
            beq     .Lwordend
            b       .Lendfor
            .Lwordend:
            mov     r7, 0
            //go to next iteration   
            .Lendfor:
            add     r1, r1, 1
            b       .Lfor 
    .Lendwhile:
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
