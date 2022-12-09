# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "set.h"

typedef struct trans{
    int start;
    char input;
    int out;
} trans;


typedef struct group_set{
    int len;
    int * arr[100];
} group_set;

int find_transition(trans * transitions, int state, char sym, int n){
    for(int i = 0; i < n; i++){
        if(transitions[i].start == state && transitions[i].input == sym){
            return transitions[i].out;
        }
    }
    return 0;
}

int search(int * arr,  group_set w){
    for(int i = 0; i < w.len; i++){
        int flag = 1;
        for(int j = 0 ; j < 100; j++){
            if(arr[j] != w.arr[i][j]) flag = 0;
        }
        if(flag){
            return i;
        }
    }
    return 0;
}

int main(){
    int num_states, num_inputs, num_final_state, num_trans;
    int start_state = 0; // assuming 0 as start state
    char input_symbols[100];
    int states[100] = {0}, final_state[100], final_set[100] = {0};
    trans transitions[100];
    trans pruned_transitions[100];
    printf("Enter the number of states: ");
    scanf("%d", &num_states);
    // num_states = 3;
    printf("The States are: ");
    for(int i = 0; i < num_states; i++){
        printf("%d ",i);
        states[i] = 1;
    }
    printf("\nEnter the number of input symbols: ");
    scanf("%d", &num_inputs);
    // num_inputs = 2;
    printf("Enter the Input Symbols: ");
    for(int i = 0; i < num_inputs; i++){
        scanf("%c",input_symbols + i);
        if(!(input_symbols[i] >= 'a' && input_symbols[i] <= 'z') && !(input_symbols[i] >= '0' && input_symbols[i] <= '9')){
            i--;
        }
    }
    // input_symbols[0] = 'a';
    // input_symbols[1] = 'b';
    printf("The Input Symbols are : ");
    for(int i = 0; i < num_states; i++){
        printf("%c ",input_symbols[i]);
    }
    printf("\nEnter the number of final states: ");
    scanf("%d", &num_final_state);
    printf("Enter the Final States: ");
    for(int i = 0; i < num_final_state; i++){
        scanf("%d",final_state + i);
        final_set[final_state[i]] = 1;
    }
    // num_final_state = 1;
    // final_state[0] = 2;
    // final_set[2] = 1;

    printf("Enter the number of transitions: ");
    scanf("%d", &num_trans);
    // num_trans = 6;
    printf("Enter the Transitions(input_state input_symbol output_state): ");
    for(int i = 0; i < num_trans; i++){
        scanf("%d",&(transitions[i].start));
        top:
        scanf("%c",&(transitions[i].input));
        if (transitions[i].input == ' ') goto top;
        scanf("%d",&(transitions[i].out));
    }

    // transitions[0].start = 0;
    // transitions[0].input = 'a';
    // transitions[0].out = 1;

    // transitions[1].start = 1;
    // transitions[1].input = 'a';
    // transitions[1].out = 1;

    // transitions[2].start = 2;
    // transitions[2].input = 'a';
    // transitions[2].out = 2;

    // transitions[3].start = 1;
    // transitions[3].input = 'b';
    // transitions[3].out = 2;

    // transitions[4].start = 0;
    // transitions[4].input = 'b';
    // transitions[4].out = 2;

    // transitions[5].start = 2;
    // transitions[5].input = 'b';
    // transitions[5].out = 2;

    printf("Transitions\n");
    for(int i = 0; i < num_trans; i++){
        printf("δ(%d,",(transitions[i].start));
        printf("%c)",(transitions[i].input));
        printf("=%d\n",(transitions[i].out));
    }

    // Find Unreachable States

    int reachable_states[100];
    int unreachable_states[100];
    int new_states[100];
    int temp[100];

    makeset(reachable_states, 100, 0);
    makeset(new_states, 100, 0);

    do{
        makemptyset(temp, 100);
        for(int i = 0 ; i < 100 ; i++){
            if(new_states[i] == 1){
                for(int j = 0; j < num_inputs ; j++){
                    int state = find_transition(transitions, i, input_symbols[j], num_trans);
                    make_union(temp, state);
                }
            }
        }
        set_difference(new_states, temp, reachable_states);
        set_union(reachable_states, reachable_states, new_states);

    }while(!(isEmpty(new_states)));

    set_difference(unreachable_states, states, reachable_states);
    set_difference(final_set, final_set, unreachable_states);

    // for(int i = 0; i < 100  ; i++){
    //     printf("%d ", reachable_states[i]);
    // }

    // for(int i = 0 ; i < 100 ; i++){
    //     printf("%d", final_set[i]);
    // }

    // Remove Unreachable Transitions

    int k = 0;

    for(int i = 0; i < 100 ; i++){
        if(reachable_states[i] == 1){
            for(int j = 0; j < num_trans;j++){
                if(transitions[j].start == i){
                    pruned_transitions[k++] = transitions[j];
                }
            }
        }
    }

    num_trans = k;

    int non_final_states[100];

    makemptyset(non_final_states, 100);
    
    set_difference(non_final_states, reachable_states, final_set);

    group_set p;
    p.len = 2;
    p.arr[0] = final_set;
    p.arr[1] = non_final_states;

    group_set w;
    w.len = 2;
    int final_set1[100], non_final_states1[100]; 
    cpyset(final_set1, final_set);
    cpyset(non_final_states1, non_final_states);
    w.arr[0] = final_set1;
    w.arr[1] = non_final_states1;

    while(w.len > 0){
        int * a = w.arr[--w.len];
        w.arr[w.len] = NULL;
        for(int j = 0; j < num_inputs; j++){
            int * x = malloc(100 * sizeof(int));;
            makemptyset(x, 100);
            for(int k = 0; k < 100; k++){
                if(a[k] == 1){
                    // printf("Hello %d %c\n", k, input_symbols[j]);
                    for(int m = 0; m < num_trans; m++){
                        // printf("%d , %c -> %d\n", pruned_transitions[m].start, pruned_transitions[m].input, pruned_transitions[m].out);
                        if(pruned_transitions[m].out == k && pruned_transitions[m].input == input_symbols[j]){
                            // printf("Hi\n");
                            x[pruned_transitions[m].start] = 1;
                        }
                    }
                }
            }

            // return 0;

            printf("%d\n",j);
            
            
            int p_size = p.len;
            int q[100];
            int q_index = 0;
            
            for(int k = 0; k < p_size; k++){

                for(int i1 = 0; i1 < p.len; i1++){
                    for(int j1 = 0; j1 < 10 ; j1++){
                        printf("p = %d ", p.arr[i1][j1]);
                    }
                    printf("\n");
                }

                for(int i1 = 0; i1 < w.len; i1++){
                    for(int j1 = 0; j1 < 10 ; j1++){
                        printf("w = %d ", w.arr[i1][j1]);
                    }
                    printf("\n");
                }

                // if(k == 2) return 0;

                int * intersection = malloc(100 * sizeof(int));
                makemptyset(intersection, 100);
                int * diff = malloc(100 * sizeof(int));
                makemptyset(diff, 100);
                
                set_intersection(intersection, x, p.arr[k]);
                set_difference(diff, p.arr[k], x);

                for(int j1 = 0; j1 < 10 ; j1++){
                        printf("a = %d ", a[j1]);
                }
                printf("\n");

                for(int j1 = 0; j1 < 10 ; j1++){
                        printf("x = %d ", x[j1]);
                }
                printf("\n");

                for(int j1 = 0; j1 < 10 ; j1++){
                        printf("y = %d ", p.arr[k][j1]);
                }
                printf("\n");

                for(int j1 = 0; j1 < 10 ; j1++){
                        printf("i = %d ", intersection[j1]);
                }
                printf("\n");

                for(int j1 = 0; j1 < 10 ; j1++){
                        printf("d = %d ", diff[j1]);
                }
                printf("\n");

                printf("%d - %d\n",k, p_size);

                

                if(!isEmpty(intersection) && !isEmpty(diff)){
                    p.arr[p.len++] = intersection;
                    p.arr[p.len++] = diff;
                    
                    q[q_index++] = k;

                    int res = search(p.arr[k], w);
                    int * intersection1 = malloc(100 * sizeof(int));
                    int * diff1 = malloc(100 * sizeof(int));
                    cpyset(intersection1, intersection);
                    cpyset(diff1, diff);
                    printf("Search Res = %d\n", res);
                    if(res){
                        w.arr[w.len++] = intersection1;
                        w.arr[w.len++] = diff1;
                        for(int m = k; m < w.len - 1; m++){
                            w.arr[m] = w.arr[m+1];
                        }
                        w.arr[--w.len] = NULL;
                    }else{
                        if(count(intersection) <= count(diff)){
                            w.arr[w.len++] = intersection1;
                        }else{
                            w.arr[w.len++] = diff1;
                        }
                    }

                }
            }

            for(int k = 0; k < q_index; k++){
                for(int m = q[k]; m < p.len - 1; m++){
                    p.arr[m] =p.arr[m + 1];
                }
                p.arr[--p.len] = NULL;
            }

            q_index = 0;
        }
        
    }


    for(int i = 0; i < p.len; i++){
        printf("{");
        for(int j = 0; j < 6 ; j++){
            if(p.arr[i][j]){
                printf(" %d ",j);
            }
        }
        printf("}\n");
    }

    // printf("New Automata\n");

    for(int i = 0; i < p.len; i++){
        if(p.arr[i][start_state]){
            printf("Start State = %d\n", i);
            break;
        }
    }

    for(int i = 0; i < p.len; i++){
        for(int j = 0; j < num_final_state; j++){
            if(p.arr[i][final_state[j]]){
                printf("Final State = %d\n", i);
            }
        }
    }

    // for(int i = 0; i < num_trans; i++){
    //     for(int j = 0; j < p.len; j++){
    //         if(p.arr[j][pruned_transitions[i].start]){
    //             pruned_transitions[i].start = j;
    //         }
    //         if(p.arr[j][pruned_transitions[i].out]){
    //             pruned_transitions[i].out = j;
    //         }
    //     }
    // }

    // printf("Transitions\n");
    // for(int i = 0; i < num_trans; i++){
    //     printf("δ(%d,",(pruned_transitions[i].start));
    //     printf("%c)",(pruned_transitions[i].input));
    //     printf("=%d\n",(pruned_transitions[i].out));
    // }




    return 0;
}