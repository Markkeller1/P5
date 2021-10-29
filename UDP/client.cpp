#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <linux/input.h>

#include "include/UDP_Com.h"


int main(int argc, char const *argv[])
{
    printf("Client started\n");
    struct input_event event;
    UDP_Com TEST;
    TEST.InitiateClient();
    TEST.ToggleDebug(false);
    
    // TEST.SendMessage();
    int rounds = 0;
    char c;
    char character;
    while(1){

        system("stty raw");
        c = getchar(); 
        // terminate when "." is pressed
        system("stty cooked");
        system("clear");        

    switch (c) {
        case 'w':  // Q key -> forward
        std::cout << "Forward" << std::endl;
        TEST.UpdateVelocity(10,10);
        break;

        case 'a':  // A key -> left
        std::cout << "Left" << std::endl;
        TEST.UpdateVelocity(0,10);
        break;

        case 's':  // S key -> backwards
        std::cout << "Backward" << std::endl;
        TEST.UpdateVelocity(-10,-10);
        break;

        case 'd':  // D key -> right
        std::cout << "Right" << std::endl;
        TEST.UpdateVelocity(10,0);
        break;

        case 'x':
        system("stty cooked");
        exit(0);
        break;

        default:
        std::cout << "h" << std::endl;
        break;
    }



        //TEST.SendMessage();
        rounds++;
        std::cout << "Rounds in loop: " << rounds << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
} 
