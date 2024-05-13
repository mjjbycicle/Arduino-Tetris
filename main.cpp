#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include "Block.h"

using namespace std;

void wait1s();
void waithalfs();
void waitquarters();
void waiteights();

int loop1second = 5e8;
double cycleTime = 0.75;

void setup() {
    return;
}

void loop() {
    for (int i = 0; i < loop1second; i++){

    }
    cout << 1 << " loop" << endl;
}

void wait(auto duration){
    std::this_thread::sleep_for(duration);
}

int main() {
    setup();
    while(true) {
        loop();
    }
}
