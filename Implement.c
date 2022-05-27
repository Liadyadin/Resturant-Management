#include "Header.h"

void Getlost(char* str, FILE* out) //function for error and exit
{
	fprintf(out, "%s", str);
	exit(1);
}
Kitch* CreatProducts(Kitch* head, FILE* in, FILE* out) //creating Manot(Nodes in a list)
{
	int flag = 0;
	Kitch* tmp;
	char* Help[MAX]; //for dynamic memory-str
	int quan, cost; //cost - for total cost
	char prem;
	while (fscanf(in, "%s %d %d %c", Help, &quan, &cost, &prem) != EOF) 
	{
		tmp = (Kitch*)malloc(sizeof(Kitch)); //dynamic memory for kitch
		if (tmp == NULL)
		{
			SendmefreeManot(tmp); //release memory
			Getlost("Can not allocate memory\n", out);
		}
		if (quan < 0 || cost < 0) //isnt legal
		{
			fprintf(out, "Error input\n");
			free(tmp);
			flag++;
		}
		if (flag == 0)
		{
			tmp->quantity = quan;
			tmp->premium = prem;
			tmp->price = cost;
			tmp->productname = (char*)malloc(1 + strlen(Help) * sizeof(char)); //create Node
			if (tmp->productname == NULL) 
			{
				while (head != NULL) //if the list isnt empty
				{
					tmp = head;
					head = head->next;
					free(tmp->productname);
					free(tmp);
				}
			}
			if (checkname(head, Help) >= 0) //if dish is already in the menu
			{
				fprintf(out, "%s is already in the menu\n", Help);
				flag++;
				free(tmp);
			}
			else
			{ //added A node in a linked list
				strcpy(tmp->productname, Help);
				tmp->next = head;
				head = tmp;
			}
		}
		flag = 0;
	}
	if (head != NULL)
		fprintf(out, "The kitchen was created!\n");
	else
		fprintf(out, "Kitchen creation has failed\n");
	return head;
}


int checkname(Kitch* head, char* str) //function for check if this dish is existent in the kitchen and return index of numtable
{
	int i = 0;
	Kitch* tmp;
	tmp = head;
	while (tmp != NULL)
	{
		if (strcmp(tmp->productname, str) == 0)
			return i;
		tmp = tmp->next;
		i++;
	}
	return (-1);
}
int checknameTable(PT head, char* str) //function for check if this dish is existent in the table and return index of numtable
{
	int i = 0;
	PT tmp;
	tmp = head;
	
	while (tmp != NULL)
	{
		if (strcmp(tmp->Dishname, str) == 0)
			return i;
		tmp = tmp->next;
		i++;
	}
	return (-1);
}
Kitch* AddItems(Kitch* head, char* Prodname, int quan, FILE* out)//Add a product to an existing dish in the kitchen
{
	Kitch* prev;
	prev = head;
	int i, n;
	i = checkname(head, Prodname);
	n = i; //n - index of this product
	if (i < 0)
		fprintf(out, "%s does not exist!", Prodname);
	else
	{
		for (i = 0; i < n; i++) 
			prev = prev->next;
		prev->quantity = prev->quantity + quan; //added quantity to an exsistent dish
		fprintf(out, "%d %s were added to the kitchen\n", quan, Prodname);
	}
	return head;
}
float OrderItem(Kitch* head, manage* table, int tablenum, char* Prodname, int quan,char pre, FILE* out) //invite dishes to the table
{
	PT PTtmp=table[tablenum-1].head1; 
	Kitch* temp;
	int i;
	float cost; //total cost after order
	
	if ((checknameTable(table[tablenum - 1].head1, Prodname) < 0)) //if this dish isn't existent in the table 
	{
		PTtmp = (PT)malloc(sizeof(T)); //memory allocation for T
		if (PTtmp == NULL)
		{
			SendmefreeTable(table, tablenum); //release memory
			Getlost("Memory allocation has failed!\n", out);
		}
		PTtmp->Dishname = (char*)malloc((1 + strlen(Prodname)) * sizeof(char)); //allocation memory for DishName
		if (PTtmp->Dishname == NULL)
		{
			SendmefreeTable(table, tablenum); //release memory
			Getlost("Memory allocation has failed!\n",out);
		}
		strcpy(PTtmp->Dishname, Prodname);

		PTtmp->q = quan;
		PTtmp ->pre = pre;
		//added Node to linked list
		PTtmp->next = table[tablenum - 1].head1;
		PTtmp->prev = NULL;

		if (table[tablenum - 1].head1 != NULL) //if the list isn't empty
		{
			table[tablenum - 1].head1->prev = PTtmp;
			PTtmp->next = table[tablenum - 1].head1;
		}
		table[tablenum - 1].head1 = PTtmp;
		i = checkname(head, Prodname);
		temp = head;
		while (i>0)
		{
			temp = temp->next;
			i--;
		}
		PTtmp->price = temp->price; 
	}
	else
	{
		i = checkname(head, Prodname);
		temp = head;
		while (i>0)
		{
			temp = temp->next;
			i--;
		}
		while (PTtmp->Dishname != Prodname)
		{
			PTtmp = PTtmp->next;
		}
		PTtmp->q = PTtmp->q + quan; //update qantity
	}
	table->tail = NULL; //update tail --> NULL
	cost=(float)((temp->price) * (PTtmp->q)); //calculating total cost
	fprintf(out, "%d %s were added to table number %d\n", quan, Prodname, tablenum);
	return cost; 
}
float RemoveItem(manage* table, int tablenum, char* todel, int qtodel, FILE* out) //remove dishes from the table
{
	PT ptr;
	PT tmp= table[tablenum - 1].head1;
	while (strcmp (tmp->Dishname, todel)!=0) //check where is a dishName in the list
	{
		tmp = tmp->next;
	}
	if (tmp->q < qtodel) //if quantity to delete is greater then number of dishes in the table
	{
		fprintf(out, "Quantity to delete is greater then number of dishes in the table\n");
		return 0;
	}
	tmp->q = tmp->q - qtodel; //update qantity if it is OK
	if (tmp->q == 0) //in case the number of dishes is 0 now and we need delete this Node 
	{
		if (tmp->prev == NULL) //delete - top of the list
		{
			ptr = tmp->next;
			ptr->prev = NULL;
			free(tmp);
			table[tablenum - 1].head1 = ptr;
			fprintf(out, "%d %s was returned to the kitchen from table number %d\n", qtodel, todel, tablenum);
		}
		else if (tmp->next == NULL)//delete - end of the list
		{
			ptr = tmp;
			ptr = ptr->prev;
			ptr->next = NULL;
			free(tmp);
			fprintf(out, "%d %s was returned to the kitchen from table number %d\n", qtodel, todel, tablenum);
		}
		else //delete- within the list
		{
			ptr = tmp;
			ptr = ptr->prev;
			ptr->next = ptr->next->next;
			ptr->next->prev = ptr;
			free(tmp);
			fprintf(out, "%d %s was returned to the kitchen from table number %d\n", qtodel, todel, tablenum);
		}
	}
	else 
		fprintf(out, "%d %s was returned to the kitchen from table number %d\n", qtodel, todel, tablenum);
	return (qtodel * tmp->price);  //calculating total cost after remove dishes
}
void Removetable(manage* table, int tablenum, FILE* out)  //invite bill and close this table
{
	int flag = 0;
	float total;
	PT tmp = table[tablenum - 1].head1;
	while (tmp != NULL)
	{
		if (tmp->pre == 'Y') //if this is a Premium dish
		{
			flag++;
		}
		tmp = tmp->next; //promote the list
	}
	total = table[tablenum - 1].bill; 
	if (flag > 0)
	{
		float todec = 0.0;
		todec = total / 10.0; //calculating 10% added for this table
		total = total + todec; //add to total cost 
		fprintf(out, "You invite Premium meal ");
	}
	else
		fprintf(out, "You didn't invite Premium meal ");
	tmp = table[tablenum - 1].head1;
	while (tmp != NULL)
	{
		fprintf(out, "%d %s\n", tmp->q, tmp->Dishname);
		tmp = tmp->next;
	}
	fprintf(out, "%g NIS please!\n", total);
	SendmefreeTable(table, tablenum); //release memory
}
void SendmefreeTable(manage* table, int tablenum) //release memory for specific table
{
	PT tmp = table[tablenum - 1].head1;
	PT ptr;
	while (tmp != NULL)
	{
		ptr = tmp;
		tmp = tmp->next;
		free(ptr->Dishname);
		free(ptr);
	}
}
void SendmefreeManot(Kitch* kit)  //release memory for Manot
{
	Kitch* tmp = kit;
	Kitch* ptr;
	while (tmp != NULL)
	{
		ptr = tmp;
		tmp = tmp->next;
		free(ptr->productname);
		free(tmp);
	}
}
