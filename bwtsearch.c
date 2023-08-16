#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
#define STORESIZE 50000005
#define LIZESIZE 1000000
//#define STORESIZE 1000000005
char last[STORESIZE];
char first[STORESIZE];
int find_id[LIZESIZE];
int find_count = 0;
char *find_line[LIZESIZE];
int find_asi[500005]={0};
int rank[50000005];
int store[128] = {0};
int start[128] = {-1};
int num_list[50];
int index_list;
void push(int num){
    index_list++;
    num_list[index_list] = num;
}
long long pop(){
    int i;
    long long sum = 0;
    for(i=index_list;i>=0;i--){
        sum = sum * 128 + num_list[i];
    }
    index_list = -1;
    return sum;
}
int main(int argc, char* argv[]){
    index_list = -1;
    if (argc != 4) {
        printf("Usage: %s <RLB_FILE_PATH> <INDEX_FILE_PATH> <QUERY_STRING>\n", argv[0]);
        return 1;
    }
    const char* rlb_file_path = argv[1];
    const char* index_file_path = argv[2];
    const char* query_string = argv[3];

    // [*]command line test
    //printf("rlb_file_path: %s\nindex_file_path: %s\nquery_string: %s\n",rlb_file_path,index_file_path,query_string);

    FILE* file = fopen(rlb_file_path, "r");
    if (file == NULL) {
        printf("Fail to open the file: %s\n", rlb_file_path);
        return 1;
    }
    FILE* file2 = fopen(index_file_path, "w");
    fclose(file2);
    int ch,pre=0;
    int i,j;
    int index = 0;
    while ((ch = fgetc(file)) != EOF) {
        if(ch < 128){
            if(index_list > -1){
                
                long long count = pop();
                for(i=0;i<count-1+3;i++){
                    last[index] = pre;
                    rank[index] = store[pre];
                    
                    store[pre] ++;
                    index++;
                }
            }
            last[index] = ch;
            rank[index] = store[ch];
            
            store[ch]++;
            index++;
            pre = ch;
        }
        else{
            push(ch - 128);
        }
    }
    fclose(file);
    int index2=0;
    for(i=0;i<128;i++){
        int count = store[i];
        for(j=0;j<count;j++){
            if(j == 0){
                start[i] = index2;
            }
            first[index2] = i;
            index2++;
        }
    }
    // [*]char input test
    // printf("index: %d\n",index);
    // return 0;
    //[*] last list test
    for(i=0;i<index;i++){
        //printf("%c",last[i]);
    }
    //printf("\n");

    //[*] rank test
    for(i=0;i<index;i++){
        //printf("%d",rank[i]);
    }
    //printf("\n");

    //[*] first list test
    for(i=0;i<index;i++){
        //printf("%c",first[i]);
    }
    //printf("\n");

    //[*] start test
    // for(i=0;i<index;i++){
    //     printf("%d",start[(int)first[i]]);
    // }
    //FM
    int i_string;
    int point1,point2;
    int n_string = strlen(query_string);
    char this_c = query_string[n_string-1];
    //printf("%c\n",this_c);
    point1 = start[(int)this_c];
    point2 = point1 + store[(int)this_c] - 1;
    //printf("%d %d\n",point1,point2);
    int tag;//1 means success, 0 means failure
    int this_rank;
    for(i=point1;i<=point2;i++){
        tag = 1;
        j = i;
        for(i_string=n_string-2;i_string>=0;i_string--){
            this_c = last[j];
            if(this_c == query_string[i_string]){
                this_rank = (int)rank[j];
                j = start[(int)this_c] + this_rank;
            }else{
                tag = 0;
                break;
            }
        }
        if(tag == 0){
            //printf("fail\n");
            continue;
        }else{
            //print the string of this line
            //printf("succeed\n");
            //printf("id-> ");//no use
            this_c = last[j];
            this_rank = (int)rank[j];
            while(this_c != ']'){
                this_rank = (int)rank[j];
                j = start[(int)this_c] + this_rank;
                this_c = last[j];
            }
            //printf("%d %d ",this_rank,(int)rank[j]);
            int sum = 0;
            int k=0;
            while(this_c != '['){
                this_rank = (int)rank[j];
                j = start[(int)this_c] + this_rank;
                this_c = last[j];
                if(this_c-'0'>=0&&this_c-'0'<=9){
                    sum = sum + (this_c-'0')*pow(10,k);
                    k++;
                }
            }
            //printf(" %d ",sum);
            if(find_asi[sum] == 0){
                find_asi[sum] = 1;
                find_id[find_count] = sum;
                find_count++;   
            }
            
            
            //printf("\n");
        }
    }

    //printf("check all the find\n");
    for(i=0;i<find_count;i++){
        for(j=i+1;j<find_count;j++){
            if(find_id[i]>find_id[j]){
                int temp = find_id[i];
                find_id[i] = find_id[j];
                find_id[j] = temp;            
            }
        }
    }
    for(i=0;i<find_count;i++){
        //printf("%d ",find_id[i]);
    }
    //printf("\n");
    point1 = start[(int)'['];
    point2 = point1 + store[(int)'['] - 1;
    this_c = '[';
    char line[1000005];
    int l_index=0;
    for(i=point1;i<=point2;i++){
        line[0] = '\0';
        l_index=0;
        //printf("\n");
        j=i;
        this_c = last[j];
        //printf("%c",this_c);
        line[l_index] = this_c;
        l_index++;
        //this_rank = (int)rank[j];
        //j = start[(int)this_c] + this_rank;
        while(this_c != ']'){
                this_rank = (int)rank[j];
                j = start[(int)this_c] + this_rank;
                this_c = last[j];
                //printf("%c",this_c);
                line[l_index] = this_c;
                l_index++;
            }
        int sum = 0;
        //printf("]");
        int k=0;
        while(this_c != '['){
                this_rank = (int)rank[j];
                j = start[(int)this_c] + this_rank;
                this_c = last[j];
                //printf("%c",this_c);
                line[l_index] = this_c;
                l_index++;
                if(this_c-'0'>=0&&this_c-'0'<=9){
                    sum = sum + (this_c-'0')*pow(10,k);
                    k++;
                }
        }
        line[l_index] = '\0';
        //printf("%d-> str:%s\n",sum,line);
        l_index++;
        if(find_asi[sum] !=0){
            for(k=0;k<find_count;k++){
                if(find_id[k] == sum){
                    break;
                }
            }
            find_line[k] = (char*)malloc(sizeof(char)*(strlen(line)+1));
            strcpy(find_line[k],line);
            //printf("->%d: ",sum);
            //printf("%s\n",line);
        }
        
    }
    for(i=0;i<find_count;i++){
        //printf("%s\n",find_line[i]);
        int n = strlen(find_line[i]);
        for(j=n-1;j>=0;j--){
            printf("%c",find_line[i][j]);
        }
        printf("\n");
    }

}
