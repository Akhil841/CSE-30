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
#include "omit.h"

enum typestate state = START;
enum typestate fState;
char currChar;
int commentBegin;

int
main(void)
{
    int c;
    int line = 1;
    int nLineCtr = 0;
    //input will be provided
    while (state != END) {
        c = getchar();
        if (c == '\n') line++;
        //action depends on state
        switch (state) {
            case START:
                state = STARTsub(c);
                if (state == END || state == SLASH) break;
                if (state == OQUOTE || state == OCHAR) {
                    commentBegin = line;
                    putchar(c);
                    break;
                }
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
            case SLASH:
                state = SLASHsub(c);
                switch (state) {
                    case CPPCOMM: break;
                    case CCOMM:
                        commentBegin = line;
                        break;
                    default:
                        putchar('/');
                        if (c != EOF) putchar(c);
                        break;
                }
                break;
            case CPPCOMM:
                state = CPPCOMMsub(c);
                switch (state) {
                    case START:
                    case END:
                        putchar(' ');
                        putchar('\n');
                        break;
                    default: break;
                }
                break;
            case CCOMM:
                state = CCOMMsub(c);
                if (c == '\n') nLineCtr++;
                break;
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
                    case CERR: break;
                    default:
                        if (c == '\n') nLineCtr++;
                        break;
                }
                break;
            case CERR:
                state = CERRsub();
                putchar(' ');
                for (int i = 0; i < nLineCtr; i++) {
                    putchar('\n');
                }
                fState = CERR;
                break;
            case OQUOTE:
                state = OQUOTEsub(c);
                switch (state) {
                    case QERR: break;
                    case OQUOTE:
                        if (c == '\\') {
                            putchar(c);
                            int next = getchar();
                            if (next == '\n') line++;
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
            case OCHAR:
                state = OCHARsub(c);
                switch (state) {
                    case CHERR: break;
                    case OCHAR:
                        if (c == '\\') {
                            putchar(c);
                            int next = getchar();
                            if (next == '\n') line++;
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
            case QERR:
                state = QERRsub();
                fState = QERR;
		        break;
            case CHERR:
                state = CHERRsub();
                fState = CHERR;
                break;
            case END:
                //unused
                break;
        }
    }
    switch (fState) {
        case CERR:
            fprintf(stderr, "Error: line %d: unterminated comment\n", commentBegin);
	        exit(EXIT_FAILURE);
        case QERR:
            fprintf(stderr, "Error: line %d: unterminated quote(\")\n", commentBegin);
	        exit(EXIT_FAILURE);
        case CHERR:
            fprintf(stderr, "Error: line %d: unterminated quote(\')\n", commentBegin);
	        exit(EXIT_FAILURE);
        default: exit(EXIT_SUCCESS);
    }
}
