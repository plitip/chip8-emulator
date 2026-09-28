#include <stdio.h>

#include <stdint.h>

uint8_t memory[4096];
uint8_t V[16];
uint16_t pc = 0x200;
uint16_t stack[16];
uint8_t sp;

int main(){

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
for (pc; pc<0x200+romsize;){
uint8_t left = memory[pc];
uint8_t right = memory[pc+1];
uint16_t opcode = left << 8 | right;
pc += 2;

printf("%04X ", opcode);
uint8_t first = opcode>>12;
uint8_t second = left & 0x0f ;
uint8_t third = right >>4 ;
uint8_t last = right & 0x0F;
int last3 = opcode & 0xFFF;

switch (first) {
    case 0x0:
    if (opcode == 0x00E0){
        break;
    }
    else if (opcode == 0x00EE){
        sp--;
        pc = stack[sp];
        break;
    }
    case 0x1:
    pc = last3;
    break;

    case 0x2:
    stack[sp] = pc;
    sp++;
    pc = last3;
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
    dk what to do; 
    }
    break;
    default:
    break;

}
}

fclose(file_pointer);
return 0;

}   