#include <stdio.h>
#include <string.h>
#include "Card.h"
#include "Terminal.h"
#include "Server.h"

ST_accountsDB_t accountsDatabase[255] = { {2000.0, RUNNING, "8989374615436851"} , {100000.0, BLOCKED, "5807007076043875"} ,
										  {3000.0, BLOCKED, "8989654136546542"} , {500000.0, BLOCKED, "8781954150540650"} ,
										  {4000.0, RUNNING, "9810246516847532"} , {300000.0, BLOCKED, "6549840620488651"} ,
										  {6000.0, RUNNING, "3216879846246544"} , {6000.0, RUNNING, "3216879846246544"}   ,
										  {5000.0, BLOCKED, "6519848789461654"} , {400000.0, RUNNING, "5048401202150842"}
									    };

ST_transaction_t transactionDatabase[255] = { 0 };
unsigned int counter = 0;

EN_transState_t recieveTransactionData(ST_transaction_t* transData){
	ST_accountsDB_t accountRefrence;
	if (isValidAccount(&(transData->cardHolderData), &accountRefrence) == ACCOUNT_NOT_FOUND) {
		transData->transState = FRAUD_CARD;
		return FRAUD_CARD;
	}
	if (isAmountAvailable(&transData->terminalData) == LOW_BALANCE) {
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		return DECLINED_INSUFFECIENT_FUND;
	}
	if (isBlockedAccount(&accountRefrence)== BLOCKED_ACCOUNT) {
		transData->transState = DECLINED_STOLEN_CARD;
		return DECLINED_STOLEN_CARD;
	}

	accountRefrence.balance = accountRefrence.balance - transData->terminalData.transAmount;
	accountsDatabase[counter].balance = accountsDatabase[counter].balance - transData->terminalData.transAmount;
	printf("New Balance= %.2f\n", accountsDatabase[counter].balance);
	if (saveTransaction(transData)== SAVING_FAILED) {
		transData->transState = INTERNAL_SERVER_ERROR;
		return INTERNAL_SERVER_ERROR;
	}
	else 
		transData->transState = APPROVED;
		return APPROVED;

}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence) {//errrorrrr
	unsigned int i = 0;
	for (i = 0;i <= 255;i++) {
		if (strcmp(accountsDatabase[i].primaryAccountNumber ,cardData->primaryAccountNumber)==0) {
			*accountRefrence = accountsDatabase[i];
			counter = i;
			return SERVER_OK;
		}
	}
	return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData) {
	if (termData->transAmount > accountsDatabase[counter].balance) {
		return LOW_BALANCE;
	}
	else
		return SERVER_OK;

}

EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	static unsigned int TranCounter = 0;
	if (TranCounter >= 255) {
		return SAVING_FAILED;
	}
	transData->transactionSequenceNumber = TranCounter;
	transactionDatabase[TranCounter] = *transData;
	if (getTransaction(TranCounter, transData) == TRANSACTION_NOT_FOUND) {
		printf("Transaction not found!\n");
		return SAVING_FAILED;
	}
	if (transData->transactionSequenceNumber == transactionDatabase[TranCounter].transactionSequenceNumber && transData->transState == transactionDatabase[TranCounter].transState) {
		TranCounter++;
		return SERVER_OK;
	}
	else {
		return SAVING_FAILED;
	}
	
}

EN_serverError_t getTransaction(unsigned long int transactionSequenceNumber, ST_transaction_t* transData) {
	unsigned int i = 0;
	for (i = 0;i <= 255;i++) {
		if (transactionDatabase[i].transactionSequenceNumber == transactionSequenceNumber) {
			*transData = transactionDatabase[i];
			return SERVER_OK;
		}
	}
	return TRANSACTION_NOT_FOUND;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) {
	if (accountRefrence->state == BLOCKED) {
		return BLOCKED_ACCOUNT;
	}
	else
		return SERVER_OK;
}
