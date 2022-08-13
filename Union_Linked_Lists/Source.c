#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "md5.h" //using void md5String(const char* input, char* output);
#include "md4.h" //using void md4String(const char* input, char* output);

#define MD4_HASH 0
#define MD5_HASH 1
#define True 1
#define False 0
typedef unsigned char HashType;
typedef unsigned char Byte;
typedef char Bool;

typedef struct msgWithHash
{
	unsigned int id;
	char* text;
	union
	{
		Byte md4[16];
		Byte md5[16];
	} hashDigest;
	HashType type;
}msgWithHash;

typedef struct msgListNode msgListNode;
struct msgListNode
{
	msgWithHash* data;
	msgListNode* next;
};
msgWithHash* createMessage(unsigned int id, const char* text, HashType type);
void printMessage(const msgWithHash* message);
msgListNode* addMessage(msgListNode* head, msgWithHash* data);
int findMsgByHashIterative(const msgListNode* head, Byte hash[16]);
int findMsgByHashRecursive(const msgListNode* head, Byte hash[16]);
msgListNode* deleteMessage(msgListNode* head, unsigned int id);
Bool verifyIntegrity(msgListNode* head, unsigned int id, Byte compareHash[16]);
msgListNode* reverseMsgList(msgListNode* head);
void freeMsgList(msgListNode* head);
/*
void print_hash(const unsigned char* p);
*/

int main() {
	
	/******************** Example 1 from PDF ********************************************/
	
	/*const char* sentence = "this is some text that I want to hash.";
	printf("text: \"%s\"\n", sentence);
	unsigned char hash[16] = { 0 };
	md5String(sentence, hash);
	printf("md5: ");
	print_hash(hash);

	printf("md4: ");
	md4String(sentence, hash);
	print_hash(hash);*/
	
	/******************** Example 2 from PDF ********************************************/

	/*const msgWithHash* msg1 = createMessage(87651233, "", MD4_HASH);
	const msgWithHash* msg2 = createMessage(3, "banana1", MD5_HASH);
	const msgWithHash* msg3 = createMessage(275, "banana2!", MD4_HASH);
	const msgWithHash* msg4 = createMessage(342234, "Hello World", MD4_HASH);

	*/
	msgListNode* head = NULL;
	
	printf("Creating messages...\n");
	const msgWithHash* msg1 = createMessage(87651233, "", MD4_HASH);
	const msgWithHash* msg2 = createMessage(3, "banana1", MD5_HASH);
	const msgWithHash* msg3 = createMessage(275, "banana2!", MD4_HASH);
	const msgWithHash* msg4 = createMessage(342234, "Hello World", MD4_HASH);
	printf("The messages:\n");
	printMessage(msg1);
	printf("\n");
	printMessage(msg2);
	printf("\n");
	printMessage(msg3);
	printf("\n");
	printMessage(msg4);
	printf("\n");

	printf("Creating List...\n");
	head = addMessage(head, msg1);
	head = addMessage(head, msg2);
	head = addMessage(head, msg3);
	head = addMessage(head, msg4);
	printf("The List has been created.\n");
	printf("\n");

	printf("Searching the item in the list by hash message(\"banana1\").\n");
	printf("The NUMBER of item in the list(recursive): ");
	printf("%d\n", findMsgByHashRecursive(head, msg2->hashDigest.md5));
	printf("The NUMBER of item in the list(iterative): ");
	printf("%d\n", findMsgByHashIterative(head, msg2->hashDigest.md5));
	printf("\n");

	printf("Deleting the message with id 3.\n");
	deleteMessage(head, 3);
	printf("The message with id 3 has been deleted from the list.\n");
	printf("\n");

	printf("Searching the message by the id (3) and his hash message.\n");
	printf("The result:");
	printf("%d\n", verifyIntegrity(head, 3, msg2->hashDigest.md4));
	printf("\n");

	printf("Reversing the list...\n");
	head = reverseMsgList(head);
	printf("The list has been reversed.\n");
	printf("\n");

	printf("Trying to free memory of the list...\n");
	freeMsgList(head);
	printf("List memory has been freed.\n");
	
}

void print_hash(const unsigned char* p) {
	for (unsigned int i = 0; i < 16; ++i) {
		printf("%02x", p[i]);
	}
	printf("\n");
}

msgWithHash* createMessage(unsigned int id, const char* text, HashType type)
{

	msgWithHash* newMessage = (msgWithHash*)calloc(1, sizeof(msgWithHash));
	if (newMessage == NULL)
	{
		printf("Alocation Faield");
		exit(0);
	}

	 newMessage->text = (char*)calloc(strlen(text)+1, sizeof(char));

	if (newMessage->text == NULL)
	{
		printf("Alocation Faield");
		exit(0);
	}

	newMessage->id = id;
	strcpy(newMessage->text, text);
	newMessage->type = type;

	switch (newMessage->type)
	{
	case  MD4_HASH:
		md4String(newMessage->text, newMessage->hashDigest.md4);
		break;
	case  MD5_HASH:
		md5String(newMessage->text, newMessage->hashDigest.md5);
		break;
	default:
		printf("ERROR Please select 0 or 1.\n");
		break;
	}

	return newMessage;
}

void printMessage(const msgWithHash* message)
{

	printf("Message id: %d\n", message->id);
	printf("Message text: ");
	puts(message->text);


	switch (message->type)
	{
	case  MD4_HASH:
		printf("Md4 hash: ");
		print_hash(message->hashDigest.md4);
		
		break;
	case  MD5_HASH:
		printf("Md5 hash: ");
		print_hash(message->hashDigest.md5);
		break;
	default:
		printf("printMessage Not Working\n");
		break;
	}
}

msgListNode* addMessage(msgListNode* head, msgWithHash* data)
{
	
	msgListNode* item = (msgListNode*)calloc(1, sizeof(msgListNode));

	item->data = data;

	if (head != NULL)
		item->next = head;
	head = item;
	
	return head;
}

int findMsgByHashIterative(const msgListNode* head, Byte hash[16])
{
	msgListNode* temp = head;
	int count = 1;
	while (temp)
	{
		if (temp->data != NULL)
		{
			if (strcmp(temp->data->hashDigest.md5, hash)==0)
			{
				 return count;
			}
		}
		else
		{
			return -2;
		}
		count++;
		temp = temp->next;
	}
	return -1;
}
int findMsgByHashRecursive(const msgListNode* head, Byte hash[16])
{
	static int count = 1;
	if (head==NULL)
	{
		return -1;
	}
	else if (head->data==NULL)
	{
		return -2;
	}
	else
	{
		if (strcmp(head->data->hashDigest.md5, hash)==0)
		{
			return count;
		}
		else
		{
			count++;
			return findMsgByHashRecursive(head->next, hash);
		}
	}
}

msgListNode* deleteMessage(msgListNode* head, unsigned int id) {
	msgListNode* temp1 = head;
	msgListNode* temp2 = head->next;

	if (temp1->data->id==id)
	{
		head = temp1->next;
		free(temp1->data->text);
		free(temp1->data);
		free(temp1);
	}

	while (temp2!=NULL)
	{
		if (temp2->data->id==id)
		{
			temp1->next = temp2->next;
			free(temp2->data->text);
			free(temp2->data);
			free(temp2);
			break;
		}
		temp1 = temp2;
		temp2 = temp2->next;
	}
	return head;	
}

Bool verifyIntegrity(msgListNode* head, unsigned int id, Byte compareHash[16]) {
	msgListNode* temp = head;
	while (temp)
	{
		if (temp->data != NULL)
		{
			if (strcmp(temp->data->hashDigest.md5, compareHash) == 0 && temp->data->id==id)
			{
				return 1;
			}
		}
		else
		{
			return 0;
		}
		temp = temp->next;
	}
	return -1;
}

msgListNode* reverseMsgList(msgListNode* head) {
	msgListNode* temp1 = head;
	msgListNode* temp2 = temp1->next;
	msgListNode* temp3 = temp2->next;

		while (temp2)
		{ 
			temp2->next = temp1;
			if (!temp3)
			{
				head->next=NULL;
				head = temp2;
				break;
			}
			temp1 = temp2;
			temp2 = temp3;
			temp3 = temp3->next;			
		}	
	return head;
}

void freeMsgList(msgListNode* head) {
	msgListNode* temp1 = head;
	msgListNode* temp2 = temp1->next;
	while (temp1)
	{
			free(temp1->data->text);
			free(temp1->data);
			free(temp1);
		
		temp1 = temp2;
		if (temp2)
		{
			temp2 = temp2->next;
		}
		
	}
}