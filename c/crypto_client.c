/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "crypto.h"

int get_transaction_ID(CLIENT* clnt){
	int* result = gettransactionid_100((void*) NULL, clnt);
	if (result == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	printf("\ntransactionID: %d\n", *result);
    
	return *result;
}

int get_challenge(int transactionID, CLIENT* clnt){
	int* result = getchallenge_100(&transactionID, clnt);
	if (result == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	printf("challenge: %d\n", *result);
	return *result;
    // se existe, retorna challenge
    // senão, retorna -1
}

int get_transaction_status(int transactionID, CLIENT* clnt){
	int* result = gettransactionstatus_100(&transactionID, clnt);
	if (result == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	printf("status: %d\n", *result);
	return *result;
    // 0 se já resolvido
    // 1 se aberto
    // -1 se inválido
}

int submit_challenge(int transactionID, int ClientID, int seed, CLIENT* clnt){
	submit s;
	s.ClientID = ClientID;
	s.seed = seed;
	s.transactionID = transactionID;

	int* result = submitchallenge_100(&s, clnt);
	if (result == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	return *result;
    // 1 se resolve
    // 0 se não resolve
    // 2 se já resolvido
    // -1 se inválido
}

int get_winner(int transactionID, CLIENT* clnt){
	int* result = getwinner_100(&transactionID, clnt);
	if (result == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	printf("winner: %d\n", *result);
	return *result;
    // ClientID
    // 0 se não possui
    // -1 se inválido
}

int* get_seed(int transactionID, CLIENT* clnt){
	int* seed = getseed_100(&transactionID, clnt);
	if (seed == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	int status = get_transaction_status(transactionID, clnt);

	int challenge = get_challenge(transactionID, clnt);

	printf("seed: %d\n", *seed);
	int* result = (int*)malloc(3*sizeof(int));
	result[0] = status;
	result[1] = *seed;
	result[2] = challenge;
	return result;
    // se existe, retorna challenge
    // senão, retorna -1
}

void minerar(CLIENT* clnt){
	int transactionID = get_transaction_ID(clnt);
	int challenge = get_challenge(transactionID, clnt);
	int ClientID = 1;

	for (int i = 0; ; i++){			
		int seed = i;
		printf("Seed: %d\n", i);
		int resp = submit_challenge(transactionID, ClientID, seed, clnt);
		if(resp == 1){
			printf("Voce venceu o desafio %d\n\n", transactionID);
			return;
		}
		else if(resp == 2){
			printf("O desafio %d ja foi solucionado\n\n", transactionID);
			return;
		}
		else if(resp == -1){
			printf("Nao ha desafios pendentes\n\n");
			return;
		}
	}
}

int main(int argc, char *argv[]){
	CLIENT *clnt;
	char *host;
	int transactionID, clientID;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];

	clnt = clnt_create (host, PROG, VERSAO, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}

	int option = 7;
	while (option != 0){
		printf("Escolha uma opcao:\n");
		printf("1: getTransactionID\n");
		printf("2: getChallenge\n");
		printf("3: getTransactionStatus\n");
		printf("4: getWinner\n");
		printf("5: getSeed\n");
		printf("6: Minerar\n");
		printf("0: Sair\n");
		scanf("%d", &option);

		switch (option){
		case 0:
			exit(0);
			break;
		case 1: 
			get_transaction_ID(clnt);
			break;
		case 2:
			scanf("%d", &transactionID);
			get_challenge(transactionID, clnt);
			break;
		case 3:
			scanf("%d", &transactionID);
			get_transaction_status(transactionID, clnt);
			break;
		case 4:
			scanf("%d", &transactionID);
			get_winner(transactionID, clnt);
			break;
		case 5:
			scanf("%d", &transactionID);
			get_seed(transactionID, clnt);
			break;
		case 6:
			minerar(clnt);
			break;
		default:
			break;
		}
	}
	
	exit (0);	

}