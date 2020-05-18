/*
 * @Author: your name
 * @Date: 2020-05-18 09:48:55
 * @LastEditTime: 2020-05-18 18:33:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /try/src/mysql/test.cpp
 */ 
#include <mysql/mysql.h>
#include <stdio.h>

using namespace std;


int main()
{
    MYSQL conn;
    int res = 0;
    MYSQL_RES *result = NULL;
    MYSQL_FIELD *field = NULL;
    mysql_init(&conn);
    if (mysql_real_connect(&conn, "localhost", "jack", "lovezrj", "day_list_user",0,NULL, CLIENT_FOUND_ROWS))
    {
        printf("connect success!\n");
        res = mysql_query(&conn, "select cipher from User where account = '1129075520'");
        if (res)
            printf("error!\n");
        else 
            printf("ok.\n");
        result = mysql_store_result(&conn);
        int rowcount = mysql_num_rows(result);
        int fieldcount = mysql_num_fields(result);
        for (int i = 0; i < fieldcount; ++i)
        {
            field = mysql_fetch_field_direct(result,i);
            printf("%s\n", field -> name);
        }
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row(result);
        /*while (row)
        {
            for (int i=0; i<fieldcount; i++)
                printf("%s", row[i]);
            printf("\n");
            row = mysql_fetch_row(result);
        }*/
        printf("%s", row[0]);
        mysql_close(&conn);
    }
}