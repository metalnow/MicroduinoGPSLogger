#include "Arduino.h"
#include <SoftwareSerial.h>

#define _ML_MAX_BUFF 64 // buffer size

typedef enum
{
  ML_ERR_NONE = 0,
  ML_ERR_TIMEOUT  = 1,
} ML_Error;

typedef enum
{
  ML_NONE = 0,
  ML_INITIAL = 1,
} ML_Status;

typedef void (*status_callback) (uint8_t, uint32_t);

class ArduinoMAVLink
{
public:  
  ArduinoMAVLink(SoftwareSerial * serial);
  ArduinoMAVLink(HardwareSerial * serial);
  
  void begin(uint16_t baud); 
  void SetStatusCallback( status_callback callback );  
  bool Initialize();
  void FlyHere( double lat, double lon, double alt );  
  void SetMode( uint8_t fltMode );
  bool Received(); 
  
protected:

  SoftwareSerial * telemSwserial;
  HardwareSerial * telemHwSerial;
  
private:
  void StatusCallback( uint8_t status, uint32_t msg );
  void Write( uint8_t * buffer, uint16_t length );

  static char _receive_buffer[_ML_MAX_BUFF];   
  
  static status_callback cb_status;
  
  uint8_t mavlinkversion;
  uint8_t type;
  uint8_t autopilot;
  uint8_t sysid;
  uint8_t compid;
  uint8_t recvpacketcount;
  
  uint8_t error;
};


       
       
       

