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
//using namespace std;
#include <iostream>
#include <sstream>
#include "TASK1.H"

class myServer : public TCPserver{
public:
	myServer(int port , int size) : TCPserver(port, size)
	{
		;
	};
	string myResponse(string input)
	{
		int pwdLength;
		int symbSetSize;
		string ReturnValue;
		stringstream sr;
		std::cout << "Triggered the myServer" << std::endl;

		if(input.compare(0,8,"GENERATE") == 0){
			sscanf(input.c_str(), "GENERATE[%i,%i]", &pwdLength,&symbSetSize);
			sr << "Passwortlänge:" << pwdLength << ";" << "Symbolzahl:" << symbSetSize;
			ReturnValue = sr.str;
		}
		/*else if(input == "2"){
			ReturnValue = "No";
		}*/
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
