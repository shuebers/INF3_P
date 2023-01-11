/*
 * server.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */
#include <cstdio> // standard input and output library
#include <cstdlib> // this includes functions regarding memory allocation
#include <cstring> // contains string functions
#include <cerrno> //It defines macros for reporting and retrieving error conditions through error codes
#include <ctime> //contains various functions for manipulating date and time

#include <unistd.h> //contains various constants
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <arpa/inet.h> // defines in_addr structure
#include <sys/socket.h> // for socket creation
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses


#include "SIMPLESOCKET.H"
#include "TASK1.H"
#include "SHA256.H"
using namespace std;
#include <iostream>
#include <string>
#include <sstream>

class myServer : public TCPserver{
public:
	
	myServer(int port , int size) : TCPserver(port, size)
	{
		password = new TASK1::BlackBoxSafe(4, 4);
		std::cout << "Constructor Password:" << password->pwd_ << std::endl;
	};
	TASK1::BlackBoxSafe* password;
	string myResponse(string input)
	{
		int serverpwdLength = 4;
		int serversymbSetSize = 1;
		string ReturnValue;
		stringstream serverResponse;
		//std::cout << "Triggered the myServer" << std::endl;


		if(input.compare(0,8,"GENERATE") == 0){
			delete password;
			sscanf(input.c_str(), "GENERATE[%i,%i]", &serverpwdLength,&serversymbSetSize);
			serverResponse << "Passwort generiert.\nPasswortlänge: " << serverpwdLength << "\n" << "Symbolzahl: " << serversymbSetSize;
			ReturnValue = serverResponse.str();
			password = new TASK1::BlackBoxSafe(serverpwdLength, serversymbSetSize);
		}
		else if(input == "Client bereit"){
			ReturnValue = "Server bereit"; //Server bereit
		}
		else if(input.compare(0,8,"CHECKPWD") == 0){
			string tempstring = input.substr(8,input.length());
			std::cout <<"Password read from string:" << tempstring << std::endl;
			std::cout <<"Current Encrypted Password of Server:" << password->pwd_ << std::endl; //Test
			std::cout <<"       Received Password (Encrypted):" << sha256(tempstring)<< std::endl; //Test
			ReturnValue = password->input(tempstring);
		}
		else{
			ReturnValue = "Error";
		}
		return ReturnValue;
	}
};


int main(){
	srand(time(nullptr));
	myServer ms(2022, 25);
	ms.run();

	//TASK1::BlackBoxUnsafe bs(4,4);
	//std::cout << bs.pwd_ << std::endl;
	//TCPserver srv(2022,25);
	//srv.run();
}
