#ifndef Ristorante
#define Ristorante
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAB 15 //tables max
#define MAX 101 
typedef struct KitchProd //struct for Manot Inputs
{
	char* productname;
	int price;
	int quantity;
	char premium;
	struct Kitch* next;
}Kitch;

typedef struct Table //struct for Manot per table
{
	struct Table* next;
	struct Table* prev;
	char* Dishname;
	int q;
	char pre;
	int price;
}T,*PT; //Bidirectional list

typedef struct manager //manager struct for the tables array
{
	PT head1, tail;
	float bill;
}manage;

void Getlost(char* str, FILE* out); //function for error and exit
Kitch* CreatProducts(Kitch* head, FILE* in, FILE* out); //creating Manot(Nodes in a list) 
int checkname(Kitch* head, char* str); //function for check if this dish is existent in the kitchen and return index of numtable
int checknameTable(PT head, char* str); //function for check if this dish is existent in the table and return index of numtable
Kitch* AddItems(Kitch* head, char* Prodname, int quan,FILE* out); //Add a product to an existing dish in the kitchen
float OrderItem(Kitch* head, manage* table, int tablenum, char* Prodname, int quan,char pre, FILE* out); //invite dishes to the table
float RemoveItem(manage* table, int tablenum, char* todel, int qtodel, FILE* out); //remove dishes from the table
void Removetable(manage* table , int tablenum, FILE* out); //invite bill and close this table
void SendmefreeTable(manage* table, int tablenum); //release memory for specific table
void SendmefreeManot(Kitch* kit); //release memory for Manot
#endif
