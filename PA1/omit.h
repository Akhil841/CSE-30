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

enum typestate STARTsub(int c);
enum typestate SLASHsub(int c);
enum typestate CPPCOMMsub(int c);
enum typestate CCOMMsub(int c);
enum typestate CSTARsub(int c);
enum typestate CERRsub(void);
enum typestate OQUOTEsub(int c);
enum typestate OCHARsub(int c);
enum typestate QERRsub(void);
enum typestate CHERRsub(void);
#endif
