/*
sudo apt-get install libmysqlclient-dev
sudo gcc -o basicsql $(mysql_config --cflags) basicsql.c $(mysql_config --libs)
*/

#include <mysql.h>
#include <stdio.h>
#include <my_global.h>


main() {
   MYSQL *conn;

        char *server = "get.rekt.info";
        char *user = "upload";
        char *password = "jemoeder123";
        char *database = "Project_FYS";

   // Make the connection to the Mysql-database on server.
   conn = mysql_init(NULL);
   if (!mysql_real_connect(conn, server,
         user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }

   //een insert into query : insert into tabelnaam VALUES(kolomnaam1, kolomnaam2, etc... )
   if (mysql_query(conn, "INSERT INTO Camera VALUES(1,20151843)" )!=0)
      {exit(1);}

   /* close connection */
   mysql_close(conn);
}