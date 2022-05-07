#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "hashdb.h"

//uncomment the next line when you want to use your routine
#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * vehicle_lookup
 *          look for vehicle in the database
 *          vehicle must match both plate and state strings
 * args
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  argv    for error printing
 *
 * returns  pointer to vehicle if found NULL otherwise
 */

struct vehicle *
vehicle_lookup(struct vehicle **hashtab, uint32_t tabsz, char *plate,
    char *state, char **argv)
{
    /* get hashed value to determine which hash chain to search */
    uint32_t hashVal = hash(plate, argv) % tabsz;
    /* initialize current vehicle struct that we are searching 
     * as the first member of the chain
     */
    struct vehicle *curr = *(hashtab + hashVal);
    /* Check each hash chain member and stop at a null pointer
     * if the state and plate of the current vehicle match 
     * what we are looking for, return that vehicle
     */
    while (curr) {
        if (strcmp(state, curr->state) == 0 && 
        strcmp(plate, curr->plate) == 0) {
            return curr;
        }
        /* if the current vehicle is not correct, advance to the next */
        curr = curr->next;
    }
    /* if the vehicle is not found return NULL */
    return NULL;
}
#endif
