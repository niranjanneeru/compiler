# include <stdio.h>
# include <stdlib.h>


int find(int * parent, int i){
    return parent[i] == 1;
}

void make_union(int * parent,int i){
    parent[i] = 1;
}

void makeset(int * parent, int n, int root){
    for(int i = 0; i < n; i++){
        parent[i] = 0;
    }
    parent[root] = 1;
}

void makemptyset(int * parent, int n){
    for(int i = 0; i < n; i++){
        parent[i] = 0;
    }
}

int isEmpty(int * new_states){
    for(int i = 0; i < 100; i++){
        if(new_states[i] == 1) return 0;
    }
    return 1;
}

void cpyset(int * s1, int * s2){
    for(int i =0 ; i < 100; i++){
        s1[i] = s2[i];
    }
}

void set_difference(int * s1, int * s2, int * s3){
    int temp[100];
    makemptyset(temp, 100);
    for(int i = 0; i < 100; i++){
        if(s3[i]) continue;
        if(s2[i]) temp[i] = 1;
    }
    cpyset(s1, temp);
}

void set_union(int * s1, int * s2, int * s3){
    int temp[100];
    makemptyset(temp, 100);
    for(int i = 0; i < 100; i++){
        if(s2[i] || s3[i]) temp[i] = 1;
    }
    cpyset(s1, temp);
}

void set_intersection(int * s1, int * s2, int * s3){
    int temp[100];
    makemptyset(temp, 100);
    for(int i = 0; i < 100; i++){
        if(s2[i] && s3[i]) temp[i] = 1;
    }
    cpyset(s1, temp);
}

int count(int * s1){
    int ans = 0;
    for(int i = 0; i < 100; i++){
        ans += s1[i];
    }
    return ans;
}
