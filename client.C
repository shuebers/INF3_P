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

using namespace std;

int main() {
	srand(time(NULL));
	TCPclient c;
	string host = "localhost";
	string msg;
	stringstream ss;

	//connect to host
	c.conn(host , 2022);

	int pwdLength;
	int symbSetSize;


	ss << "GENERATE[" << pwdLength << "," << symbSetSize << "]";
	msg = ss.str;

	cout << "client sends:" << msg << endl;
	c.sendData(msg);
	msg = c.receive(32);
	cout << "got response:" << msg << endl;
	//sleep(1);
	c.sendData("BYEBYE");
	msg = c.receive(32);
	cout << "got response:" << msg << endl;


}
