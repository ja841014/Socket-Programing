/*
reference: 
		beej website
		HASPMAP in C kaushikbaruah.com
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

#define PORT 21872
#define MAXLINE 1000
#define INT_MAX 2147483647
char target[10] = {};
char Source_Node[1024] = {};
int source_node;
char message[1024] = {};


//////////////////////////build up the hashmap////////////////////////
struct node{
	int key;
	int val;
	struct node *next;
};
struct table{
	int size;
	struct node **list;
};
struct table *createTable(int size){
	struct table *t = (struct table*)malloc(sizeof(struct table));
	t -> size = size;
	t -> list = (struct node**)malloc(sizeof(struct node*)*size);
	int i;
	for(i = 0; i < size; i++){
		t -> list[i] = NULL;
	}
	return t;
}
int hashCode(struct table *t, int key){
	if(key < 0){
		return -(key%t -> size);
	}
	return key%t -> size;
}
void insert(struct table *t, int key, int val){
	int pos = hashCode(t,key);
	struct node *list = t -> list[pos];
	struct node *newNode = (struct node*)malloc(sizeof(struct node));
	struct node *temp = list;
	while(temp){
		if(temp -> key == key){
			temp -> val = val;
			return;
		}
		temp = temp -> next;
	}
	newNode -> key = key;
	newNode -> val = val;
	newNode -> next = list;
	t -> list[pos] = newNode;
}
int lookup(struct table *t, int key){
	int pos = hashCode(t, key);
	struct node *list = t -> list[pos];
	struct node *temp = list;
	while(temp){
		if(temp -> key == key){
			return temp -> val;
		}
		temp = temp -> next;
	}
	return -1;
}
///////////////////////////////////////////////////////////////////////
int isletter(char c){
	return (((int)c >= 97 && (int)c<= 122) || ((int)c >= 65 && (int)c <= 90));
}
///////////////////////////////////////////////////////////////////////

int buff5[1024];
int count_vertex = 0;
int min_length[1024];

void dijkstra(){
	//int source_node;
	int receive_node;// need to change to the receive one
	int first_node;
	int second_node;
	int edge_weight;
	int total_node;
	
	char blank[] = " ";
	int vertexnum = 0 ;
	int num;
	int alt_node[1024];
	memset(alt_node,0,sizeof(alt_node));
	int temp[1024];
	memset(temp,0,sizeof(temp));

	int count = 0;
	int k[100];
	memset(k,0,sizeof(k));
	char *value_start;
	//char *vertex_m[][];
	int buff2;
	char buff[100];
	FILE *tagetmap;
	char *token;

	tagetmap = fopen("targetmap.txt","r");
	if(NULL == tagetmap){
		perror("Open Failed");
		exit(1);
	}
	fgets(buff,1024,tagetmap); //ignore MAPID
	memset(buff,0,sizeof(buff));
	
	fgets(buff,1024,tagetmap); // Tt propagation info

	strcpy(message, buff);
	strcat(message, blank);
	fgets(buff,1024,tagetmap); //Tp info
	strcat(message, buff);
	strcat(message, blank);
	
	//code below is correct
	while(fscanf(tagetmap, "%d", &num) != EOF){
		//fscanf(tagetmap, "%s", buff);
		//token = strtok(NULL, " ");
		k[count] = num; // get the map int value
		//memset(buff,0,sizeof(buff));
		//printf("%s\n", token);
/////		printf("%d\n", k[count]);		
		count++;
		if(feof(tagetmap)){
			break;
		}
	}

	// all nodes are saved
	for(int i = 0; i < (count)/3; i++){

		int j = 3;
		int q = 3;

		q = q * i;	
		j = j * i + 1;	
		temp[i*2] = k[q];
		temp[i*2+1] = k[j];
		//alt_node[i] = i;

		// build cost matrix
	//	cost_matrix[k[first_node]][k[second_node]] = k[edge_weight];

	}


	//printf("%u\n",cost_matrix );
	// bubble sort	
	for(int i = 0; i < (2*(count))/3; i++){
		for(int l = i+1; l < (2*(count))/3; l++){
			if( i+1 < (2*count)/3 && temp[i] < temp[l]){
				int change = temp[i];
				temp[i] = temp[l];
				temp[l] = change;
			}
		}
	}
	// minus the smaller one let it be the 0
	int range = temp[(2*(count)/3)-1];

	//printf("%d\n",temp[(2*(count)/3)-1] );

	// calculate total node
	for(int i = 0; i < (2*(count))/3; i++){
		if( i + 1 < (2*(count))/3 && temp[i] != temp[i+1]){
			vertexnum++;
		}
	}
	total_node = vertexnum +1;
	////// store the node number 
	char buff4[1024];
	memset(buff4, 0, sizeof(buff4));
	sprintf(buff4, "%d", total_node);// change int to char!!
	strcat(message, buff4);
	strcat(message, blank);

////	printf("%d\n",vertexnum+1 );
	//int (*cost_matrix)[2] = (int(*)[2])malloc(sizeof(int)*6*6);
	int cost_matrix[total_node][total_node];
	memset(cost_matrix,0,sizeof(cost_matrix));

/*	for(int i = 0;i<16;i++){
		printf("%d\n", temp[i]);
	}
*/
//// initialize cost_matrix
	for(int a = 0 ; a < total_node; a++){
		for(int b = 0; b < total_node; b++){
			if(a == b){
				cost_matrix[a][b] = 0;
				//printf("%d\t%d\t%d\n", a,b,cost_matrix[a][b]);
			}
			else{
				cost_matrix[a][b] = INT_MAX;
				//printf("%d\t%d\t%d\n", a,b,cost_matrix[a][b]);
			}
			//printf("\n");
		}
		
	}
	//////////////////hashmap///////////////////////// 
	struct table *z = createTable(100);
	int x;
	int y = 0; 
	char buff3[1024];
	
	memset(buff3, 0, sizeof(buff3));
	for(int i = 0; i < count; i++){
		//printf("%d\t", i);
		if(i%3 != 2){
			//printf("%d\n", i);
			if(lookup(z, k[i]) == (-1)){
				insert(z, k[i], y);
				buff5[y] = k[i];
				//printf("%d\t%d\t%d\n",k[i],i, y);
				/////store original node label////
				sprintf(buff3, "%d", k[i]);// change int to char!!
				strcat(message, buff3);
				strcat(message, blank);
				y++;
				//printf("%d\t%d\t%d\n",k[i],i, y);
			}
			count_vertex = y;
		}
	}	
	/*for(int i = 0; i < (count/3); i++){
		printf("%d\n", lookup(z, k[i]));
	}
	*/

	//give the value to cost_matrix
	for(int i = 0; i < (count)/3; i++){
		first_node = 3*i;
		second_node = 3*i + 1;
		edge_weight = 3*i + 2;
		cost_matrix[ lookup(z,k[first_node]) ][ lookup(z, k[second_node])] = k[edge_weight];
		cost_matrix[ lookup(z,k[second_node]) ][ lookup(z, k[first_node])] = k[edge_weight];
		//cost_matrix[k[first_node]-range][k[second_node]-range] = k[edge_weight];
		//cost_matrix[k[second_node]-range][k[first_node]-range] = k[edge_weight];
	}

//examine
/*	for(int i = 0; i < total_node; i++){
		for(int j = 0; j < total_node; j++){
			//printf("cost_matrix[%d][%d] = %2d\n,",i,j,cost_matrix[i][j] );
		}
		printf("\n");
	}*/


//////////Dijkstra Algorithm////////
	//source_node = receive_node - range;// int
	//source_node = 0;
	//source_node = source_node - range;
	int t = 0;
	int flag[total_node];
	memset(flag,0,sizeof(flag));
	int dis[total_node];
	memset(dis,0,sizeof(dis));
	int min;
	int tmp;
	int prev_node[total_node];
	memset(prev_node,0,sizeof(prev_node));
	int countpp = 0;
	int counti = 0;
	source_node = lookup(z, source_node); //map the source node

	for(int i = 0; i < total_node; i++){
		dis[i] = cost_matrix[source_node][i]; //find the relationsip with others nodes 
		flag[source_node] = 1;// mark the node which is already visited

	}

	for(int i = 0; i < total_node; i++){
		//printf("%d\n",counti);
		counti++;
		
		min = INT_MAX;
		for(int k = 0; k < total_node; k++){
			//printf("%d\n", flag[countpp]);
			if( flag[countpp] == 0 && dis[countpp] < min){//find the most shortest path to start
				min = dis[countpp];
				//printf("%d\n",min );
				t=countpp;
			}
			countpp++;
		}
		countpp = 0;
		flag[t] = 1; // obtained the shortest path

		for(int j = 0; j < total_node; j++){
			tmp = (cost_matrix[t][j] == INT_MAX ? INT_MAX : (min + cost_matrix[t][j]));
			//printf("%d\n", tmp);
			//printf("%d\n", min);
			if(flag[j] == 0 && tmp < dis[j]){
				dis[j] = tmp;
				//printf("updated%d\n", tmp);
				//prev[j] = t;
			}
			//printf("do u update?\n");
		}

	}


	//////transmit the min length
	 for(int i = 0; i < total_node; i++){
	 	char str[1024];
	 	min_length[i] = dis[i];
	 	memset(str,0,sizeof(str));
	 	//printf("%d\t", dis[i]); 
	 	sprintf(str, "%d", dis[i]); //change int to char!!!
	 	strcat(message, str);
	 	strcat(message, blank);
	}
	
///////////////////////////////////////////////////

	fclose(tagetmap);
}


int map_countline = 0;
void Detmap(){
	int count1 = 0;
	int count = 0;
	int map_endline[100];
	
	int count_line = 0; 
	char s[100];
	char d[100];
	char *p;
	//char target[] = "A"; //we have to change the target which get from AWS
	int pointer = 0;
	FILE *fp;
	FILE *tagetmap;
	tagetmap = fopen("targetmap.txt","w");
	fp = fopen("map.txt","r");
	if(NULL == fp){
		perror("Open Failed");
		exit(1);
	}
	memset(s,0,sizeof(s));
	memset(d,0,sizeof(d));
	//char c = fgetc(fp);//get 1 cahracter at a time
	//feof() To examine that whether fp end it or not.
	while(1){
		if(feof(fp)){
			break;
		}
		
		fgets(s,1024,fp);
		//printf("%s\n", s); 
		//p = fgetc(fp);
		p = strstr(s, target); //compare s with target Map_id p==null => no match
		count_line++; //locate the line which map id is at

		if(p != NULL){
			if(feof(fp) == 1){
			break;
			}
			// find alphabets
			fputs(s, tagetmap);//put the data in the new file
			//printf("%s\n", s);
			
			//map_startline = count_line;
			//printf("Find the target map ID at line:%u\n", count_line);
			//printf("%ld\n",ftell(fp) );
			pointer = ftell(fp);
			while(1){ ////////////maybe has the problem/////////////
				map_countline = count1;
				//printf("map_countline%d\n",map_countline );
				fgets(s,1024,fp);

				if(feof(fp) == 1){
				break;
				}

				if(isletter(s[0]) == 1){
					break;
				}
				fputs(s, tagetmap); //put the data in the new file

				//while(fscanf(tagetmap, "%d", &num) != EOF)
				
				count1++;
				//printf("%s\n", s);
				
			}
			
		}
		else{//find the specific Map_ID
			if(feof(fp) == 1){
			break;
			}
			if(isletter(s[0]) == 1){
				//printf("you find a letter at line:%u\n", count_line);
				//printf("%ld\n",ftell(fp) );
				map_endline[count] = count_line;
				count++;
			}
			
		}
		
	}
	//Create the Map
	rewind(fp);
	fseek(fp, pointer-3, SEEK_SET);
	fgets(d,100,fp);
	//printf("%s\n", d); 
	
	fclose(tagetmap);
	fclose(fp);
}

int main(int argc, char *argv[]){
	
	int sock;
	struct sockaddr_in my_addr, remote_addr; //built my addr and remote addr.
	//int remoteAddr_len = sizeof(remote_addr);
	char recemessage[100] = {};
	//char message[] = {"Hello, client. I am server A"};
	socklen_t remoaddr;
	remoaddr =sizeof(remote_addr);
	//addr_size = sizeof()

	//Create an UDP socket
	while(1){
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		printf("Fail to create an UDP socket.\n");
		exit(1);
	}
	else{
		//printf("Create UDP Socket Successful\n");
	}
	bzero((char *)& my_addr, sizeof(my_addr));	
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	my_addr.sin_port = htons(PORT);

	//bind server address to socket
	if(bind(sock, (struct sockaddr* )&my_addr, sizeof(my_addr)) < 0){
		//perror("Bind Failed\n");
		exit(1);
	}
	else{
		printf("The Server A is up and running using UDP on port <%d>\n", PORT);
	}
	//receive the datagram
	int n = recvfrom(sock, recemessage, sizeof(recemessage), 0, (struct sockaddr* )&remote_addr, &remoaddr);
	if(n < 0){
		printf("Receive failed\n");
		exit(0);
	}
	else{
	//	printf("Receive Successful\n");
		recemessage[n] = '\0';
	//	puts(recemessage);
	}




	memset(target, 0 , sizeof(target));
	char *p;
	p = strtok(recemessage, " ");
	
	strcpy(target, p);
	//printf("%s\n", target );
	p = strtok(NULL, " ");
	if(p){
		strcpy(Source_Node, p);
		//printf("%s\n", Source_Node );
	}

	source_node = atoi(Source_Node);// change char to int!!!


	Detmap();
	dijkstra();

	printf("The Server A has constructed a list of 1 maps:\n");
	printf("----------------------------------------------\n");
	printf("Map ID\tNum Vertices\tNum Edges\n");
	printf("----------------------------------------------\n");
	printf("%s\t\t%d\t\t%d\n",target, count_vertex, map_countline-2 );
	printf("----------------------------------------------\n");
	printf("The Server A has received input for finding shortest paths: starting vertex <%s> of map <%s>\n", Source_Node, target);
 	printf("----------------------------------------------\n");
 	printf("Destination\tMin Length\n");
 	printf("----------------------------------------------\n");
	for(int i = 0; i < (count_vertex); i++){
 		printf("%d\t\t\t%d\n",buff5[i], min_length[i]);
	}
	printf("----------------------------------------------\n");
	
	//send the response
	int a = sendto(sock, message, sizeof(message), 0, (struct sockaddr* )&remote_addr, sizeof(remote_addr));
	if(a < 0){
		printf("Send Failed\n");
		exit(0);
	}
	else{
		printf("The Server A has sent shortest paths to AWS.\n");
	}

	close(sock);
	}
	
}
