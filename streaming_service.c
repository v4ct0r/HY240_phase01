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
        printf(" %d :\n", temp->uid);
        printf("  Suggested: " );
        temp_sug=temp->suggestedHead ;
        i=1;
        while(temp_sug!=NULL )  {
            printf("mid:%d,%d",temp_sug->info.mid,i);
            i++;
            temp_sug=temp_sug->next;
            if(temp_sug!=NULL){
                printf(" , ");
            }
        }
        temp_hist=temp->watchHistory;
        i=1;
        printf("\n  Watch History:");
        while (temp_hist!=NULL){
            printf("mid:%d,%d",temp_hist->info.mid,i);
            i++ ;
            temp_hist=temp_hist->next ;
            if(temp_hist!=NULL){
                printf(" , ");
            }
        }
        printf("\n");
        temp = temp->next;
    }
    printf("DONE\n");
}
void print_U(){
    struct user *temp = user_head;
    if(temp == NULL){
        return;
    }
    printf(" Users = ");
    while(temp->uid != user_Sentinel->uid) {
        printf("uid:%d", temp->uid);
        temp = temp->next;
        if(temp->uid!= user_Sentinel->uid)
            printf(",");
    }
    printf("\nDONE\n");
}

/*print new movies*/
void print_A(){
    struct new_movie *temp = new_movies_head;
    printf(" New movies = ");
    while(temp != NULL) {
        printf("mid:%u,category:%d,year:%u", temp->info.mid, temp->category, temp->info.year);
        temp = temp->next;
        if(temp!=NULL)
            printf(" , ");
    }
    printf("\nDONE\n");
}
void print_F(int uid, movieCategory_t category1,movieCategory_t category2, unsigned year){
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
    printf("F<uid:%d><category1:%d><category2:%d><year:%u>\n",uid,category1,category2,year);
    printf("  Suggested: ");
    struct suggested_movie *temp_sug = temp->suggestedHead;
    int i = 1 ;
    while(temp_sug != NULL){
        printf("mid:%d,%d",temp_sug->info.mid,i);
        i++;
        temp_sug = temp_sug->next;
        if(temp_sug != NULL){
            printf(" , ");
        }
    }
    printf("\nDONE\n");
}

int register_user(int uid){
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
    struct user *prev = user_head;
    /*check if there is a user with the same uid*/
    while(temp->uid != user_Sentinel->uid) {
        if (temp->uid == uid) {
            return -1;
        }
        prev = temp;
        temp = temp->next;
    }
    /*place the new user before the sentinel*/
    NewUser->next = user_Sentinel;
    prev->next = NewUser;
    return 0;
}

void unregister_user(int uid){
    /*check if there is a user with the same uid*/
    struct user *temp = user_head;
    struct user *prev = NULL;
    while(temp->uid != user_Sentinel->uid ) {
        if (temp->uid == uid) {
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    if(temp->uid == user_Sentinel->uid){
        return;
    }
    /*free suggested list*/
    temp->suggestedHead = NULL;
    temp->suggestedTail = NULL;
    /*free watch history*/
    temp->watchHistory = NULL;
    /*remove user from list*/
    if(prev == NULL){   /*an einai o prwtos*/
        if(temp->next == user_Sentinel){
            user_head = NULL;
        }
        else{
            user_head = temp->next;
        }
    }
    else{
        prev->next = temp->next;  /*bazw to next tou prohgoumenou na deixnei sto next tou user pou thelw na diagrapsw*/
    }
    /*free(temp);*/
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
    printf("Categorized Movies:\n");
    char *category_names[6] = {"Horror","Sci-fi","Drama","Romance","Documentary","Comedy"};
    struct movie *temp;
    int i ;
    int counter ;
    for ( i = 0 ; i < 6 ; i++){
        temp = category_table[i];
        printf("  %s: ",category_names[i]);
        counter = 1 ;
        while(temp != NULL){
            printf("mid:%u,%d",temp->info.mid,counter);
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
void print_users_history(int uid, unsigned mid){
    printf("W<uid:%d><%u>\n",uid,mid);
    struct user *temp = user_head;
    int i ;
    while(temp->uid != user_Sentinel->uid){
        printf(" User %d Watch History: ",temp->uid);
        struct movie *temp_movie = temp->watchHistory;
        i= 1 ;
        while(temp_movie != NULL){
            printf("mid:%u,%d",temp_movie->info.mid,i);
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
    printf("W<uid:%d><mid:%u>\n",uid ,temp->watchHistory->info.mid);
    struct movie *temp_movie = temp->watchHistory;
    printf(" User %d Watch History: ",temp->uid);
    while (temp_movie!= NULL){
        printf("mid:%u",temp_movie->info.mid);
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
void print_S(int uid){
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
    printf("S<uid:%d>\n",uid);
    printf("  Suggested: ");
    struct suggested_movie *temp_sug = temp->suggestedHead;
    int i = 1 ;
    while(temp_sug != NULL){
        printf("mid:%d,%d",temp_sug->info.mid,i);
        i++;
        temp_sug = temp_sug->next;
        if(temp_sug != NULL){
            printf(" , ");
        }
    }
    printf("\nDONE\n");
}

int suggest_movies(int uid) {/*fiaxnw to suggestedHead kai to suggestedTail se temp structs kai meta ta bazw sto user */
    struct user *temp = user_head;
    struct user *temp_user = NULL;
    struct suggested_movie *NewHead = NULL;
    struct suggested_movie *tempHead = NULL;
    struct suggested_movie *NewTail = NULL;
    struct suggested_movie *tempTail = NULL;
    struct movie *temp_hist;
    /*check if the uid is in the list of users*/
    while (temp->uid != user_Sentinel->uid) {
        if (temp->uid == uid) {
            break;
        }
        temp = temp->next;
    }
    if (temp->uid == user_Sentinel->uid) {
        return -1;
    }

    temp = user_head;
    int i = 0;
    while (temp->uid != user_Sentinel->uid) {
        temp_hist = temp->watchHistory;
        if(temp_hist == NULL && temp->uid != uid){ /*an den exei watch history kai den einai o user pou psaxnw skip*/
            temp = temp->next;
            continue;
        }
        if (temp->uid != uid && temp_hist != NULL) {
            if (i % 2 == 0) {
                if (NewHead == NULL) {
                    NewHead = (struct suggested_movie *) malloc(sizeof(struct suggested_movie));
                    tempHead = NewHead;
                    tempHead->prev = NULL;
                } else {
                    tempHead->next = (struct suggested_movie *) malloc(sizeof(struct suggested_movie));
                    tempHead->next->prev = tempHead; /*bazo to prev tou epomenou na deixnei ston current*/
                    tempHead = tempHead->next;
                }
                tempHead->info.mid = temp_hist->info.mid;
                temp->watchHistory = temp->watchHistory->next;
            } else {
                if (NewTail == NULL) {
                    NewTail = (struct suggested_movie *) malloc(sizeof(struct suggested_movie));
                    tempTail = NewTail;
                    tempTail->next = NULL;
                } else {/*kanei insert sthn arxh dhladh to prwto stoixeio pou mpenei tha einai to teleutaio kai ta epomena tha benoun sto prev tou*/
                    /*handle tous pointers*/
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

    if (tempHead != NULL && tempTail != NULL) {
        tempHead->next = tempTail; /*connect the two lists*/
        tempTail->prev = tempHead;
        temp_user->suggestedHead = NewHead;
        temp_user->suggestedTail = NewTail;
    } else if (tempHead != NULL && tempTail == NULL) {
        tempHead->next = NULL;
        temp_user->suggestedHead = NewHead;
        temp_user->suggestedTail = tempHead;/*teamHead einai to teleutaio stoixeio*/
    }
    /* Yparxoun mono treis periptoseis:
    1. Na bei stin 'if' estw mia fora kai tha dimiourgithei toulachiston ena NewHead.
    2. An bei parapano tha dimiourgithei toulachiston ena NewTail (first if).
    3. Na den bei kamia, ola paramenoun NULL. */
    return 0;
}
struct suggested_movie* createList(movieCategory_t category, unsigned year) {
    /*ftiaxnei mia lista me ta movies pou exoun megalyterh h idia th xronia*/
    struct suggested_movie *temp_sug =  NULL;
    struct suggested_movie *Head_sug = NULL;

    struct movie *temp_movie = category_table[category];
    while(temp_movie != NULL){
        if(temp_movie->info.year >= year){/*an einai idia h megaluterh h xronia Create node*/
            struct suggested_movie *Newnode = (struct suggested_movie*) malloc(sizeof(struct suggested_movie));
            Newnode->info = temp_movie->info;
            Newnode->next = NULL;
            Newnode->prev = temp_sug;
            /*Insert*/
            if(temp_sug == NULL){
                temp_sug = Newnode;
                Head_sug = temp_sug;
            }
            else{
                temp_sug->next = Newnode;
                temp_sug = temp_sug->next;
            }
        }
        temp_movie = temp_movie->next;
    }
    return Head_sug;
}

struct suggested_movie* mergeLists(struct suggested_movie* head1, struct suggested_movie* head2) {
    struct suggested_movie* result_head = NULL;
    struct suggested_movie* result = NULL;

    if(head1 == NULL){/*an to prwto einai keno*/
        return head2;
    }
    else if(head2 == NULL){/*an to deutero einai keno*/
        return head1;
    }
    while (1){
        if(head1!=NULL && head2!=NULL && head1->info.mid <= head2->info.mid){ /*an to mid tou prwtou einai mikrotero h iso me to mid tou deuterou*/
            if(result == NULL){
                result = head1;
                result_head = result;
                head1 = head1->next;
                result->next = NULL;
                result->prev = NULL;
            }
            else{
                result->next = head1;
                head1->prev = result;
                head1 = head1->next;
                result = result->next;
                result->next = NULL;
            }
        }
        else if(head1!=NULL && head2!=NULL && head1->info.mid > head2->info.mid){ /*an to mid tou prwtou einai megalutero apo to mid tou deuterou*/
            if(result == NULL){
                result = head2;
                result_head = result;
                head2 = head2->next;
                result->next = NULL;
                result->prev = NULL;
            }
            else{
                result->next = head2;
                head2->prev = result;
                head2 = head2->next;
                result = result->next;
                result->next = NULL;
            }
        }else if(head1==NULL && head2!=NULL){ /*an teleiwsoun ta nodes tou prwtou list*/
            result->next = head2;
            head2->prev = result;
            head2 = head2->next;
            result = result->next;
            result->next = NULL;
        } else if(head1!=NULL && head2==NULL){/*an teleiwsoun ta nodes tou deuterou list*/
            result->next = head1;
            head1->prev = result;
            head1 = head1->next;
            result = result->next;
            result->next = NULL;
        }
        else{/*an teleiwsoun kai ta 2 lists*/
            return result_head;
        }
    }
}

int filtered_movie_search(int uid, movieCategory_t category1,movieCategory_t category2, unsigned year){
    struct user *temp = user_head;
    struct user *temp_user = NULL;
    while (temp->uid != user_Sentinel->uid) {/*psaxnw ton user*/
        if(temp->uid == uid) {
            temp_user = temp;
            break;
        }
        temp = temp->next;
    }
    if(temp_user == NULL){
        return -1;
    }

    struct suggested_movie *Head_sug1 = createList(category1, year);
    struct suggested_movie *Head_sug2 = createList(category2, year);

    struct suggested_movie *mergedHead = mergeLists(Head_sug1, Head_sug2);
    if (mergedHead == NULL){
        return 0;
    }
    if(temp_user->suggestedHead == NULL){
        temp_user->suggestedHead = mergedHead;
    }
    else{
        temp_user->suggestedTail->next = mergedHead;
        mergedHead->prev = temp_user->suggestedTail;
    }
    /*psaxnw to telos tis listas na balw to tail*/
    struct suggested_movie *temp_sug = temp_user->suggestedHead;
    while(temp_sug->next != NULL){
        temp_sug = temp_sug->next;
    }
    temp_user->suggestedTail = temp_sug;
    return 0;
}
void take_off_movie(unsigned mid){
    printf("T<%u>\n",mid);
    struct user *temp_user = user_head;
    struct suggested_movie *temp_sug;
    struct suggested_movie *prev_sug;
    while(temp_user->uid != user_Sentinel->uid){
        temp_sug = temp_user->suggestedHead;
        prev_sug = NULL;
        while(temp_sug != NULL){/*elegnxei an to mid einai sth suggested list tou user temp*/
            if(temp_sug->info.mid == mid){
                if(prev_sug == NULL){/*an einai to prwto sth lista suggested tou user*/
                    temp_user->suggestedHead = temp_sug->next;/*bazei to next tou prohgoumenou na deixnei sto next tou user_temp*/
                    if(temp_user->suggestedHead == NULL){/*an einai to proto-teleutaio sth lista suggested tou user*/
                        temp_user->suggestedTail = NULL;/*kane kai to tail NULL*/
                    }
                    else{/*an den einai to teleutaio sth lista suggested tou user*/
                        temp_user->suggestedHead->prev = NULL;/*kane to prev tou epomenou NULL*/
                    }
                }
                else{
                    prev_sug->next = temp_sug->next; /*bazei to next tou prohgoumenou na deixnei sto next tou user_temp*/
                    if(temp_sug->next == NULL){/*an einai to teleutaio sth lista suggested tou user*/
                        temp_user->suggestedTail = prev_sug;/*ftiaxnei to tail na deixnei sto prohgoumeno*/
                    }
                    else{
                        temp_sug->next->prev = prev_sug; /*bazei to prev tou epomenou na deixnei ston prohgoumeno*/
                    }
                }
                printf( " mid:%u removed from user:%d suggested list\n",mid , temp_user->uid);
                break;/*an to vrei stamataei na psaxnei*/
            }
            prev_sug = temp_sug;
            temp_sug = temp_sug->next;
        }
        temp_user = temp_user->next;
    }

    /*remove from category table*/
    char *category_names[6] = {"Horror","Sci-fi","Drama","Romance","Documentary","Comedy"};
    struct movie *temp_movie;
    struct movie *prev_movie = NULL;
    int i ;
    for(i=0 ; i<6 ; i++){
        temp_movie = category_table[i];
        while(temp_movie != NULL){
            if(temp_movie->info.mid == mid){
                if(prev_movie == NULL){/*an einai to prwto sth lista suggested tou user*/
                    category_table[i] = temp_movie->next;
                }
                else{
                    prev_movie->next = temp_movie->next; /*bazei to next tou prohgoumenou na deixnei sto next tou user_temp*/
                }
                printf( " %u removed from %s Category list\n",mid ,category_names[i]);
                break;
            }
            prev_movie = temp_movie;
            temp_movie = temp_movie->next;
        }
        /*print the category list that the movie was removed from*/
        if(temp_movie != NULL){
            printf("  %s: ",category_names[i]);
            temp_movie = category_table[i];
            int counter = 1 ;
            while(temp_movie != NULL){
                printf("%u,%d",temp_movie->info.mid,counter);
                counter++;
                temp_movie = temp_movie->next;
                if(temp_movie != NULL){
                    printf(" , ");
                }
            }
            printf("\n");
        }

    }
    printf("DONE\n");


}
