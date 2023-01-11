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

int main() {
	srand(time(NULL));
	TCPclient c;
	string host = "localhost";
	string msg;
	stringstream clientRequest;
	string SYMBOLS = "ABCDEFGHIJKLMNOPQRTSTUVWXYZabcdefghijklmopqrstuvwxyz0123456789";
	int pwdLength = 4;
	int symbSetSize = 5;
	int counter = 0;
	//connect to host
	c.conn(host , 2022);

	msg = "Client bereit";
	cout << "client sends:" << msg << endl;
	c.sendData(msg);
	msg = c.receive(128);
	cout << "got response:" << msg << endl;
	sleep(1);

	for(int i=0; i<8; i++){

		if(msg == "Server bereit" || msg.compare(0,15, "ACCESS ACCEPTED") == 0){

			clientRequest << "GENERATE[" << pwdLength << "," << symbSetSize << "]";
			msg = clientRequest.str();
			clientRequest.str("");//Clearing the stringstream for further use

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			msg = c.receive(128);
			cout << "got response:" << msg << endl;
			sleep(1);
		}
		else if(msg.compare(0,19,"Passwort generiert.") == 0 || msg.compare(0,13, "ACCESS DENIED") == 0){
			TASK1::BlackBoxUnsafe Testpwd(pwdLength, symbSetSize); //This needs to be in new //Mattis

			clientRequest << "CHECKPWD" << Testpwd.pwd_;//First message(CHECKPWD) tells server to check pwd, every other Symbol is the PWD
			msg = clientRequest.str();
			clientRequest.str("");//Clearing the stringstream for further use

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			//delete Testpwd;
			msg = c.receive(128);
			cout << "got response:" << msg << endl;
			counter++;
			sleep(1);
		}
		else{
			msg = "Error";

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			msg = c.receive(128);
			cout << "got response:" << msg << endl;
			sleep(1);
		}

	}


	c.sendData("BYEBYE");
	cout << "got response:" << msg << endl;


}
