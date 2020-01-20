/*
reference beej
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>


#define PORT 24872

int main(int argc , char *argv[]){
		
	int sock = 0;
	struct sockaddr_in server;
	
	char Map_ID = *argv[1];
	char Source_Node = *argv[2];
	char File_Size = *argv[3];
	char blank[] = " "; 
	char buff[1024];
	char buff1[1024];
	strcpy(buff1, argv[2]);
	char buff2[1024];
	strcpy(buff2, argv[3]);
	/*strcpy(buff, argv[1]);
	strcpy(buff, argv[2]);
	strcpy(buff, argv[3]);*/
	strcpy(buff, argv[1]);
	strcat(buff, blank);
	strcat(buff, argv[2]);
	strcat(buff, blank);
	strcat(buff, argv[3]);
	
	//char *message = "sssss";
	char recemessage[2048] = {};

//for(int i = 1; i < 4; i++){
	
	//create socket
	sock = socket(AF_INET, SOCK_STREAM , 0);
	if(sock < 0){
		perror("Fail to create a Socket.");
		exit(1);
	}
	//printf("Client Socket Created Successful.\n");
	bzero(& server, sizeof(server));
	// socket connect
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(PORT);

	
	int connecterr = connect(sock, (struct sockaddr *) &server, sizeof(server));
	if(connecterr < 0){
		perror("Connection Error");
		close(sock);
		exit(1);
	}
	printf("The client is up and running\n");
	printf("The client has sent query to AWS using TCP: start vertex <%s>; map <%c>; file size <%s>.\n",buff1, Map_ID, buff2);
	// send a message to server
	//send(int socket, const char* message, message size, 0 )
	int sendwrong = send(sock, buff, strlen(buff), 0);
	
	
	if(sendwrong < 0){
		perror("Send Failed");
		close(sock);
		exit(1);
	}
	//printf("Sent Successful: %s\n", buff);
	//receive a message
	//recv(socket, char message, message size, 0)
	recv(sock, recemessage, sizeof(recemessage), 0);
	//printf("Receive Successful:%s\n", recemessage );


////////////////////////seperate///////////////////////////////////
	char node_num[1024];
	memset(node_num, 0 , sizeof(node_num));
	int intnode_num;

	char trans[1024];
	memset(trans, 0 , sizeof(trans));
	double Trans;

	int NODENUMBER_label[1024];
	memset(NODENUMBER_label, 0 , sizeof(NODENUMBER_label));

	float tp[1024];
	memset(tp, 0 , sizeof(tp));

	float toal_delay[1024];
	memset(toal_delay, 0 , sizeof(toal_delay));

	int min_length[1024];
	memset(min_length, 0 , sizeof(min_length));

	char *q;
	q = strtok(recemessage, " ");
	
	//printf("%s\n", p );
	
	//copy *type to type buffer[] 
	strcpy(node_num, q);
	//printf("%s\n", node_num );
	intnode_num = atoi(node_num);
////////////////////////////////////
	q = strtok(NULL, " ");
	if(q){
		strcpy(trans, q);
		//printf("trans,%s\n", trans );
	}
	Trans = atof(trans);
/////////////put original node label(int)////////////////////
	for(int i = 0; i < intnode_num; i++){
		q = strtok(NULL, " ");
		if(q){
			NODENUMBER_label[i] = atoi(q);//////put the strtok in the matrix		
		//printf("node label,%d\n", NODENUMBER_label[i] );
		}
	}
/////////////////put Tt delay(double)///////////////////////////////////////
	for(int i = 0; i < intnode_num; i++){
		q = strtok(NULL, " ");
		if(q){
		tp[i] = atof(q);//////put the strtok in the matrix		
		//printf("Tp,%5.2f\n", tp[i] );
		}
	}
/////////////put total delay////////////////////////
	for(int i = 0; i < intnode_num; i++){
		q = strtok(NULL, " ");
		if(q){
		toal_delay[i] = atof(q);//////put the strtok in the matrix		
		//printf("total,%5.2f\n", toal_delay[i] );
		}
	}
///////////////////////////////////////////////////////////////////////	
	/////////put total delay////////////////////////
	for(int i = 0; i < intnode_num; i++){
		q = strtok(NULL, " ");
		if(q){
		min_length[i] = atoi(q);//////put the strtok in the matrix		
		//printf("min_length%d\n", min_length[i] );
		}
	}


	printf("The client has received results from AWS:\n");
	printf("-----------------------------------------\n" );
	printf("Destination\tMin length\tTt\tTp\tDelay\n");
	printf("-----------------------------------------\n");
	for(int i = 0; i < intnode_num; i++){
		if(NODENUMBER_label[i] != atoi(buff1)){
			printf("%d\t\t\t%d\t%.2f\t%.2f\t%.2f\n",NODENUMBER_label[i],min_length[i], Trans, tp[i], toal_delay[i]);
		}
		else{
			
		}
	}
	printf("-----------------------------------------\n");
	
	close(sock);
//}
	return 0;
	
}
