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
    START, /* initial/default state */
    SLASH, /* slash detected (potential comment) */
    CPPCOMM, /* C++-style (//) comment detected */
    CCOMM, /* C-style (block) comment detected */
    CSTAR, /* closing * of C-style comment detected */
    CERR, /* file ends before C-style comment is terminated */
    OQUOTE, /* opening quote character detected */
    OCHAR, /* opening character character detected */
    QERR, /* file ends before closing quote character is detected */
    CHERR, /* file ends before closing character character is detected */
    END /* end of file */
};

/*
 * Function prototypes for each DFA state
 */
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
