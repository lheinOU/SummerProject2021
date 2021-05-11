# SummerProject2021
Repository for our Summer Project
 
 # Code Breakdown
 
   ## Constants
   ```C++
   //Constants
  const int PiezoInputPin = A3;
   ```
  The input pin for the piezo element is preset based upon which input was physically used. 
  It should be noted that the piezo element needs to use one of the analog pins on the arduino board.
  
  ## Variables
   ```C++
  //Variables
  int kPattern[5] = { 0,1,-1,1,1 };
  int tPattern[5] = { 0,1,1,-1,-1 };

  int lastKnockDelta = NULL;
  int lastTimeDelta = NULL;

  int lastKnock = NULL;
  int currKnock = NULL;
  int lastTime = NULL;
  int currTime = NULL;
  int currStep = 0;

  bool DoorOpen;
  ```
   - kPattern & tPattern (arrays): 
      - The knock (force) and time pattern deltas to be matched
      - 1 = harder/faster, -1 = softer/slower
   - currKnockDelta & currTimeDelta:
      - deltas found to compare to patterns
   - lastKnock && lastTime:
      - sensor & time values from previous step 
      - used to determine deltas
   - currKnock && currTime:
      - sensor & time values from current step 
      - used to determine deltas
   - curStep
      - Keeps track of which index in array to compare for pattern matching
   - DoorOpen
      - Boolean that controls if arduino reads sensor values in loop
      - currently unused 
 ## Setup
  ```C++
  void setup() {
	  Serial.begin(9600);
  }
  ```
  All this function currently does is start up the serial port. It will be used more when we need to initate servo motors for door locks. 
 ## Loop
  ```C++
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
  ```
  So long as *DoorOpen* is not true, the loop function will:
  - Grab a timestamp (in milliseconds)
  - Grab a sensor reading 
  - Assign global variables for sensor and time
  - runs functions to get deltas
  - runs function to compare deltas to pattern

 ## Current Delta
  ```C++
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
			lastKnockDelta = 0;
			lastKnock = input;
			break;
		case 't':
			//t = TIME
			lastTimeDelta = 0;
			lastTime = input;
			break;
		}
		currStep = 1;
		return NULL;
	}
}
  ```
 The *CurrentDelta* function takes in:
 - a character representing knock or time reading
 - the input reading 
 - the input from the previous step
 - the previous delta to see if this delta is harder/faster or softer/shorter
 
 First, it checks *lastDelta* to see if we are in the first step. If it is the first round, the function sets intial values to generate deltas in the next round, as well as returning a NULL value for the *PatternMatcher* function.
 If it is not the first loop, a delta(integer) is generated from taking the *input* - *lastInput*.
 If this delta is larger than the last delta it assigns a 1 value (harder/faster) to the variable. Else, it will assign a -1 to signify a smaller delta (softer/shorter)
 
 ## PatternMatcher
   ```C++
void PatternMatcher(int kDelta, int tDelta, int step) {
	//check to make sure there is something to compare
	if (kDelta != NULL || tDelta != NULL) {
		if (step < sizeof(kPattern + 1)) {
			//Check if both patterns are matched
			if (kDelta == kPattern[step] && tDelta == tPattern[step]) {				
				lastKnock = currKnock;
				lastTime = currTime;
        lastKnockDelta = kDelta;
        lastTimeDelta = tDelta;
				step++;
			}
			else {
				Serial.println("Failure...Reseting...");
				//Failure, reset
				currStep = 0;
				lastKnock = NULL;
				lastKnockDelta = NULL;
				lastTime = NULL;
				lastTimeDelta = NULL;
				
			}
		}
		else {
			//Completed all steps
			OpenLock();
		}
	}
	else { return; }
	
}
  ```
  The deltas generated in the step above are used as variables in the *PatternMatcher* function. 
  First, this function checks to see if the values returned null from the above function (*CurrentDelta*).
  If there are values, the fucntion will check to make sure we have not already completed all of the steps.
  If *currSteps >= The Size of the Pattern Array*, then we have completed the pattern and the *OpenLock* function is called. 
  
  So long as we are not finished, nor null values are presented, this function will compare each delta to the required value in the pattern arrays. 
  If not, the steps reset and we return to the begining of the process. If yes, It rotates the deltas to the *last variables* for the next round and increases the step counter.
  
  ## DoorOopen
  ```C++
  void OpenLock() {
	  //TODO Lock controls
  }
  ```
  
  Will be used to control lock mechanism. 
