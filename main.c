/*
 * ============================================
 * file: main.c
 * @Author John Malliotakis (jmal@csd.uoc.gr)
 * @Version 23/10/2023
 *
 * @e-mail hy240@csd.uoc.gr
 *
 * @brief Main function
 *        for CS240 Project Phase 1,
 *        Winter Semester 2023-2024
 * @see   Compile using supplied Makefile by running: make
 * ============================================
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "streaming_service.h"

/* Maximum input line size */
#define MAX_LINE 1024

/* 
 * Uncomment the following line to
 * enable debugging prints
 * or comment to disable it
 */
/* #define DEBUG */
#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else
#define DPRINT(...)
#endif /* DEBUG */

void init_structures(void)
{
    /*
     * TODO: Initialize your
     * global structures here,
     * i.e., the user list (and sentinel
     * node), new releases list, category
     * table
     */
    /* Initialize user list*/
    user_head = NULL ;
    user_Sentinel = NULL;
    /* Initialize new releases list */
    new_movies_head = NULL;
    /* Initialize category table */
    int i ;
    for (i = 0; i < 6; i++)
    {
        category_table[i] = NULL;
    }

}

void destroy_structures(void)
{
    /*
     * TODO: For a bonus
     * empty all lists and stacks
     * and free all memory associated
     * with list/stack nodes here
     */
}

int main(int argc, char *argv[])
{
    FILE *event_file;
    char line_buffer[MAX_LINE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    event_file = fopen(argv[1], "r");
    if (!event_file) {
        perror("fopen error for event file open");
        exit(EXIT_FAILURE);
    }

    init_structures();
    while (fgets(line_buffer, MAX_LINE, event_file)) {
        char *trimmed_line;
        char event;
        int uid;
        unsigned mid, year;
        movieCategory_t category1, category2;
        /*
         * First trim any whitespace
         * leading the line.
         */
        trimmed_line = line_buffer;
        while (trimmed_line && isspace(*trimmed_line))
            trimmed_line++;
        if (!trimmed_line)
            continue;
        /*
         * Find the event,
         * or comment starting with #
         */
        if (sscanf(trimmed_line, "%c", &event) != 1) {
            fprintf(stderr, "Could not parse event type out of input line:\n\t%s",
                    trimmed_line);
            fclose(event_file);
            exit(EXIT_FAILURE);
        }

        switch (event) {
            /* Comment, ignore this line */
            case '#':
                break;
            case 'R':
                if (sscanf(trimmed_line, "R %d", &uid) != 1) {
                    fprintf(stderr, "Event R parsing error\n");
                    break;
                }
                register_user(uid);
                /*printf("R <%d>\n   ",uid);
                print_U();*/
                break;
            case 'U':
                if (sscanf(trimmed_line, "U %d", &uid) != 1) {
                    fprintf(stderr, "Event U parsing error\n");
                    break;
                }
                unregister_user(uid);
                /*printf("U <%d>\n   ",uid);
                print_U();*/
                break;
            case 'A':
                if (sscanf(trimmed_line, "A %u %d %u", &mid, &category1,
                           &year) != 3) {
                    fprintf(stderr, "Event A parsing error\n");
                    break;
                }
                add_new_movie(mid, category1, year);
                /*printf("A<%u><%d><%u>\n  ",mid,category1,year);
                print_A();*/
                break;

            case 'D':
                distribute_new_movies();
                /*printf("D\nCategorized Movies:\n");
                print_movies();*/
                break;
            case 'W':
                if (sscanf(trimmed_line, "W %d %u", &uid, &mid) != 2) {
                    fprintf(stderr, "Event W parsing error\n");
                    break;
                }
                /* printf("\n");*/
                watch_movie(uid, mid);
                print_users_history();
                /* print_user_history(uid);*/
                break;
            case 'S':
                if (sscanf(trimmed_line, "S %d", &uid) != 1) {
                    fprintf(stderr, "Event S parsing error\n");
                    break;
                }
                suggest_movies(uid);
                break;
                /* case 'F':
                     if (sscanf(trimmed_line, "F %d %d %d %u", &uid, &category1,
                                 &category2, &year) != 4) {
                         fprintf(stderr, "Event F parsing error\n");
                         break;
                     }
                     filtered_movie_search(uid, category1, category2, year);
                     break;
                 case 'T':
                     if (sscanf(trimmed_line, "T %u", &mid) != 1) {
                         fprintf(stderr, "Event T parsing error\n");
                         break;
                     }
                     take_off_movie(mid);
                     break;
                 */case 'M':
                printf("M\nCategorized Movies:\n");
                print_movies();
                break;
            case 'P':
                printf("P \n ");
                print_users();
                break;
            default:
                fprintf(stderr, "WARNING: Unrecognized event %c. Continuing...\n",
                        event);
                break;
        }
    }
    fclose(event_file);
    destroy_structures();
    return 0;
}
