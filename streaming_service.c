#include "streaming_service.h"
#include <stdio.h>
#include <stdlib.h>

struct user *user_head ;
struct user *user_Sentinel;
struct  new_movie *new_movies_head;
struct movie *category_table[6];



void print_users(){
    struct user *temp = user_head;
    struct suggested_movie *temp_sug ;
    struct movie *temp_hist ;
    int i ;
    printf("Users:\n");
    while(temp->uid != user_Sentinel->uid) {
        printf("   %d :\n", temp->uid);
        printf("    Suggested: " );
        temp_sug=temp->suggestedHead ;
        i=1;
        while(temp_sug!=NULL )  {
            printf("<%d,%d>",temp_sug->info.mid,i);
            i++;
            temp_sug=temp_sug->next;
        }
        temp_hist=temp->watchHistory;
        i=1;
        printf("\n    Watch History:");
        while (temp_hist!=NULL){
            printf("<%d,%d>",temp_hist->info.mid,i);
            i++ ;
            temp_hist=temp_hist->next ;
        }
        printf("\n");
        temp = temp->next;
    }
    printf("DONE\n");
}
void print_U(){
    struct user *temp = user_head;
    printf("Users = ");
    while(temp->uid != user_Sentinel->uid) {
        printf("%d", temp->uid);
        temp = temp->next;
        if(temp->uid!= user_Sentinel->uid)
            printf(",");
    }
    printf("\nDONE\n");
}


/*print new movies*/
void print_A(){
    struct new_movie *temp = new_movies_head;
    printf("  New movies = ");
    while(temp != NULL) {
        printf("MID:%u CAT:%d YEAR:%u", temp->info.mid, temp->category, temp->info.year);
        temp = temp->next;
        if(temp!=NULL)
            printf(" , ");
    }
    printf("\nDONE\n");
}
int register_user(int uid){
    /*check if there is a user with the same uid*/
    struct user *NewUser = (struct user*) malloc(sizeof(struct user));
    NewUser->uid = uid;
    NewUser->suggestedHead = NULL;
    NewUser->suggestedTail = NULL;
    NewUser->watchHistory = NULL;
    if (user_head == NULL){
        user_head = NewUser;
        user_Sentinel=(struct user*) malloc(sizeof(struct user));
        user_Sentinel->uid = -1;
        NewUser->next = user_Sentinel;
        return 0;
    }
    struct user *temp = user_head;
    struct user *prev = NULL;
    while(temp->uid != user_Sentinel->uid) {
        if (temp->uid == uid) {
            return -1;
        }
        prev = temp;
        temp = temp->next;
    }
    /*create new user*/
    NewUser->next = user_Sentinel;
    prev->next = NewUser;
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
    /*free suggested movies
    struct suggested_movie *temp_suggested = temp->suggestedHead;
    while(temp_suggested != NULL){
        struct suggested_movie *temp_suggested_next = temp_suggested->next;
        free(temp_suggested);
        temp_suggested = temp_suggested_next;
    }*/
    temp->suggestedHead = NULL;
    temp->suggestedTail = NULL;
    /*free watch history
    struct movie *temp_movie = temp->watchHistory;
    while(temp_movie != NULL){
        struct movie *temp_movie_next = temp_movie->next;
        free(temp_movie);
        temp_movie = temp_movie_next;
    }*/
    temp->watchHistory = NULL;

    /*remove user from list*/
    if(prev == NULL){
        user_head = temp->next;
    }
    else{
        prev->next = temp->next;
    }
    /*free user
    free(temp);*/

}

int add_new_movie(unsigned mid, movieCategory_t category, unsigned year){
    struct new_movie *NewMovie = (struct new_movie*) malloc(sizeof(struct new_movie));
    NewMovie->info.mid = mid ;
    NewMovie->category = category ;
    NewMovie->info.year = year ;
    NewMovie->next = NULL ;

    struct new_movie *temp = new_movies_head;
    struct new_movie *prev = NULL;
    while(temp != NULL) {   /*find the first bigger mid and insert behind it*/
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

    return 0;
}
void Insert(struct new_movie *pMovie) {
    struct movie *Newnode = (struct movie*) malloc(sizeof(struct movie));
    Newnode->info.mid = pMovie->info.mid;
    Newnode->info.year = pMovie->info.year;
    Newnode->next = NULL;
    if (category_table[pMovie->category] == NULL){
        category_table[pMovie->category] =  Newnode;
        return;
    }
    struct movie *temp = category_table[pMovie->category];
    struct movie *last = NULL;
    while (temp != NULL){
        last = temp;
        temp = temp->next;
    }
    last->next = Newnode;

}
/*EVENT D*/
void distribute_new_movies(void){
    struct new_movie *temp = new_movies_head;
    struct new_movie *prev = NULL;
    while (temp != NULL){
        Insert(temp);
        prev = temp;
        temp = temp->next;
        free(prev);
    }
    new_movies_head = NULL;
}
void print_movies(void){
    char *category_names[6] = {"Horror","Sci-fi","Drama","Romance","Documentary","Comedy"};
    struct movie *temp;
    int i ;
    int counter ;
    for ( i = 0 ; i < 6 ; i++){
        temp = category_table[i];
        printf("  %s: ",category_names[i]);
        counter = 1 ;
        while(temp != NULL){
            printf("<%u,%d>",temp->info.mid,counter);
            counter++;
            temp = temp->next;
            if(temp != NULL){
                printf(" , ");
            }
        }
        printf("\n");
    }
    printf("DONE\n");
}

void Insert_W(struct user *pUser ,struct movie *pMovie) {
    /*Insert in the beginning because it's a stack*/
    struct movie *Newnode = (struct movie*) malloc(sizeof(struct movie));
    Newnode->info.mid = pMovie->info.mid;
    Newnode->info.year = pMovie->info.year;

    Newnode->next = pUser->watchHistory;
    pUser->watchHistory = Newnode;
}
void print_users_history(){
    struct user *temp = user_head;
    int i ;
    while(temp->uid != user_Sentinel->uid){
        printf("  User %d Watch History: ",temp->uid);
        struct movie *temp_movie = temp->watchHistory;
        i= 1 ;
        while(temp_movie != NULL){
            printf("<%u,%d>",temp_movie->info.mid,i);
            i++ ;
            temp_movie = temp_movie->next;
            if(temp_movie != NULL){
                printf(" , ");
            }
        }
        printf("\n");
        temp = temp->next;
    }
    printf("DONE\n");
    printf("\n");
}

void print_user_history(int uid){
    struct user *temp = user_head;
    while(temp->uid != user_Sentinel->uid){
        if(temp->uid == uid){
            break;
        }
        temp = temp->next;
    }
    if(temp->uid == user_Sentinel->uid){
        return;
    }
    struct movie *temp_movie = temp->watchHistory;
    printf("  User %d Watch History: ",temp->uid);
    while (temp_movie!= NULL){
        printf("<%u>",temp_movie->info.mid);
        temp_movie = temp_movie->next;
        if(temp_movie != NULL){
            printf(" , ");
        }
    }
    printf("\nDONE\n");
}

int watch_movie(int uid, unsigned mid){
    struct user *temp_user = user_head;

    while(temp_user->uid != user_Sentinel->uid) {
        if (temp_user->uid == uid) {
            break;
        }
        temp_user = temp_user->next;
    }
    if(temp_user->uid == user_Sentinel->uid){
        return -1;
    }
    printf("W<%d><%u>\n",uid ,mid);
    int i ;
    int flag = 0;
    struct movie *temp_movie;
    for(i = 0 ; i < 6 ; i++){
        temp_movie = category_table[i];
        while(temp_movie != NULL){
            if(temp_movie->info.mid == mid){
                flag = 1;
                break;
            }
            temp_movie = temp_movie->next;
        }
        if(flag == 1){
            break;
        }
    }
    if(flag == 0){
        return -1;
    }
    Insert_W(temp_user,temp_movie);
    return 0;
}
int suggest_movies(int uid) {/*fiaxnw to suggestedHead kai to suggestedTail se temp structs kai meta ta bazw sto user */
    struct user *temp = user_head;
    struct user *temp_user = NULL;
    struct suggested_movie *NewHead = NULL;
    struct suggested_movie *tempHead = NULL;
    struct suggested_movie *NewTail = NULL;
    struct suggested_movie *tempTail = NULL;
    struct movie *temp_hist;

    int i = 0;
    while (temp->uid != user_Sentinel->uid) {
        temp_hist = temp->watchHistory;
        if (temp->uid != uid && temp_hist != NULL) {
            if (i % 2 == 0) {
                if (NewHead == NULL) {
                    NewHead = (struct suggested_movie *) malloc(sizeof(struct suggested_movie));
                    tempHead = NewHead;
                    tempHead->prev = NULL;
                } else {
                    tempHead->next = (struct suggested_movie *) malloc(sizeof(struct suggested_movie));
                    tempHead->next->prev = tempHead;
                    tempHead = tempHead->next;
                }
                tempHead->info.mid = temp_hist->info.mid;
                temp->watchHistory = temp->watchHistory->next;
            } else {
                if (NewTail == NULL) {
                    NewTail = (struct suggested_movie *) malloc(sizeof(struct suggested_movie));
                    tempTail = NewTail;
                    tempTail->next = NULL;
                } else {/*kanei insert sthn arxh dhladh to prwto stoixeio pou mpenei tha einai to teleutaio kai ta epomena tha mpenoun sto prev tou*/
                    /*handle oi pointers*/
                    tempTail->prev = (struct suggested_movie *) malloc(sizeof(struct suggested_movie));
                    tempTail->prev->next = tempTail; /*bazo to next tou prohgoumenou tha deixnei sto tempTail*/
                    tempTail = tempTail->prev;
                }
                /*insert*/
                tempTail->info.mid = temp_hist->info.mid;
                temp->watchHistory = temp->watchHistory->next;
            }
            i++;
        } else if (temp->uid == uid) {
            temp_user = temp;
        }
        temp = temp->next;
    }
    if (temp_user != user_Sentinel) {
        if (tempHead != NULL && tempTail != NULL) {
            tempHead->next = tempTail; /*connect the two lists*/
            tempTail->prev = tempHead;
            NewTail->next = NULL; /*to teleutaio tis list tha deixnei se NULL*/
            temp_user->suggestedHead = NewHead;
            temp_user->suggestedTail = NewTail;
        } else if (tempHead != NULL && tempTail == NULL) {
            tempHead->next = NULL;
            temp_user->suggestedHead = NewHead;
        }
        /* Yparxoun mono treis periptoseis:
        1. Na bei stin 'if' estw mia fora kai tha dimiourgithei toulachiston ena NewHead.
        2. An bei parapano tha dimiourgithei toulachiston ena NewTail (first if).
        3. Na den bei kamia, ola paramenoun NULL. */
    }
}

