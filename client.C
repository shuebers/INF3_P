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
	//connect to host
	c.conn(host , 2022);

	msg = "Client bereit";
	cout << "client sends:" << msg << endl;
	c.sendData(msg);
	msg = c.receive(128);
	cout << "got response:" << msg << endl;
	sleep(1);

	for(int i=0; i<4; i++){

		if(msg == "Server bereit"){

			clientRequest << "GENERATE[" << pwdLength << "," << symbSetSize << "]";
			msg = clientRequest.str();

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			msg = c.receive(128);
			cout << "got response:" << msg << endl;
			sleep(1);
		}
		else if(msg.compare(0,18,"Passwort generiert") == 0){
			msg = "CHECKPWDBBBB"; //First message(CHECKPWD) tells server to check pwd, every other Symbol is the PWD (here BBBB)

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			msg = c.receive(128);
			cout << "got response:" << msg << endl;
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
