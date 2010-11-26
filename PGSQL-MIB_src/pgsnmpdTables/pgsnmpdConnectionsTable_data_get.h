/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.18.2.1 $ of : mfd-data-get.m2c,v $
 *
 * $Id: pgsnmpdConnectionsTable_data_get.h,v 1.1 2007/10/30 11:44:41 eggyknap Exp $
 *
 * @file pgsnmpdConnectionsTable_data_get.h
 *
 * @addtogroup get
 *
 * Prototypes for get functions
 *
 * @{
 */
#ifndef PGSNMPDCONNECTIONSTABLE_DATA_GET_H
#define PGSNMPDCONNECTIONSTABLE_DATA_GET_H

#ifdef __cplusplus
extern "C" {
#endif

/* *********************************************************************
 * GET function declarations
 */

/* *********************************************************************
 * GET Table declarations
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table pgsnmpdConnectionsTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * pgsnmpdConnectionsTable is subid 1 of pgsnmpdTables.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.27645.1.1.1, length: 10
*/
    /*
     * indexes
     */
    int pgsnmpdConnID_map(char **mib_pgsnmpdConnID_val_ptr_ptr, size_t *mib_pgsnmpdConnID_val_ptr_len_ptr, char *raw_pgsnmpdConnID_val_ptr, size_t raw_pgsnmpdConnID_val_ptr_len, int allow_realloc);

    int pgsnmpdConnHost_map(char **mib_pgsnmpdConnHost_val_ptr_ptr, size_t *mib_pgsnmpdConnHost_val_ptr_len_ptr, char *raw_pgsnmpdConnHost_val_ptr, size_t raw_pgsnmpdConnHost_val_ptr_len, int allow_realloc);
    int pgsnmpdConnHost_get( pgsnmpdConnectionsTable_rowreq_ctx *rowreq_ctx, char **pgsnmpdConnHost_val_ptr_ptr, size_t *pgsnmpdConnHost_val_ptr_len_ptr );
    int pgsnmpdConnPort_map(char **mib_pgsnmpdConnPort_val_ptr_ptr, size_t *mib_pgsnmpdConnPort_val_ptr_len_ptr, char *raw_pgsnmpdConnPort_val_ptr, size_t raw_pgsnmpdConnPort_val_ptr_len, int allow_realloc);
    int pgsnmpdConnPort_get( pgsnmpdConnectionsTable_rowreq_ctx *rowreq_ctx, char **pgsnmpdConnPort_val_ptr_ptr, size_t *pgsnmpdConnPort_val_ptr_len_ptr );
    int pgsnmpdConnDbName_map(char **mib_pgsnmpdConnDbName_val_ptr_ptr, size_t *mib_pgsnmpdConnDbName_val_ptr_len_ptr, char *raw_pgsnmpdConnDbName_val_ptr, size_t raw_pgsnmpdConnDbName_val_ptr_len, int allow_realloc);
    int pgsnmpdConnDbName_get( pgsnmpdConnectionsTable_rowreq_ctx *rowreq_ctx, char **pgsnmpdConnDbName_val_ptr_ptr, size_t *pgsnmpdConnDbName_val_ptr_len_ptr );


int pgsnmpdConnectionsTable_indexes_set_tbl_idx(pgsnmpdConnectionsTable_mib_index *tbl_idx, char *pgsnmpdConnID_val_ptr,  size_t pgsnmpdConnID_val_ptr_len);
int pgsnmpdConnectionsTable_indexes_set(pgsnmpdConnectionsTable_rowreq_ctx *rowreq_ctx, char *pgsnmpdConnID_val_ptr,  size_t pgsnmpdConnID_val_ptr_len);




#ifdef __cplusplus
}
#endif

#endif /* PGSNMPDCONNECTIONSTABLE_DATA_GET_H */
/** @} */
