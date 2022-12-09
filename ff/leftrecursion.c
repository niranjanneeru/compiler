# include <stdio.h>
# include <string.h>

typedef struct production{
    char start;
    char end[20];
    int rl;
} production;

char non_terminals[10] = {'\0'};
char terminals[30] = {'\0'};
int hasEpsilon[10] = {0};
production productions[30];
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

int get_non_terminal_index(char c){
    for(int i = 0; i < strlen(non_terminals); i++){
        if(non_terminals[i] == c) return i;
    }
    return -1;
}

void print_arr(char * temp){
    while (*temp != '\0'){
        printf("%c ", *temp);
        temp++;
    }
    printf("\n");
}

void print_productions(){
    printf("Productions\n");
    for (int i = 0; i < n; i++)
        if(productions[i].rl)
            printf("%c -> %s\n", productions[i].start, productions[i].end);
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
        productions[i].rl = 1;
        add_as_state(productions[i].start);
    }

    for(int i = 0 ; i < n ; i++){
        char * rhs = productions[i].end;
        while(*rhs != '\0'){
            if(isTerminal(*rhs)){
                add_as_terminals(*rhs);
            }
            rhs++;
        }
    }
    // print_productions();
    // print_arr(non_terminals);
    // print_arr(terminals);

    // Direct Left Recursion

    int arr[30] = {0};
    int num_non_terminals = strlen(non_terminals);
    for(int i = 0;  i < num_non_terminals; i++){
        int arr[30] = {0};
        for(int j = 0; j < n; j++){
            if(non_terminals[i] == productions[j].start){
                if(isEpsilon(productions[j].end[0])) arr[j] = 1;
                else if(isTerminal(productions[j].end[0])) arr[j] = 2;
                else arr[j] = 3;
            }
        }

        for(int j = 0; j < 30 ; j++){
            
        }

    }


    // Indirect Left Recursion
    num_non_terminals = strlen(non_terminals);
    for(int i = 0;  i < num_non_terminals; i++){
        for(int j = 0; j < n; j++){
            if(isEpsilon(productions[j].end[0])) continue;
            if(isTerminal(productions[j].end[0])) continue;
            printf("DEBUG: %c -> %s\n", productions[j].start, productions[j].end);
            if(get_non_terminal_index(productions[j].end[0]) < get_non_terminal_index(productions[j].start)){
                printf("Hi\n");
                char * t = strdup(productions[j].end);
                memcpy(t, productions[j].end + 1, strlen(productions[j].end) - 1);   
                productions[j].rl = 0;
                for(int k = 0; k < n; k++){
                    if(productions[j].end[0] == productions[k].start){
                        production p;
                        p.start = productions[j].start;
                        p.rl = 1;
                        strcat(p.end, productions[k].end);
                        strcat(p.end, t);
                        productions[n++] = p;
                    }
                }
            }
        }
    }

    print_productions();


    return 0;
}