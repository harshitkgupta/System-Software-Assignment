#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
#include"node.h"

/* Function for allocating Memory for node in run time */
NODE* createNode(int data)
{
	NODE* temp=(NODE*)malloc(sizeof(NODE));
	temp->value=data;
	temp->parent=temp->child=temp->sibling=NULL;
	return temp;
}
/* Function for insering  nodes in N ary Tree */
void insertChild(FILE* fp,NODE* inode)
{
	int k,value,i;
	NODE *temp,*new;
	if(fp==stdin)
		fprintf(stdout,"enter the number of children for the  node which has value %d\n  ",inode->value);
	else 
		fscanf(fp,"%d",&value);
	fscanf(fp,"%d",&k);
	if(k==0) return;
	if(fp==stdin)
		fprintf(stdout,"enter values for children \n");
	for(i=0;i<k;i++)
	{
		fscanf(fp,"%d",&value);
		new=(NODE*)createNode(value);
		if(inode->child==NULL)
		{
			inode->child=new;
			new->parent=inode;
		}
		else
		{	temp=inode->child;
			while(temp->sibling!=NULL)
			{
				temp=temp->sibling;
			}
			temp->sibling=new;
			new->parent=temp->parent;
		}

	}
}
/* Function for displaying all nodes in N ary Tree */
void traverse(FILE *fp,NODE* start)
{
	NODE *temp;
	if(start==NULL) return;
	fprintf(fp,"\n node=%d ",start->value);
	fprintf(fp,"parent=");
	if(start->parent)
	{	fprintf(fp,"%d ",start->parent->value);
	
		temp=start->parent->child;
	}	
	else	
		temp=start;
	if(temp)
	{	fprintf(fp," sibling=");
	
		while(temp)
		{	if(temp!=start)
			fprintf(fp,"%d ",temp->value);
			temp=temp->sibling;
		}
	}
	fprintf(fp," children=");
	temp=start->child;
	while(temp)
	{
		fprintf(fp,"%d ",temp->value);
		temp=temp->sibling;
	}	
	traverse(fp,start->child);
	traverse(fp,start->sibling);
}
/*Function for creating N ary Tree */
NODE* createTree(FILE *fp)
{
	int value;
	NODE *root,*temp;
	if(fp==stdin)
		fprintf(stdout,"enter the  value for the root node \n");
	fscanf(fp,"%d",&value);
	root=(NODE*)createNode(value);

	temp=root;
	while(temp!=NULL)
	{
		insertChild(fp,temp);
		if(temp->child)
			temp=temp->child;
		else if(temp->sibling)
			temp=temp->sibling;
		else 
		{	
			while(temp->parent->sibling==NULL)
			{
			temp=temp->parent;
			if(temp==root)
				break;
			}
			if(temp!=root)
			temp=temp->parent->sibling;
			else
				temp=NULL;
		}	
	}
	return root;

}