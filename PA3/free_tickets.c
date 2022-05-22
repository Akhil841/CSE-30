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
 * free_tickets
 *      tears down the database freeing all allocated memory
 *
 *      go down each hash chain and free all the vehicles and the tickets
 *      for each vehicle
 * args
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 */
void
free_tickets(struct vehicle **hashtab, uint32_t tabsz)
{
    /* store number of tickets removed here */
    int count = 0;
    /* iterate through each hash chain */
    for (uint32_t i = 0; i < tabsz; i++) {
        /* store current hash chain starting vehicle here */
        struct vehicle *curV = *(hashtab + i);
        /* if the starting vehicle isn't null */
        if (curV) {
            /* store current vehicle within current hash chain here */
            struct vehicle *curCurV = curV;
            /* store next vehicle here to easily go from one to the next */
            struct vehicle *vIterator = curCurV;
            /* while the next vehicle isn't null */
            while (vIterator) {
                /* update iterator */
                vIterator = curCurV->next;
                /* do a similar process to this one to remove all the tickets
                 * on this vehicle 
                 */
                struct ticket *curT = curCurV->head;
                struct ticket *tIterator = curT;
                while (tIterator) {
                    tIterator = curT->next;
                    struct ticket *temp = curT;
                    free(temp);
                    /* increase count of freed tickets by 1*/
                    count++;
                    /* go to the next vehicle */
                    curT = tIterator;
                }
                struct vehicle *temp = curCurV;
                /* free the current vehicle */
                free(temp->state);
                free(temp->plate);
                free(temp);
                /* go to the next vehicle */
                curCurV = vIterator;
            }
        }
    }
    /* free the total number of vehicles */
    free(hashtab);
    /* print the number of vehicles freed */
    printf("Total tickets freed: %i\n", count);
}
#endif
