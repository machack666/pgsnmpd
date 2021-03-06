/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 1.18.2.1 $ of : mfd-data-get.m2c,v $ 
 *
 * $Id: pgsnmpdConnectionsTable_data_get.c,v 1.1 2007/10/30 11:44:41 eggyknap Exp $
 */
/* standard Net-SNMP includes */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/* include our parent header */
#include "pgsnmpdConnectionsTable.h"


/** @defgroup data_get data_get: Routines to get data
 *
 * TODO:230:M: Implement pgsnmpdConnectionsTable get routines.
 * TODO:240:M: Implement pgsnmpdConnectionsTable mapping routines (if any).
 *
 * These routine are used to get the value for individual objects. The
 * row context is passed, along with a pointer to the memory where the
 * value should be copied.
 *
 * @{
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

/* ---------------------------------------------------------------------
 * TODO:200:r: Implement pgsnmpdConnectionsTable data context functions.
 */

/*---------------------------------------------------------------------
 * PGSQL-MIB::pgsnmpdConnectionEntry.pgsnmpdConnID
 * pgsnmpdConnID is subid 1 of pgsnmpdConnectionEntry.
 * Its status is Current, and its access level is NoAccess.
 * OID: .1.3.6.1.4.1.27645.1.1.1.1.1
 * Description:
Identifying string for this connection. Default is host:port:database.
 *
 * Attributes:
 *   accessible 0     isscalar 0     enums  0      hasdefval 0
 *   readable   0     iscolumn 1     ranges 1      hashint   1
 *   settable   0
 *   hint: 255a
 *
 * Ranges:  0 - 255;
 *
 * Its syntax is DisplayString (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.  (Max 255)
 *
 *
 *
 * NOTE: NODE pgsnmpdConnID IS NOT ACCESSIBLE
 *
 *
 */
/**
 * map a value from its original native format to the MIB format.
 *
 * @retval MFD_SUCCESS         : success
 * @retval MFD_ERROR           : Any other error
 *
 * @note parameters follow the memset convention (dest, src).
 *
 * @note generation and use of this function can be turned off by re-running
 * mib2c after adding the following line to the file
 * default-node-pgsnmpdConnID.m2d :
 *   @eval $m2c_node_skip_mapping = 1@
 *
 * @remark
 *  If the values for your data type don't exactly match the
 *  possible values defined by the mib, you should map them here.
 *  Otherwise, just do a direct copy.
 */
int
pgsnmpdConnID_map(char **mib_pgsnmpdConnID_val_ptr_ptr, size_t *mib_pgsnmpdConnID_val_ptr_len_ptr, char *raw_pgsnmpdConnID_val_ptr, size_t raw_pgsnmpdConnID_val_ptr_len, int allow_realloc)
{
    int converted_len;

    netsnmp_assert(NULL != raw_pgsnmpdConnID_val_ptr);
    netsnmp_assert((NULL != mib_pgsnmpdConnID_val_ptr_ptr) && (NULL != mib_pgsnmpdConnID_val_ptr_len_ptr));
    
    DEBUGMSGTL(("verbose:pgsnmpdConnectionsTable:pgsnmpdConnID_map","called\n"));
    
    /*
     * TODO:241:r: |-> Implement pgsnmpdConnID non-integer mapping
     * it is hard to autogenerate code for mapping types that are not simple
     * integers, so here is an idea of what you might need to do. It will
     * probably need some tweaking to get right.
     */
    /*
     * if the length of the raw data doesn't directly correspond with
     * the length of the mib data, set converted_len to the
     * space required.
     */
    converted_len = raw_pgsnmpdConnID_val_ptr_len; /* assume equal */
    if((NULL == *mib_pgsnmpdConnID_val_ptr_ptr) || (*mib_pgsnmpdConnID_val_ptr_len_ptr < converted_len)) {
        if(! allow_realloc) {
            snmp_log(LOG_ERR,"not enough space for value mapping\n");
            return SNMP_ERR_GENERR;
        }
        *mib_pgsnmpdConnID_val_ptr_ptr = realloc( *mib_pgsnmpdConnID_val_ptr_ptr, converted_len * sizeof(**mib_pgsnmpdConnID_val_ptr_ptr));
        if(NULL == *mib_pgsnmpdConnID_val_ptr_ptr) {
            snmp_log(LOG_ERR,"could not allocate memory\n");
            return SNMP_ERR_GENERR;
        }
    }
    *mib_pgsnmpdConnID_val_ptr_len_ptr = converted_len;
    memcpy( *mib_pgsnmpdConnID_val_ptr_ptr, raw_pgsnmpdConnID_val_ptr, converted_len );

    return MFD_SUCCESS;
} /* pgsnmpdConnID_map */


/**
 * set mib index(es)
 *
 * @param tbl_idx mib index structure
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 *
 * @remark
 *  This convenience function is useful for setting all the MIB index
 *  components with a single function call. It is assume that the C values
 *  have already been mapped from their native/rawformat to the MIB format.
 */
int
pgsnmpdConnectionsTable_indexes_set_tbl_idx(pgsnmpdConnectionsTable_mib_index *tbl_idx, char *pgsnmpdConnID_val_ptr,  size_t pgsnmpdConnID_val_ptr_len)
{
    DEBUGMSGTL(("verbose:pgsnmpdConnectionsTable:pgsnmpdConnectionsTable_indexes_set_tbl_idx","called\n"));

    /* pgsnmpdConnID(1)/DisplayString/ASN_OCTET_STR/char(char)//L/a/w/e/R/d/H */
     tbl_idx->pgsnmpdConnID_len = sizeof(tbl_idx->pgsnmpdConnID);
    /*
     * make sure there is enough space for pgsnmpdConnID data
     */
    if ((NULL == tbl_idx->pgsnmpdConnID) ||
        (tbl_idx->pgsnmpdConnID_len < (pgsnmpdConnID_val_ptr_len * sizeof(tbl_idx->pgsnmpdConnID[0])))) {
        snmp_log(LOG_ERR,"not enough space for value\n");
        return MFD_ERROR;
    }
    tbl_idx->pgsnmpdConnID_len = pgsnmpdConnID_val_ptr_len * sizeof(tbl_idx->pgsnmpdConnID[0]);
    memcpy( tbl_idx->pgsnmpdConnID, pgsnmpdConnID_val_ptr, tbl_idx->pgsnmpdConnID_len );
    

    return MFD_SUCCESS;
} /* pgsnmpdConnectionsTable_indexes_set_tbl_idx */

/**
 * @internal
 * set row context indexes
 *
 * @param reqreq_ctx the row context that needs updated indexes
 *
 * @retval MFD_SUCCESS     : success.
 * @retval MFD_ERROR       : other error.
 *
 * @remark
 *  This function sets the mib indexs, then updates the oid indexs
 *  from the mib index.
 */
int
pgsnmpdConnectionsTable_indexes_set(pgsnmpdConnectionsTable_rowreq_ctx *rowreq_ctx, char *pgsnmpdConnID_val_ptr,  size_t pgsnmpdConnID_val_ptr_len)
{
    DEBUGMSGTL(("verbose:pgsnmpdConnectionsTable:pgsnmpdConnectionsTable_indexes_set","called\n"));

    if(MFD_SUCCESS != pgsnmpdConnectionsTable_indexes_set_tbl_idx(&rowreq_ctx->tbl_idx
                                   , pgsnmpdConnID_val_ptr, pgsnmpdConnID_val_ptr_len
           ))
        return MFD_ERROR;

    /*
     * convert mib index to oid index
     */
    rowreq_ctx->oid_idx.len = sizeof(rowreq_ctx->oid_tmp) / sizeof(oid);
    if(0 != pgsnmpdConnectionsTable_index_to_oid(&rowreq_ctx->oid_idx,
                                    &rowreq_ctx->tbl_idx)) {
        return MFD_ERROR;
    }

    return MFD_SUCCESS;
} /* pgsnmpdConnectionsTable_indexes_set */


/*---------------------------------------------------------------------
 * PGSQL-MIB::pgsnmpdConnectionEntry.pgsnmpdConnHost
 * pgsnmpdConnHost is subid 2 of pgsnmpdConnectionEntry.
 * Its status is Current, and its access level is NoAccess.
 * OID: .1.3.6.1.4.1.27645.1.1.1.1.2
 * Description:
Host name, IP address, or other identifier of the host PostgreSQL server for this connection
 *
 * Attributes:
 *   accessible 0     isscalar 0     enums  0      hasdefval 0
 *   readable   0     iscolumn 1     ranges 1      hashint   1
 *   settable   0
 *   hint: 255a
 *
 * Ranges:  0 - 255;
 *
 * Its syntax is DisplayString (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.  (Max 255)
 *
 *
 *
 * NOTE: NODE pgsnmpdConnHost IS NOT ACCESSIBLE
 *
 *
 */
/*---------------------------------------------------------------------
 * PGSQL-MIB::pgsnmpdConnectionEntry.pgsnmpdConnPort
 * pgsnmpdConnPort is subid 3 of pgsnmpdConnectionEntry.
 * Its status is Current, and its access level is NoAccess.
 * OID: .1.3.6.1.4.1.27645.1.1.1.1.3
 * Description:
PostgreSQL server port number for this connection
 *
 * Attributes:
 *   accessible 0     isscalar 0     enums  0      hasdefval 0
 *   readable   0     iscolumn 1     ranges 1      hashint   1
 *   settable   0
 *   hint: 255a
 *
 * Ranges:  0 - 255;
 *
 * Its syntax is DisplayString (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.  (Max 255)
 *
 *
 *
 * NOTE: NODE pgsnmpdConnPort IS NOT ACCESSIBLE
 *
 *
 */
/*---------------------------------------------------------------------
 * PGSQL-MIB::pgsnmpdConnectionEntry.pgsnmpdConnDbName
 * pgsnmpdConnDbName is subid 4 of pgsnmpdConnectionEntry.
 * Its status is Current, and its access level is NoAccess.
 * OID: .1.3.6.1.4.1.27645.1.1.1.1.4
 * Description:
Name of the database this connection is connected to
 *
 * Attributes:
 *   accessible 0     isscalar 0     enums  0      hasdefval 0
 *   readable   0     iscolumn 1     ranges 1      hashint   1
 *   settable   0
 *   hint: 255a
 *
 * Ranges:  0 - 255;
 *
 * Its syntax is DisplayString (based on perltype OCTETSTR)
 * The net-snmp type is ASN_OCTET_STR. The C type decl is char (char)
 * This data type requires a length.  (Max 255)
 *
 *
 *
 * NOTE: NODE pgsnmpdConnDbName IS NOT ACCESSIBLE
 *
 *
 */


/** @} */
