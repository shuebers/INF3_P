# INFORMATIK 3 PROJEKT 
## PROJECT: PASSWORD CRACKER
## Description:
1. A password gets generated and encrypted on a server.  
2. Then a client sends its unencrypted password to the server.  
3. The server compares both passwords and sends back if it was the right password.  
4. The clients tries new passwords until it gets the right one. 

## Manual:
 `Make all` to compile the code
 `Make doc` to make more detailed Doxygen documentation
 `./Server [port]` to open local server on port
 `./Client [port] [pwd_length] [symb_amount] [-s or -r]` connect client programm to server and start password cracking attempts.

## Project Goals
### Client
* [X] Send data from client to server
* [X] Tell server to generate password of specified lenght and specified characters
* [X] Generate password on client
* [X] Send password to server until passwords match
* [X] Counter for the amount of tries
### Server
* [X] Get responses from server
* [X] Generate random encrypted password with sepcified lenght and characters
* [X] Check if passwords match and send response back to client
* [X] Delete current password

### Documentation
* [X] Header with team details
* [X] Task
* [X] Link to git
* [X] Description of program
* [X] UML
* [X] Net protocol chart
* [X] Programm testing with charts
* [X] Evaluation of testing

#### Credits:
Source code for **INFORMATIK 3 PROJEKT** by **SIMON H. & MATTIS T.**
