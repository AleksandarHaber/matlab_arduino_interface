/* The Arduino code for establishing the communication between MATLAB and Arduino 

To illustrate the main ideas, we use an example of receiving the data for controlling the stepper motor. 
The Arduino code receives the sequence:

'ABCD>'
where 
A- is the direction- A  in {0,1}
BCD- a sequence of numbers defining the number of steps (1<steps<999)
> - is the end marker

For example, 

0345>

0- is the direction 
345- move the stepper for 345 steps

Author: Aleksandar Haber
Date: March 2017
City University of New York, College of Staten Island
*/

const int numChar=5;        // the number of characters we want to receive 
char receivedChar[numChar]; // the array for received data
boolean newData=false;      // the indicator for new data, when new data arrives and receivedChar is filled, newData=true;

int dir=0;                  // movement direction
int distance=0;             // the number of steps
char endMarker='>';         // the end marker character                 


void setup() 
{
    Serial.begin(9600);     // we need this for serial communication
}


void loop() 
{
   if(Serial.available()>0) // If something appears on the serial port, run this
   {
      record();             // fill in the array receivedChar
      execute();            // set the variables "dir" and "distance", here you can enter your code for driving the stepper motor
      sendBack();           // this is to confirm that the code is running properly, we send back to MATLAB (or to an Arduino Serial monitor) what was received
   }
}

void record()
{
  static int index=0;  // this is the index for filling in "receivedChar", this variable does not need to be static, it can be a global variable...
  char rChar;   // received character, that is an output of Serial.read() function

  while (Serial.available()>0 && newData==false)
  {
    rChar=Serial.read();
    delay(2);
    if (rChar!=endMarker && index<=4)
    { 
      receivedChar[index]=rChar;
      index++;
    }
    else
    {
       receivedChar[index]='\0';  // terminate the string 
       index=0;                   // reset the counter
       newData=true;              // set the "newData" indicator to true, meaning that the new data has arrived, and we can proceed further
    }
  }
}

void execute()
{
  // these two arrays are needed for the "atoi" function
  char tmpDir[2];   // this one stores the direction
  char tmpSteps[4]; // this one stores the number of steps
  
  
  if (newData==true)
  {
     tmpDir[0]=receivedChar[0];
     tmpDir[1]='\0';
     dir=atoi(tmpDir); //converts string to an integer,

     for(int j=0; j<=2; j++)
     {
        tmpSteps[j]=receivedChar[j+1];
     }
     tmpSteps[3]='\0';
     distance=atoi(tmpSteps);
     delay(1);
     newData=false;  
     
      // to free the buffer  (is something is still there)
      while (Serial.available()>0)   
      {
        Serial.read();
      } 

  }
}

void sendBack()
{

Serial.print(distance); // send back the distance, the number can be read from the Serial Monitor, or from MATLAB
//Serial.println(dir); // send back the direction, the number can be read from the Serial Monitor, or from MATLAB
Serial.write('\r');    // this is the 'CR' terminator for MATLAB, see the MATLAB code
}




