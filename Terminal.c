#include <stdio.h>
#include <string.h>
#include "Card.h"
#include "Terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	unsigned char TranDate[20];
	printf("Enter transaction date: ");
	gets(TranDate);
	if (strlen(TranDate) != 10 || TranDate[2] != '/' || TranDate[5] != '/' || TranDate[3] > '1' || (TranDate[3] == '1' && TranDate[4] > '2') || TranDate[0]>'3' || (TranDate[0]== '3' && TranDate[1] > '1')) {
		return WRONG_DATE;
	}
	else {
		strcpy_s(termData->transactionDate, sizeof(termData->transactionDate), TranDate);
		return TERMINAL_OK;
	}
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {
	unsigned char exp[6],trDate[11];
	strcpy_s(exp , sizeof(cardData.cardExpirationDate), cardData.cardExpirationDate);
	strcpy_s(trDate , sizeof(termData.transactionDate), termData.transactionDate);
	if (trDate[8] > exp[3]) {
		return EXPIRED_CARD;
	}
	else if ((trDate[8] == exp[3]) && trDate[9] > exp[4]) {
		return EXPIRED_CARD;
	}
	else if (trDate[3] > exp[0] && (trDate[8] == exp[3]) && trDate[9] == exp[4]) {
		return EXPIRED_CARD;
	}
	else if (trDate[3] <= exp[0] && (trDate[8] == exp[3]) && trDate[9] == exp[4] && trDate[4] > exp[1]) {
		return EXPIRED_CARD;
	}
	else return TERMINAL_OK;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData) {

}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	float TranAmount=0;
	printf("Enter transaction amount: ");
	scanf_s("%f", &TranAmount);
	if (TranAmount <=0) {
		return INVALID_AMOUNT;
	}
	else {
		termData->transAmount = TranAmount;
		return TERMINAL_OK;
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	if (termData->maxTransAmount < termData->transAmount) {
		return EXCEED_MAX_AMOUNT;
	}
	else
		return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {
	float MaxTranAmount = 0;
	printf("Enter max transaction amount: ");
	scanf_s("%f", &MaxTranAmount);
	if (MaxTranAmount <= 0) {
		return INVALID_MAX_AMOUNT;
	}
	else {
		termData->maxTransAmount = MaxTranAmount;
		return TERMINAL_OK;
	}
}
