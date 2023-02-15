/*
 * server.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

/*! 
 *
 * \file 	  server.C
 * 
 * \brief     Contains the code that generates random encrypted passwords 
 * on given perimeters and compares them to given passwords.
 * 
 * \details   This class generates passwords using the SHA256 encryption on given perimeters.
 * 				It can compare a given password with the generated password thus making it secure.
 * \author    Mattis Tornede
 * \author    Simon Huebers
 * \version   1.2
 * \date      2023
 * \pre       Needs client.C to check passwords.
 * \bug       Port sometimes isnt freed after closing.
 * \copyright GNU Public License.
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
/**
 *
 * \class myServer
 *
 * \brief Implements a server that generates random encrypted passwords
 * 	and can compare it to given passwords
 *
 */
class myServer : public TCPserver{
public:
	/**
	 *
	 * \brief Constructor of myServer
	 *
	 * \param port defines the port on which you can connect to the server.
	 *
	 * \param size defines the max datasize of a command.
	 *
	 */
	myServer(int port , int size) : TCPserver(port, size)
	{
		password = new TASK1::BlackBoxSafe(4, 4);
	};

	TASK1::BlackBoxSafe* password;

	/**
	 *
	 * \brief Response of the server to given commands.
	 *
	 * \param input is the input for command that the server reads.
	 *
	 * \return string containing the response of the server.
	 *
	 */
	string myResponse(string input)
	{
		int serverpwdLength ;
		int serversymbSetSize ;
		string ReturnValue;
		stringstream serverResponse;


		if(input.compare(0,8,"GENERATE") == 0){
			delete password;
			sscanf(input.c_str(), "GENERATE[%i,%i]", &serverpwdLength,&serversymbSetSize);
			serverResponse << "Passwort generated.\nPasswortlaenge: " << serverpwdLength << "\n" << "Symbolzahl: " << serversymbSetSize;
			ReturnValue = serverResponse.str();
			password = new TASK1::BlackBoxSafe(serverpwdLength, serversymbSetSize);
		}
		else if(input.compare(0,12,"Client ready")==0){
			ReturnValue = "Server ready"; //Server ready for input
		}
		else if(input.compare(0,8,"CHECKPWD") == 0){
			string tempstring = input.substr(8,serverpwdLength);
			//std::cout <<"Password read from string:" << tempstring << std::endl;
			//std::cout <<"Current Encrypted Password of Server:" << password->pwd_ << std::endl; //Test
			//std::cout <<"       Received Password (Encrypted):" << sha256(tempstring)<< std::endl; //Test
			ReturnValue = password->input(tempstring);
		}
		else{
			ReturnValue = "Error";
		}
		return ReturnValue;
	}
};


int main(int argc, char *argv[]){
	
	int port;
	if(argc < 2){
		cout << "Server port number missing. \nPossible commands: ./server [port]" << endl;
		exit(0);
	}
	if(argc > 2){
		cout << "Too many arguments. \nPossible commands: ./server [port]" << endl;
		exit(0);
	}
	port = atoi(argv[1]);
	cout << "server used port number: " << port << endl;


	srand(time(nullptr) + 25); //+25 to have different random values between client and server
	myServer ms(port, 25);
	ms.run();

}
