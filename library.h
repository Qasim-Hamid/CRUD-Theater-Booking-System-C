#ifndef LIBRARY_H
#define LIBRARY_H

#define NUM_ROWS 5
#define NUM_COLUMNS 7
#define ARR_BUFFER 100
#define STR_BUFFER 200
#define LINE_BUFFER 500
#define FILENAME "movieinfo.txt"

typedef struct {
    char title[STR_BUFFER];
    int id;
    int seating[NUM_ROWS][NUM_COLUMNS];
} storage;

void dataupdate(int arr_len, storage *storedmovie);
void seats_print(int id_input, storage *moviesdata, int num_movies);
void movie_booking(int array_len, storage* movieset);
void dataread(storage *movieinfo);
void writedata();
int id_index(long int id_input, int num_movies, storage *moviesdata);
int max_seats(int id_ind, storage* movieset);
int arrlen();
void movie_remove(int arr_len, storage *movieset);
void print_id_title(storage* moviesarr, int arr_len);
void create_file();

#endif