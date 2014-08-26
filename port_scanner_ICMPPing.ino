/****************************************************/
// Small Arduino Portable Port Scanner
// by Marco Ramilli, http://marcoramilli.blogspot.com
// Updated by Mike Papadopoulos 08/19/2014
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <ICMPPing.h> // https://github.com/BlakeFoster/Arduino-Ping

byte ip[] = {};
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//byte local_IP[] = {Ethernet.localIP()};
byte basenetwork[] = { 192,168,0,3 }; // Test a Class C network, put here the NetID, doesn't matter what HostID you choose.
String msg;
EthernetClient client;
int led = 9;
File myFile;
IPAddress pingAddr();

SOCKET pingSocket = 0;

char buffer [256];
ICMPPing ping(pingSocket, (uint16_t)random(0, 255));


void setup()
{
  pinMode(10, OUTPUT);
  if(!SD.begin(4)){
    return;
  }
  pinMode(led, OUTPUT); //pin selected to control LED
  Serial.begin(9600);
  delay(1000);
  Serial.println("DHCP Querying");

  //DHCP Settings ...
  if(!getNC()){
    Ethernet.begin(mac, ip );
    Serial.println("Setting Default Network Configurations");
  }
}
void loop()
{
  Serial.print("pinging...");
  printArray(&Serial, ".", basenetwork, 4, 10);
  msg="Server: X.X.X.";
  msg += (basenetwork[3]); //building the log string
  
  ICMPEchoReply echoReply = ping(basenetwork, 4);
  if (echoReply.status == SUCCESS)
  {
    Serial.println("Host Alive");
    for(int port=2; port<= 100; port++) // set port range
    { 
      if (client.connect(basenetwork, port)) 
      {
        Serial.print("Port:"); 
        Serial.print(port); 
        Serial.println(" *OPEN* ");
        digitalWrite(led, HIGH); // set pin 9 low
        delay(500);
        digitalWrite(led, LOW); // set pin 9 low
        msg += (" Port:");
        msg += (port);
        msg +=(" OPEN ");
      } 
      client.flush();
      client.stop();
   }
  Serial.println(msg);
  //myFile = SD.open("popeye/scan.txt", FILE_WRITE);
  //if(myFile) {
   //myFile.println(msg);
   //myFile.close();
   //}
  }
  else // can be remove, only for debugging
  {
    Serial.println("Host unreachable");
  }
  if(basenetwork[3] <= 254){
    Serial.println("Calculating new Address");
    basenetwork[3] = basenetwork[3] + 1;
  }
  else{
    Serial.println("Resetting Address");
    basenetwork[3] = 1;
  }
  
}
//DHCP client
int getNC(){
  int result = Ethernet.begin(mac);
  if (result == 1){
    byte buffer[6];
    Serial.print("ip address: ");
    Serial.print(Ethernet.localIP());
    Serial.println("");
    Serial.print("READY \n");
    return 1;
  }
  else{
    Serial.print("No DHCP, Running in default conf");
    return 0;
  }
}
//printArray funciton
void printArray(Print *output, char* delimeter, byte* data, int len, int base){
  char buf[10] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0  };
  for(int i = 0; i < len; i++)
  {
    if(i != 0)
      output->print(delimeter);
    output->print(itoa(data[i], buf, base));
  }
  output->println();
}
