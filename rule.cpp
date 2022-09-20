#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>

using namespace std;

void swapGenerations();
void printGeneration();
void noiseFill();
void checkerboardFill();

/*
    General idea: 
    The rule determines the next state of a cell, depending on the state of each cell and its two neighbors. 
    For example, if the current cell is set and its neighbors are not, this is 010, or situation 2. 
    In the next generation, our cell will be set if the ith bit of the rule is 1. 
    For example, if the rule was 6, 00000110, bit 2 is set and thus in the next generation our cell will be set.
*/
int rules[8]; 

#define WIDTH 200
//here we crop off the sides so it fits and appears infinite
#define DISPLAY_BORDER 30


int * currentGeneration = (int *)calloc(1, WIDTH * sizeof(int));
int * nextGeneration =    (int *)calloc(1, WIDTH * sizeof(int));
int * temp =              (int *)calloc(1, WIDTH * sizeof(int));

int main(int argc, char* argv[]){
    if(argc < 3)
    {
        cerr << "supply rule and depth as arguments" << endl;
        exit(0);
    }
    int rule = atoi(argv[1]);
    int depth = atoi(argv[2]);
    if(argc == 4){ //specify initial conditions
        if(strcmp(argv[3], "r") == 0){
            noiseFill();
        }
        else if(strcmp(argv[3], "c") == 0){
            currentGeneration[WIDTH/2] = 1;
        }
        else if(strcmp(argv[3], "x") == 0){
            checkerboardFill();
        }
        else{
            cerr << "choose initial conditions from | r - /dev/random | c - one cell set in center | x - checkerboard | [default is center]" << endl;
            exit(0);
        }

    } else { //default initial conditions
        currentGeneration[WIDTH/2] = 1;
    }

    for(int i = 0 ; i < 8 ; i++)
    {
        rules[i] = ( rule >> i ) & 1; //extract ith bit
    }

    int i = depth;
    while (i-- > 0){

        printGeneration();
        //ideas: each cell gathers neighbors OR 
        //temp array which you add first 4 to cell-1, loop and add 2 to cell, loop and add 1 to cell+1
        //same O(n) and first is more readable, PLUS eliminates needing another array to store sums. investigate
        for(int cell = 1 ; cell < WIDTH -1 ; cell++){
            int ruleToUse = currentGeneration[cell-1] * 4 + currentGeneration[cell] * 2 + currentGeneration[cell+1];
            //cout << "rule: " << ruleToUse << endl;
            nextGeneration[cell] = rules[ruleToUse];
        }

        swapGenerations();
    }

}

void noiseFill(){
    ifstream random;
    random.open("/dev/random");
    char * buf = (char *) calloc(WIDTH, sizeof(char));
    random.read(buf, WIDTH);

    cout << buf << endl;

    for(int i = 0 ; i < WIDTH ; i++){
       // cout << atoi(buf+i);
        currentGeneration[i] = (buf[i] > 10) ? 1 : 0;
    }
    random.close();
}

void checkerboardFill(){
    for(int i = 0 ; i < WIDTH ; i++){
        currentGeneration[i] = i % 2;
    }
}

void printGeneration(){
    for(int cell = DISPLAY_BORDER ; cell < WIDTH - DISPLAY_BORDER ; cell++){
        if(currentGeneration[cell])
            cout << "#";
        else
            cout << "_";
    }
    cout << endl;
}
void swapGenerations(){
    temp = currentGeneration;
    currentGeneration = nextGeneration;
    nextGeneration = temp;
}
