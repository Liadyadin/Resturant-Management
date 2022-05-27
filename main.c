#include "Header.h"

int main()
{
	FILE* inst; //file for Instructions
	FILE* in; //file for Manot
	FILE* out; //file for Output
	Kitch* head, *tmp;
	PT tbl;
	manage headers[TAB]; 
	char dish[MAX];
	int i, whichfunc, addquan, flag = 0, tablenum;
	head = NULL;
	for (i = 0; i < TAB; i++) //initialization for the tables
	{
		headers[i].head1 = NULL;
		headers[i].tail = NULL;
		headers[i].bill = 0;
	}

	in = fopen("Manot.txt", "r"); //for reading
	if (in == NULL) //if the file isn't existent
	{
		Getlost("Can not open file!",out);
	}
	out = fopen("Pelet.txt", "wt"); //for writing
	if (out == NULL) //if the file isn't existent
	{
		Getlost("Can not open file!",out);
	}
	inst = fopen("Instructions.txt", "r"); //for reading
	if (inst == NULL) //if the file isn't existent
	{
		Getlost("Can not open file!",out);
	}
	while (fscanf(inst, "%d", &whichfunc) != EOF) //loop on Instructions file
	{
		if ((whichfunc == 1) && (flag == 0)) //function for creating Manot(Nodes in a list) 
		{
			head = CreatProducts(head, in, out);
			fclose(in);
			flag = 1; //for Once
		}
		if (whichfunc == 2) //Add a product to an existing dish in the kitchen
		{
			fscanf(inst, "%s %d", dish, &addquan);
			if (addquan < 0) //in case number less than 0
				fprintf(out, "%d can not be added as a quantity!");
			else
				AddItems(head, dish, addquan,out);
		}

		if (whichfunc == 3) //function to invite dishes to the table
		{
			fscanf(inst, "%d %s %d", &tablenum, dish, &addquan);
			i = checkname(head, dish); //function for check if this dish is existent in the kitchen' and return index of numtable
			if (i < 0) //in case function returned -1
				fprintf(out, "Sorry, we dont have %s\n", dish);
			else if ((tablenum-1 > TAB) || (tablenum-1 < 0)) //in case number isn't existent
				fprintf(out, "Sorry, this table does not exist\n");
			else 
			{
				tmp = head;
				while (i > 0)
				{
					tmp = tmp->next;
					i--;
				}
				if (addquan < 0 || tmp->quantity < addquan) //in case quantity isn't a legel
					fprintf(out, "Sorry, there are not enough %s dishes\n", dish);
				else //invite dishes to the table
				{
					headers[tablenum-1].bill = headers[tablenum - 1].bill+ OrderItem(head, headers,tablenum, dish, addquan,tmp->premium,out);
					tmp->quantity = tmp->quantity - addquan; //update quantity
				}
			}
		}
		if (whichfunc == 4) //function to remove dishes from the table
		{
			fscanf(inst, "%d %s %d", &tablenum, dish, &addquan);
			tbl = headers[tablenum - 1].head1;
			if (tbl == NULL) //in case table has not ordered yet 
			{
				fprintf(out, "table %d has not ordered yet\n", tablenum);
			}
			else if (checknameTable(tbl, dish) < 0) //function for check if this dish is existent in the table and return index of numtable
				fprintf(out, "%s does not exist in table %d!\n",dish, tablenum);
			else if (checkname(head, dish) < 0)
				fprintf(out, "%s does not exist in the kitchen\n",dish, tablenum);
			else //call the function and update bill
			{
				headers[tablenum - 1].bill = headers[tablenum - 1].bill - RemoveItem(headers, tablenum, dish, addquan,out);
			}
		}
		if (whichfunc == 5) //invite bill and close this table
		{
			fscanf(inst, "%d", &tablenum);
			tbl = headers[tablenum - 1].head1;
			if (tbl == NULL)
			{
				fprintf(out, "table %d does not exist\n", tablenum);
			}
			else //free memory and bill
			Removetable(headers, tablenum,out);
		}
	} //close the files 
	fclose(in);
	fclose(inst);
	fclose(out);
	return 0;
}