/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

#include <string>
#include <iostream>
#include <unistd.h> //contains various constants
#include <sstream>

#include "SIMPLESOCKET.H"
#include "TASK1.H"

using namespace std;

int main(int argc, char *argv[]) {
	
	int port;
	if(argc != 2){
		cout << "client port number missing." << endl;
		exit(0);
	}
	port = atoi(argv[1]);
	cout << "client used port number: " << port << endl;

	TASK1::BlackBoxUnsafe* Testpwd;

	srand(time(nullptr));
	TCPclient c;
	string host = "localhost";
	string msg;
	stringstream clientRequest;
	string SYMBOLS = "ABCDEFGHIJKLMNOPQRTSTUVWXYZabcdefghijklmopqrstuvwxyz0123456789";
	int pwdLength = 5;
	int symbSetSize = 5;
	int counter = 0;
	int guessedcounter = 0;
	//connect to host
	c.conn(host , port);
	msg = "Client bereit";
	cout << "client sends:" << msg << endl;
	c.sendData(msg);
	msg = c.receive(128);
	cout << "got response:" << msg << endl;
	//sleep(1);

	while(guessedcounter < 50){

		if(msg == "Server bereit" || msg.compare(0,15, "ACCESS ACCEPTED") == 0){

			clientRequest << "GENERATE[" << pwdLength << "," << symbSetSize << "]";
			msg = clientRequest.str();
			clientRequest.str("");//Clearing the stringstream for further use

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			msg = c.receive(128);
			cout << "got response:" << msg << endl;
			//sleep(1);
		}
		else if(msg.compare(0,19,"Passwort generiert.") == 0 || msg.compare(0,13, "ACCESS DENIED") == 0){
			Testpwd = new TASK1::BlackBoxUnsafe(pwdLength, symbSetSize); //This needs to be in new //Mattis

			clientRequest << "CHECKPWD" << Testpwd->pwd_;//First message(CHECKPWD) tells server to check pwd, every other Symbol is the PWD
			msg = clientRequest.str();
			clientRequest.str("");//Clearing the stringstream for further use

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			delete Testpwd;
			msg = c.receive(128);
			cout << "got response:" << msg << endl;
			if(msg.compare(0,15, "ACCESS ACCEPTED") == 0){
				guessedcounter++;
				
			}
			counter++;
			//sleep(1);
		}
		else{
			msg = "Error";

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			msg = c.receive(128);
			cout << "got response:" << msg << endl;
			//sleep(1);
		}

	}


	c.sendData("BYEBYE");
	msg = c.receive(128);
	cout << "got response:" << msg << endl;
	cout << "amount of guessed passwords:" << guessedcounter << endl;
	cout << "Total amount of guesses:" << counter << endl;
	cout << "Mid time per password:" << counter/guessedcounter << endl;


}
