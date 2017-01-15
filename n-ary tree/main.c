#include<stdio.h>
#include"node.h"
int main()
{
	int choice;
	NODE  *root;
	FILE *fp;
	printf("We are going to enter nodes in N ary Tree \n");
	printf("Enter 1 for input from terminal \n");
	printf("Enter 2 for input from file \t");
	scanf("%d",&choice);
	switch(choice)
	{	/* for taking input from terminal */
		case 1:root=createTree(stdin);
				break;
		/* for taking input from file input.txt */		
		case 2:fp=fopen("input.txt","r");
				if(fp==NULL)
					fprintf(stderr,"Error In opeining Input File \n");
				else		
					root=createTree(fp);		
				break;
		default:fprintf(stderr,"You have entered wrong choice \n");	
	}
	printf("We are going to print n ary tree \n");
	printf("Enter 1 for output in terminal \n");
	printf("Enter 2 for output in file \t");
	scanf("%d",&choice);
	switch(choice)
	{	/* for taking output to terminal */
		case 1:traverse(stdout,root);
				break;
		/*for taking output to file output.txt */		
		case 2:fp=fopen("output.txt","w");
				if(fp==NULL)
					fprintf(stderr,"Error In opeining Output File \n");
				else		
					traverse(fp,root);		
				break;
		default:fprintf(stderr,"You have entered wrong choice \n");	
	}
	return 0;
}