#ifndef _NODE_H_
#define _NODE_H_
#endif
/* Structure Declaration for NODE of N ary Tree */
typedef struct _NODE
{
	int value;
	struct _NODE * parent;
	struct _NODE * child;
	struct _NODE * sibling;
}NODE;
/*prevents the C++ compiler from mangling the function name*/
#ifdef __cplusplus
extern "C" {
#endif
	/* Function Declarations */
	NODE* createNode(int data);
	void insertChild(FILE *fp,NODE* inode);
	void traverse(FILE *fp,NODE* start);
	NODE* createTree(FILE *fp);
#ifdef __cplusplus
		}
#endif		