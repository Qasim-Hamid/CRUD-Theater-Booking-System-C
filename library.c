#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "library.h"

/*
Updates data file by overwriting file with the storage array passed in, uses arr_len to minimize iterations
*/
void dataupdate(int arr_len, storage *storedmovie)
{
    FILE* fp;
    int i, r, c;

    fp = fopen(FILENAME, "w+");

    for (i = 0; i < arr_len; i++)
    {
        fprintf(fp, "%d %s\n\n", storedmovie[i].id, storedmovie[i].title);
        //Nested loops used to print seats to file with 1s or 0s using correct format
        for (r = 0; r < NUM_ROWS; r++)
        {
            for (c = 0; c < NUM_COLUMNS; c++) 
            {
                fprintf(fp, "%d ", storedmovie[i].seating[r][c]);
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);

    

    return;
}

/*
Reads data file to check if line contains id, returing the resulting amount of items
*/
int arrlen()
{
    int arr_len = 0;
    char check_str[LINE_BUFFER], prev_str[LINE_BUFFER] = "\n";
    FILE* fp;
    fp = fopen(FILENAME, "r");

    while (fgets(check_str, LINE_BUFFER, fp))
    {
        if ((check_str[0] != '0' && prev_str[0] == '\n') && check_str[1] != ' ')
            {
                arr_len += 1;
            }
        strcpy(prev_str, check_str);
    }

    fclose(fp);
    return arr_len;
}

/*
updates array passed into argument as a pointer with data file's information
*/
void dataread(storage *movieinfo)
{
    FILE* fp;
    int r, c, i;
    char line[LINE_BUFFER];
    i = 0;

    fp = fopen(FILENAME, "r");
    while (!feof(fp))
    {
        storage* m = movieinfo + i;
        fgets(line, LINE_BUFFER, fp);
        sscanf(line, "%d %[^\n]", &m -> id, &m -> title);
        for (r = 0; r < NUM_ROWS; r++)
        {
            for (c = 0; c < NUM_COLUMNS; c++) 
            {
                fscanf(fp, "%d ", &m -> seating[r][c]);
            }
            fscanf(fp, "\n");
        }
        i++;
    }
    fclose(fp);
    return; 
}

/*
Writes new movies into movies array, updating file each time a movie is added. 
*/
void writedata()
{
    FILE* fp;
    storage movie, movies[ARR_BUFFER];
    int added_mov, i, r, c, m, tot_movies, max_mov, movie_count;
    long int movie_id;
    char title[STR_BUFFER], str_added_mov[4], str_movie_id[STR_BUFFER];
    bool id_in_use;

    movie_count = arrlen(movies);
    max_mov = ARR_BUFFER - movie_count;
    
    //Checks if movie cap has been reached, can be inreased by updating ARR_BUFFER
    if (max_mov == 0)
    {
        printf("There is no room to store anymore movies");
        return;
    }
    
    //validating input for number of movies written
    do
    {
        printf("\nEnter the number of movies you would like to add: ");
        fgets(str_added_mov, STR_BUFFER, stdin);
        added_mov = strtol(str_added_mov, NULL, 10);
        if (added_mov > max_mov || added_mov <= 0)
        {
            printf("There is room to store 1 - %d movies, try again\n", max_mov);
        }
    } while (added_mov > max_mov || added_mov <= 0);

    tot_movies = movie_count;
    //Loop iterated after a movie is added
    for (m = 0; m < added_mov; m++)
    {
        fp = fopen(FILENAME, "a+");
        dataread(movies);
        
        //Validating title to not be empty / start with space
        do
        {
            printf("\nEnter title of movie: ");
            fgets(title, STR_BUFFER, stdin);
            if (title[0] == '\n' || title[0] == ' ')
            {
                printf("You must enter a valid title (Cannot be empty or start with a space)\n");
            }
        } while (title[0] == '\n' || title[0] == ' ');


        //Validating movie ID to be 9 digits (not starting with 0) and be unique
        do
        {
            printf("\nEnter the movie ID: ");
            fgets(str_movie_id, STR_BUFFER, stdin);
            movie_id = strtol(str_movie_id, NULL, 10);
            
            if (movie_id < 100000000 || movie_id > 999999999)
            {
                printf("The ID must be a 9 digit number, try again\n\n");
            }

            id_in_use = false;
            for (i = 0; i < tot_movies; i++)
            {
                if (movie_id == movies[i].id)
                {
                    printf("You must enter a unique ID (%ld is already in use)", movie_id);
                    id_in_use = true;
                    break;
                }
            }
        } while ((movie_id < 100000000 || movie_id > 999999999) || (id_in_use == true));
        //updated tot_movies to read new movie added at end of file while checking for unique ID
        tot_movies += 1;

        fprintf(fp, "%ld %s\n", movie_id, title);

        //adds empty seating of movie
        for (r = 0; r < NUM_ROWS; r++)
        {
            for (c = 0; c < NUM_COLUMNS; c++) 
            {
                fprintf(fp, "%d ", 0);
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
        fclose(fp);
    }

    return;
}

/*
Reads movies array to compare inputted ID from user to exisiting ID, returning its index if it exists.
Returns -1 if no ID matches. 
*/
int id_index(long int id_input, int num_movies, storage *moviesdata)
{
    int i, movie_index;

    for (i = 0; i < num_movies; i++)
    {
        if (moviesdata[i].id == id_input)
        {
            movie_index = i;
            return movie_index;
        }
    }
    return -1;
}

/*
Prints seats of ID passed in, using information from passed movies array
*/
void seats_print(int id_input, storage *moviesdata, int num_movies)
{
    int i, r, c;

    printf("\nID: %d\nTitle: %s\n", moviesdata[id_input].id, moviesdata[id_input].title);
    printf("Available Seats (0 = Available, 1 = Taken): \n\n");
    printf("     ");
    
    for (i = 0; i < NUM_COLUMNS; i++) 
    {
        printf("%d ", i+1);
    }
    
    printf("\n\n");  
    for (r = 0; r < NUM_ROWS; r++)
    {
        printf("%d    ", r+1);
        for (c = 0; c < NUM_COLUMNS; c++) 
        {
           printf("%d ", moviesdata[id_input].seating[r][c]);
        }
        printf("\n");
    }
    return;
}


/*
Allows user to enter ID of movie and books seats if available
*/
void movie_booking(int array_len, storage* movieset) //Fix the input to take only array_len items
{
    int num_seats, seat_r, seat_c, j, id_ind, seat_cap;
    char str_id[2], str_num_seats[3], row_col[7];
    long int id;
    bool format_check, book_pass;

    if (array_len == 0)
    {
        printf("There are currently no movies airing\n");
        return;
    }
    print_id_title(movieset, array_len);

    //Validating ID, displaying seats for selected movie
    do
        {
            printf("\nEnter the ID of the movie you would like to book: ");
            fgets(str_id, STR_BUFFER, stdin);
            id = strtol(str_id, NULL, 10);
            id_ind = id_index(id, array_len, movieset);
            if (id_ind != -1)
            {
                seats_print(id_ind, movieset, array_len);
            }
            else 
            {
                printf("You have entered an invalid ID, try again\n");
            }
        } while (id_ind == -1);
    
    
    seat_cap = max_seats(id_ind, movieset);
    
    //Checks if any seats are available
    if (seat_cap == 0)
    {
        printf("\nNo available seats for this movie");
        return;
    }
    
    //Checks if number of seats is in range of available seats, if -1 is entered function is exited
    do
    {
        printf("\nHow many seats would you like to book (-1 to stop booking): ");
        fgets(str_num_seats, STR_BUFFER, stdin);
        num_seats = strtol(str_num_seats, NULL, 10);

        if (num_seats == -1)
        {
            printf("No seats booked\n");
            return;
        }
        if (num_seats <= 0 || num_seats > seat_cap)
        {
            printf("You must enter a number within the range of avabilable seats (0 - %d)\n", seat_cap);
        }
    } while (num_seats <= 0 || num_seats > seat_cap);

    //Runs for each seat user is booking
    for (j = 0; j < num_seats; j++) 
    {
        //Validates seat is in range and is open
        do
        {
            printf("\nEnter the row and column of the seat (r,c): ");
            fgets(row_col, STR_BUFFER, stdin);
            format_check = false;
            if (sscanf(row_col, "%d,%d", &seat_r, &seat_c) == 2)
            {
                seat_r -= 1;
                seat_c -= 1;
                if ((seat_r >= 0 && seat_r <= NUM_ROWS) && (seat_c >= 0 && seat_c <= NUM_COLUMNS))
                {
                    format_check = true;
                    book_pass = false;
                    if (movieset[id_ind].seating[seat_r][seat_c] == 0 && format_check)
                    {
                        movieset[id_ind].seating[seat_r][seat_c] = 1;
                        book_pass = true;
                        printf("Seat has succuesfully been booked, press enter to continue ");
                        getchar();
                    }
                    else
                    {
                        printf("This seat has already been booked\n");
                    }
                }
                else 
                {
                    printf("Enter a seat in range. Row range(1 - %d), Column range(1 - %d)\n", NUM_ROWS, NUM_COLUMNS);
                }
            }
            else
            {
                printf("The format you've used is incorrect (r,c):\n");
            }
        } while (format_check == false || book_pass == false);

        
        seats_print(id_ind, movieset, array_len);
    }

    //updates movie set
    dataupdate(array_len, movieset);

    return;
}

//Removes a movie from file after validating it through the storage array.
void movie_remove(int arr_len, storage *movieset)
{
    FILE *fp, *temp;
    int movie_id, i, lines_moved, j, id_ind;
    char move_str[STR_BUFFER], str_id[STR_BUFFER];
    
    /*
    lines_moved is the number of lines a movie needs for its information in the data file
    one of the 1s is the ID and title line, the other 1 represents a blank line imbetween
    the ID and the seating information. The final 1 represents the space imbetween two 
    pieces of information. This is added to number of rows for total lines_moved.
    */
    lines_moved = NUM_ROWS + 1 + 1 + 1;
    //checks if there are no movies airing
    if (arr_len == 0)
    {
        printf("There are no movies currenlty airing\n");
        return;
    }

    //Writes fp lines to temp file without movie which is being removed
    fp = fopen(FILENAME, "r");
    temp = fopen("temp.txt", "w");
    
    //Validating ID
    do
    {
        printf("\nEnter the ID of the movie you would like to remove, or input -1 to exit: ");
        fgets(str_id, STR_BUFFER, stdin);
        movie_id = strtol(str_id, NULL, 10);
        if (movie_id == -1)
        {
            printf("Succusfully exited\n");
            return;
        }
        id_ind = id_index(movie_id, arr_len, movieset);
        if (id_ind != -1)
        {
            seats_print(id_ind, movieset, arr_len);
            printf("\nSuccusfully removed");
        }
        else 
        {
            printf("You have entered an invalid ID, try again\n");
        }
    } while (id_ind == -1);

    //Writes to temp file
    for (i = 0; i < arr_len; i++)
    {
        for (j = 0; j < lines_moved; j++)
        {
            if (i != id_ind)
            {
                fgets(move_str, STR_BUFFER, fp);
                fprintf(temp, move_str);
            }
            else
            {
                fgets(move_str, STR_BUFFER, fp);
            }
        }
            
    }

    fclose(fp);
    fclose(temp);
    
    //the old file is deleted, with the temp file taking its name
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    return;
}

/*
prints IDs and Titles of all movies in movie array
*/
void print_id_title(storage* moviesarr, int arr_len)
{
    int i;

    for (i = 0; i < arr_len; i++)
    {
        printf("\nTitle: %s\nID: %d\n", moviesarr[i].title, moviesarr[i].id);
    }
}

/*
Checks amount of available seats for an ID's index (recieved from passing id into id_index function),
returning this number.
*/
int max_seats(int id_ind, storage* movieset)
{
    int r, c, booked_seats, open_seats;

    booked_seats = 0;
    for (r = 0; r < NUM_ROWS; r++)
    {
        for (c = 0; c < NUM_COLUMNS; c++) 
        {
           if (movieset[id_ind].seating[r][c] == 1)
           {
               booked_seats += 1;
           }
        }
    }
    open_seats = (NUM_ROWS * NUM_COLUMNS) - booked_seats;
    return open_seats;
}


/*
Checks if a file exists on the system, if not writes to a file (creating the file).
*/
void create_file()
{
    FILE *fp;
    if (fp = fopen(FILENAME, "r"))
    {
        printf("File exists");
        fclose(fp);
    }
    else
    {
        fp = fopen(FILENAME, "w");
        fclose(fp);
    }
    return;
}