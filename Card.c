#include <stdio.h>
#include <string.h>
#include "Card.h"


EN_cardError_t getCardHolderName(ST_cardData_t* cardData) 
{
	unsigned char name[50]={0};
	printf("Enter card name: ");
	gets(name);
	if (strlen(name) < 20 || strlen(name) > 24) {
		return WRONG_NAME;
	}
	else {
		strcpy_s(cardData->cardHolderName,sizeof(cardData->cardHolderName), name);
		return CARD_OK;
	}
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) 
{
	unsigned char EXP[10];
	printf("Enter card exipry date: ");
	gets(EXP);
	if (strlen(EXP) !=5 || EXP[2]!='/' || EXP[0]>'1' || (EXP[0]=='1' && EXP[1]>'2')) {
		return WRONG_EXP_DATE;
	}
	else {
		strcpy_s(cardData->cardExpirationDate, sizeof(cardData->cardExpirationDate), EXP);
		return CARD_OK;
	}
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	unsigned char PAN[50];
	printf("Enter card PAN: ");
	gets(PAN);
	if (strlen(PAN) < 16 || strlen(PAN) > 19) {
		return WRONG_PAN;
	}
	else {
		strcpy_s(cardData->primaryAccountNumber, sizeof(cardData->primaryAccountNumber), PAN);
		return CARD_OK;
	}
}