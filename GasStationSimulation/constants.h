#ifndef __Constans__
#define __Constans__

#define NUMBER_OF_CUSTOMER 50
#define NUMBER_OF_PUMPS 4
#define NUMBER_OF_TANKS 4
#define TANK_SIZE 500
#define MIN_LEVEL 200
#define PUMP_RATE 0.5
#define FUEL82 82
#define FUEL87 87
#define FUEL92 92
#define FUEL97 97
#define MAX_AMOUNT 70
#define MAX_NAME_LENGTH 20


// customer information data struct
struct customerInfo {
	char name[MAX_NAME_LENGTH];
	int creditCard;
	int fuelType;
	int fuelAmount;
};

// pump information data struct
struct pumpInfo {
	int pumpID;
	char userName[MAX_NAME_LENGTH];
	int creditCard;
	int fuelType;
	int fuelAmount;
};

#endif