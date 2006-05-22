/* $Copyright:
*
* Copyright 2004-2006 by the Massachusetts Institute of Technology.
* 
* All rights reserved.
* 
* Export of this software from the United States of America may require a
* specific license from the United States Government.  It is the
* responsibility of any person or organization contemplating export to
* obtain such a license before exporting.
* 
* WITHIN THAT CONSTRAINT, permission to use, copy, modify, and distribute
* this software and its documentation for any purpose and without fee is
* hereby granted, provided that the above copyright notice appear in all
* copies and that both that copyright notice and this permission notice
* appear in supporting documentation, and that the name of M.I.T. not be
* used in advertising or publicity pertaining to distribution of the
* software without specific, written prior permission.  Furthermore if you
* modify this software you must label your software as modified software
* and not distribute it in such a fashion that it might be confused with
* the original MIT software. M.I.T. makes no representations about the
* suitability of this software for any purpose.  It is provided "as is"
* without express or implied warranty.
* 
* THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
* MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* 
* Individual source code files are copyright MIT, Cygnus Support,
* OpenVision, Oracle, Sun Soft, FundsXpress, and others.
* 
* Project Athena, Athena, Athena MUSE, Discuss, Hesiod, Kerberos, Moira,
* and Zephyr are trademarks of the Massachusetts Institute of Technology
* (MIT).  No commercial use of these trademarks may be made without prior
* written permission of MIT.
* 
* "Commercial use" means use of a name in a product or other for-profit
* manner.  It does NOT prevent a commercial firm from referring to the MIT
* trademarks in order to convey information (although in doing so,
* recognition of their trademark status should be given).
* $
*/


/*
 * Lists implementation.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "CredentialsCache.h"
#include "generic_lists.h"

/* this is an incomplete enumeration just including the generic type */
enum cc_list_type {
    generic = 0
};

/**
 * cci_generic_iterate_has_next()
 *
 * Purpose: Determine if an iterator has a next element
 *
 * Return:  1 if another element exists
 *          0 if no additional elements exist
 *
 * Errors:  None
 *
 */
cc_int32 
cci_generic_iterate_has_next(cc_generic_iterate_t *iterate) 
{
    return ((iterate == NULL || iterate->next == NULL) ? 0 : 1);
}

/**
 * cci_generic_iterate_next()
 *
 * Purpose: Retrieve the next element from an iterator and advance
 *          the iterator
 *
 * Return:  non-NULL, the next element in the iterator
 *          NULL, the iterator list is empty or iterator is invalid
 *
 * Errors:  ccErrBadParam
 *
 */
cc_int32
cci_generic_iterate_next(cc_generic_iterate_t *iterator, cc_generic_list_node_t** nodepp) 
{
    cc_generic_list_node_t* ret;
    
    if (iterator == NULL || nodepp == NULL)
        return ccErrBadParam;

    ret = iterator->next;
    if (iterator->next != NULL)
        iterator->next = iterator->next->next;

    *nodepp = ret;
    return ccNoError;
}

/**
 * cci_generic_list_new()
 *
 * Purpose: Allocate new generic list
 *
 * Return:  non-NULL, an empty list
 *          NULL, failure
 *
 * Errors:  ccErrNoMem
 *
 */
cc_int32
cci_generic_list_new(cc_generic_list_head_t ** listpp) 
{
    cc_generic_list_head_t* ret = (cc_generic_list_head_t *)malloc(sizeof(cc_generic_list_head_t));
    if (ret == NULL)
        return ccErrNoMem;
	
    ret->type = generic;
    ret->head = ret->tail = NULL;
    *listpp = ret;

    return ccNoError;
}

/**
 * cci_generic_list_append()
 *
 * Purpose: Appends a new node containing a copy of 'len' bytes of 'data' 
 *
 * Return:  non-NULL, a pointer to the newly allocated node
 *          NULL, failure
 *
 * Errors:  ccErrNoMem,ccErrBadParam
 *
 */
cc_int32
cci_generic_list_append(cc_generic_list_head_t *head, void *data, cc_uint32 len, cc_generic_list_node_t** nodepp) 
{
    cc_generic_list_node_t* new_node;

    if ( data == NULL || len == 0 )
        return ccErrBadParam;

    new_node = (cc_generic_list_node_t *)malloc(sizeof(cc_generic_list_node_t));
    if (new_node == NULL)
        return ccErrNoMem;

    new_node->data = malloc(len);
    if ( new_node->data == NULL ) {
        free(new_node);
        return ccErrNoMem;         
    }
    
    memcpy(new_node->data,data,len);
    new_node->len = len;

    if (head->head == NULL) { /*empty list*/
        head->head = new_node;
        head->tail = new_node;
	    new_node->next = new_node->prev = NULL;
    } else {
        new_node->prev = head->tail;
        head->tail->next = new_node;
        head->tail = new_node;
		new_node->next = NULL;
    }
	if (nodepp != NULL)
	    *nodepp = new_node;
    return ccNoError;
}

/**
 * cci_generic_list_prepend()
 *
 * Purpose: Prepends a new node containing a copy of 'len' bytes of 'data'
 *
 * Return:  non-NULL, a pointer to the newly allocated node
 *          NULL, failure
 *
 * Errors:  ccErrNoMem, ccErrBadParam
 *
 */
cc_int32 
cci_generic_list_prepend(cc_generic_list_head_t *head, void *data, cc_uint32 len, cc_generic_list_node_t** nodepp) 
{
    cc_generic_list_node_t* new_node;

    if ( data == NULL || len == 0 )
        return ccErrBadParam;

    new_node = (cc_generic_list_node_t *)malloc(sizeof(cc_generic_list_node_t));
    if (new_node == NULL)
        return ccErrNoMem;

    new_node->data = malloc(len);
    if ( new_node->data == NULL ) {
        free(new_node);
        return ccErrNoMem;
    }
    
    memcpy(new_node->data,data,len);
    new_node->len = len;
	
    if (head->head == NULL) { /*empty list*/
        head->head = new_node;
        head->tail = new_node;
        new_node->prev = new_node->next = NULL;
    } else {
        new_node->next = head->head;
        head->head->prev = new_node;
        new_node->prev = NULL;
        head->head = new_node;
    }

	if (nodepp != NULL)
		*nodepp = new_node;

    return ccNoError;
}

/**
 * cci_generic_list_remove_element()
 *
 * Purpose: Remove a node from the list
 *
 * Return:  0, success
 *         -1, failure
 *
 * Errors:  ccErrBadParam
 *
 */
cc_int32 
cci_generic_list_remove_element(cc_generic_list_head_t* head, cc_generic_list_node_t* rem) 
{
    if (head->head == NULL || rem == NULL)
        return ccErrBadParam;

    if (head->head == rem && head->tail == rem) { /*removing only element of list*/
        head->head = head->tail = NULL;
    } else if (head->head == rem) { /*removing head*/
        head->head = head->head->next;
    } else if (head->tail == rem) { /*removing tail*/
        head->tail = head->tail->prev;
        head->tail->next = NULL;
    } else {
        rem->prev->next = rem->next;
        rem->next->prev = rem->prev;
    }
    free(rem);
    return ccNoError;
}

/**
 * cci_generic_free_element()
 *
 * Purpose: Free the memory associated with a node
 *
 * Return:  0, success
 *         -1, failure
 *
 * Errors:  ccErrBadParam
 *
 */
cc_int32
cci_generic_free_element(cc_generic_list_node_t* node)
{
    if ( node == NULL )
        return ccErrBadParam;

    if ( node->data ) {
        free(node->data);
        node->data = NULL;
    }
    node->len = 0;
    node->next = node->prev = NULL;
    free(node);
    return ccNoError;
}


/**
 * cci_generic_list_destroy()
 *
 * Purpose: Deallocate a list and all of its contents
 *
 * Return:  0, success
 *         -1, failure
 *
 * Errors:  ccErrBadParam
 */
cc_int32
cci_generic_list_destroy(cc_generic_list_head_t* head) 
{
    cc_generic_list_node_t *cur, *next;
    cc_int32 ret = ccNoError;

    if ( head == NULL )
        return ccErrBadParam;
	
    for (cur = head->head; ret == ccNoError && cur != NULL; cur = next) {
        next = cur->next;
        ret = cci_generic_free_element(cur);
    }       
    free(head);
    return(ret);
}

/**
 * cci_generic_list_copy()
 *
 * Purpose: Copy a list
 *
 * Return:  non-NULL, a new list
 *          NULL, failure
 *
 * Errors:  ccErrBadParam, ccErrNoMem
 *
 */
cc_int32
cci_generic_list_copy(cc_generic_list_head_t* head, cc_generic_list_head_t** headpp) 
{
    cc_generic_list_head_t* copy;
    cc_generic_list_node_t *src_node, *dst_node;
    cc_int32 code;

    if (head == NULL || headpp == NULL)
        return ccErrBadParam;

    code = cci_generic_list_new(&copy);
    if (code != ccNoError)
        return code;

    for (src_node = head->head; src_node != NULL; src_node = src_node->next) {
        code = cci_generic_list_append(copy, src_node->data, src_node->len, &dst_node);
        if (code != ccNoError) {
            cci_generic_list_destroy(copy);
            return code;
        }
    }
    *headpp = copy;
    return ccNoError;
}

/**
 * cci_generic_list_iterator()
 *
 * Purpose: Allocate an iterator for the specified list
 *
 * Return:  non-NULL, an iterator
 *          NULL, failure
 *
 * Errors:  ccErrNoMem
 *
 */
cc_int32
cci_generic_list_iterator(cc_generic_list_head_t *head, cc_generic_iterate_t** headpp) 
{
    cc_generic_iterate_t* iterator;

    if ( head == NULL || headpp == NULL )
        return ccErrBadParam;

    iterator = (cc_generic_iterate_t*)malloc(sizeof(cc_generic_iterate_t));
    if (iterator == NULL)
        return ccErrNoMem;
    
    iterator->next = head->head;
    *headpp = iterator;
    return ccNoError;
}

/**
 * cci_generic_free_iterator()
 *
 * Purpose: Deallocate memory associated with an iterator
 *
 * Return:  0, success
 *         -1, failure
 *
 * Errors:  ccErrBadParam
 *
 */
cc_int32
cci_generic_free_iterator(cc_generic_iterate_t* iterator)
{
    if ( iterator == NULL )
        return ccErrBadParam;

    iterator->next = NULL;
    free(iterator);
    return ccNoError;
}



