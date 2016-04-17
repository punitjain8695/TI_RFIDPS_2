char TagCode[12];  //  for incoming serial data (12 byte RFID Tag Code via Serial Input (RXD) )
int Rate = 1; // Rate per meter
int Passenger_Count = 0; //  People inside the Vehicle
char Passenger_Number[10]; //  Passenger number
int Passenger_Status[10]; //  Passenger Status [HIGH for Inside, LOW for outside]
char Passenger_TagCode[10][12]; //  RFID Tag Code of Each Individual
char in_count = 0, out_count = 0;
long Passenger_Balance[10]; // Balance of Each passenger
int Distance=0;
int Passenger_Enter[10]; // Distance at which a particular passenger entered the vehicle
int Passenger_Fare[10]; // Balance of Each passenger
int passset=0; // To check if the last read RFID Tag is in the vehicle
int CurrentNumber = 0; //To store the Passenger Number whose card has been read while Exiting
int c=0;

// Include application, user and local libraries
#include "Energia.h"
#include "SPI.h"
#include "LCD_SharpBoosterPack_SPI.h"
#define LEDR RED_LED  // Defining the RED LED as LEDR
#define LEDG GREEN_LED  // Defining the GREEN LED as LEDG

LCD_SharpBoosterPack_SPI myScreen;

void setup() {
  
  //LCD Initialization start
  #if defined(__MSP430__)
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    
#elif defined(__LM4F120H5QR__)
    SPI.Select(2);
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV128); 
#endif
    
    myScreen.begin();
    
    myScreen.setFont(0);  // Sets the font Size to Small
    myScreen.text(5, 11, "Radio Frequency");
    myScreen.text(7, 22, "Identification");
    myScreen.text(15, 33, "( R F I D )");
    myScreen.text(7, 44, "based Modified");
    myScreen.text(7, 55, "Public Vehicle");
    myScreen.text(14, 66, "Fare Payment");
    myScreen.text(31, 77, "System");
    myScreen.flush();
   
    delay(2500);
    myScreen.clear();
    myScreen.setFont(0);  // Sets the font Size to Small
    myScreen.text(5, 5, "Passenger #: " );
    myScreen.text(5, 35, "Mtrs: " );
    myScreen.text(5, 55, "Fare: " ); 
    myScreen.text(5, 75, "Bal: " ); 
    myScreen.flush();
          
  //LCD Initialization end
  
    pinMode(LEDR, OUTPUT); // Declaring the Pin to which the LED is connected as OUTPUT Pin
    pinMode(LEDG, OUTPUT); // Declaring the Pin to which the LED is connected as OUTPUT Pin
  
	Serial.begin(9600);	// opens serial port, sets data rate to 9600 bps

    for(int i=0;i<10;i++)
          {
            Passenger_Balance[i] = 5000;
          }

}

void loop() {
int passet = 0;
Distance=(millis()*14)/1000;
String dist =  String(Distance, DEC);
myScreen.setFont(0);  // Sets the font Size to Small
          myScreen.text(10, 85, dist); // Prints the Distance travelled
          myScreen.flush(); 

	// send data only when you receive data:
	if (Serial.available() > 0) {
          digitalWrite(LEDG, HIGH);
         
          Serial.readBytes(TagCode, 12);  // read the incoming 12 bytes of the RFID TAG
          
          myScreen.setFont(0);  // Sets the font Size to Small
          myScreen.text(10, 21, TagCode); // Prints the value of the Tag Code of Last Passenger Entered
          //myScreen.text(80, 1, in_count); 
          myScreen.flush(); 
          
          for (int i=0;i<10;i++)
          {
            c=0;
            for(int j=0;j<13;j++)
            {
            if(TagCode[j]==Passenger_TagCode[i][j])
            {
            c++;
            }
            else
            {
            break;
            }
            }
          if(c>12)
          {
            digitalWrite(LEDR, HIGH);
            passet=1;
            out_count=i;
            goto passset1;
          }
          else
          {
            passet=0;
          }
          }
          
          
          
          if(passet==0) //  Passenger Enters (Tag read for 1st time)
          {
          for(int i=0;i<13;i++)
          {
            Passenger_TagCode[in_count][i] = TagCode[i];
          }
          Passenger_Number[in_count] = in_count;
          Passenger_Enter[in_count] = Distance;
          
          myScreen.setFont(0);  // Sets the font Size to Small
          String temp =  String(Passenger_Number[in_count], DEC);
          myScreen.text(80, 5, temp); // Prints the Passenger Number
          
          
          in_count++;
          passet=3;
          }
          
          passset1:
          if(passet==1)
          {
          int tempdistance = Distance-Passenger_Enter[out_count];          
          Passenger_Fare[out_count] = tempdistance*Rate/5;
          Passenger_Balance[out_count] = Passenger_Balance[out_count]-Passenger_Fare[out_count];
          
          myScreen.setFont(0);  // Sets the font Size to Small
          String tempdist =  String(tempdistance, DEC);
          myScreen.text(38, 35, "       ");
          myScreen.text(38, 35, tempdist); // Prints the Distance Travelled by the Passenger
          myScreen.setFont(1);  // Sets the font Size to Large
          String tempfare =  String(Passenger_Fare[out_count], DEC);
          myScreen.text(38, 50, "     ");
          myScreen.text(38, 50, tempfare); // Prints the Fare Deducted from account of the Passenger
          //myScreen.flush();
          //myScreen.setFont(0);  // Sets the font Size to Large
          String tempbal =  String(Passenger_Balance[out_count], DEC);
          myScreen.text(35, 70, "     "); 
          myScreen.text(35, 70, tempbal); // Prints the Balance of account of the Passenger
          myScreen.flush();
          
          for(int i=0;i<13;i++)
          {
            Passenger_TagCode[out_count][i] = '\0';
          }
          
          Passenger_Enter[out_count]=0;
          Passenger_Fare[out_count]=0;
          passet=3;
          }

digitalWrite(LEDR, LOW);

digitalWrite(LEDG, LOW);

}
}
