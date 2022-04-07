/*
 * subroutines for omit program
 * one subroutine for each state of the DFA
 */
#include <stdio.h>
#include "omit.h" /* header file containing state enum */

/*
 * function STARTsub()
 *
 * Operation: returns the next state given an input character,
 * following the DFA in dfa.pdf
 * assumes: current state is START
 * params: current character
 * returns: the next state given an input character,
 * following the DFA in dfa.pdf
 */
enum typestate 
STARTsub(int ch) {
    /*
     * Follows the DFA:
     * If the file has ended return END,
     * if the next character is a slash return SLASH,
     * if the next character is a quotation return OQUOTE,
     * if the next character is an opening character return OCHAR
     * otherwise return START
     */
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

/*
 * function SLASHsub()
 *
 * Operation: returns the next state given an input character,
 * following the DFA in dfa.pdf
 * assumes: current state is SLASH
 * params: current character
 * returns: the next state given an input character,
 * following the DFA in dfa.pdf
 */
enum typestate
SLASHsub(int ch) {
    /*
     * Follows the DFA:
     * If the next character is a slash return CPPCOMM,
     * since two slashes represents the beginning of a
     * C++-style comment
     * If the next character is a star return CCOMM,
     * since a slash and a star represents the beginning of a
     * C-style comment
     * otherwise return START because it's just a character proceeding a slash
     */
    switch (ch) {
        case '/': 
            return CPPCOMM;
        case '*': 
            return CCOMM;
        default: 
            return START;
    }
}

/*
 * function CPPCOMMsub()
 *
 * Operation: returns the next state given an input character,
 * following the DFA in dfa.pdf
 * assumes: current state is CPPCOMM
 * params: current character
 * returns: the next state given an input character,
 * following the DFA in dfa.pdf
 */
enum typestate
CPPCOMMsub(int ch) {
    /*
     * Follows the DFA:
     * If the next character is a newline return START,
     * because C++-style comments end at a newline
     * If the file has ended return END, because C++-style comments
     * end at the end of a file, and the file has ended so no more
     * operations are necessary
     * otherwise return CPPCOMM because the comment still has not ended
     */
    switch (ch) {
        case '\n': 
            return START;
        case EOF: 
            return END;
        default: 
            return CPPCOMM;
    }
}

/*
 * function CCOMMsub()
 *
 * Operation: returns the next state given an input character,
 * following the DFA in dfa.pdf
 * assumes: current state is CCOMM
 * params: current character
 * returns: the next state given an input character,
 * following the DFA in dfa.pdf
 */
enum typestate
CCOMMsub(int ch) {
    /*
     * Follows the DFA:
     * If the file has ended return CERR
     * because this means that the file ended
     * before terminating the comment
     * If the current character is a star return CSTAR
     * because the comment may potentially end with the character
     * after
     * Otherwise return CCOMM because the comment has not ended yet
     */
    switch (ch) {
        case EOF: 
            return CERR;
        case '*': 
            return CSTAR;
        default: 
            return CCOMM;
    } 
}

/*
 * function CSTARsub()
 *
 * Operation: returns the next state given an input character,
 * following the DFA in dfa.pdf
 * assumes: current state is CSTAR
 * params: current character
 * returns: the next state given an input character,
 * following the DFA in dfa.pdf
 */
enum typestate
CSTARsub(int ch) {
    /*
     * Follows the DFA:
     * If the current character is a slash return START
     * because this means that the C-style comment has ended
     * If the file has ended, return CERR because this means
     * that the file has ended before the C-style comment was terminated
     * Otherwise return CCOMM because the star was a false alarm and the
     * C-style comment has not ended yet
     */
    switch (ch) {
        case '/': 
            return START;
        case EOF: 
            return CERR;
        default: 
            return CCOMM;
    }
}

/*
 * function CERRsub()
 *
 * Operation: returns END
 * assumes: current state is CERR
 * params: none
 * returns: END
 */
enum typestate
CERRsub(void) {
    /*
     * Return END because this error only occurs when the file ends
     * The reason we branch out the errors is because we want the
     * main function to know what kind of error to report
     */
    return END;
}

/*
 * function OQUOTEsub()
 *
 * Operation: returns the next state given an input character,
 * following the DFA in dfa.pdf
 * assumes: current state is OQUOTE
 * params: current character
 * returns: the next state given an input character,
 * following the DFA in dfa.pdf
 */
enum typestate
OQUOTEsub(int ch) {
    /*
     * Follows the DFA:
     * If the file has ended return QERR because
     * this means that the file ends before the quote is terminated
     * If the current character is a quotation mark, return START
     * because this means that the quote has ended
     * Otherwise return OQUOTE because the quote has not ended and we
     * are still looking for the end of the quote
     */
    switch (ch) {
        case EOF: 
            return QERR;
        case '\"': 
            return START;
        default: 
            return OQUOTE;
    }
}

/*
 * function OCHARsub()
 *
 * Operation: returns the next state given an input character,
 * following the DFA in dfa.pdf
 * assumes: current state is OCHAR
 * params: current character
 * returns: the next state given an input character,
 * following the DFA in dfa.pdf
 */
enum typestate
OCHARsub(int ch) {
    /*
     * Follows the DFA:
     * If the file has ended return CHERR because
     * this means that the file ends before the character is terminated
     * If the current character is a character mark, return START
     * because this means that the character has ended
     * Otherwise return OCHAR because the character has not ended and we
     * are still looking for the end of the character
     */
    switch (ch) {
        case EOF: 
            return CHERR;
        case '\'': 
            return START;
        default: 
            return OCHAR;
    }
}

/*
 * function QERRsub()
 *
 * Operation: returns END
 * assumes: current state is QERR
 * params: none
 * returns: END
 */
enum typestate
QERRsub(void) {
    /*
     * Return END because this error only occurs when the file ends
     * The reason we branch out the errors is because we want the
     * main function to know what kind of error to report
     */
    return END;
}

/*
 * function CHERRsub()
 *
 * Operation: returns END
 * assumes: current state is CHERR
 * params: none
 * returns: END
 */
enum typestate
CHERRsub(void) {
    /*
     * Return END because this error only occurs when the file ends
     * The reason we branch out the errors is because we want the
     * main function to know what kind of error to report
     */
    return END;
}
