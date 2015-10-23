#include <stdio.h>
#include <stdlib.h>

#define LIMIT 8000

const int RIGHT = 0;
const int D_RIGHT = 1;
const int DOWN = 2;
const int D_LEFT = 3;
const int LEFT = 4;
const int U_LEFT = 5;
const int UP = 6;
const int U_RIGHT = 7;
const int NEUTRAL = 8;

//Other Notes:
//The first 7 words are not input data.
//Input data starts on the 8th word with Player 1 then Player 2.

// Format is as follows
// 08 FF FF FF
// move word, then 3 button words

// 08 FF FF FF is pure neutral. No hands on controller.

//LP Press: 3F FC FF ==> 0011 1111 X 1111 1100 X 1111 1111
//LP Hold: BF FE FF  ==> 1011 1111 X 1111 1110 X 1111 1111
//LP Release: 7F FD FF > 0111 1111 X 1111 1101 X 1111 1111

//MP Press: CF FC FF ==> 1100 1111 X 1111 1100 X 1111 1111
//MP Hold: EF FE FF  ==> 1110 1111 X 1111 1110 X 1111 1111
//MP Release: DF FD FF > 1101 1111 X 1111 1101 X 1111 1111

//HP Press: F3 FC FF ==> 1111 0011 X 1111 1100 X 1111 1111
//HP Hold: FB FE FF  ==> 1111 1011 X 1111 1110 X 1111 1111
//HP Release: F7 FD FF > 1111 0111 X 1111 1101 X 1111 1111

//LK Press: FC F3 FF ==> 1111 1100 X 1111 0011 X 1111 1111
//LK Hold: FE FB FF  ==> 1111 1110 X 1111 1011 X 1111 1111
//LK Release: FD F7 FF > 1111 1101 X 1111 0111 X 1111 1111

//MK Press: FF 33 FF ==> 1111 1111 X 0011 0011 X 1111 1111
//MK Hold: FF BB FF  ==> 1111 1111 X 1011 1011 X 1111 1111
//MK Release: FF 77 FF > 1111 1111 X 0111 0111 X 1111 1111

//HK Press: FF C3 FF ==> 1111 1111 X 1100 0011 X 1111 1111
//HK Hold: FF EB FF  ==> 1111 1111 X 1110 1011 X 1111 1111
//HK Release: FF D7 FF > 1111 1111 X 1101 0111 X 1111 1111

//Normal double button presses are the bitwise-and of the two buttons and 
//the actions done at that time.
//Note that button A can be pressed while B is held. So we get
// (B hold) & (A Press)

//Biggest Example is Throw
//LP+LK Press: 3C F0 FF > 0011 1100 X 1111 0000 X 1111 1111
//LP+LK Hold: BE FA FF => 1011 1110 X 1111 1010 X 1111 1111
//LP+LK Release: 7D F5 FF 0111 1101 X 1111 0101 X 1111 1111

//Here is an exception to the rule
//Dash does not register without that change in the last word
//LP+MP Press: 0F FC CF > 0000 1111 X 1111 1100 X 1100 1111
//LP+MP Hold: AF FE EF => 1010 1111 X 1111 1110 X 1110 1111
//LP+HP Release: 5F FD DF 0101 1111 X 1111 1101 X 1101 1111

//Here is another exception.
//This could be used to check for supers
//LK+MK Press: FC 33 3F > 1111 1100 X 0011 0011 X 0011 1111
//LK+MK Hold: FE BB BF => 1111 1110 X 1011 1011 X 1011 1111
//LK+MK Release: FD 77 7F 1111 1101 X 0111 0111 X 0111 1111

//Snaps and assist calls do not seem to use that last word

int to_b_2(unsigned int);
void print(int, int);

void read_data();
void write_data(const char *filename);
void write_neutral(FILE *f);

int main(int argc, char **argv){
  //read_data();
  write_data("round_0008.rnd");
  return 0;
}

void read_data(){
  FILE *f = fopen("./round_0001.rnd", "r");

  int i;
  int s1, s2, s3, s4;
  for(i = 0; i < 4160+8; i+=4){
    s1 = fgetc(f);
    s2 = fgetc(f);
    s3 = fgetc(f);
    s4 = fgetc(f);
    if(s1 == EOF || s2 == EOF || s3 == EOF || s4 == EOF) break;
    if(i < 4160) continue;
    print(i, s1);
    print(i+1, s2);
    print(i+2, s3);
    print(i+3, s4);
  }
  printf("\n");

  fclose(f);
}

void write_line(int s1, int s2, int s3, int dir, FILE *f){
  char move[4];
  move[0] = s1;
  move[1] = s2;
  move[2] = s3;
  move[3] = dir;
  fwrite(move, sizeof(char), 4, f);
}


void write_data(const char* filename){
  FILE *f = fopen(filename, "w");

  char data[8];
  data[0] = 49;
  data[1] = 48;
  data[2] = 1;
  data[3] = 98;
  data[4] = 46;
  data[5] = 0;
  data[6] = 0;
  data[7] = 8;

  fwrite(data, sizeof(char), 8, f);

  int i;
  for(i = 0; i < 512; ++i){
    write_neutral(f);
    write_neutral(f);
  }

  int s1, s2, s3;
  s1 = 255;
  s2 = 255;
  s3 = 255;
  for(s3 = 0; s3 < 256; ++s3){
    for(i = 0; i < 5; ++i){
      write_line(s1, s2, s3, 8, f);
      write_neutral(f);
    }
  }
  
  fclose(f);
}

void write_neutral(FILE *f){
  char move[4];
  move[0] = 255;
  move[1] = 255;
  move[2] = 255;
  move[3] = 8;
  fwrite(move, sizeof(char), 4, f);
}



void print(int i, int s){
  printf("%X:\t%i\t-\t%08i\n", i, s,  to_b_2(s));
}

int to_b_2(unsigned int s){
  if (!s || s == 1) {
    return s;
  }


  return (to_b_2(s >> 1) * 10) + (s % 2);
}
