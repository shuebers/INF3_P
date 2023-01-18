/*! \mainpage Documentation of PROJEKT: PASSWORDCRACKER
 \section intro_sec Introduction
  How the passwordcracker works:  
  * Server: You create a local server.  
   The server can be accessed over the port defined when launching
   the server.  
   It can create a random encrypted password when it is told so. The length and possible characters are defined in the command you sent to the server.
  * Client: You create a client programm that accesses the server. After it asks the server to create a random encrypted password the client will then try to guess the password. Its modes for cracking are random or systematically. When used random it will generate random passwords of the same length. When used systematically it starts with AAA...A then tries AAA...B, AAA...C, AAA...D#, ... ,ZZZ...Z, until it finds the right password.
  
  * Bonjur
 \section install_sec Installation
 1. Download code
 2. Use command "make all"
  \subsection step1 Starting the Server
 * Open a  server by "./server [port]"
  \subsection step2 Starting the Client
* Start client by "./client [port] [-r or -s]"
 * etc...
 */