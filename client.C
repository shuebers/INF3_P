/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

/**
 *
 * \file client.C
 *
 * \brief Contains the code to generate and crack passwords
 * on a server
 *
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
	int pwdLength;
	int symbSetSize;

	if(argc < 2){
		cout << "Client port number missing." << endl;
		exit(0);
	}
	else if(argc < 3){
		cout << "Length of password missing." << endl;
		exit(0);
	}
	else if(argc < 4){
		cout << "Size of alphabet missing." << endl;
		exit(0);
	}
	else if(argc < 5){
			cout << "Method of password generation missing." << endl;
			exit(0);
		}
	else if(argc > 5){
		cout << "too many arguments." << endl;
		exit(0);
	}
	port = atoi(argv[1]);
	cout << "client used port number: " << port << endl;

	pwdLength = atoi(argv[2]);
	if(pwdLength < 4){
		cout << "password needs to be at least 4 symbols long." << endl;
		exit(0);
	}
	if(pwdLength > 10){
			cout << "password can only be 10 symbols long." << endl;
			exit(0);
		}

	symbSetSize = atoi(argv[3]);

	if(symbSetSize < 1){
			cout << "Size of Alphabet needs to be at least 1." << endl;
			exit(0);
		}
	if(symbSetSize > TASK1::SYMBOLS.length()){
			cout << "Size of alphabet is limited to " << TASK1::SYMBOLS.length() << " symbols." << endl;
			exit(0);
		}

	pwdAlgorithm = argv[4];
	if(pwdAlgorithm == "-r"){
		cout << "Using random password generation" << endl << endl;
	}
	else if(pwdAlgorithm == "-s"){
		cout << "Using systematic password generation" << endl << endl;
	}
	else{
		cout << "Use '-r' for random password generation or '-s' for systematic password generation!" << endl;
		exit(0);
	}

	cout << "Length of password: " << pwdLength << endl;
	cout << "Size of Alphabet used: " << symbSetSize << endl << endl;

	TASK1::BlackBoxUnsafe* Testpwd;

	srand(time(nullptr));
	TCPclient c;
	string host = "localhost";
	string msg;
	stringstream clientRequest;
	int generatedPasswords = 50;
	int counter = 0;
	int guessedcounter = 0;
	int attempts[generatedPasswords];
	string Syspwd("");
	//connect to host
	c.conn(host , port);
	msg = "Client ready";
	cout << "client sends:" << msg << endl;
	c.sendData(msg);
	msg = c.receive(128);
	cout << "got response:" << msg << endl;
	sleep(4);

	while(guessedcounter < generatedPasswords){

		if(pwdAlgorithm == "-r" && (msg == "Server ready" || msg.compare(0,15, "ACCESS ACCEPTED") == 0)){

			clientRequest << "GENERATE[" << pwdLength << "," << symbSetSize << "]";
			msg = clientRequest.str();
			clientRequest.str("");//Clearing the stringstream for further use

			//cout << "client sends:" << msg << endl;
			c.sendData(msg);
			msg = c.receive(128);
			//cout << "got response:" << msg << endl << endl;
			counter = 0;
		}

		else if(pwdAlgorithm == "-r" && (msg.compare(0,19,"Passwort generated.") == 0 || msg.compare(0,13, "ACCESS DENIED") == 0)){
			Testpwd = new TASK1::BlackBoxUnsafe(pwdLength, symbSetSize);


			clientRequest << "CHECKPWD" << Testpwd->pwd_;//First message(CHECKPWD) tells server to check pwd, every other Symbol is the PWD
			msg = clientRequest.str();
			clientRequest.str("");//Clearing the stringstream for further use

			//cout << "client sends:" << msg << endl;
			c.sendData(msg);
			delete Testpwd;
			msg = c.receive(128);
			//cout << "got response:" << msg << endl << endl;
			counter++;
			if(msg.compare(0,15, "ACCESS ACCEPTED") == 0){
				attempts[guessedcounter] = counter;
				guessedcounter++;
			}

		}

		else if(pwdAlgorithm == "-s" && (msg == "Server ready" || msg.compare(0,15, "ACCESS ACCEPTED") == 0)){

			clientRequest << "GENERATE[" << pwdLength << "," << symbSetSize << "]";
			msg = clientRequest.str();
			clientRequest.str("");//Clearing the stringstream for further use

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			msg = c.receive(128);
			cout << "got response:" << msg << endl << endl;
			Syspwd = firstPassword(pwdLength);
			counter = 0;

			//sleep(5);
		}


		else if(pwdAlgorithm == "-s" && (msg.compare(0,19,"Passwort generated.") == 0 || msg.compare(0,13, "ACCESS DENIED") == 0)){
			clientRequest << "CHECKPWD" << Syspwd;
			msg = clientRequest.str();
			clientRequest.str("");//Clearing the stringstream for further use

			//cout << "client sends:" << msg << endl;
			c.sendData(msg);
			msg = c.receive(128);
			//cout << "got response:" << msg << endl << endl;

			counter++;
			Syspwd = nextPassword(Syspwd, symbSetSize, pwdLength-1);

			if(msg.compare(0,15, "ACCESS ACCEPTED") == 0){
				attempts[guessedcounter] = counter;
				guessedcounter++;
			}

			//sleep(5);
		}

		else{
			msg = "Error";

			cout << "client sends:" << msg << endl;
			c.sendData(msg);
			msg = c.receive(128);
			cout << "got response:" << msg << endl << endl;
		}

	}


	c.sendData("BYEBYE");
	msg = c.receive(128);
	cout << "got response:" << msg << endl << endl;
	if(pwdAlgorithm == "-r"){
			cout << "Using random password generation" << endl << endl;
	}
	else if(pwdAlgorithm == "-s")
	{
			cout << "Using systematic password generation" << endl << endl;
	}

	cout << "Length of passwords:" << pwdLength << endl;
	cout << "Size of alphabet used:" << symbSetSize << endl;
	cout << "amount of guessed passwords:" << guessedcounter << endl << endl;
	for(int i = 0; i < generatedPasswords; i++){
		cout << "Attempts needed to crack password number " << i+1 << ": " << attempts[i] << endl;
	}
	for(int i = 0; i < generatedPasswords; i++){
			cout << attempts[i] << endl;
		}
	cout << endl;
	int totalguesses = 0;
	for(int i = 0; i < generatedPasswords; i++){
			totalguesses += attempts[i];
		}
	cout << "Total amount of guesses:" << totalguesses << endl;
	cout << "Mean time per password:" << totalguesses/guessedcounter << endl;

}
/**
 *
 * \fn firstPassword
 *
 * \brief generates the first password of the systematic cracker
 *
 */
string firstPassword(int length){
	char firstpwd[length];
	for(int i = 0; i < length; i++)
	{
		firstpwd[i] = TASK1::SYMBOLS[0];
	}
	return firstpwd;
}
/**
 *
 * \fn nextPassword
 *
 * \brief generates the next password in line of the systematic cracker
 *
 */
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
