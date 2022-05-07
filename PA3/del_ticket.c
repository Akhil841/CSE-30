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
//#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * del_ticket
 *      remove ticket (summons) from the database
 *      look up the summons for a vehicle and pay it (remove the ticket) 
 *
 *      Find the vehicle by the state and plate. if not found return -1
 *
 *      convert the summ string to a long long int using
 *      using strtosumid. This long long int is how the summons number
 *      is stored in the database in struct ticket.
 *      After being converted you can use it find a summons id match
 *      in the linked list of tickets attached to the struct vehicle.      
 *
 *      After you have the vehicle, find the summons on the ticket chain
 *      that matches the converted summons string (summid in the example)
 *      example:
 *        unsigned long long summid
 *        if (strtosumid(summ, &summid, argv) != 0)
 *           return -1;
 *        now search the ticket chain to find that summons
 *        struct ticket *tptr;
 *        if (tptr->summons == summid)
 *          found it
 *           
 *      If found, delete it from the ticket chain. If the vehicle
 *      no longer has any tickets, then remove the vehicle from the hash chain
 *      Otherwise you must update the tot_fine and cnt_ticket fields in
 *      the struct vehicle so the are accurate after removing the summons
 *      Make sure to delete all space that malloc()'d 
 *
 * Inputs:
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *  fineTab table mapping fine code to description and fine   
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  summ    summon id string to be found
 *  argv    for error printing
 *
 * returns 0 if ok -1 if not found or error
 */

int
del_ticket(struct vehicle **hashtab, uint32_t tabsz, struct fine *fineTab,
    char *plate, char *state, char *summ, char **argv)
{
    /* get hashed value to determine which hash chain to search */
    uint32_t hashVal = hash(plate, argv) % tabsz;
    /* initialize current vehicle struct that we are searching 
     * as the first member of the chain
     */
    struct vehicle *dmt = *(hashtab + hashVal);
    struct vehicle *prevV = dmt;
    /* Check each hash chain member and stop at a null pointer
     * if the state and plate of the current vehicle match 
     * what we are looking for, return that vehicle
     */
    int found = 0;
    while (dmt) {
        if (strcmp(state, dmt->state) == 0 && 
        strcmp(plate, dmt->plate) == 0) {
            found = 1;
            break;
        }
        /* if the current vehicle is not correct, advance to the next */
        prevV = dmt;
        dmt = dmt->next;
    }
    /* if the vehicle is not found return -1 */
    if (found == 0) 
        return -1;
    /* get summons id of correct ticket to delete */
    unsigned long long summid;
    if (strtosumid(summ, &summid, argv) != 0)
        return -1;
    /* current ticket variable for iterating through vehicle tickets */
    struct ticket *curr = dmt->head;
    /* previous ticket variable for iterating through vehicle tickets */
    struct ticket *prev = curr;
    found = 0;
    /* check all tickets to see if they have the same summid as what 
     * the user is asking for and use it if it does
     */
    while (curr) {
        if (curr->summons == summid) {
            found = 1;
            break;
        }
        /* advance both variables by 1 at the end of each iteration */
        prev = curr;
        curr = curr->next;
    }
    /* return -1 if the ticket is not found */
    if (found == 0)
        return -1;
    /* get the cost of the fine from the table */
    int fineVal = (fineTab + curr->code)->fine;
    /* adjust the vehicle constants accordingly */
    dmt->tot_fine -= fineVal;
    dmt->cnt_ticket -= 1;
    /* if this is the first ticket for the vehicle, 
     * set the head to the next ticket */
    if (prev == curr)
        dmt->head = curr->next;
    /* otherwise remove the ticket from the ticket linked list */
    else
        prev->next = curr->next;
    /* if the vehicle has no more tickets we need to 
     * remove it from the hash table */
    if (dmt->cnt_ticket == 0) {
        /* if this is the first vehicle in the hash chain, set the hash 
         * chain's head to the next vehicle
         */
        if (prevV == dmt) {
            *(hashtab + hashVal) = dmt->next;
        }
        /* otherwise just remove the vehicle from the hash chain */
        else
            prevV->next = dmt->next;
        /* free the removed vehicle and the attributes that were malloc'd */
        free(dmt->state);
        free(dmt->plate);
        free(dmt);
    }
    /* free the removed ticket */
    free(curr);
    return 0;
}
#endif
