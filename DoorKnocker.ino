//Constants
const int PiezoInputPin = A3;

//Variables
int kPattern[5] = { 0,1,-1,1,1 };
int tPattern[5] = { 0,1,1,-1,-1 };

int currKnockDelta = NULL;
int currTimeDelta = NULL;

int lastKnock = NULL;
int currKnock = NULL;
int lastTime = NULL;
int currTime = NULL;
int currStep = 0;

bool DoorOpen;
void setup() {
	Serial.begin(9600);
}
void loop() {
	//Check to see if Door is open
	if (!DoorOpen) {
		unsigned long time = millis();
		int sensor = analogRead(PiezoInputPin);
		currKnock = sensor;
		currTime = time;
		int currKnockDelta = CurrentDelta('k', sensor, lastKnock, lastKnockDelta);
		int currTimeDelta = CurrentDelta('t', time, lastTime, lastTimeDelta);

		PatternMatcher(currKnockDelta, currTimeDelta, currStep);
	}
}

int CurrentDelta(char pType, int input, int lastInput, int lastDelta) {
	//Check to see if we are in step 0
	if (lastDelta != NULL) {
		int x = input = lastInput;
		if (x > lastDelta) { return 1; }
		else { return -1; }
	}
	else {
		//sets intial values
		switch (pType) {
		case 'k':
			//k = KNOCK
			currKnockDelta = 0;
			lastKnock = input;
			break;
		case 't':
			//t = TIME
			currTimeDelta = 0;
			lastTime = input;
			break;
		}
		currStep = 1;
		return NULL;
	}
}

void PatternMatcher(int kDelta, int tDelta, int step) {
	//check to make sure there is something to compare
	if (kDelta != NULL || tDelta != NULL) {
		if (step < sizeof(kPattern + 1)) {
			//Check if both patterns are matched
			if (kDelta == kPattern[step] && tDelta == tPattern[step]) {				
				lastKnock = currKnock;
				lastTime = currTime; 
				step++;
			}
			else {
				Serial.println("Failure...Reseting...");
				//Failure, reset
				currStep = 0;
				lastKnock = NULL;
				currKnockDelta = NULL;
				lastTime = NULL;
				currTimeDelta = NULL;
				
			}
		}
		else {
			//Completed all steps
			OpenLock();
		}
	}
	else { return; }
	
}
void OpenLock() {
	//TODO Lock controls
}