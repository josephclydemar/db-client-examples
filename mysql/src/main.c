#include "util/debugging.h"
#include "dotenv.h"
#include "mysql.h"

int main()
{
  env_load(".", false);
  char* db_host = getenv("DB_HOST");
  char* db_username = getenv("DB_USERNAME");
  char* db_password = getenv("DB_PASSWORD");
  char* db_name = getenv("DB_NAME");

  MYSQL* conn;
  MYSQL_RES* res;
  
  conn = mysql_init(NULL);
  ASSERT("MYSQL_INIT", conn != NULL);

  ASSERT("MYSQL_REAL_CONNECT",
    mysql_real_connect(
    conn,
    db_host,
    db_username,
    db_password,
    db_name,
    0,
    NULL,
    0
  ) == NULL);

  ASSERT("MYSQL_QUERY", mysql_query(conn, "SELECT first_name FROM users") == 0);

  res = mysql_store_result(conn);
  ASSERT("MYSQL_STORE_RESULT", res != NULL);

  mysql_free_result(res);
  mysql_close(conn);
  return 0;
}
