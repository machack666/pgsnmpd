/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf 15999 2007-03-25 22:32:02Z dts12 $
 */

#include "pgsnmpd.h"
#include "pgsqlPgAmopTable.h"

    /* TODO: if this works, move it to pgsnmpd.h */
#define CONNIDLEN 113
    /* Typical data structure for a row entry */
struct pgsqlPgAmopTable_entry {
    /* Index values */
    u_char pgsnmpdConnID[CONNIDLEN];
    size_t pgsnmpdConnID_len;
    long rdbmsDbIndex;
    long pgsqlPgAmopEntryOID;

    /* Column values */
    /*long pgsqlPgAmopEntryOID; */
    long pgsqlPgAmopAmopfamily;
    long pgsqlPgAmopAmoplefttype;
    long pgsqlPgAmopAmoprighttype;
    long pgsqlPgAmopAmopstrategy;
    long pgsqlPgAmopAmopreqcheck;
    long pgsqlPgAmopAmopopr;
    long pgsqlPgAmopAmopmethod;

    /* Illustrate using a simple linked list */
    int   valid;
    struct pgsqlPgAmopTable_entry *next;
};

struct pgsqlPgAmopTable_entry  *pgsqlPgAmopTable_head;

struct pgsqlPgAmopTable_entry *pgsqlPgAmopTable_createEntry(
                 char* pgsnmpdConnID,
                 size_t pgsnmpdConnID_len,
                 long  rdbmsDbIndex,
                 long  pgsqlPgAmopEntryOID
                );

void
pgsqlPgAmopTable_removeEntry( struct pgsqlPgAmopTable_entry *entry );

/** Initializes the pgsqlPgAmopTable module */
void
init_pgsqlPgAmopTable(void)
{
  /* here we initialize all the tables we're planning on supporting */
    initialize_table_pgsqlPgAmopTable();
}

void fill_pgsqlPgAmopTable(void) {
    PGresult *pg_db_query;
    int resultCount, i;
    struct pgsqlPgAmopTable_entry *entry;
    char pgsnmpdConnID[112] = "Default conn ID";
    size_t pgsnmpdConnID_len = strlen(pgsnmpdConnID) + 1;
    long rdbmsDbIndex = 1;

    snmp_log(LOG_INFO, "Initializing pgsqlPgAmopTable\n");
    if (PQstatus(dbconn) == CONNECTION_OK) {
        pg_db_query = PQexec(dbconn, "SELECT oid::INTEGER, amopfamily::INTEGER, amoplefttype::INTEGER, amoprighttype::INTEGER, amopstrategy, amopreqcheck, amopopr::INTEGER , amopmethod::INTEGER FROM pg_amop ORDER BY 1 ASC LIMIT 3");
    }
    else {
        snmp_log(LOG_ERR, "Can't get connected to database\n");
        return;
        /* TODO: I should signal MFD_RESOURCE_UNAVAILABLE somehow, here */
    }

    if (PQresultStatus(pg_db_query) != PGRES_TUPLES_OK) {
        snmp_log(LOG_ERR, "Didn't get any results from the database\n");
        PQclear(pg_db_query);
        return;
        /* TODO: I should signal MFD_RESOURCE_UNAVAILABLE somehow, here */
    }
    resultCount = PQntuples(pg_db_query);
    for (i = 0; i < resultCount; i++) {
        entry = pgsqlPgAmopTable_createEntry(pgsnmpdConnID, pgsnmpdConnID_len,
                 rdbmsDbIndex, atol(PQgetvalue(pg_db_query, i, 0)));
        entry->pgsqlPgAmopAmopfamily    = atol(PQgetvalue(pg_db_query, i, 1));
        entry->pgsqlPgAmopAmoplefttype  = atol(PQgetvalue(pg_db_query, i, 2));
        entry->pgsqlPgAmopAmoprighttype = atol(PQgetvalue(pg_db_query, i, 3));
        entry->pgsqlPgAmopAmopstrategy  = atoi(PQgetvalue(pg_db_query, i, 4));
        entry->pgsqlPgAmopAmopreqcheck  = atoi(PQgetvalue(pg_db_query, i, 5));
        entry->pgsqlPgAmopAmopopr       = atol(PQgetvalue(pg_db_query, i, 6));
        entry->pgsqlPgAmopAmopmethod    = atol(PQgetvalue(pg_db_query, i, 7));
    }
    snmp_log(LOG_INFO, "Finished initializing pgsqlPgAmopTable with %d entries\n", resultCount);
}

/* TODO # Determine the first/last column names */

/** Initialize the pgsqlPgAmopTable table by defining its contents and how it's structured */
void
initialize_table_pgsqlPgAmopTable(void)
{
    static oid pgsqlPgAmopTable_oid[] = {1,3,6,1,4,1,27645,1,2,3};
    size_t pgsqlPgAmopTable_oid_len   = OID_LENGTH(pgsqlPgAmopTable_oid);
    netsnmp_handler_registration    *reg;
    netsnmp_iterator_info           *iinfo;
    netsnmp_table_registration_info *table_info;
    struct pgsqlPgAmopTable_entry *entry;
    char *connID = "Default conn ID\0";
    int conn_len = strlen(connID);

    reg = netsnmp_create_handler_registration(
              "pgsqlPgAmopTable",     pgsqlPgAmopTable_handler,
              pgsqlPgAmopTable_oid, pgsqlPgAmopTable_oid_len,
              HANDLER_CAN_RONLY
              );

    table_info = SNMP_MALLOC_TYPEDEF( netsnmp_table_registration_info );
    netsnmp_table_helper_add_indexes(table_info,
                           ASN_OCTET_STR,  /* index: pgsnmpdConnID */
                           ASN_INTEGER,  /* index: rdbmsDbIndex */
                           ASN_INTEGER,  /* index: pgsqlPgAmopEntryOID */
                           0);
    table_info->min_column = COLUMN_PGSQLPGAMOPENTRYOID;
    table_info->max_column = COLUMN_PGSQLPGAMOPAMOPMETHOD;
    
    iinfo = SNMP_MALLOC_TYPEDEF( netsnmp_iterator_info );
    iinfo->get_first_data_point = pgsqlPgAmopTable_get_first_data_point;
    iinfo->get_next_data_point  = pgsqlPgAmopTable_get_next_data_point;
    iinfo->table_reginfo        = table_info;
    
    netsnmp_register_table_iterator( reg, iinfo );

    /* Initialise the contents of the table here */
    fill_pgsqlPgAmopTable();
}

/* create a new row in the (unsorted) table */
struct pgsqlPgAmopTable_entry *
pgsqlPgAmopTable_createEntry(
                 char* pgsnmpdConnID,
                 size_t pgsnmpdConnID_len,
                 long  rdbmsDbIndex,
                 long  pgsqlPgAmopEntryOID
                ) {
    struct pgsqlPgAmopTable_entry *entry;

    entry = SNMP_MALLOC_TYPEDEF(struct pgsqlPgAmopTable_entry);
    if (!entry)
        return NULL;

    memset(entry->pgsnmpdConnID, 0, CONNIDLEN);
    memcpy(entry->pgsnmpdConnID, pgsnmpdConnID, pgsnmpdConnID_len);
    entry->pgsnmpdConnID_len = pgsnmpdConnID_len;
    entry->rdbmsDbIndex = rdbmsDbIndex;
    entry->pgsqlPgAmopEntryOID = pgsqlPgAmopEntryOID;
    entry->next = pgsqlPgAmopTable_head;
    pgsqlPgAmopTable_head = entry;
    return entry;
}

/* remove a row from the table */
void
pgsqlPgAmopTable_removeEntry( struct pgsqlPgAmopTable_entry *entry ) {
    struct pgsqlPgAmopTable_entry *ptr, *prev;

    if (!entry)
        return;    /* Nothing to remove */

    for ( ptr  = pgsqlPgAmopTable_head, prev = NULL;
          ptr != NULL;
          prev = ptr, ptr = ptr->next ) {
        if ( ptr == entry )
            break;
    }
    if ( !ptr )
        return;    /* Can't find it */

    if ( prev == NULL )
        pgsqlPgAmopTable_head = ptr->next;
    else
        prev->next = ptr->next;

    SNMP_FREE( entry );   /* XXX - release any other internal resources */
}


/* Example iterator hook routines - using 'get_next' to do most of the work */
netsnmp_variable_list *
pgsqlPgAmopTable_get_first_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    *my_loop_context = pgsqlPgAmopTable_head;
    snmp_log(LOG_INFO, "*** get_first called\n");
    return pgsqlPgAmopTable_get_next_data_point(my_loop_context, my_data_context,
                                    put_index_data,  mydata );
}

netsnmp_variable_list *
pgsqlPgAmopTable_get_next_data_point(void **my_loop_context,
                          void **my_data_context,
                          netsnmp_variable_list *put_index_data,
                          netsnmp_iterator_info *mydata)
{
    struct pgsqlPgAmopTable_entry *entry = (struct pgsqlPgAmopTable_entry *)*my_loop_context;
    netsnmp_variable_list *idx = put_index_data;

    snmp_log(LOG_INFO, "*** get_next called\n");
    if ( entry ) {
        snmp_set_var_value( idx, entry->pgsnmpdConnID, sizeof(entry->pgsnmpdConnID) );
        idx = idx->next_variable;
        snmp_set_var_typed_integer( idx, ASN_INTEGER, entry->rdbmsDbIndex );
        idx = idx->next_variable;
        snmp_set_var_typed_integer( idx, ASN_INTEGER, entry->pgsqlPgAmopEntryOID );
        idx = idx->next_variable;
        *my_data_context = (void *)entry;
        *my_loop_context = (void *)entry->next;
        return put_index_data;
    } else {
        return NULL;
    }
}


/** handles requests for the pgsqlPgAmopTable table */
int
pgsqlPgAmopTable_handler(
    netsnmp_mib_handler               *handler,
    netsnmp_handler_registration      *reginfo,
    netsnmp_agent_request_info        *reqinfo,
    netsnmp_request_info              *requests) {

    netsnmp_request_info       *request;
    netsnmp_table_request_info *table_info;
    struct pgsqlPgAmopTable_entry          *table_entry;

    snmp_log(LOG_INFO, "Handler called\n");
    switch (reqinfo->mode) {
        /*
         * Read-support (also covers GetNext requests)
         */
    case MODE_GET:
        for (request=requests; request; request=request->next) {
            table_entry = (struct pgsqlPgAmopTable_entry *)
                              netsnmp_extract_iterator_context(request);
            table_info  =     netsnmp_extract_table_info(      request);
    
            switch (table_info->colnum) {
            case COLUMN_PGSQLPGAMOPENTRYOID:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->pgsqlPgAmopEntryOID);
                break;
            case COLUMN_PGSQLPGAMOPAMOPFAMILY:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->pgsqlPgAmopAmopfamily);
                break;
            case COLUMN_PGSQLPGAMOPAMOPLEFTTYPE:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->pgsqlPgAmopAmoplefttype);
                break;
            case COLUMN_PGSQLPGAMOPAMOPRIGHTTYPE:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->pgsqlPgAmopAmoprighttype);
                break;
            case COLUMN_PGSQLPGAMOPAMOPSTRATEGY:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->pgsqlPgAmopAmopstrategy);
                break;
            case COLUMN_PGSQLPGAMOPAMOPREQCHECK:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->pgsqlPgAmopAmopreqcheck);
                break;
            case COLUMN_PGSQLPGAMOPAMOPOPR:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->pgsqlPgAmopAmopopr);
                break;
            case COLUMN_PGSQLPGAMOPAMOPMETHOD:
                if ( !table_entry ) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
                snmp_set_var_typed_integer( request->requestvb, ASN_INTEGER,
                                            table_entry->pgsqlPgAmopAmopmethod);
                break;
            default:
                netsnmp_set_request_error(reqinfo, request,
                                          SNMP_NOSUCHOBJECT);
                break;
            }
        }
        break;

    }
    return SNMP_ERR_NOERROR;
}
