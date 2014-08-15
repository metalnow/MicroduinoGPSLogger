#include "../GCS_MAVLink/include/mavlink/v1.0/mavlink_types.h"
#include "../GCS_MAVLink/include/mavlink/v1.0/ardupilotmega/mavlink.h"

#define ToRad(x) (x*0.01745329252)	// *pi/180
#define ToDeg(x) (x*57.2957795131)	// *180/pi


// true when we have received at least 1 MAVLink packet
static bool mavlink_active;
static uint8_t crlf_count = 0;

static int packet_drops = 0;
static int parse_error = 0;

void DoArmDisarm( bool arm )
{
  SetMode(STABILIZE);
  SendCommand(MAV_CMD_COMPONENT_ARM_DISARM, motor_armed ? 0.0f : 1.0f );
}

void DoFlyHere( double lat, double lon, double alt )
{
  SetMode(GUIDED);
  SendNavCommand( MAV_CMD_NAV_WAYPOINT, 2, 0, lat, lon, alt );
}

void DoTakeoff()
{
  SendNavCommand( MAV_CMD_NAV_TAKEOFF, 0, 0, 0, 0, 5 /*meters*/ );
  SendNavCommand( MAV_CMD_NAV_LOITER_UNLIM, 0, 1, 0, 0, 0 /*meters*/ );
  SendCommand( MAV_CMD_MISSION_START );
  //SetMode(AUTO);
}

void DoLand()
{
  SendCommand( MAV_CMD_NAV_LAND );
//  SendNavCommand( MAV_CMD_NAV_LAND, 0, 0, 0, 0, 0 );
//  SetMode(AUTO);
}

void DoRTL()
{
  SendCommand( MAV_CMD_NAV_RETURN_TO_LAUNCH );
//  SendNavCommand( MAV_CMD_NAV_RETURN_TO_LAUNCH, 0, 0, 0, 0, 0 );
//  SetMode(AUTO);  
}

void DoLoiter()
{
  SetMode(LOITER);
}

// 0:Stabilize,1:Acro,2:AltHold,3:Auto,4:Guided,5:Loiter,6:RTL,7:Circle,9:Land,10:OF_Loiter,11:Drift,13:Sport
void SetMode( uint8_t fltMode )
{
  mavlink_set_mode_t mode;
  
  mode.target_system = sysid;
  mode.base_mode = MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;
  mode.custom_mode = fltMode;  
  
  mavlink_message_t msg;
	mavlink_msg_set_mode_encode( 255/*sysid*/, MAV_COMP_ID_MISSIONPLANNER/*compid*/, &msg, &mode);
    
  uint16_t len = mavlink_msg_to_send_buffer(packbuffer, &msg);
  
  Serial.write( packbuffer, len );
}

void SendCommand( uint16_t cmd, float p1, float p2, float p3, float p4, float p5, float p6, float p7, uint8_t confirmation )
{
  mavlink_command_long_t command;
  memset(&command, 0, sizeof(command));
  command.param1 = p1;
  command.param2 = p2;
  command.param3 = p3;
  command.param4 = p4;
  command.param5 = p5;
  command.param6 = p6;
  command.param7 = p7;
  command.command = cmd;  // MAV_CMD
  command.target_system = sysid;
  command.target_component = compid;
  command.confirmation = confirmation;
  
  if ( cmd == MAV_CMD_COMPONENT_ARM_DISARM )
    command.target_component = MAV_COMP_ID_SYSTEM_CONTROL;
    
  mavlink_message_t msg;
  mavlink_msg_command_long_encode( 255/*sysid*/, MAV_COMP_ID_MISSIONPLANNER/*compid*/, &msg, &command);
  uint16_t len = mavlink_msg_to_send_buffer(packbuffer, &msg);
  Serial.write( packbuffer, len );
}

void SendNavCommand( uint16_t cmd, uint8_t current, uint8_t index, double lat, double lon, double alt, float p1, float p2, float p3, float p4 )
{
  mavlink_mission_item_t wp;
  wp.target_system = sysid;
  wp.target_component = compid; // MSG_NAMES.MISSION_ITEM

  wp.command = cmd;

  wp.current = current; // 0 = no , 2 = guided mode, 3 = change alt
  wp.autocontinue = 1;

  wp.frame = MAV_FRAME_GLOBAL_RELATIVE_ALT;
  wp.y = lon;
  wp.x = lat;
  wp.z = alt;

  wp.param1 = p1;
  wp.param2 = p2;
  wp.param3 = p3;
  wp.param4 = p4;

  wp.seq = index; // waypoint index at 0
  
  mavlink_message_t msg;
  mavlink_msg_mission_item_encode(255/*sysid*/, MAV_COMP_ID_MISSIONPLANNER/*compid*/, &msg, &wp);
  uint16_t len = mavlink_msg_to_send_buffer(packbuffer, &msg);
  
  Serial.write( packbuffer, len );
}

void request_mavlink_rates()
{
  
    const int  maxStreams = 6;
    const uint8_t MAVStreams[maxStreams] = {MAV_DATA_STREAM_RAW_SENSORS,
        MAV_DATA_STREAM_EXTENDED_STATUS,
        MAV_DATA_STREAM_RC_CHANNELS,
        MAV_DATA_STREAM_POSITION,
        MAV_DATA_STREAM_EXTRA1, 
        MAV_DATA_STREAM_EXTRA2};
    const uint16_t MAVRates[maxStreams] = {0x02, 0x02, 0x05, 0x02, 0x05, 0x02};
    for (int i=0; i < maxStreams; i++) {
        mavlink_msg_request_data_stream_send(MAVLINK_COMM_0,
            apm_mav_system, apm_mav_component,
            MAVStreams[i], MAVRates[i], 1);
    }
    
}

void read_mavlink(){
    mavlink_message_t msg; 
    mavlink_status_t status;

    //grabing data 
    while(Serial.available() > 0) { 
        uint8_t c = Serial.read();

        /* allow CLI to be started by hitting enter 3 times, if no
        heartbeat packets have been received */
        
        if (mavlink_active == 0 && millis() < 20000 && millis() > 5000) {
            if (c == '\n' || c == '\r') {
                crlf_count++;
            } else {
                crlf_count = 0;
            }
            if (crlf_count == 3) {
                //uploadFont();
            }
        }

        //trying to grab msg  
        if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
            mavlink_active = 1;
            //handle msg
            switch(msg.msgid) {
            case MAVLINK_MSG_ID_HEARTBEAT:
                {
                  
                    mavbeat = 1;
                    apm_mav_system    = msg.sysid;
                    apm_mav_component = msg.compid;
                    apm_mav_type      = mavlink_msg_heartbeat_get_type(&msg);            
                    custom_mode = (uint8_t)mavlink_msg_heartbeat_get_custom_mode(&msg);
                    //Mode (arducoper armed/disarmed)
                    base_mode = mavlink_msg_heartbeat_get_base_mode(&msg);
                    if(getBit(base_mode,7)) motor_armed = 1;
                    else motor_armed = 0;

                    nav_mode = 0;          
                    lastMAVBeat = millis();
                    if(waitingMAVBeats == 1){
                        enable_mav_request = 1;
                    }
                }
                break;
            case MAVLINK_MSG_ID_SYS_STATUS:
                {

                    vbat_A = (mavlink_msg_sys_status_get_voltage_battery(&msg) / 1000.0f); //Battery voltage, in millivolts (1 = 1 millivolt)
                    curr_A = mavlink_msg_sys_status_get_current_battery(&msg); //Battery current, in 10*milliamperes (1 = 10 milliampere)         
                    battery_remaining_A = mavlink_msg_sys_status_get_battery_remaining(&msg); //Remaining battery energy: (0%: 0, 100%: 100)

                }
                break;

            case MAVLINK_MSG_ID_GPS_RAW_INT:
                {
                    apm_lat = mavlink_msg_gps_raw_int_get_lat(&msg) / 10000000.0f;
                    apm_lon = mavlink_msg_gps_raw_int_get_lon(&msg) / 10000000.0f;
                    apm_fix_type = mavlink_msg_gps_raw_int_get_fix_type(&msg);
                    apm_satellites_visible = mavlink_msg_gps_raw_int_get_satellites_visible(&msg);
                }
                break; 
            case MAVLINK_MSG_ID_VFR_HUD:
                {
                    apm_airspeed = mavlink_msg_vfr_hud_get_airspeed(&msg);
                    apm_groundspeed = mavlink_msg_vfr_hud_get_groundspeed(&msg);
                    apm_heading = mavlink_msg_vfr_hud_get_heading(&msg); // 0..360 deg, 0=north
                    apm_throttle = mavlink_msg_vfr_hud_get_throttle(&msg);
                    apm_alt = mavlink_msg_vfr_hud_get_alt(&msg);
                    apm_climb = mavlink_msg_vfr_hud_get_climb(&msg);
                }
                break;
            case MAVLINK_MSG_ID_ATTITUDE:
                {
                    apm_pitch = ToDeg(mavlink_msg_attitude_get_pitch(&msg));
                    apm_roll = ToDeg(mavlink_msg_attitude_get_roll(&msg));
                    apm_yaw = ToDeg(mavlink_msg_attitude_get_yaw(&msg));
                }
                break;
            case MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT:
                {
                  nav_roll = mavlink_msg_nav_controller_output_get_nav_roll(&msg);
                  nav_pitch = mavlink_msg_nav_controller_output_get_nav_pitch(&msg);
                  nav_bearing = mavlink_msg_nav_controller_output_get_nav_bearing(&msg);
                  wp_target_bearing = mavlink_msg_nav_controller_output_get_target_bearing(&msg);
                  wp_dist = mavlink_msg_nav_controller_output_get_wp_dist(&msg);
                  alt_error = mavlink_msg_nav_controller_output_get_alt_error(&msg);
                  aspd_error = mavlink_msg_nav_controller_output_get_aspd_error(&msg);
                  xtrack_error = mavlink_msg_nav_controller_output_get_xtrack_error(&msg);
                }
                break;
            case MAVLINK_MSG_ID_MISSION_CURRENT:
                {
                    wp_number = (uint8_t)mavlink_msg_mission_current_get_seq(&msg);
                }
                break;
            case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
                {
                    chan1_raw = mavlink_msg_rc_channels_raw_get_chan1_raw(&msg);
                    chan2_raw = mavlink_msg_rc_channels_raw_get_chan2_raw(&msg);
                    apm_chan5_raw = mavlink_msg_rc_channels_raw_get_chan5_raw(&msg);
                    apm_chan6_raw = mavlink_msg_rc_channels_raw_get_chan6_raw(&msg);
                    apm_chan7_raw = mavlink_msg_rc_channels_raw_get_chan7_raw(&msg);
                    apm_chan8_raw = mavlink_msg_rc_channels_raw_get_chan8_raw(&msg);
                    apm_rssi = mavlink_msg_rc_channels_raw_get_rssi(&msg);
                }
                break;
            case MAVLINK_MSG_ID_WIND:
                {
                    apm_winddirection = mavlink_msg_wind_get_direction(&msg); // 0..360 deg, 0=north
                    apm_windspeed = mavlink_msg_wind_get_speed(&msg); //m/s
                    apm_windspeedz = mavlink_msg_wind_get_speed_z(&msg); //m/s
                }
                break;
            default:
                //Do nothing
                break;
            }
        }
        
        delayMicroseconds(138);
        //next one
    }
    // Update global packet drops counter
    packet_drops += status.packet_rx_drop_count;
    parse_error += status.parse_error;

}
