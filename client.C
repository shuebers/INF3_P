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

string firstPassword(int length);
string nextPassword(string oldPassword, int symbSize, int currentPosition);

int main(int argc, char *argv[]) {
	
	int port;
	string pwdAlgorithm;
	if(argc < 2){
		cout << "client port number missing." << endl;
		exit(0);
	}
	if(argc > 3){
		cout << "too many arguments." << endl;
		exit(0);
	}
	port = atoi(argv[1]);
	cout << "client used port number: " << port << endl << endl;

	pwdAlgorithm = argv[2];
	if(pwdAlgorithm == "-r"){
		cout << "Using random password generation" << endl << endl;
	}
	else if(pwdAlgorithm == "-s"){
		cout << "Using systematic password generation" << endl << endl;
	}

	TASK1::BlackBoxUnsafe* Testpwd;

	srand(time(nullptr));
	TCPclient c;
	string host = "localhost";
	string msg;
	stringstream clientRequest;
	int pwdLength = 4;
	int symbSetSize = 4;
	int generatedPasswords = 50;
	int counter = 0;
	int guessedcounter = 0;
	int attempts[generatedPasswords];
	string Syspwd("");
	//connect to host
	c.conn(host , port);
	msg = "Client bereit";
	cout << "client sends:" << msg << endl;
	c.sendData(msg);
	msg = c.receive(128);
	cout << "got response:" << msg << endl;
	//sleep(1);

	while(guessedcounter < generatedPasswords){

		if(pwdAlgorithm == "-r" && (msg == "Server bereit" || msg.compare(0,15, "ACCESS ACCEPTED") == 0)){

			clientRequest << "GENERATE[" << pwdLength << "," << symbSetSize << "]";
			msg = clientRequest.str();
			clientRequest.str("");//Clearing the stringstream for further use

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			msg = c.receive(128);
			cout << "got response:" << msg << endl << endl;
			counter = 0;
			//sleep(1);
		}

		else if(pwdAlgorithm == "-r" && (msg.compare(0,19,"Passwort generiert.") == 0 || msg.compare(0,13, "ACCESS DENIED") == 0)){
			Testpwd = new TASK1::BlackBoxUnsafe(pwdLength, symbSetSize); //This needs to be in new //Mattis


			clientRequest << "CHECKPWD" << Testpwd->pwd_;//First message(CHECKPWD) tells server to check pwd, every other Symbol is the PWD
			msg = clientRequest.str();
			clientRequest.str("");//Clearing the stringstream for further use

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			delete Testpwd;
			msg = c.receive(128);
			cout << "got response:" << msg << endl << endl;
			counter++;
			if(msg.compare(0,15, "ACCESS ACCEPTED") == 0){
				attempts[guessedcounter] = counter;
				guessedcounter++;
			}

			//sleep(1);
		}

		else if(pwdAlgorithm == "-s" && (msg == "Server bereit" || msg.compare(0,15, "ACCESS ACCEPTED") == 0)){

			clientRequest << "GENERATE[" << pwdLength << "," << symbSetSize << "]";
			msg = clientRequest.str();
			clientRequest.str("");//Clearing the stringstream for further use

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			msg = c.receive(128);
			cout << "got response:" << msg << endl << endl;
			Syspwd = firstPassword(pwdLength);
			//sleep(1);
			counter = 0;
		}


		else if(pwdAlgorithm == "-s" && (msg.compare(0,19,"Passwort generiert.") == 0 || msg.compare(0,13, "ACCESS DENIED") == 0)){
			clientRequest << "CHECKPWD" << Syspwd;
			msg = clientRequest.str();
			clientRequest.str("");//Clearing the stringstream for further use

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			//delete Testpwd;
			msg = c.receive(128);
			cout << "got response:" << msg << endl << endl;

			counter++;
			Syspwd = nextPassword(Syspwd, symbSetSize, pwdLength-1);

			if(msg.compare(0,15, "ACCESS ACCEPTED") == 0){
				attempts[guessedcounter] = counter;
				guessedcounter++;
			}

			//sleep(1);
		}

		else{
			msg = "Error";

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			msg = c.receive(128);
			cout << "got response:" << msg << endl << endl;
			//sleep(1);
		}

	}


	c.sendData("BYEBYE");
	msg = c.receive(128);
	cout << "got response:" << msg << endl;
	if(pwdAlgorithm == "-r"){
			cout << "Using random password generation" << endl << endl;
	}
	else if(pwdAlgorithm == "-s")
	{
			cout << "Using systematic password generation" << endl << endl;
	}

	cout << "amount of guessed passwords:" << guessedcounter << endl << endl;
	for(int i = 0; i < generatedPasswords; i++){
		cout << "Benötigte Versuche für Passwort Nummer " << i+1 << ": " << attempts[i] << endl;
	}
	cout << endl;
	int totalguesses = 0;
	for(int i = 0; i < generatedPasswords; i++){
			totalguesses += attempts[i];
		}
	cout << "Total amount of guesses:" << totalguesses << endl;
	cout << "Mean time per password:" << totalguesses/guessedcounter << endl;

}

string firstPassword(int length){
	char firstpwd[length];
	for(int i = 0; i < length; i++)
	{
		firstpwd[i] = TASK1::SYMBOLS[0];
	}
	return firstpwd;
}

string nextPassword(string oldPassword, int symbSize, int currentPosition)
{
	if(currentPosition < 0)
	{
		return oldPassword;
	}
	char workChar = oldPassword[currentPosition];
	if(workChar == TASK1::SYMBOLS[symbSize-1]) 		//Overflow
	{
		workChar = TASK1::SYMBOLS[0];
		oldPassword[currentPosition] = workChar;
		return nextPassword(oldPassword, symbSize, currentPosition-1);
	}

	int index;
	for(int i = 0; i<TASK1::SYMBOLS.length(); i++)			// Add one
	{
		if(workChar == TASK1::SYMBOLS[i])
		{
			index = i+1;
			break;
		}
	}
	if(index == TASK1::SYMBOLS.length())
	{
		index = index-1;						//HACK
	}
 	oldPassword[currentPosition] = TASK1::SYMBOLS[index];
	return oldPassword;
}
