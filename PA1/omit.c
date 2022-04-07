/*
 * Programming assignment 1 
 *
 * remove c comments from a source file and write to stdout
 * comments are replaced with a single space
 * handle single and double quoted sequences
 * comments and quoted sequences can span multiple lines
 *
 * error handling:
 *     prints starting line for unterminated comments or quote sequences
 */

#include <stdio.h>
#include <stdlib.h>
#include "omit.h" /* header file containing state enum */

static enum typestate state = START; /* DFA state */
static enum typestate fState; /* ending state, for error telemetry purposes */
static int commentBegin; /* stores where a comment begins, for error telemetry
                            purposes. is updated when a new comment starts. */

/*
 * function main()
 * 
 * Operation: reads input file character by character, writes input file
 * to output, but with comments removed
 * assumes: input file exists
 * params: none
 * returns: 0 if all comments were removed successfully, 1 if the file has
 * errors preventing the program from removing all comments properly
 */
int
main(void)
{
    int c; /* input character */
    int line = 1; /* line count */
    int nLineCtr = 0; /* number of newlines since a C-style comment has begun */
    while (state != END) {
        /* get next char and perform action according to the DFA */
        c = getchar();
        /* update line counter if necessary */
        if (c == '\n') 
            line++;
        switch (state) {
            /* if the state is currently START (initial/default state):
             * get the next state 
             * if the next state is END or SLASH,
             * do nothing because we have reached EOF
             * or we could be at a potential comment,
             * respectively.
             * if the next state is OQUOTE or OCHAR,
             * print it and store the beginning of the quote or
             * the character in case the quote or character is not
             * terminated
             * if it's any other character,
             * print it and continue
             */
            case START:
                state = STARTsub(c);
                switch (state) {
                    case END:
                    case SLASH:
                        break;
                    case OQUOTE:
                    case OCHAR:
                        commentBegin = line;
                        putchar(c);
                        break;
                    default:
                        putchar(c);
                        break;
                }
                break;
            /*
             * If the current state is SLASH (slash detected):
             * get the next state
             * if the nextstate is CPPCOMM, do nothing
             * because C++-style comments only last one line
             * we will not print commented code
             * if the next state is CCOMM, store the beginning of the C-style
             * comment so we can tell the user where the comment begins if it
             * is not terminated
             * otherwise, print both the slash and the next character if there
             * is one, since we did not print the slash in START.
             */
            case SLASH:
                state = SLASHsub(c);
                switch (state) {
                    case CPPCOMM: break;
                    case CCOMM:
                        commentBegin = line;
                        break;
                    default:
                        putchar('/');
                        if (c != EOF) 
                            putchar(c);
                        break;
                }
                break;
            /*
             * If the state is CPPCOMM (C++-style comment detected):
             * get the next state
             * If the comment has ended, whether
             * due to the file ending or a newline being found,
             * print a space and a newline to replace the commented code
             * Otherwise, do nothing because the current character is to 
             * be commented out.
             */
            case CPPCOMM:
                state = CPPCOMMsub(c);
                switch (state) {
                    case START:
                    case END:
                        putchar(' ');
                        putchar('\n');
                        break;
                    default: 
                        break;
                }
                break;
            /*
             * If the state is CCOMM (beginning of C-style comment detected)
             * get the next state
             * If the current character is a newline, update the newline counter
             */
            case CCOMM:
                state = CCOMMsub(c);
                if (c == '\n') nLineCtr++;
                break;
            /*
             * If the state is CSTAR (closing * of C-style comment detected)
             * get the next state
             * If the next state is START (if the comment has ended):
             * put a space to replace all the commented text
             * print one newline for each omitted newline
             * reset newline counter
             * If the next state is CERR (if the file ends before the comment
             * is terminated) do nothing
             * Otherwise, if the current character is a newline, update the
             * newline counter and continue searching for a closing * character
             */
            case CSTAR:
                state = CSTARsub(c);
                switch (state) {
                    case START:
                        putchar(' ');
                        for (int i = 0; i < nLineCtr; i++) {
                            putchar('\n');
                        }
                        nLineCtr = 0;
                        break;
                    case CERR: 
                        break;
                    default:
                        if (c == '\n') 
                            nLineCtr++;
                        break;
                }
                break;
            /*
             * If the state is CERR (the file ends before a C-style comment
             * is terminated):
             * get the next state (which is always END)
             * put a space in place of all the code commented out
             * print a newline for each newline detected
             * (in other words, act as if the the comment was closed at EOF)
             * store the final state as CERR so the function knows to print
             * an error message
             */
            case CERR:
                state = CERRsub();
                putchar(' ');
                for (int i = 0; i < nLineCtr; i++) {
                    putchar('\n');
                }
                fState = CERR;
                break;
            /*
             * If the state is OQUOTE (if an opening quotation was detected)
             * get the next state
             * If the next state is QERR (the file ends before the quote
             * is terminated) do nothing because this behavior is covered
             * in the QERR case
             * If the next state is OQUOTE:
             * if the next character is a backslash, print the backslash
             * as well as the character that follows it to prevent the program
             * from interpreting \" or \' as an opening or closing quote
             * otherwise just print the next character and continue searching for
             * a closing quote
             * if the state is any other state, the quote has ended so just print
             * the current character
             */
            case OQUOTE:
                state = OQUOTEsub(c);
                switch (state) {
                    case QERR: 
                        break;
                    case OQUOTE:
                        if (c == '\\') {
                            putchar(c);
                            int next = getchar();
                            if (next == '\n') 
                                line++;
                            putchar(next);
                            break;
                        }
                        else {
                            putchar(c);
                            break;
                        }
                    default:
                        putchar(c);
                        break;
                }
                break;
            /*
             * If the state is OCHAR (if an opening character was detected)
             * get the next state
             * If the next state is CHERR (the file ends before the character 
             * is terminated) do nothing because this behavior is covered
             * in the CHERR case
             * If the next state is OCHAR:
             * if the next character is a backslash, print the backslash
             * as well as the character that follows it to prevent the program
             * from interpreting \" or \' as an opening or closing character
             * otherwise just print the next character and continue searching for
             * a closing character
             * if the state is any other state, the character has ended so just print
             * the current character 
             */
            case OCHAR:
                state = OCHARsub(c);
                switch (state) {
                    case CHERR: 
                        break;
                    case OCHAR:
                        if (c == '\\') {
                            putchar(c);
                            int next = getchar();
                            if (next == '\n') 
                                line++;
                            putchar(next);
                            break;
                        }
                        else {
                            putchar(c);
                            break;
                        }
                    default:
                        putchar(c);
                        break;
                }
                break;
            /*
             * If the current state is QERR (if the file ends before a quote is 
             * terminated):
             * get the next state (which is always END)
             * store the final state so the function knows to print a quotation
             * error
             */
            case QERR:
                state = QERRsub();
                fState = QERR;
		        break;
            /*
             * If the current state is CHERR (if the file ends before a character is 
             * terminated):
             * get the next state (which is always END)
             * store the final state so the function knows to print a character
             * error
             */
            case CHERR:
                state = CHERRsub();
                fState = CHERR;
                break;
            /*
             * If the next state is END (the file has ended):
             * This code is never reached because the while loop
             * always ends before this condition is evaluated, but
             * a switch statement should account for all possible cases
             * so the code is here anyway
             * since the code does nothing, it does nothing
             */
            case END:
                break;
        }
    }
    /* Print an error depending on the final state 
     * and exit with failure
     */
    switch (fState) {
        case CERR:
            fprintf(stderr, "Error: line %d: unterminated comment\n", 
                commentBegin);
	        exit(EXIT_FAILURE);
        case QERR:
            fprintf(stderr, "Error: line %d: unterminated quote(\")\n", 
                commentBegin);
	        exit(EXIT_FAILURE);
        case CHERR:
            fprintf(stderr, "Error: line %d: unterminated quote(\')\n", 
                commentBegin);
	        exit(EXIT_FAILURE);
        /* If there are no errors then exit with a success */
        default: 
            exit(EXIT_SUCCESS);
    }
}
