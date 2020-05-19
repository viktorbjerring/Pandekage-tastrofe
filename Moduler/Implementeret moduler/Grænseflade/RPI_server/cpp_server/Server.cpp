#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#include "Server.hpp"

using namespace std;

Server::Server(int port, int maxConnections) : port_(port), maxConnections_(maxConnections) {
	serverSocket_ = socket(AF_INET, SOCK_STREAM, 0); //Create socket
	if (serverSocket_ < 0) 
        error("ERROR opening server socket");	
}

int Server::serverListen() {
	// Setup server struct
	serverAddr_.sin_family = AF_INET;    //Setup addr
    serverAddr_.sin_port = htons(port_);
    serverAddr_.sin_addr.s_addr = INADDR_ANY;

	// Bind socket
	int err = bind(serverSocket_, (struct sockaddr *) &serverAddr_, sizeof(serverAddr_));  //Bind addr and socket
    if (err < 0)
        error("ERROR: bind not good");

	// Listen defined in socket.h
	listen(serverSocket_, maxConnections_);    //Create buffer for socket

	unsigned int clilen = sizeof(clientAddr_);
    while(1)
    {
        cout << "Waiting for client" << endl;
        int clientSocket = accept(serverSocket_, (struct sockaddr *) &clientAddr_, &clilen);
        if (clientSocket < 0) {
            error("ERROR on accept");
		}
        else {
            readTextTCP(serverBuff_, BUFSIZE, clientSocket);
            cout << "Tekst fra client:" << serverBuff_ << endl;
            //long fileSize = check_File_Exists(serverBuff);
			writeTextTCP(clientSocket, serverBuff_);
            //sendFile(serverBuff, fileSize, clientSocket);
        }

    }
}


void Server::error(const char* msg){ // Private error handling function
	perror(msg);
    exit(0);
}

/*	**
 * Læser en tekststreng fra en socket
 *
 * @param inFromServer stream that holds data from server
 * @return En streng modtaget fra en socket
 */
void Server::readTextTCP(char *text, int length, int inFromServer)
{
    char ch;
    int pos=0, len;

    len = read(inFromServer, &ch, 1);
    if(len != 1){}
    while(ch != 0)
    {
        if(pos < length)
            text[pos++] = ch;
        len = read(inFromServer, &ch, 1);
    }
    return text;
}

/**
 * Skriver en tekststreng til en socket med 0 terminering
 *
 * @param outToServer Stream hvortil der skrives data til socket
 * @param line Teksten der skal skrives til socket
 */
void Server::writeTextTCP(int outToServer, char *line)
{
    int len;
    do {
        len = write(outToServer, line, strlen(line)+1);
    } while(len < 0);
}