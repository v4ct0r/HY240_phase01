#include "streaming_service.h"
#include <stdio.h>
#include <stdlib.h>

struct user *user_head = NULL;
struct  new_movie *new_movies_head = NULL;

void print_users(){
    struct user *temp = user_head;
    while(temp != NULL) {
        printf("%d\n", temp->uid);
        temp = temp->next;
    }
    printf("\n");
}


int register_user(int uid){
    /*check if there is a user with the same uid*/
    struct user *temp = user_head;
    struct user *last = NULL;
    while(temp != NULL) {
        if (temp->uid == uid) {
            return -1;
        }
        last = temp;
        temp = temp->next;
    }
    /*create new user*/
    struct user *new_user = (struct user*) malloc(sizeof(struct user));
    new_user->uid = uid;
    new_user->suggestedHead = NULL;
    new_user->suggestedTail = NULL;
    new_user->watchHistory = NULL;
    new_user->next = NULL;
    if(user_head == NULL){
        user_head = new_user;
    }
    else{
        last->next = new_user;
    }

    return 0;
}
void unregister_user(int uid){
    /*check if there is a user with the same uid*/
    struct user *temp = user_head;
    struct user *prev = NULL;
    while(temp != NULL) {
        if (temp->uid == uid) {
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    if(temp == NULL){
        return;
    }
    /*free suggested movies*/
    struct suggested_movie *temp_suggested = temp->suggestedHead;
    while(temp_suggested != NULL){
        struct suggested_movie *temp_suggested_next = temp_suggested->next;
        free(temp_suggested);
        temp_suggested = temp_suggested_next;
    }

    /*free watch history*/
    struct movie *temp_movie = temp->watchHistory;
    while(temp_movie != NULL){
        struct movie *temp_movie_next = temp_movie->next;
        free(temp_movie);
        temp_movie = temp_movie_next;
    }

    /*remove user from list*/
    if(prev == NULL){
        user_head = temp->next;
    }
    else{
        prev->next = temp->next;
    }

    /*free user*/
    free(temp);


}

int add_new_movie(unsigned mid, movieCategory_t category, unsigned year){
    struct new_movie *NewMovie = (struct new_movie*) malloc(sizeof(struct new_movie));
    NewMovie->info.mid = mid ;
    NewMovie->category = category ;
    NewMovie->info.year = year ;
    NewMovie->next = NULL ;














    /*struct new_movie *temp = new_movies_head;
    struct new_movie *prev = NULL;
    while(temp != NULL) {
        if (temp->info.mid > mid) {
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    if(prev == NULL){
        new_movies_head = NewMovie;
    }
    else{
        prev->next = NewMovie;
    }
    NewMovie->next = temp;
    */



    return 0;

}

/*print new movies*/
void print_new_movies(){
    struct new_movie *temp = new_movies_head;
    while(temp != NULL) {
        printf("%u %d %u\n", temp->info.mid, temp->category, temp->info.year);
        temp = temp->next;
    }
    printf("\n");
}

