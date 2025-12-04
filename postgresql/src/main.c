#include "util/debugging.h"
#include "util/logging.h"
#include "dotenv.h"
#include "libpq-fe.h"
#include <stdio.h>
#include <stdlib.h>

#define DB_CONNINFO_SIZE 512
#define SQL_QUERY_BUF    1024
#define DB_PORT "5432"

int main(int argc, char** argv)
{
  ASSERT("ARGS", argc > 1);
  int query_result_limit = atoi(argv[1]);
  char* db_host = NULL;
  char* db_username = NULL;
  char* db_password = NULL;
  char* db_name = NULL;
  
  char conninfo_str[DB_CONNINFO_SIZE] = {0};
  PGconn* conn = NULL;
  ConnStatusType conn_status;
  
  char sql_query_buf[SQL_QUERY_BUF] = {0};
  PGresult* pg_exec_result = NULL;
  ExecStatusType pg_exec_status;
  int pg_exec_result_rows_count = 0;
  int pg_exec_result_cols_count = 0;


  /* load database credential values */
  env_load(".", false);
  db_host = getenv("DB_HOST");
  db_username = getenv("DB_USERNAME");
  db_password = getenv("DB_PASSWORD");
  db_name = getenv("DB_NAME");
  snprintf(
    conninfo_str,
    DB_CONNINFO_SIZE,
    "dbname=%s user=%s password=%s host=%s port=%s",
    db_name, db_username, db_password, db_host, DB_PORT
  );
  DEBUG_LOG("CONNINFO",  "[%s]", conninfo_str);


  /* establish connection with the database server */
  conn = PQconnectdb(conninfo_str);
  ASSERT("PQconnectdb", conn != NULL);
  ERROR_LOG("PQ_ERROR_MESSAGE", "%s", PQerrorMessage(conn));

  conn_status = PQstatus(conn);
  ASSERT("PQstatus", conn_status == CONNECTION_OK);
  INFO_LOG("STATUS", "Successfully connected!");


  /* execute an SQL statement */
  snprintf(
    sql_query_buf,
    SQL_QUERY_BUF,
    "SELECT first_name, email, country_of_birth FROM employees LIMIT %d",
    query_result_limit
  );
  pg_exec_result = PQexec(conn, sql_query_buf);
  ASSERT("PQexec", pg_exec_result != NULL);

  pg_exec_status = PQresultStatus(pg_exec_result);
  ERROR_LOG("PQ_RESULT_STATUS|PQ_RESULT_ERROR_MESSAGE",
    "[%s] msg: %s",
    PQresStatus(pg_exec_status),
    PQresultErrorMessage(pg_exec_result)
  );
  ASSERT("PQresultStatus", pg_exec_status == PGRES_TUPLES_OK);


  pg_exec_result_rows_count = PQntuples(pg_exec_result);
  pg_exec_result_cols_count = PQnfields(pg_exec_result);
  for (int row_idx = 0; row_idx < pg_exec_result_rows_count; ++row_idx) {
    for (int col_idx = 0; col_idx < pg_exec_result_cols_count; ++col_idx) {
      /*INFO_LOG("COL", "[%s]", PQfname(pg_exec_result, col_idx));*/
      /*INFO_LOG("DATA", "(%2d,%2d): [%s]", row_idx, col_idx, PQgetvalue(pg_exec_result, row_idx, col_idx));*/
      printf("%15s: %30s    ", PQfname(pg_exec_result, col_idx), PQgetvalue(pg_exec_result, row_idx, col_idx));
    }
    printf("\n");
  }


  PQclear(pg_exec_result);
  PQfinish(conn);
  return 0;
}

