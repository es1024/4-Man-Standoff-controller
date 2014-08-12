#include <cstdio>
#include <cstring>

int main(int argc, char** argv){
   char* t;
   t = strtok(argv[2]," ");
   int len = strlen(t);
   int max = -50, maxP;
   for(int i=1;i<4;i++){
      int cur;
      t = strtok(NULL," ");
      if(t[0]=='-'){cur = -1*(t[1]-'0');}
      else{cur = t[0]-'0';}
      if(cur>max){
         max = cur;
         maxP = i;
      }
   }
   if(len == 1){printf("P\n"); return 0;}
   if(len == 3){printf("T%d\n",maxP); return 0;}
   printf("S%d\n",maxP);
   return 0;
}
