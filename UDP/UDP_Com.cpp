#include "include/UDP_Com.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void UDP_Com::EncodeMessage(){
    UDP_Com::JSON_Message = Message.dump(3);
}

void UDP_Com::DecodeMessage(std::string JSON_message){
    Message = nlohmann::json::parse(JSON_message);
}

//konstant flow af positioner fra generede trajectory, fartøj hastighed og om den position er crack eller ej (bitumenflow)
void UDP_Com::UpdateMessage(float posx, float posy, float velx, float vely, float accx, float accy, int bitflow){ //Updater json Message med ny data
    Message =
    {
        {"Position", 
            {
                {"X", posx}, 
                {"Y", posy} 
            }
        },
        {"Velocity",
            {  
                {"X", velx}, 
                {"Y", vely} 
            }
        },
        {"Acceleration",
            { 
                {"X", accx}, 
                {"Y", accy} 
            }
        },
        {"BitumenFlow", bitflow} 
    }; 

    EncodeMessage();

    if(debug == true){
        std::cout << "Message updated with new information\n" << std::endl;
    }
    
} 

void UDP_Com::SendMessage(){  //CLIENT, send message to server
    int sockfd;
    char buffer[MAXLINE];
    char Mesg[] = "0";
    strcpy(Mesg,JSON_Message.c_str());

    struct sockaddr_in     servaddr;
  
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
  
    memset(&servaddr, 0, sizeof(servaddr));
      
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
      
    int n, len;
      
    sendto(sockfd, (const char *)Mesg, strlen(Mesg),
        MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
            sizeof(servaddr));

    if(debug == true){
        std::cout << Message.dump(3) << "\n Message sent to server\n" << std::endl;
    }
    
    close(sockfd);

}

void UDP_Com::ReceiveMessage(){  //SERVER, receive message from client
    
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
      
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
      
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
      
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
      
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
            sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
      
    int len, n;
    len = sizeof(cliaddr);  //len is value/resuslt
  
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                (socklen_t*)&len);
    buffer[n] = '\0';
    
    //Convert buffer char array to string and decode
    std::string RawStringMsg = convertToString(buffer, sizeof(buffer)); 
    DecodeMessage(RawStringMsg);
        
    if(debug == true){
        std::cout << RawStringMsg << "\nMessage received from client\n" << std::endl;
    }
    
}

std::string UDP_Com::convertToString(char* a, int size){
    std::string s = a;
    return s;
}

void UDP_Com::PrintMessage(){
    std::cout << Message.dump(3) << std::endl;
}

void UDP_Com::ToggleDebug(bool status){
    debug = status;
}