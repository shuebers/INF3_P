/*
 * TASK1.C
 *
 *  Created on: 10.09.2019
 *      Author: aml
 */

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "TASK1.H"
#include "SHA256.H"

namespace TASK1{



string BlackBoxUnsafe::getSymbols(){
	return string(charSymbArray_);
};

BlackBoxUnsafe::BlackBoxUnsafe(int pwdLength, int symbSetSize){
	if((symbSetSize < 1) || (symbSetSize > SYMBOLS.length()) ){
		lengthSymbArray_ = SYMBOLS.length();
	}else{
		lengthSymbArray_ = symbSetSize;
	}
	// extract and save the subset of valid symbols
	charSymbArray_ = new char [lengthSymbArray_ + 1];
	strncpy(charSymbArray_, SYMBOLS.c_str(), lengthSymbArray_);
	// create and save random password
	pwd_ = this->randomPwd(pwdLength);
	//cout << "Generated password:" << pwd_ << endl; //Mattis
	return;
}

string BlackBoxUnsafe::input(string strPwd){
	if(strPwd.compare(pwd_) == 0){
		return string("ACCESS ACCEPTED");
	}
	return string("ACCESS DENIED");
}

string BlackBoxUnsafe::randomPwd(int l){
	int symbolIdx;
	if(l < MINIMAL_PWD_LENGTH){ l = MINIMAL_PWD_LENGTH;};
	pwd_ = string("");
	for(int i=0; i < l; i++){
		symbolIdx = rand() % lengthSymbArray_;
		pwd_ += charSymbArray_[symbolIdx];
	}
	return pwd_;
}

BlackBoxSafe::BlackBoxSafe(int pwdLength, int symbSetSize) : BlackBoxUnsafe(pwdLength, symbSetSize){
	pwd_ = sha256(pwd_);
	return;
}

string BlackBoxSafe::input(string strPwd){
	string shastrPwd = sha256(strPwd);
	if(shastrPwd.compare(pwd_) == 0){
			return string("ACCESS ACCEPTED");
		}
		return string("ACCESS DENIED");
}

void demoTASK1_00(){
	string pwd("meinpassword");
	cout << "pwd   : " << pwd << endl;
	cout << "SHA256: " << sha256(pwd) << endl;
	pwd = ("einpassword");
	cout << "pwd   : " << pwd << endl;
	cout << "SHA256: " << sha256(pwd) << endl;
	return;
}

void demoTASK1_01(){
	TASK1::BlackBoxSafe bs(4,4);
	/* For understanding
	-> Creating object balckboxsafe bs with password lenght of 4 and size of password array of 4
	-> Creates random password in Blackbockunsafe end stores it in pwd_
	-> encrypts the password via SHA256 in pwd_
	*/
	cout << bs.pwd_ << endl; //Print out encrypted password

	cout << bs.input("ABAB") << endl; //compares passwords in SHA256
	return;
}

void demoTASK1_02(){
	TASK1::BlackBoxSafe bs(6,6);
	/* For understanding
	-> Creating object balckboxsafe bs with password lenght of 4 and size of password array of 4
	-> Creates random password in Blackbockunsafe end stores it in pwd_
	-> encrypts the password via SHA256 in pwd_
	*/
	cout << bs.pwd_ << endl; //Print out encrypted password

	//cout << bs.input("ABAB") << endl; //compares passwords in SHA256

	bool solved = 0; //Mattis
	int counter = 0; //Mattis
	while (solved == 0 ){ //Mattis	//Compares passwords until match
		TASK1::BlackBoxUnsafe bu(6,6); //Mattis //Generates random character password
		counter ++; //Mattis
		if (bs.input(bu.pwd_) == string("ACCESS ACCEPTED")){ //Mattis //Compares random characters to password until it matches
			cout << bs.input(bu.pwd_) << ", Password:" << bu.pwd_ << ", Amount of Tries:" << counter << endl; //Mattis //Show password
			solved = 1; // Mattis //Stop loops
		}
	}

	return;
}
}


