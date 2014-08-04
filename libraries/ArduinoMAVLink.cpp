#include "ArduinoMAVLink.h"
#include "include/mavlink/v1.0/common/mavlink.h"        // Mavlink interface
#include "include/mavlink/v1.0/ardupilotmega/ardupilotmega.h"

//char ArduinoMAVLink::_receive_buffer[_ML_MAX_BUFF]; 
status_callback ArduinoMAVLink::cb_status;

ArduinoMAVLink::ArduinoMAVLink(SoftwareSerial * serial)
{
  telemSwserial = serial;
}

ArduinoMAVLink::ArduinoMAVLink(HardwareSerial * serial)
{
  telemHwSerial = serial;
}

void ArduinoMAVLink::begin(uint16_t baud)
{
  if ( telemSwserial )
    telemSwserial->begin(baud);
  else
    telemHwSerial->begin(baud);

  delay(10);
}

void ArduinoMAVLink::SetStatusCallback( status_callback callback )
{
  cb_status = callback;
}

#define HEART_BEAT_COMFIRM 2
#define INITIAL_TIMEOUT 30000
bool ArduinoMAVLink::Initialize()
{
  error = ML_ERR_NONE;
  unsigned long timer = millis();
  
  Stream * _serial;
  if ( telemSwserial )
    _serial = telemSwserial;
  else
    _serial = telemHwSerial;
    
  mavlink_message_t msg;
  mavlink_status_t status;
  mavlink_heartbeat_t hb;
  
  uint8_t bufIdx = 0;
  uint8_t hb_count = 0;
  
  while (hb_count < HEART_BEAT_COMFIRM)
  {  
    unsigned long elapsed_time = millis() - timer;
    StatusCallback( ML_INITIAL, elapsed_time );
    if ( elapsed_time > INITIAL_TIMEOUT )      
    {
      error = ML_ERR_TIMEOUT;
      return false;
    }
    
    while (_serial->available() > 0) 
    {
      uint8_t c = _serial->read();
      //_receive_buffer[bufIdx++] = c;
      
      // Try to get a new message
      if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) 
      {
        // Handle message    
        switch(msg.msgid)
        {
          case MAVLINK_MSG_ID_HEARTBEAT:
          {
            
            //char * offset = _receive_buffer+6;
            //memcpy( &hb, offset, sizeof(mavlink_heartbeat_t) );
            mavlink_msg_heartbeat_decode(&msg, &hb);
            if ( hb.type != MAV_TYPE_GCS )
            {
              if ( hb_count == 0 )
              {
                mavlinkversion = hb.mavlink_version;
                type = hb.type;
                autopilot = hb.autopilot;
                sysid = msg.sysid;
                compid = msg.compid;
                recvpacketcount = msg.seq;
              }
              else
              {
                if ( sysid != msg.sysid || compid != msg.compid )
                  hb_count--;
              }
              hb_count++;
            }
          }
          break;
          default:
          //Do nothing
          break;
        }
        //bufIdx = 0;
      }
      // And get the next one
      //if ( bufIdx >= _ML_MAX_BUFF )
      //  bufIdx = 0;
      
    }
  }
  
  return hb_count==HEART_BEAT_COMFIRM;  
}

void ArduinoMAVLink::StatusCallback( uint8_t status, uint32_t msg )
{
  if ( cb_status )
    cb_status(status, msg);
}

static uint8_t packbuffer[MAVLINK_MAX_PACKET_LEN];

// 0:Stabilize,1:Acro,2:AltHold,3:Auto,4:Guided,5:Loiter,6:RTL,7:Circle,9:Land,10:OF_Loiter,11:Drift,13:Sport
void ArduinoMAVLink::SetMode( uint8_t fltMode )
{
  mavlink_set_mode_t mode;
  
  mode.target_system = sysid;
  mode.base_mode = MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;
  mode.custom_mode = fltMode;  
  
  mavlink_message_t msg;
	mavlink_msg_set_mode_encode( 255/*sysid*/, MAV_COMP_ID_MISSIONPLANNER/*compid*/, &msg, &mode);
    
  uint16_t len = mavlink_msg_to_send_buffer(packbuffer, &msg);
  
  Write( packbuffer, len );
}

void ArduinoMAVLink::FlyHere( double lat, double lon, double alt )
{
  mavlink_mission_item_t wp;
  wp.target_system = sysid;
  wp.target_component = compid; // MSG_NAMES.MISSION_ITEM

  wp.command = MAV_CMD_NAV_WAYPOINT;

  wp.current = 2; // 0 = no , 2 = guided mode
  wp.autocontinue = 1;

  wp.frame = MAV_FRAME_GLOBAL_RELATIVE_ALT;
  wp.y = lon;
  wp.x = lat;
  wp.z = alt;

  wp.param1 = 0;
  wp.param2 = 0;
  wp.param3 = 0;
  wp.param4 = 0;

  wp.seq = 0; // waypoint index at 0
  
  mavlink_message_t msg;
  mavlink_msg_mission_item_encode(255/*sysid*/, MAV_COMP_ID_MISSIONPLANNER/*compid*/, &msg, &wp);
  uint16_t len = mavlink_msg_to_send_buffer(packbuffer, &msg);
  
  Write( packbuffer, len );
}

void ArduinoMAVLink::Write( uint8_t * buffer, uint16_t length )
{
  Stream * _serial;
  if ( telemSwserial )
    _serial = telemSwserial;
  else
    _serial = telemHwSerial;
  
  _serial->write(buffer, length);
}

bool ArduinoMAVLink::Received()
{
  bool got = false;
  Stream * _serial;
  if ( telemSwserial )
    _serial = telemSwserial;
  else
    _serial = telemHwSerial;
    
  mavlink_message_t msg;
  mavlink_status_t status;
      
  while (_serial->available() > 0) 
  {
    uint8_t c = _serial->read();
    
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) 
    {
      // Handle message    
      switch(msg.msgid)
      {
        case MAVLINK_MSG_ID_HEARTBEAT:
        {
        }
        break;
        case MAVLINK_MSG_ID_COMMAND_LONG:
        // EXECUTE ACTION
        break;
        default:
        //Do nothing
        break;
      }
      
      got = true;
    }
    // And get the next one
  }
  
  return got;
}

/*
void loop() {
        // The default UART header for your MCU 
 int sysid = 20;                   ///< ID 20 for this airplane
    int compid = MAV_COMP_ID_IMU;     ///< The component sending the message is the IMU, it could be also a Linux process
    int type = MAV_TYPE_QUADROTOR;   ///< This system is an airplane / fixed wing
 
// Define the system type, in this case an airplane
    uint8_t system_type = MAV_TYPE_FIXED_WING;
    uint8_t autopilot_type = MAV_AUTOPILOT_GENERIC;
 
    uint8_t system_mode = MAV_MODE_PREFLIGHT; ///< Booting up
    uint32_t custom_mode = 0;                 ///< Custom mode, can be defined by user/adopter
    uint8_t system_state = MAV_STATE_STANDBY; ///< System ready for flight
    // Initialize the required buffers
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
 
// Pack the message
    mavlink_msg_heartbeat_pack(sysid,compid, &msg, type, autopilot_type, system_mode, custom_mode, system_state);
 
// Copy the message to the send buffer
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
 
// Send the message with the standard UART send function
// uart0_send might be named differently depending on
// the individual microcontroller / library in use.
     delay(1000);
     mySerial.write(buf, len);
     comm_receive();
}
*/
       

