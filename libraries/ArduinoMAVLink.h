#include <SoftwareSerial.h>

class ArduinoMAVLink
{
public:  
  ArduinoMAVLink(SoftwareSerial * serial);
  
  void comm_receive();
protected:

  SoftwareSerial * _serial;  
};


       
       
       

