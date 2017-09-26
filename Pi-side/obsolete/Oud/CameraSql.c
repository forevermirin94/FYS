//ook hier, waar is m'n about en waar zijn m'n compiler arguments?

#include <mysql.h>
#include <stdio.h>
#include <my_global.h>

main()
{
	MYSQL *conn;

        char	*server 	= "get.rekt.info";
        char	*user 		= "upload";
        char	*password 	= "jemoeder123";
        char	*database 	= "Project_FYS";

	// Make the connection to the Mysql-database on server.
	conn = mysql_init(NULL);
 	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) 
	{
      		fprintf(stderr, "%s\n", mysql_error(conn));
      		exit(1);
   	}

   	//waarden injecten in de table camera
   	if (mysql_query(conn, "INSERT INTO Camera VALUES(1,20151843)" )!=0)
      		exit(1);

   	//sluit de verbinding met de database
   	mysql_close(conn);
}


