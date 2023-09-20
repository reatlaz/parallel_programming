#include <stdio.h>
int main(void) {
   // FILE * fh = fopen("barsiq.jpeg", "rb");
   // FILE * res = fopen("newbarsiq.jpeg", "wb");

   FILE * fh = fopen("kek.txt", "rb");
   FILE * res = fopen("newkek.txt", "wb");
   // int size = 200;
   fseek(fh, 0, SEEK_END);
   long size = ftell(fh);
   char f[size];

   fgets(f, size, fh);

   printf("%s", f);
   // int x = 5;
   // for(int i=0; i < size; i++){
   //    f[i] += (i*x) & 256;
   // }


   fputs(f, fh);

   fclose(fh);
   fclose(res);
   return 0;
}