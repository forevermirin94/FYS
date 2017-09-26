//runnen met
//gcc -o request $(mysql_config --cflags) request.c $(mysql_config --libs) -std=gnu99
#include <my_global.h>
#include <mysql.h>

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con)); //foutmelding voor mmysql connectie
  mysql_close(con);
  exit(1);
}

int main(int argc, char **argv)
{
  MYSQL *con = mysql_init(NULL); // mysql verbinding maken

  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }

  if (mysql_real_connect(con, "get.rekt.info", "upload", "IT_101-FYS","Project_FYS", 0, NULL, 0) == NULL)
  {finish_with_error(con);}

  if (mysql_query(con, "SELECT (Employee_id) FROM Employee")){finish_with_error(con);} // gewoon een select query

  MYSQL_RES *result = mysql_store_result(con);// opgehaalde data  in results storen

  if (result == NULL){finish_with_error(con);}

    //We get the result set using the mysql_store_result() function. The MYSQL_RES is a structure for holding a result set.
    //geen zin om dit te vertalen naar NL

  int num_fields = mysql_num_fields(result); // We get the number of fields (columns) in the table.
  const char* rfidTags[num_fields]; // string array declararen

    //We fetch the rows and print them to the screen. en slaat het op in string array

  MYSQL_ROW row;

  while ((row = mysql_fetch_row(result)))
  {
      for(int i = 0; i < num_fields; i++)
      {
          printf("%s ", row[i] ? row[i] : "NULL");
                  rfidTags[i] = row[i];//opslaan in rfidtag arrays
      }
          printf("\n");

	// for(int a = 0; a < num_fields; a++){
	// printf("%s", rfidTags[a],"\n");}// testen van array: rfidtags
  }
  mysql_free_result(result);
  mysql_close(con);

  exit(0);
}