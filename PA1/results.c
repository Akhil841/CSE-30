 











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
     
    while (state != END) {
         
         
        c = getchar();
        if (c == '\n') line++;
         
        switch (state) {
            case START:
                if (c == EOF) state = END;
                else if (c == '/') state = SLASH;
                else if (c == '\"') {
                    commentBegin = line;
                    state = OQUOTE;
                    putchar('\"');
                }
                else if (c == '\'') {
                    commentBegin = line;
                    state = OCHAR;
                    putchar('\'');
                }
                else putchar(c);
                break;
            case SLASH:
                if (c == '/') state = CPPCOMM;
                else if (c == '*') {
                    commentBegin = line;
                    state = CCOMM;
                }
                else {
                    putchar('/');
                    if (c != EOF) putchar(c);
                    state = START;
                }
                break;
            case CPPCOMM:
                if (c == '\n') {
                    state = START;
                    putchar(' ');
                    putchar('\n');
                }
                break;
            case CCOMM:
                if (c == EOF) state = CERR;
                else if (c == '*') state = CSTAR;
                else if (c == '\n') nLineCtr++;
                break;
            case CSTAR:
                if (c == '/') {
                    state = START;
                    putchar(' ');
                    for (int i = 0; i < nLineCtr; i++) {
                        putchar('\n');
                    }
                    nLineCtr = 0;
                }
                else if (c == EOF) state = CERR;
                else {
                    if (c == '\n') nLineCtr++;
                    state = CCOMM;
                }
                break;
            case CERR:
                putchar(' ');
                 
                for (int i = 0; i < nLineCtr; i++) {
                    putchar('\n');
                }
                fState = CERR;
                state = END;
                break;
            case OQUOTE:
                if (c == EOF) state = QERR;
                else if (c == '\\') {
                    putchar(c);
                    int next = getchar();
                    if (next == '\n') line++;
                    putchar(next);
                }
                else if (c == '\"') {
                    state = START;
                    putchar(c);
                }
                else putchar(c);
                break;
            case OCHAR:
                if (c == EOF) state = CHERR;
                else if (c == '\\') {
                    putchar(c);
                    int next = getchar();
                    if (next == '\n') line++;
                    putchar(next);
                }
                else if (c == '\'') {
                    state = START;
                    putchar(c);
                }
                else putchar(c);
                break;
            case QERR:
                fState = QERR;
                state = END;
		        break;
            case CHERR:
                fState = CHERR;
                state = END;
                break;
            case END:
                 
                break;
        }
    }
    if (fState == CERR) {
	    fprintf(stderr, "Error: line %d: unterminated comment\n", commentBegin);
	    exit(EXIT_FAILURE);	
    }
    if (fState == QERR) {
	    fprintf(stderr, "Error: line %d: unterminated quote(\")\n", commentBegin);
	    exit(EXIT_FAILURE);
    }
    if (fState == CHERR) {
	    fprintf(stderr, "Error: line %d: unterminated quote(\')\n", commentBegin);
	    exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
