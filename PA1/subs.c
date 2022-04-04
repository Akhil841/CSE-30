/*
 * subroutines for omit program
 * one subroutine for each state of the DFA
 */
#include <stdio.h>
#include "omit.h"
enum typestate 
STARTsub(int ch) {
    switch (ch) {
        case EOF:
            return END;
        case '/':
            return SLASH;
        case '\"':
            return OQUOTE;
        case '\'':
            return OCHAR;
        default:
            return START;
    }
}

enum typestate
SLASHsub(int ch) {
    switch (ch) {
        case '/': return CPPCOMM;
        case '*': return CCOMM;
        default: return START;
    }
}

enum typestate
CPPCOMMsub(int ch) {
    switch (ch) {
        case '\n': return START;
        case EOF: return END;
        default: return CPPCOMM;
    }
}

enum typestate
CCOMMsub(int ch) {
    switch (ch) {
        case EOF: return CERR;
        case '*': return CSTAR;
        default: return CCOMM;
    } 
}

enum typestate
CSTARsub(int ch) {
    switch (ch) {
        case '/': return START;
        case EOF: return CERR;
        default: return CCOMM;
    }
}

enum typestate
CERRsub(void) {
    return END;
}

enum typestate
OQUOTEsub(int ch) {
    switch (ch) {
        case EOF: return QERR;
        case '\"': return START;
        default: return OQUOTE;
    }
}

enum typestate
OCHARsub(int ch) {
    switch (ch) {
        case EOF: return CHERR;
        case '\'': return START;
        default: return OCHAR;
    }
}

enum typestate
QERRsub(void) {
    return END;
}

enum typestate
CHERRsub(void) {
    return END;
}
