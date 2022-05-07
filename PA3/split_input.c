#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "misc.h"
#include "hashdb.h"

//uncomment the next line when you want to use your routine
#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * split_input
 *
 * usage:   input buffer csv format, builds table of pointers to each column
 * assumes: input buffer has at least one char in it (not counting the '\0')
 * args:
 *  buf     one record from read from standind input stream by getline()
 *  delim   the input field delimiter
 *  cnt     number of fields (in table)
 *  table   point to a table of pointers to the start of each record
 *  lineno  line number of record (for passing to dropmesg)
 *  argv    for drop mesg to print name of program
 * returns: 0 if all the columns were found,
 *          -1 if a record did not have all the columns
 *          -1 if a record had an bad field value
 */
int
split_input(char *buf, char delim, int cnt, char **table, unsigned long lineno,
            char **argv)
{
    /*
     * 0 start
     * 1 quoted field
     * 2 unquoted field
     * 3 end quote
     * 4 MFQ (missing final quote)
     * 5 NTP (not terminated properly)
     * 6 QNA (quote not allowed)
     * 7 end
     */
    int state = 0;
    int colCnt = 0;
    char *cur = buf;
    while (*cur) {
        switch (state) {
            case 0: /* initial state */
                /* if possible, put current character's pointer in the table */
                if (colCnt < cnt)
                    *(table + colCnt) = cur;
                /* update table column count */
                colCnt++;
                /* do different things depending on the value of
                 * the current character
                 */
                /* if the current character is a quote,
                 * put the function in quote state and increment buffer ptr
                 */
                if (*cur == '\"') {
                    state = 1;
                    cur++;
                    continue;
                }
                /* if the current character is a newline,
                 * null out the newline and end the while loop
                 */
                if (*cur == '\n') {
                    *cur = '\0';
                    continue;
                }
                /* if the current character is the delimiter,
                 * null out the delimiter and advance
                 */
                if (*cur == delim) {
                    *cur = '\0';
                    cur++;
                    continue;
                }
                /* for any other character, put the function in an
                 * unquoted state and advance
                 */
                else {
                    state = 2;
                    cur++;
                    continue;
                }
                break;
            case 1: /* quoted state */
                /* if the current character is a newline,
                 * then the final quote is missing, since all
                 * the other fields thus far have been fine
                 */
                if (*cur == '\n') {
                    state = 4; /* MFQ state */
                    continue;
                }
                /* if the current chracter is an ending quote, 
                 * put the function in the end quote state and advance
                 */
                if (*cur == '\"') {
                    state = 3; /* end quote state */
                    cur++;
                    continue;
                }
                /* otherwise, it's still a quoted field so we can advance */
                else {
                    cur++;
                    continue;
                }
                break;
            case 2: /* unquoted state */
                /* if the current character is the delimeter,
                 * null out the character, return to the initial state,
                 * and advance
                 */
                if (*cur == delim) {
                    *cur = '\0';
                    state = 0;
                    cur++;
                    continue;
                }
                /* if the current character is a newline, null out the
                 * newline and end the while loop
                 */
                if (*cur == '\n') {
                    *cur = '\0';
                    continue;
                }
                /* if the current character is a quote, then
                 * return a quote not allowed error, since unquoted fields 
                 * should not have quotes*/
                if (*cur == '\"') {
                    state = 6; /* QNA state */
                    continue;
                }
                /* otherwise just advance by 1 since we're still in the 
                 * unquoted field
                 */
                else {
                    cur++;
                    continue;
                }
                break;
            case 3: /* end quote state*/
                /* if the current character is the delimiter, null
                 * out the current character, set the current state to start,
                 * and advance
                 */
                if (*cur == delim) {
                    *cur = '\0';
                    state = 0; /* starting state */
                    cur++;
                    continue;
                }
                /* if the current character is a newline, null
                 * out the newline and end the while loop
                 */
                if (*cur == '\n') {
                    *cur = '\0';
                    continue;
                }
                if (*cur == '\"') {
                    state = 1; /* quote state */
                    cur++;
                    continue;
                }
                /* if the current character is anything else, the
                 * quote is not terminated properly and we should therefore
                 * return that state
                 */
                else {
                    state = 5; /* NTP state */
                    continue;
                }
                break;
            case 4: /* MFQ state */
                dropmsg("Quoted field missing final quote", lineno, argv);
                return -1;
            case 5: /* NTP state */
                dropmsg("Quoted field not terminated properly", lineno, argv);
                return -1;
            case 6: /* QNA state */
                dropmsg("A \" is not allowed inside unquoted field", lineno, argv);
                return -1;
            case 7: /* unused end state */
                /* immediately end the while loop */
                *cur = '\0';
                continue;
                break;
        }
    }
    /* if we have too few columns */
    if (colCnt < cnt) {
        dropmsg("too few columns", lineno, argv);
        return -1;
    }
    /* if we have too many columns */
    if (colCnt > cnt) {
        dropmsg("too many columns", lineno, argv);
        return -1;
    }
    /* otherwise, split occurred successfully */
    return 0;
}
#endif
