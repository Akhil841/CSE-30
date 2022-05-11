#include <stdio.h>
#include <stdlib.h>
#include "cw.h"

char buffer[BUFSZ];
int  cntchar;   // count of chars IN words,not \n, \t or ' '
int  cntword;   // number of words
int  cntline;   // number of \n

int setup(int, char **);
int rd(void);
void result(void); 

/*
 * usage: ccw [file]
 */
int
main(int argc, char **argv)
{
    if (setup(argc, argv) != 0)
        return EXIT_FAILURE;
    
    /*
     * in a loop until end of file
     * call rd() to get a buffer of chars 
     * rd() returns the number of valid chars in the buffer
     * <= 0 signals end of file
     * the buffer contains an array of chars, not a string
     */
    
/*
 * add your code to
 * (1)  count the number of characters in the words
 * (not including any spaces, newlines or tabs,
 * (2) the number of words. words are delimited by space, \n or \t
 * (3) the number of \n in the input
 */

    /*
     * result() prints the summary of the values in the global
     * variables above
     * Until you complete result.S you can uncomment #define _PA4DBG_
     * to test your code make sure #define _PA4DBG_ is commented out
     * when you submit your code.
     */
//#define _PA4DBG_
#ifdef _PA4DBG_
     printf("lines:%8.8d words:%8.8d Chars:%8.8d\n", cntline,cntword,cntchar);
#else
    result();
#endif
    return EXIT_SUCCESS;
}
