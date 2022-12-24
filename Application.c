#include <stdio.h>
#include <string.h>
#include "Card.h"
#include "Terminal.h"
#include "Server.h"
#include "Application.h"

void appStart(void) {
	ST_cardData_t new_card;
	ST_terminalData_t new_term;
	while (getCardHolderName(&new_card) == WRONG_NAME) {
		printf("Wrong name format!\n");
	 }
	
	while (getCardExpiryDate(&new_card) == WRONG_EXP_DATE) {
		printf("Wrong date format!\n");
	}
	
	while (getCardPAN(&new_card) == WRONG_PAN) {
		printf("Wrong PAN format!\n");
	 }
	
	while (getTransactionDate(&new_term) == WRONG_DATE) {
		printf("Wrong date format!\n");
	 }
	
	if (isCardExpired(new_card, new_term) == EXPIRED_CARD) {
		printf("Card Expired!\n");
		printf("Transaction Declined!\n");
		return;
	}
	
	while (setMaxAmount(&new_term) == INVALID_MAX_AMOUNT) {
		printf("Amount must be positive!\n");
	 }
	 
	while (getTransactionAmount(&new_term) == INVALID_AMOUNT) {
		printf("Amount must be positive!\n");
	 }
	
	if (isBelowMaxAmount(&new_term) == EXCEED_MAX_AMOUNT) {
		printf("Exceeded Max amount!\n");
		printf("Transaction Declined!\n");
		return;
	}
	
	ST_transaction_t new_transaction;
	new_transaction.cardHolderData = new_card;
	new_transaction.terminalData = new_term;
	ST_accountsDB_t new_account;
	switch (recieveTransactionData(&new_transaction))
	{
	case FRAUD_CARD:
		printf("Declined invalid account!\n");
		break;

	case DECLINED_INSUFFECIENT_FUND:
		printf("Declined insuffecient funds!\n");
		break;

	case DECLINED_STOLEN_CARD:
		printf("Declined Stolen card!\n");
		break;

	case INTERNAL_SERVER_ERROR:
		printf("Internal Server Error!\n");
		break;

	case APPROVED:
		printf("Transaction Approved!\n");
		break;

	default:
		break;
	}
	return;
}