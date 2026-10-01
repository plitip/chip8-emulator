#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

uint8_t memory[4096];
uint8_t V[16];
uint8_t keys[16];
uint16_t pc = 0x200;
uint8_t screen[2048];
uint16_t stack[16];
uint8_t sp;
uint8_t DT;

uint8_t ST;
uint16_t I;

uint8_t font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
void draw_term() {
    for(int row_counter =0 ;32>row_counter; row_counter++){
        for(int pixel_counter =0 ;64>pixel_counter; pixel_counter++){
            if (screen[row_counter * 64 + pixel_counter] == 1) {
        printf("#");
        } else {
        printf(" ");
        
        }
        }
        printf("\n");
        }
}
void clear_screen(int value) {
    for(int clear_counter =0 ;2048>clear_counter; clear_counter++){
    screen[clear_counter] = value;
        }
}
int main(){
srand(time(NULL));
FILE *file_pointer = fopen("IBMLogo.ch8", "rb");

if (file_pointer == NULL) {

printf("Rom couldn't open\n");

return 1;

}

fseek(file_pointer, 0, SEEK_END);

long romsize = ftell(file_pointer);
rewind(file_pointer);
  

if (romsize > 3584){
printf("rom too big");
fclose(file_pointer);
return 1;

}

fread(&memory[0x200], 1, romsize, file_pointer );
    int fontcounter = 0;
    for(;fontcounter <80;){
    memory[0x050+fontcounter]= font[fontcounter];
    fontcounter++;
    }
for (pc; pc<0x200+romsize;){
uint8_t left = memory[pc];
uint8_t right = memory[pc+1];
uint16_t opcode = left << 8 | right;
pc += 2;
uint8_t first = opcode>>12;
uint8_t second = left & 0x0f ;
uint8_t third = right >>4 ;
uint8_t last = right & 0x0F;
int last3 = opcode & 0xFFF;


switch (first) {
    case 0x0:
    if (opcode == 0x00E0){
        clear_screen(0);
        break;
    }
    else if (opcode == 0x00EE){
        sp--;
        pc = stack[sp];
        break;
    }
    break;
    case 0x1:
    pc = last3;
    break;

    case 0x2:
    stack[sp] = pc;
    sp++;
    pc = last3;
    break;
    case 0x3:
    if(V[second]== right){
        pc+=2;
    }
    break;
    case 0x4:
    if(V[second]!= right){
        pc+=2;
    }
    break;
    case 0x5:
    if (last == 0 ){
        if(V[second] == V[third]){
            pc+=2;
        }
    }
    break;
    case 0x6:
    V[second] = right;
    break;

    case 0x7:
    V[second]= V[second] + right; 
    break;
    case 0x8:
    if (last == 0){
    V[second]= V[third] ; 
    }
    if (last == 1){
    V[second] = V[second] | V[third] ; 
    }
    if (last == 2){
    V[second] = V[second] & V[third] ; 
    }
    if (last == 3){
    V[second] = V[second] ^ V[third] ; 
    }
    if (last == 4){
    int Vsum = V[second] + V[third];
    V[second] = Vsum;
    if (Vsum > 255){
        V[15]= 1;
    } else{
        V[15]= 0;
    }
    }
    if (last == 5){
    if ( V[second] >= V[third]){
        V[15]= 1;
        
    }else{
        V[15]= 0;
    }
    V[second] = V[second] - V[third]; 
    }
    if (last == 6){
        V[15] = V[second] & 0x01;
        V[second] = V[second] / 2;
    }
    if (last == 7){
    if (V[third] >= V[second]){
    V[15]= 1;
    }else{
        V[15]=0;
    }
    V[second] = V[third]-V[second];
    }
    if (last == 0xE){
    V[15] = (V[second] & 0x80) >>7;
    V[second] = V[second] << 1;
    }
    
    break;

    case 0x9:
    if (last == 0x0){
    if (V[second] != V[third]){
        pc+=2;
    }
    }
    break;  
    case 0xA:
    I = last3;
    break;
    case 0xB:
    pc = last3 + V[0];
    break;
    case 0xC:
    V[second] = (rand() % 256) & right;
    break;
    case 0xD:
    uint8_t x = V[second] % 64;
    uint8_t y = V[third] % 32;
    V[15] = 0;
    int Icounter = 0;
    int co_counter = 0;
    for(;Icounter < last; Icounter++) {
    uint8_t sprite_row = memory[I+Icounter];
    for(;co_counter < 8; co_counter++) {
    if (screen[(Icounter+y)*64 +x+co_counter] & ((sprite_row <<co_counter) & 0x80)>>7 == 1){
    V[15] = 1;
    }
    uint8_t screen_storing = screen[(Icounter+y)*64 +x+co_counter] ^ ((sprite_row <<co_counter) & 0x80)>>7;
    screen[(Icounter+y)*64 +x+co_counter] = screen_storing;
}
    co_counter = 0;
}
    draw_term();
    break;
    case 0xE:
    if (right == 0xA1){
    if (keys[V[second]] == 0){
    pc+=2;
}
    }   
    if (right == 0x9E){
    if (keys[V[second]] == 1){
    pc+=2;
}
}
break;
    case 0xF:
    int catchkey = 0;
    int catchcounter = 0;
    int catchnumber = 0;
    if (right == 0x0A){
    while (catchkey==0){
    if (catchcounter>=16){
        catchkey+=1;
        pc-=2;
    }
    for (;catchcounter<16;catchcounter++)
    if (keys[catchcounter] == 0){
} else{
    catchkey+=1;
    catchnumber = catchcounter;
    catchcounter+=16;
    V[second] = catchnumber;
}
    }
    }
    if (right == 0x07){
    V[second]= DT;
    }
    if (right == 0x15){
    DT = V[second];
    }
    if (right == 0x18){
    ST = V[second];
    }
    if (right == 0x1E){
    I =I +V[second];
    }
    if (right == 0x29){
    I = 0x050 +V[second]* 5;
    }
    if (right == 0x33){
    int Ihundred =  (V[second] /100) %10;
    int Itens=  (V[second]/10) %10;
    int Iones =  V[second]%10;
    memory[I]= Ihundred;
    memory[I+1]= Itens;
    memory[I+2]= Iones;
    }
    int Icounter2 = 0;
    if (right == 0x55){
    for (;Icounter2<=second; Icounter2++){

    memory[I+Icounter2] = V[Icounter2];
    
    }
    }
    Icounter2 = 0;
    if (right == 0x65){
    for (;Icounter2<=second; Icounter2++){

    V[Icounter2]= memory[I+Icounter2];
    
    }
    }
    break;
    default:
    break;

}
}

fclose(file_pointer);
return 0;

}   