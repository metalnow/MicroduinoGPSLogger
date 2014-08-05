#include "Arduino.h"
#include <SoftwareSerial.h>

#define _ML_MAX_BUFF 256 // buffer size

// Auto Pilot modes
// ----------------
#define STABILIZE 0                     // hold level position
#define ACRO 1                          // rate control
#define ALT_HOLD 2                      // AUTO control
#define AUTO 3                          // AUTO control
#define GUIDED 4                        // AUTO control
#define LOITER 5                        // Hold a single location
#define RTL 6                           // AUTO control
#define CIRCLE 7                        // AUTO control
#define LAND 9                          // AUTO control
#define OF_LOITER 10                    // Hold a single location using optical flow sensor
#define DRIFT 11                        // DRIFT mode (Note: 12 is no longer used)
#define SPORT 13                        // earth frame rate control
#define FLIP        14                  // flip the vehicle on the roll axis
#define AUTOTUNE    15                  // autotune the vehicle's roll and pitch gains
#define POSHOLD     16                  // position hold with manual override
#define NUM_MODES   17


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
  void SetMode( uint8_t fltMode );
  bool Received(); 
  bool isArm();
  void DoFlyHere( double lat, double lon, double alt );  
  void DoArmDisarm( bool arm );
  void DoTakeoff();
  void DoLand();
  void DoRTL();
  
protected:

  SoftwareSerial * telemSwserial;
  HardwareSerial * telemHwSerial;
  
private:
  void StatusCallback( uint8_t status, uint32_t msg );
  void SendCommand( uint16_t cmd, float p1 = 0, float p2 = 0, float p3 = 0, float p4 = 0, float p5 = 0, float p6 = 0, float p7 = 0, uint8_t confirmation = 0);
  void SendNavCommand( uint16_t cmd, double lat, double lon, double alt, float p1 = 0, float p2 = 0, float p3 = 0, float p4 = 0 );
  void Write( uint8_t * buffer, uint16_t length );

  //static char _receive_buffer[_ML_MAX_BUFF];   
  
  static status_callback cb_status;
  
  uint8_t mavlinkversion;
  uint8_t type;
  uint8_t autopilot;
  uint8_t sysid;
  uint8_t compid;
  uint8_t recvpacketcount;
  uint32_t custom_mode;
  uint8_t base_mode;
  uint8_t system_status;
  
  uint8_t error;
};


       
       
       

