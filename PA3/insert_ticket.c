#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "hashdb.h"
#include "readsubs.h"

//uncomment the next line when you want to use your routine
#//define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * insert_ticket
 *      add a ticket (summons) to the database
 *
 *      if the vehicle (plate state) is not in the database then
 *      a new struct vehicle entry must be made for it in the hash table.
 *      New vehicle entries are made at the the FRONT of the collison chain
 *
 *      If the vehicle is already in the hash chain, you only need to add 
 *      a new ticket to the ticket chain for this summons.
 *
 *      Tickets are added to the ticket chain linked to the struct vehicle
 *      at the END of the ticket chain.
 *
 *      Also you must update the tot_fine and cnt_tickets in the struct vehicle
 *      to account for the new summons(ticket) just added
 * args:
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *          example hashval = hash(plate, argv) % tabsz;
 *  fineTab pointer fine table. maps code number to text description & fine cost 
 *          you use this table to get the amount of the fine when calculating
 *          the tot_fine field in struct vehicle.
 *          example:
 *              struct vehicle *vhpt;
 *              vhpt->tot_fine += fineTab[code].fine;
 *  summ    summons id string to be added
 *          this string needs to be converted to a number to be stored in the
 *          database. This saves space and is faster to compare than a string         
 *          Conversion function is supplied. use example:
 *               long long summid;
 *               if (strtosumid(summ, argv, &summid) != 0)
 *                  error handling
 *               new_ticket->summons = summid;
 *  plate   plate id string to be added
 *  state   state id string to be added
 *  date    date of summons string
 *          this string needs to be converted to a number to be stored in the
 *          database. This saves space and is faster to compare than a string
 *          The encoding into a number uses Linux time format.
 *          Conversion function is supplied. use example:
 *              time_t dateval;
 *              if (strtoDate(date, &dateval, argv) != 0)
 *                  error handling
 *              new_ticket->date = dateval;
 *  code    summons code integer value used an an index into the fines table
 *  argv    for error printing
 *
 * returns 0 if ok -1 for all errors
 */

int
insert_ticket(struct vehicle **hashtab, uint32_t tabsz, struct fine *fineTab, char *summ,
            char *plate, char *state, char *date, int code, char **argv)
{
    /* hash the licence plate number to get the vehicle's location */
    uint32_t hashVal = hash(plate, argv) % tabsz;
    /* store vehicle in this variable */
    struct vehicle *nv;
    /* whether the vehicle already exists in the database */
    int found = 1;
    /* assign nv to its existing counterpart or malloc its size to it if it 
     * doesn't exist
     */
    if (!(nv = vehicle_lookup(hashtab, tabsz, plate, state, argv))) {
        found = 0;
        if (!(nv = malloc(sizeof(*nv)))) {
            /* this will only occur if there isn't 
             * enough memory to malloc a new vehicle 
             */
            fprintf(stderr, "Not enough memory");
            return -1;
        }
    }
    /* put the vehicle, if new, to the front of the hash chain and get its
     * successor to prepare for initializing the vehicle 
     */
    struct vehicle *nextV = NULL;
    if (!found) {
        nextV = *(hashtab + hashVal);
        *(hashtab + hashVal) = nv;
    }
    /* Get summid and date in the correct format for ticket initialization */
    unsigned long long summid;
    if (strtosumid(summ, &summid, argv) != 0)
        return -1;
    time_t inDate;
    if (strtoDate(date, &inDate, argv) != 0)
        return -1;
    /* malloc and initialize ticket */
    struct ticket *newticket = malloc(sizeof(*newticket));
    *newticket = (struct ticket) {.summons = summid, 
                                  .date = inDate, 
                                  .code = code, 
                                  .next = NULL};
    /* initialize vehicle if it doesn't already exist */
    if (!found) {
        *nv = (struct vehicle) {.state = strdup(state), 
                                .plate = strdup(plate), 
                                .tot_fine = 0, 
                                .cnt_ticket = 0, 
                                .next = nextV, 
                                .head = newticket};
    }
    /* otherwise find the end of the vehicle's ticket LL to store the ticket */
    else {
        struct ticket *evt = nv->head;
        struct ticket *prev = evt;
        while (evt) {
            prev = evt;
            evt = evt->next;
        }
        /* store the ticket at the end of the ticket LL */
        prev->next = newticket;
    }
    /* update the vehicle's total fine and # of tickets */
    nv->tot_fine += (fineTab + code)->fine;
    nv->cnt_ticket += 1;
    /* return 0 for success */
    return 0;
}
#endif
