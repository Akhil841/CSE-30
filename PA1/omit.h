/* 
 * the next two lines are the header guards
 */
#ifndef _OMIT_H
#define _OMIT_H
/*
 * header file for omit
 * type definition (enum) for DFA states
 * function prototypes
 */
enum typestate {
    START,
    SLASH,
    CPPCOMM,
    CCOMM,
    CSTAR,
    CERR,
    OQUOTE,
    OCHAR,
    QERR,
    CHERR,
    END
};
#endif
