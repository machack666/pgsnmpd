/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.43.2.3 $ of : mfd-interface.m2c,v $
 *
 * $Id: pgsqlPgAggregateTable_interface.h,v 1.1 2008/01/17 11:49:24 eggyknap Exp $
 */
/** @defgroup interface: Routines to interface to Net-SNMP
 *
 * \warning This code should not be modified, called directly,
 *          or used to interpret functionality. It is subject to
 *          change at any time.
 * 
 * @{
 */
/*
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 * ***                                                               ***
 * ***  NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE  ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THIS FILE DOES NOT CONTAIN ANY USER EDITABLE CODE.      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THE GENERATED CODE IS INTERNAL IMPLEMENTATION, AND      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***    IS SUBJECT TO CHANGE WITHOUT WARNING IN FUTURE RELEASES.   ***
 * ***                                                               ***
 * ***                                                               ***
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 */
#ifndef PGSQLPGAGGREGATETABLE_INTERFACE_H
#define PGSQLPGAGGREGATETABLE_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif


#include "pgsqlPgAggregateTable.h"

/* ********************************************************************
 * Table declarations
 */

/* PUBLIC interface initialization routine */
void _pgsqlPgAggregateTable_initialize_interface(pgsqlPgAggregateTable_registration_ptr user_ctx,
                                    u_long flags);

    pgsqlPgAggregateTable_rowreq_ctx * pgsqlPgAggregateTable_allocate_rowreq_ctx(void);
void pgsqlPgAggregateTable_release_rowreq_ctx(pgsqlPgAggregateTable_rowreq_ctx *rowreq_ctx);

int pgsqlPgAggregateTable_index_to_oid(netsnmp_index *oid_idx,
                            pgsqlPgAggregateTable_mib_index *mib_idx);
int pgsqlPgAggregateTable_index_from_oid(netsnmp_index *oid_idx,
                              pgsqlPgAggregateTable_mib_index *mib_idx);

/*
 * access to certain internals. use with caution!
 */
void pgsqlPgAggregateTable_valid_columns_set(netsnmp_column_info *vc);


#ifdef __cplusplus
}
#endif

#endif /* PGSQLPGAGGREGATETABLE_INTERFACE_H */
