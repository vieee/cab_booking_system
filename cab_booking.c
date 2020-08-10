#include <stdio.h>
#include <mysql/mysql.h>
#include<string.h>

float fare(char name_pl[],char name_des[],char name_car[]);

float fare(char name_pl[],char name_des[],char name_car[])//Calculating fare 
{   
    int charge;
    charge = (int)name_pl[0]*(int)name_des[4]*(int)name_car[2]; // random but constant fare generator
    return charge/100;
}

int main(int argc, char **argv)
{   
	int enter =1;
    printf("******************************************\n");
    printf("*                                        *\n");
    printf("*               CABS                     *\n");
    printf("*                                        *\n");
    printf("******************************************\n");

    printf("HELLO! WELCOME TO OUR CAB BOOKING SYSTEM\n\n");
	while(enter==1)
	{
        int flag=1,availability=0;
        float fare1;
        char name_pl[20],name_des[20],name_car[20];

        //allocating space for queries and the variables within queries 
        int query_len=53,name_len=11;
        int len = query_len + name_len + 1;
        char * insert_query = (char *) malloc(len);

        int query_len1=89,name_len1=11;
        int len1 = query_len1 + name_len1 + 1;
        char * stmt1 = (char *) malloc(len1);

        int query_len2=68,name_len2=11;
        int len2 = query_len2+ name_len2 + 1;
        char * stmt2 = (char *) malloc(len2);
        
        MYSQL *con = mysql_init(NULL);//initialization
    
        if (con == NULL) //check for errors
        {
            fprintf(stderr, "%s\n", mysql_error(con));
            exit(1);
        }

        if (mysql_real_connect(con, "localhost", "root", "trekRajat160!!!", NULL, 0, NULL, 0) == NULL) //conecting with a database system
        {
            fprintf(stderr, "%s\n", mysql_error(con));
            mysql_close(con);
            exit(1);
        }  
        
        if (mysql_query(con, "USE testdb1")) //using a database
        {
            fprintf(stderr, "%s\n", mysql_error(con));
            mysql_close(con);
            exit(1);
        }

        while (flag!=0)//loop till entered correct pickup location
        {
            printf("Following are the Pickup Location in which we offer out cab services in:\n1.MumbaiAirport\n2.Andheri\n3.Vasai\n4.Ghatkopar\n5.Bandra\n6.Malad\n7.Churchgate\n8.Colaba\n9.Bhayandar\n10.Thane\n\n");
            printf("PLEASE ENTER YOUR PICKUP LOCATION:\n");
            fscanf(stdin, "%s", name_pl);
        
            if (mysql_query(con, "SELECT PickupLocation FROM INFORMATION") != 0)//selecting all the pickup locations
            {
                fprintf(stderr, "%s\n", mysql_error(con));
                exit(1);
            } 
            else 
            {
                MYSQL_RES *query_results = mysql_store_result(con);//storing the results from the database 
                if (query_results) 
                { // make sure there *are* results..
                    MYSQL_ROW row;

                    while((row = mysql_fetch_row(query_results)) !=0)
                    {
                        if(strcmp(row[0],name_pl)==0)//comparing the entered pickup location and the database pickup location's'
                        {   
                            printf("Pickup Location selected\n\n");
                            flag=0;
                        }
                        
                    }
                
                    if(flag!=0)
                        printf("SORRY THE ENTERED PICKUP LOCATION IS NOT WITHIN OUR REACH PLEASE ENTER AGAIN:\n");
                    
                    /* Free results when done */
                    mysql_free_result(query_results);
                }
            }
        }

        flag=1;

        while (flag!=0)//loop till entered correct drop location
        {
            printf("Following are the Pickup Location in which we offer out cab services in:\n1.MumbaiAirport\n2.Andheri\n3.Vasai\n4.Ghatkopar\n5.Bandra\n6.Malad\n7.Churchgate\n8.Colaba\n9.Bhayandar\n10.Thane\n\n");
            printf("PLEASE ENTER YOUR DESTINATION:\n");
            fscanf(stdin, "%s", name_des);
            
            if (mysql_query(con, "SELECT Destination FROM INFORMATION") != 0)//selecting all the destinations
            {
                fprintf(stderr, "%s\n", mysql_error(con));
                exit(1);
            } 
            else 
            {
                MYSQL_RES *query_results = mysql_store_result(con);//stroting the results 
                if (query_results) 
                { 
                    MYSQL_ROW row;

                    while((row = mysql_fetch_row(query_results)) !=0)
                    {
                        if(strcmp(row[0],name_des)==0)
                        {   
                            printf("Destination selected\n\n");
                            flag=0;
                        }
                    }
                    if(flag!=0)
                        printf("SORRY THE ENTERED DESTINATION IS NOT WITHIN OUR REACH PLEASE ENTER AGAIN:\n");
                
                    mysql_free_result(query_results);
                }
            }
        }

        flag=1;

        while(flag!=0)//loop till entered correct car type
        {   
            printf("Following are the variety of cars we offer:\n1.Mini\n2.Micro\n3.PrimeSedan\n4.PrimeSUV\n5.Lux\n\n");
            printf("PLEASE SELECT THE TYPE OF CAR YOU WANT:\n");
            scanf("%s",name_car);

            if(mysql_query(con,"SELECT Typesofcars FROM CARS")!=0)//Select all cartypes 
            {   
                fprintf(stderr, "%s\n", mysql_error(con));
                exit(1);
            }
            else
            {
                MYSQL_RES *query_results = mysql_store_result(con);
                if(query_results)
                {
                    MYSQL_ROW row;
                    while((row= mysql_fetch_row(query_results))!=0)
                    {   
                        if(strcmp(row[0],name_car)==0)
                        {   
                            snprintf(insert_query, len, "SELECT Availability FROM CARS WHERE Typesofcars =('%s')", name_car);
                            //Getting the availability of the car the user selects

                            if (mysql_query(con, insert_query) != 0)
                            {
                                fprintf(stderr, "%s\n", mysql_error(con));
                                exit(1);
                            }
                            else
                            {
                                MYSQL_RES *query_results = mysql_store_result(con);
                                if(query_results)
                                {
                                    MYSQL_ROW row;
                                    
                                    while((row= mysql_fetch_row(query_results))!=0)
                                    {   
                                        availability=atoi(row[0]);
                                    }

                                    if(availability>0)//only if the car is available 
                                    {
                                        printf("Car Selected\n\n");
                                        snprintf(stmt1, len1, "UPDATE CARS SET Availability = Availability-1 WHERE Typesofcars = ('%s') and Availability>0", name_car);//if the entered car is found reduce the availability in the database by 1

                                        if (mysql_query(con, stmt1) != 0)
                                        {
                                            fprintf(stderr, "%s\n", mysql_error(con));
                                            exit(1);
                                        }
                                        
                                        flag=0;
                                    }
                                    else
                                    {
                                        printf("Car not available at the moment please try again\n");
                                        flag=2;
                                    }
                                    
                                } 
                            }
                        }
                    }
                    if(flag==1)
                    {   
                        printf("Invalid car name please enter again\n");
                    }
                } 
                mysql_free_result(query_results);  
            }
            
        }

        fare1 = fare(name_pl,name_des,name_car);
        printf("Your Pickup Location is: %s\n",name_pl);
        printf("Your Destination is: %s\n",name_des);
        printf("Your Car is :%s\n",name_car);
        printf("The fare of your travel is as follows :%f\n",fare1);
        printf("THANK YOU FOR TRAVELLING WITH US!!\n\n");
        
        printf("Do you want to keep using our cab booking system? \nEnter 1 for Yes\n Enter 2 for No");
        scanf("%d",&enter);

        if(enter==2)
        {
            snprintf(stmt2, len2, "UPDATE CARS SET Availability = Availability+1 WHERE Typesofcars = ('%s')", name_car);//after the user exits +1 the availability of the selected car
            if (mysql_query(con, stmt2) != 0)
            {
                fprintf(stderr, "%s\n", mysql_error(con));
                exit(1);
            }
        }
        mysql_close(con);//close the connection
    
	}
    return 0;
}
