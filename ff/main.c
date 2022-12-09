# include <stdio.h>
# include <string.h>

typedef struct production{
    char start;
    char end[10];
} production;

char non_terminals[10] = {'\0'};
char terminals[30] = {'\0'};
char first[30][10] = {'\0'};
char follow[30][10] = {'\0'};
int hasEpsilon[10] = {0};
production productions[10];
int n;
    

void add_as_state(char c){
    int i = 0;
    while(non_terminals[i] != '\0'){
        if(non_terminals[i] == c){
            return;
        }
        i++;
    }
    i = 0;
    while(non_terminals[i] != '\0') i++;
    non_terminals[i] = c;
}

void add_as_terminals(char c){
    int i = 0;
    while(terminals[i] != '\0'){
        if(terminals[i] == c){
            return;
        }
        i++;
    }
    i = 0;
    while(terminals[i] != '\0') i++;
    terminals[i] = c;
}

int isEpsilon(char c){
    if(c == 'e'){
        return 1; // True
    }
    return 0; // False
}

int isTerminal(char c){
    if(c >= 'A' && c <= 'Z') return 0;
    if(isEpsilon(c)) return 0;
    return 1; // False
}

void add_to_first(int index, char c){
    char * temp = first[index];
    while(*temp != '\0'){
        if(*temp == c) return;
        temp++;
    }
    temp = first[index];
    while(*temp != '\0') temp++;
    *temp = c;
}

void add_to_follow(int index, char c){
    char * temp = follow[index];
    while(*temp != '\0'){
        if(*temp == c) return;
        temp++;
    }
    temp = follow[index];
    while(*temp != '\0') temp++;
    *temp = c;
}

int get_non_terminal_index(char c){
    for(int i = 0; i < strlen(non_terminals); i++){
        if(non_terminals[i] == c) return i;
    }
    return -1;
}

void find_first(int index, int flag, int s){
    for(int i = s ; i < n; i++){
        if(productions[i].start == non_terminals[index]){
            // printf("%c -> %s\n", productions[i].start, productions[i].end);
            if(isEpsilon(productions[i].end[0])){
                // if(!flag) printf(" e ");
                hasEpsilon[index] = 1;
                add_to_first(index, 'e');
            }else if(isTerminal(productions[i].end[0])){
                // printf(" %c ", productions[i].end[0]);
                add_to_first(index, productions[i].end[0]);
            }else{
                int temp = get_non_terminal_index(productions[i].end[0]);
                if(temp == -1) return;
                // printf(" %c ", productions[i].end[0]);
                // printf("%d\n", temp);
                find_first(temp, 0, i + 1);
                for(int j = 0; j < strlen(first[temp]); j++){
                    add_to_first(index, first[temp][j]);
                }
                int k = 1;
                check:
                if(hasEpsilon[temp]){
                    temp = get_non_terminal_index(productions[i].end[k]);
                    if(temp == -1) return;
                    // printf(" %c ", productions[i].end[k]);
                    // printf("%d\n", temp);
                    find_first(temp , 1, i + 1);
                    k++;
                    for(int j = 0; j < strlen(first[temp]); j++){
                        add_to_first(index, first[temp][j]);
                    }
                    goto check;
                }
                
            }
        }
    }
}

void find_follow(int index){
    if(productions[0].start == non_terminals[index]) add_to_follow(index, '$');
    for(int i = 0; i < n; i++){
        char * tmp = productions[i].end;
        while(*tmp != '\0'){
            if(isEpsilon(*tmp)) break;
            if(non_terminals[index] == *tmp){
                if(*(tmp+1) != '\0'){
                    if(isTerminal(*(tmp+1))){
                        add_to_follow(index, *(tmp+1));
                    }else{
                        int j = 1;
                        char * t;
                        top:
                        t = first[get_non_terminal_index(*(tmp+j))];
                        int flag = 0;
                        while(*t != '\0'){
                            if(isEpsilon(*t)) flag = 1;
                            else add_to_follow(index, *t);
                            t++;
                        }
                        if(!flag) return; 
                        j++;
                        if(*(tmp+j) != '\0') goto top;
                        else goto edge;
                    }
                }else{
                    char * p;
                    edge:
                    p = follow[get_non_terminal_index(productions[i].start)];
                    while(*p != '\0'){
                        add_to_follow(index, *p);
                        p++;
                    }
                }
            }
            tmp++;
        }
    }


}

int main(int argc, char ** argv){

    FILE * fp;
    fp = fopen (argv[1], "r");
    char str[1000];
    fgets(str, 1000 , fp);
    fscanf(fp, "%d\n", &n);
    
    for (int i = 0; i < n; i++){
        fscanf(fp,"%c", &(productions[i].start));
        fscanf(fp,"%s\n", productions[i].end);
        add_as_state(productions[i].start);
    }

    printf("Productions\n");
    for (int i = 0; i < n; i++){
        printf("%c -> %s\n", productions[i].start, productions[i].end);
    }

    char * temp = non_terminals;
    while (*temp != '\0'){
        printf("%c ", *temp);
        temp++;
    }
    printf("\n");

    int i = 0;
    while (non_terminals[i] != '\0'){
        // printf("Finding First of %c:", non_terminals[i]);
        find_first(i, 0, 0);
        // printf("\n");
        i++;
    }

    // First of all terminals

    for(int i = 0 ; i < n ; i++){
        // printf("%d", i);
        char * rhs = productions[i].end;
        // printf("%s\n",productions[i].end);
        while(*rhs != '\0'){
            if(isTerminal(*rhs)){
                add_as_terminals(*rhs);
            }
            // printf("%c\n", *rhs);
            rhs++;
        }
    }

    for(int i = 0; i < strlen(terminals); i++){
        add_to_first(n + i, terminals[i]);
    }

    for(int i = 0 ; i < strlen(non_terminals); i++){
        printf("First(%c) = { ", non_terminals[i]);
        for(int  j = 0 ; j < strlen(first[i]); j++){
            printf("%c", first[i][j]);
            if(j != strlen(first[i]) - 1){
                printf(", ");
            }
        }
        printf(" }\n");
    }

    for(int i = 0 ; i < strlen(terminals); i++){
        printf("First(%c) = { ", terminals[i]);
        for(int  j = 0 ; j < strlen(first[n + i]); j++){
            printf("%c", first[n + i][j]);
            if(j != strlen(first[n + i]) - 1){
                printf(", ");
            }
        }
        printf(" }\n");
    }

    // FOLLOW

    printf("Assuming Start Symbol -> %c\n", productions[0].start);

    add_to_follow(get_non_terminal_index(productions[0].start), '$');


    for(int i = 0; i < strlen(non_terminals); i++){
        find_follow(get_non_terminal_index(non_terminals[i]));
    }

    for(int i = 0 ; i < strlen(non_terminals); i++){
        printf("Follow(%c) = { ", non_terminals[i]);
        for(int  j = 0 ; j < strlen(follow[i]); j++){
            printf("%c", follow[i][j]);
            if(j != strlen(follow[i]) - 1){
                printf(", ");
            }
        }
        printf(" }\n");
    }

    return 0;
}
