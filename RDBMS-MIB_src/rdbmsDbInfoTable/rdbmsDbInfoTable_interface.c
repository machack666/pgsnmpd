/*-------------------------------------------------------------------------
 * rdbmsDbInfoTable_interface.c
 *
 *      RDBMSMIB mib module.
 *
 *      Copyright (c) 2004-2007, PostgreSQL Global Development Group
 *      Author: Joshua Tolley
 *
 * $Id: rdbmsDbInfoTable_interface.c,v 1.3 2007/09/13 14:20:43 h-saito Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "pgsnmpd.h"

/* include our parent header */
#include "rdbmsDbInfoTable.h"
#include "rdbmsDbInfoTable_interface.h"

/**********************************************************************
 **********************************************************************
 ***
 *** Table rdbmsDbInfoTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * rdbmsDbInfoTable is subid 2 of rdbmsObjects.
 * Its status is Current.
 * OID: .1.3.6.1.2.1.39.1.2, length: 9
*/
typedef struct rdbmsDbInfoTable_interface_ctx_s {

   netsnmp_container              *container;
   netsnmp_cache                  *cache; /* optional cache */

   rdbmsDbInfoTable_registration_ptr      user_ctx;
   
   netsnmp_table_registration_info  tbl_info;

   netsnmp_baby_steps_access_methods access_multiplexer;

} rdbmsDbInfoTable_interface_ctx;

static rdbmsDbInfoTable_interface_ctx rdbmsDbInfoTable_if_ctx;

static void _rdbmsDbInfoTable_container_init(
    rdbmsDbInfoTable_interface_ctx *if_ctx);


static Netsnmp_Node_Handler _mfd_rdbmsDbInfoTable_pre_request;
static Netsnmp_Node_Handler _mfd_rdbmsDbInfoTable_post_request;
static Netsnmp_Node_Handler _mfd_rdbmsDbInfoTable_object_lookup;
static Netsnmp_Node_Handler _mfd_rdbmsDbInfoTable_get_values;
/**
 * @internal
 * Initialize the table rdbmsDbInfoTable 
 *    (Define its contents and how it's structured)
 */
void
_rdbmsDbInfoTable_initialize_interface(rdbmsDbInfoTable_registration_ptr reg_ptr,  u_long flags)
{
    netsnmp_baby_steps_access_methods *access_multiplexer =
        &rdbmsDbInfoTable_if_ctx.access_multiplexer;
    netsnmp_table_registration_info *tbl_info = &rdbmsDbInfoTable_if_ctx.tbl_info;
    netsnmp_handler_registration *reginfo;
    netsnmp_mib_handler *handler;
    int    mfd_modes = 0;

    DEBUGMSGTL(("internal:rdbmsDbInfoTable:_rdbmsDbInfoTable_initialize_interface","called\n"));


    /*************************************************
     *
     * save interface context for rdbmsDbInfoTable
     */
    /*
     * Setting up the table's definition
     */
    netsnmp_table_helper_add_indexes(tbl_info,
                                  ASN_INTEGER, /** index: rdbmsDbIndex */
                             0);

    /*  Define the minimum and maximum accessible columns.  This
        optimizes retrival. */
    tbl_info->min_column = RDBMSDBINFOTABLE_MIN_COL;
    tbl_info->max_column = RDBMSDBINFOTABLE_MAX_COL;

    /*
     * save users context
     */
    rdbmsDbInfoTable_if_ctx.user_ctx = reg_ptr;

    /*
     * call data access initialization code
     */
    rdbmsDbInfoTable_init_data(reg_ptr);

    /*
     * set up the container
     */
    _rdbmsDbInfoTable_container_init(&rdbmsDbInfoTable_if_ctx);
    if (NULL == rdbmsDbInfoTable_if_ctx.container) {
        snmp_log(LOG_ERR,"could not initialize container for rdbmsDbInfoTable\n");
        return;
    }
    
    /*
     * access_multiplexer: REQUIRED wrapper for get request handling
     */
    access_multiplexer->object_lookup = _mfd_rdbmsDbInfoTable_object_lookup;
    access_multiplexer->get_values = _mfd_rdbmsDbInfoTable_get_values;

    /*
     * no wrappers yet
     */
    access_multiplexer->pre_request = _mfd_rdbmsDbInfoTable_pre_request;
    access_multiplexer->post_request = _mfd_rdbmsDbInfoTable_post_request;


    /*************************************************
     *
     * Create a registration, save our reg data, register table.
     */
    DEBUGMSGTL(("rdbmsDbInfoTable:init_rdbmsDbInfoTable",
                "Registering rdbmsDbInfoTable as a mibs-for-dummies table.\n"));		 
    handler = netsnmp_baby_steps_access_multiplexer_get(access_multiplexer);
    reginfo = netsnmp_handler_registration_create("rdbmsDbInfoTable", handler,
                                                  rdbmsDbInfoTable_oid,
                                                  rdbmsDbInfoTable_oid_size,
                                                  HANDLER_CAN_BABY_STEP |
                                                  HANDLER_CAN_RONLY
                                                  );
    if(NULL == reginfo) {
        snmp_log(LOG_ERR,"error registering table rdbmsDbInfoTable\n");
        return;
    }
    reginfo->my_reg_void = &rdbmsDbInfoTable_if_ctx;

    /*************************************************
     *
     * set up baby steps handler, create it and inject it
     */
    if( access_multiplexer->object_lookup )
        mfd_modes |= BABY_STEP_OBJECT_LOOKUP;
    if( access_multiplexer->set_values )
        mfd_modes |= BABY_STEP_SET_VALUES;
    if( access_multiplexer->irreversible_commit )
        mfd_modes |= BABY_STEP_IRREVERSIBLE_COMMIT;
    if( access_multiplexer->object_syntax_checks )
        mfd_modes |= BABY_STEP_CHECK_OBJECT;

    if( access_multiplexer->pre_request )
        mfd_modes |= BABY_STEP_PRE_REQUEST;
    if( access_multiplexer->post_request )
        mfd_modes |= BABY_STEP_POST_REQUEST;
    
    if( access_multiplexer->undo_setup )
        mfd_modes |= BABY_STEP_UNDO_SETUP;
    if( access_multiplexer->undo_cleanup )
        mfd_modes |= BABY_STEP_UNDO_CLEANUP;
    if( access_multiplexer->undo_sets )
        mfd_modes |= BABY_STEP_UNDO_SETS;
    
    if( access_multiplexer->row_creation )
        mfd_modes |= BABY_STEP_ROW_CREATE;
    if( access_multiplexer->consistency_checks )
        mfd_modes |= BABY_STEP_CHECK_CONSISTENCY;
    if( access_multiplexer->commit )
        mfd_modes |= BABY_STEP_COMMIT;
    if( access_multiplexer->undo_commit )
        mfd_modes |= BABY_STEP_UNDO_COMMIT;
    
    handler = netsnmp_baby_steps_handler_get(mfd_modes);
    netsnmp_inject_handler(reginfo, handler);

    /*************************************************
     *
     * inject row_merge helper with prefix rootoid_len + 2 (entry.col)
     */
    handler = netsnmp_get_row_merge_handler(reginfo->rootoid_len + 2);
    netsnmp_inject_handler(reginfo, handler);

    /*************************************************
     *
     * inject container_table helper
     */
    handler =
        netsnmp_container_table_handler_get(tbl_info,
                                            rdbmsDbInfoTable_if_ctx.container,
                                            TABLE_CONTAINER_KEY_NETSNMP_INDEX);
    netsnmp_inject_handler( reginfo, handler );

    /*************************************************
     *
     * inject cache helper
     */
    if(NULL != rdbmsDbInfoTable_if_ctx.cache) {
        handler = netsnmp_cache_handler_get(rdbmsDbInfoTable_if_ctx.cache);
        netsnmp_inject_handler( reginfo, handler );
    }

    /*
     * register table
     */
    netsnmp_register_table(reginfo, tbl_info);
} /* _rdbmsDbInfoTable_initialize_interface */

void
rdbmsDbInfoTable_valid_columns_set(netsnmp_column_info *vc)
{
    rdbmsDbInfoTable_if_ctx.tbl_info.valid_columns = vc;
} /* rdbmsDbInfoTable_valid_columns_set */

/**
 * @internal
 * convert the index component stored in the context to an oid
 */
int
rdbmsDbInfoTable_index_to_oid(netsnmp_index *oid_idx,
                         rdbmsDbInfoTable_mib_index *mib_idx)
{
    int err = SNMP_ERR_NOERROR;
    
    /*
     * temp storage for parsing indexes
     */
    /*
     * rdbmsDbIndex(1)/INTEGER/ASN_INTEGER/long(long)//l/a/w/e/R/d/h
     */
    netsnmp_variable_list var_rdbmsDbIndex;

    /*
     * set up varbinds
     */
    memset( &var_rdbmsDbIndex, 0x00, sizeof(var_rdbmsDbIndex) );
    var_rdbmsDbIndex.type = ASN_INTEGER;

    /*
     * chain temp index varbinds together
     */
    var_rdbmsDbIndex.next_variable =  NULL;


    DEBUGMSGTL(("verbose:rdbmsDbInfoTable:rdbmsDbInfoTable_index_to_oid","called\n"));

        /* rdbmsDbIndex(1)/INTEGER/ASN_INTEGER/long(long)//l/a/w/e/R/d/h */
    snmp_set_var_value(&var_rdbmsDbIndex, (u_char*)&mib_idx->rdbmsDbIndex,
                       sizeof(mib_idx->rdbmsDbIndex));


    err = build_oid_noalloc(oid_idx->oids, oid_idx->len, (size_t *)&oid_idx->len,
                           NULL, 0, &var_rdbmsDbIndex);
    if(err)
        snmp_log(LOG_ERR,"error %d converting index to oid\n", err);

    /*
     * parsing may have allocated memory. free it.
     */
    snmp_reset_var_buffers( &var_rdbmsDbIndex );

    return err;
} /* rdbmsDbInfoTable_index_to_oid */

/**
 * extract rdbmsDbInfoTable indexes from a netsnmp_index
 *
 * @retval SNMP_ERR_NOERROR  : no error
 * @retval SNMP_ERR_GENERR   : error
 */
int
rdbmsDbInfoTable_index_from_oid(netsnmp_index *oid_idx,
                         rdbmsDbInfoTable_mib_index *mib_idx)
{
    int err = SNMP_ERR_NOERROR;
    
    /*
     * temp storage for parsing indexes
     */
    /*
     * rdbmsDbIndex(1)/INTEGER/ASN_INTEGER/long(long)//l/a/w/e/R/d/h
     */
    netsnmp_variable_list var_rdbmsDbIndex;

    /*
     * set up varbinds
     */
    memset( &var_rdbmsDbIndex, 0x00, sizeof(var_rdbmsDbIndex) );
    var_rdbmsDbIndex.type = ASN_INTEGER;

    /*
     * chain temp index varbinds together
     */
    var_rdbmsDbIndex.next_variable =  NULL;


    DEBUGMSGTL(("verbose:rdbmsDbInfoTable:rdbmsDbInfoTable_index_from_oid","called\n"));

    /*
     * parse the oid into the individual index components
     */
    err = parse_oid_indexes( oid_idx->oids, oid_idx->len,
                             &var_rdbmsDbIndex );
    if (err == SNMP_ERR_NOERROR) {
        /*
         * copy out values
         */
    mib_idx->rdbmsDbIndex = *((long *)var_rdbmsDbIndex.val.string);


    }

    /*
     * parsing may have allocated memory. free it.
     */
    snmp_reset_var_buffers( &var_rdbmsDbIndex );

    return err;
} /* rdbmsDbInfoTable_index_from_oid */


/* *********************************************************************
 * @internal
 * allocate resources for a rdbmsDbInfoTable_rowreq_ctx
 */
rdbmsDbInfoTable_rowreq_ctx *
rdbmsDbInfoTable_allocate_rowreq_ctx(void)
{
    rdbmsDbInfoTable_rowreq_ctx *rowreq_ctx =
                  SNMP_MALLOC_TYPEDEF(rdbmsDbInfoTable_rowreq_ctx);

    DEBUGMSGTL(("internal:rdbmsDbInfoTable:rdbmsDbInfoTable_allocate_rowreq_ctx","called\n"));

    if(NULL == rowreq_ctx) {
        snmp_log(LOG_ERR,"Couldn't allocate memory for a "
                 "rdbmsDbInfoTable_rowreq_ctx.\n");
    }

    rowreq_ctx->oid_idx.oids = rowreq_ctx->oid_tmp;

    rowreq_ctx->rdbmsDbInfoTable_data_list = NULL;
    rowreq_ctx->rdbmsDbInfoTable_reg = rdbmsDbInfoTable_if_ctx.user_ctx;


    return rowreq_ctx;
} /* rdbmsDbInfoTable_allocate_rowreq_ctx */

/*
 * @internal
 * release resources for a rdbmsDbInfoTable_rowreq_ctx
 */
void
rdbmsDbInfoTable_release_rowreq_ctx(rdbmsDbInfoTable_rowreq_ctx *rowreq_ctx)
{
    DEBUGMSGTL(("internal:rdbmsDbInfoTable:rdbmsDbInfoTable_release_rowreq_ctx","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);
    

    /*
     * free index oid pointer
     */
    if(rowreq_ctx->oid_idx.oids != rowreq_ctx->oid_tmp)
        free(rowreq_ctx->oid_idx.oids);

    SNMP_FREE(rowreq_ctx);
} /* rdbmsDbInfoTable_release_rowreq_ctx */

/**
 * @internal
 * wrapper
 */
static int
_mfd_rdbmsDbInfoTable_pre_request(netsnmp_mib_handler *handler,
                            netsnmp_handler_registration *reginfo,
                            netsnmp_agent_request_info *agtreq_info,
                            netsnmp_request_info *requests)
{
    int rc = rdbmsDbInfoTable_pre_request(rdbmsDbInfoTable_if_ctx.user_ctx);
    if (MFD_SUCCESS != rc) {
        /*
         * nothing we can do about it but log it
         */
        DEBUGMSGTL(("internal:rdbmsDbInfoTable","error %d from "
                    "rdbmsDbInfoTable_pre_request\n", rc));
        netsnmp_request_set_error_all(requests, SNMP_VALIDATE_ERR(rc));
    }
    
    return SNMP_ERR_NOERROR;
} /* _mfd_rdbmsDbInfoTable_pre_request */

/**
 * @internal
 * wrapper
 */
static int
_mfd_rdbmsDbInfoTable_post_request(netsnmp_mib_handler *handler,
                             netsnmp_handler_registration *reginfo,
                             netsnmp_agent_request_info *agtreq_info,
                             netsnmp_request_info *requests)
{
    rdbmsDbInfoTable_rowreq_ctx *rowreq_ctx;
    int rc = rdbmsDbInfoTable_post_request(rdbmsDbInfoTable_if_ctx.user_ctx);
    if (MFD_SUCCESS != rc) {
        /*
         * nothing we can do about it but log it
         */
        DEBUGMSGTL(("internal:rdbmsDbInfoTable","error %d from "
                    "rdbmsDbInfoTable_post_request\n", rc));
    }
    
    /*
     * if there are no errors, check for and handle row creation/deletion
     */
    rc = netsnmp_check_requests_error(requests);
    if ((SNMP_ERR_NOERROR == rc) &&
        (NULL !=
         (rowreq_ctx = netsnmp_container_table_row_extract(requests)))) {
        if (rowreq_ctx->rowreq_flags & MFD_ROW_CREATED) {
            rowreq_ctx->rowreq_flags &= ~MFD_ROW_CREATED;
            CONTAINER_INSERT(rdbmsDbInfoTable_if_ctx.container, rowreq_ctx);
        }
        else if (rowreq_ctx->rowreq_flags & MFD_ROW_DELETED) {
            CONTAINER_REMOVE(rdbmsDbInfoTable_if_ctx.container, rowreq_ctx);
            rdbmsDbInfoTable_release_rowreq_ctx(rowreq_ctx);
        }
    }

    return SNMP_ERR_NOERROR;
} /* _mfd_rdbmsDbInfoTable_post_request */

/**
 * @internal
 * wrapper
 */
static int
_mfd_rdbmsDbInfoTable_object_lookup(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *agtreq_info,
                         netsnmp_request_info *requests)
{
    rdbmsDbInfoTable_rowreq_ctx *rowreq_ctx =
                  netsnmp_container_table_row_extract(requests);
    
    DEBUGMSGTL(("internal:rdbmsDbInfoTable:_mfd_rdbmsDbInfoTable_object_lookup","called\n"));

    /*
     * get our context from mfd
     * rdbmsDbInfoTable_interface_ctx *if_ctx =
     *             (rdbmsDbInfoTable_interface_ctx *)reginfo->my_reg_void;
     */

    if(NULL == rowreq_ctx) {
        netsnmp_request_set_error_all(requests, SNMP_ERR_NOCREATION);
    }
    else {
        rdbmsDbInfoTable_row_prep(rowreq_ctx);
    }

    return SNMP_ERR_NOERROR;
} /* _mfd_rdbmsDbInfoTable_object_lookup */

/***********************************************************************
 *
 * GET processing
 *
 ***********************************************************************/
/*
 * @internal
 * Retrieve the value for a particular column
 */
NETSNMP_STATIC_INLINE int
_rdbmsDbInfoTable_get_column( rdbmsDbInfoTable_rowreq_ctx *rowreq_ctx,
                       netsnmp_variable_list *var, int column )
{
    int rc = SNMPERR_SUCCESS;
    
    DEBUGMSGTL(("internal:rdbmsDbInfoTable:_mfd_rdbmsDbInfoTable_get_column","called\n"));


    netsnmp_assert(NULL != rowreq_ctx);

    switch(column) {

    /* rdbmsDbInfoProductName(1)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H */
    case COLUMN_RDBMSDBINFOPRODUCTNAME:
    var->type = ASN_OCTET_STR;
rc = rdbmsDbInfoProductName_get(rowreq_ctx, (char **)&var->val.string, &var->val_len );
        break;

    /* rdbmsDbInfoVersion(2)/DisplayString/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H */
    case COLUMN_RDBMSDBINFOVERSION:
    var->type = ASN_OCTET_STR;
rc = rdbmsDbInfoVersion_get(rowreq_ctx, (char **)&var->val.string, &var->val_len );
        break;

    /* rdbmsDbInfoSizeUnits(3)/INTEGER/ASN_INTEGER/long(u_long)//l/A/w/E/r/d/h */
    case COLUMN_RDBMSDBINFOSIZEUNITS:
    var->val_len = sizeof(u_long);
    var->type = ASN_INTEGER;
rc = rdbmsDbInfoSizeUnits_get(rowreq_ctx, (u_long *)var->val.string );
        break;

    /* rdbmsDbInfoSizeAllocated(4)/INTEGER/ASN_INTEGER/long(long)//l/A/W/e/R/d/h */
    case COLUMN_RDBMSDBINFOSIZEALLOCATED:
    var->val_len = sizeof(long);
    var->type = ASN_INTEGER;
rc = rdbmsDbInfoSizeAllocated_get(rowreq_ctx, (long *)var->val.string );
        break;

    /* rdbmsDbInfoSizeUsed(5)/INTEGER/ASN_INTEGER/long(long)//l/A/w/e/R/d/h */
    case COLUMN_RDBMSDBINFOSIZEUSED:
    var->val_len = sizeof(long);
    var->type = ASN_INTEGER;
rc = rdbmsDbInfoSizeUsed_get(rowreq_ctx, (long *)var->val.string );
        break;

    /* rdbmsDbInfoLastBackup(6)/DateAndTime/ASN_OCTET_STR/char(char)//L/A/w/e/R/d/H */
    case COLUMN_RDBMSDBINFOLASTBACKUP:
    var->type = ASN_OCTET_STR;
rc = rdbmsDbInfoLastBackup_get(rowreq_ctx, (char **)&var->val.string, &var->val_len );
        break;

     default:
         snmp_log(LOG_ERR,"unknown column %d in _rdbmsDbInfoTable_get_column\n", column);
         break;
    }

    return rc;
} /* _rdbmsDbInfoTable_get_column */

int
_mfd_rdbmsDbInfoTable_get_values(netsnmp_mib_handler *handler,
                         netsnmp_handler_registration *reginfo,
                         netsnmp_agent_request_info *agtreq_info,
                         netsnmp_request_info *requests)
{
    rdbmsDbInfoTable_rowreq_ctx *rowreq_ctx =
                  netsnmp_container_table_row_extract(requests);
    netsnmp_table_request_info * tri;
    u_char                     * old_string;
    void                      (*dataFreeHook)(void *);
    int                        rc;

    DEBUGMSGTL(("internal:rdbmsDbInfoTable:_mfd_rdbmsDbInfoTable_get_values","called\n"));

    netsnmp_assert(NULL != rowreq_ctx);
    
    for(;requests; requests = requests->next) {
        /*
         * save old pointer, so we can free it if replaced
         */
        old_string = requests->requestvb->val.string;
        dataFreeHook = requests->requestvb->dataFreeHook;
        if(NULL == requests->requestvb->val.string) {
            requests->requestvb->val.string = requests->requestvb->buf;
            requests->requestvb->val_len = sizeof(requests->requestvb->buf);
        }
        else if(requests->requestvb->buf == requests->requestvb->val.string) {
            if(requests->requestvb->val_len != sizeof(requests->requestvb->buf))
                requests->requestvb->val_len = sizeof(requests->requestvb->buf);
        }

        /*
         * get column data
         */
        tri = netsnmp_extract_table_info(requests);
        if(NULL == tri)
            continue;
        
        rc = _rdbmsDbInfoTable_get_column(rowreq_ctx, requests->requestvb, tri->colnum);
        if(rc) {
            if(MFD_SKIP == rc) {
                requests->requestvb->type = ASN_PRIV_RETRY;
                rc = SNMP_ERR_NOERROR;
            }
        }
        else if (NULL == requests->requestvb->val.string) {
            snmp_log(LOG_ERR,"NULL varbind data pointer!\n");
            rc = SNMP_ERR_GENERR;
        }
        if(rc)
            netsnmp_request_set_error(requests, SNMP_VALIDATE_ERR(rc));

        /*
         * if the buffer wasn't used previously for the old data (i.e. it
         * was allcoated memory)  and the get routine replaced the pointer,
         * we need to free the previous pointer.
         */
        if(old_string && (old_string != requests->requestvb->buf) &&
           (requests->requestvb->val.string != old_string)) {
            if(dataFreeHook)
                (*dataFreeHook)(old_string);
            else
                free(old_string);
        }
    } /* for results */

    return SNMP_ERR_NOERROR;
} /* _mfd_rdbmsDbInfoTable_get_values */

/***********************************************************************
 *
 * SET processing
 *
 ***********************************************************************/

/*
 * NOT APPLICABLE (per MIB or user setting)
 */
/***********************************************************************
 *
 * DATA ACCESS
 *
 ***********************************************************************/
/**
 * @internal
 */
static int
_cache_load(netsnmp_cache *cache, void *vmagic)
{
    DEBUGMSGTL(("internal:rdbmsDbInfoTable:_cache_load","called\n"));

    if((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR, "invalid cache for rdbmsDbInfoTable_cache_load\n");
        return -1;
    }

    /** should only be called for an invalid or expired cache */
    netsnmp_assert((0 == cache->valid) || (1 == cache->expired));
    
    /*
     * call user code
     */
    return rdbmsDbInfoTable_cache_load((netsnmp_container*)cache->magic);
} /* _cache_load */

/**
 * @internal
 */
static void
_cache_item_free(rdbmsDbInfoTable_rowreq_ctx *rowreq_ctx, void *context)
{
    DEBUGMSGTL(("internal:rdbmsDbInfoTable:_cache_item_free","called\n"));

    if(NULL == rowreq_ctx)
        return;

    rdbmsDbInfoTable_release_rowreq_ctx(rowreq_ctx);
} /* _cache_item_free */

/**
 * @internal
 */
static void
_cache_free(netsnmp_cache *cache, void *magic)
{
    netsnmp_container *container;

    DEBUGMSGTL(("internal:rdbmsDbInfoTable:_cache_free","called\n"));

    if((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR, "invalid cache in rdbmsDbInfoTable_cache_free\n");
        return;
    }

    container = (netsnmp_container*)cache->magic;

    /*
     * call user code
     */
    rdbmsDbInfoTable_cache_free(container);
    
    /*
     * free all items. inefficient, but easy.
     */
    CONTAINER_CLEAR(container,
                    (netsnmp_container_obj_func *)_cache_item_free,
                    NULL);
} /* _cache_free */

/**
 * @internal
 * initialize the iterator container with functions or wrappers
 */
void
_rdbmsDbInfoTable_container_init(rdbmsDbInfoTable_interface_ctx *if_ctx)
{
    DEBUGMSGTL(("internal:rdbmsDbInfoTable:_rdbmsDbInfoTable_container_init","called\n"));
    
    /*
     * set up the cache
     */
    if_ctx->cache = netsnmp_cache_create(30, /* timeout in seconds */
                                         _cache_load, _cache_free,
                                         rdbmsDbInfoTable_oid,
                                         rdbmsDbInfoTable_oid_size);

    if(NULL == if_ctx->cache) {
        snmp_log(LOG_ERR, "error creating cache for rdbmsDbInfoTable\n");
        return;
    }

    if_ctx->cache->flags = NETSNMP_CACHE_DONT_INVALIDATE_ON_SET;

    rdbmsDbInfoTable_container_init(&if_ctx->container, if_ctx->cache);
    if(NULL == if_ctx->container)
        if_ctx->container = netsnmp_container_find("rdbmsDbInfoTable:table_container");
    if(NULL == if_ctx->container) {
        snmp_log(LOG_ERR,"error creating container in "
                 "rdbmsDbInfoTable_container_init\n");
        return;
    }
    if_ctx->cache->magic = (void*)if_ctx->container;
} /* _rdbmsDbInfoTable_container_init */

