#include "Pump.h"
#include <sstream>
#include <stdio.h>
#include <string>
#include <ctime>


// pump constructor
Pump::Pump(int pump_ID)
{
	// assign each pump with an ID
	pumpID = pump_ID;

	// create an ostringstream
	// assign each pump with a name
	std::ostringstream oss;
	oss << pumpID;
	pumpName = "Pump" + oss.str();
	pumpPS = "PS" + oss.str();
	pumpCS = "CS" + oss.str();

	// set up pipline to talk with pump and mutex
	pipeline = new CTypedPipe<struct customerInfo>(pumpName, 1);

	// create semaphores 
	EntryGate = new CSemaphore(pumpName + "EntryGate", 0, 1);
	ExitGate = new CSemaphore(pumpName + "ExitGate", 0, 1);
	Full = new CSemaphore(pumpName + "Full", 0, 1);
	Empty = new CSemaphore(pumpName + "Empty", 0, 1);

	// create datapool
	pumpDatapool = new CDataPool(pumpName, sizeof(pumpInfo));
	myPumpData = (struct pumpInfo *)(pumpDatapool->LinkDataPool());

	// datapool semaphores
	PS = new CSemaphore(pumpPS, 0, 1);
	CS = new CSemaphore(pumpCS, 1, 1);
}


// pump destructor
Pump::~Pump()
{
	// delete the created pointer to release memeory
	delete EntryGate;
	delete ExitGate;
	delete Full;
	delete Empty;

}


// pump main 
int Pump::main(void)
{
	// craete the data struct to store the info send through pipeline
	struct customerInfo currentCustomer;
	
	// each pump runs the following logic
	while (1) {
	
		// traffic logic: control the entering of the customer
		EntryGate->Signal();
		Full->Wait();

		// read from the pipeline
		pipeline->Read(&currentCustomer);
		
		printf("Pipeline %d Current user: %-*s  CreditCard: %d %d %d %d  FuelType: %d  Amount: %d \n",
			pumpID,
			MAX_NAME_LENGTH,
			currentCustomer.name,
			currentCustomer.creditCard_1,
			currentCustomer.creditCard_2,
			currentCustomer.creditCard_3,
			currentCustomer.creditCard_4,
			currentCustomer.fuelType,
			currentCustomer.fuelAmount);
		
		CS->Wait();
		
		myPumpData->pumpID = pumpID;
		strcpy_s(myPumpData->userName, currentCustomer.name);
		myPumpData->creditCard_1 = currentCustomer.creditCard_1;
		myPumpData->creditCard_2 = currentCustomer.creditCard_2;
		myPumpData->creditCard_3 = currentCustomer.creditCard_3;
		myPumpData->creditCard_4 = currentCustomer.creditCard_4;
		myPumpData->fuelType = currentCustomer.fuelType;
		myPumpData->fuelAmount = currentCustomer.fuelAmount;
		myPumpData->purchaseTime = currentCustomer.purchaseTime;
		
		PS->Signal();




		SLEEP(1000);
		
		// traffic logic: finshed pumping and exict from the waitlist
		ExitGate->Signal();
		Empty->Wait();
	}
	return 0;
}
