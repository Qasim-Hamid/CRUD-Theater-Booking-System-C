/*
Written by: Qasim Hamid
Last edited: March 8th 2022

The movie booking system is a program created for theaters to have an easy to use 
terminal that allows them to add movies, remove movies, book seats and view 
information on movies that are currently airing. The movies' information are read and
written to a datafile, which is generated the first time the program is run. 
By default this file is called movieinfo.txt, but can be changed in the library.h
file, by modifying '#define FILENAME (name of file)'.
*/
#include <stdio.h>
#include <stdlib.h>
#include "library.h"


int main(int argc, char* argv[]) 
{
     
    int id_ind, movie_len;
    long int id, choice;
    storage movies[ARR_BUFFER];
    char choice_str[STR_BUFFER];
    char str_id[STR_BUFFER];
    char menu[200] = {"\nTheater booking system\n"
           "\n"
           "Available choices: \n"
           "1) View Airing Movies\n"
           "2) Book a Seat\n"
           "3) Add a Movie\n"
           "4) Remove a Movie\n"
           "5) Quit\n"
           "\n"
           "Input a number corresponding to one of the actions above: "};

    create_file();

    while (choice != 5)
    {
        printf("%s", menu);
        choice = 0;
        do
        {
            fgets(choice_str, 200, stdin);
            choice = strtol(choice_str, NULL, 10);
            if (choice < 1 || choice > 5)
            {
                printf("Enter a valid choice (1-5): ");
            }
        } while (choice < 1 || choice > 5);

        switch(choice)
        {
            //Displays airing movies
            case 1:
            {
                //updates movies before function is ran
                dataread(movies);
                movie_len = arrlen(movies);
                
                //Checks if atleast 1 movie is airing
                if (movie_len != 0)
                {
                    print_id_title(movies, movie_len);

                    //ID Validation
                    do
                    {
                        printf("\nEnter the ID of the movie you would like to view the seats of, or input -1 to exit: ");
                        fgets(str_id, STR_BUFFER, stdin);
                        id = strtol(str_id, NULL, 10);
                        if (id == -1)
                        {
                            printf("Succusfully exited\n");
                            break;
                        }
                        id_ind = id_index(id, movie_len, movies);
                        if (id_ind != -1)
                        {
                            seats_print(id_ind, movies, movie_len);
                        }
                        else 
                        {
                            printf("You have entered an invalid ID, try again\n");
                        }
                    } while (id_ind == -1);
                }
                //If no movies are airing
                else
                {
                    printf("There are no movies currently airing\n");
                }
                printf("\nPress enter to continue ");
                //getchar is used to pause program till user presses enter
                getchar();
                break;
            }
            //Allows user to book a seat
            case 2:
            {
                dataread(movies);
                movie_booking(arrlen(movies), movies);
                printf("\nPress enter to continue ");
                getchar();
                break;
            }
            //User can write a book to data file
            case 3:
            {
                writedata();
                printf("\nPress enter to continue ");
                getchar();
                break;
            }
            //User can remove a book from data file
            case 4: 
            {
                dataread(movies);
                movie_remove(arrlen(movies), movies);
                printf("\nPress enter to continue ");
                getchar();
                break;
            }
            //Exits program
            case 5:
            {
                printf("Thank you for using the program, now exiting");
                return 0;
            }
            //Kept for purpose of security in case of error that was not accounted for
            default:
            {
                printf("\nInvalid input, press enter to reopen menu ");
                getchar();
                break;
            }
        }
    }

    return 0;
}
