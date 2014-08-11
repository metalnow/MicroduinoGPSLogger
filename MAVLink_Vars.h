//MAVLink session control
static boolean      mavbeat = 0;
static float        lastMAVBeat = 0;
static boolean      waitingMAVBeats = 1;
static uint8_t      apm_mav_type;
static uint8_t      apm_mav_system; 
static uint8_t      apm_mav_component;
static boolean      enable_mav_request = 0;
static uint8_t      custom_mode = 0;                   // Navigation mode from RC AC2 = CH5, APM = CH8
static uint8_t      nav_mode = 0;               // Navigation mode from RC AC2 = CH5, APM = CH8
static uint8_t      base_mode=0;
static bool         motor_armed = 0;
static float        vbat_A = 0;                 // Battery A voltage in milivolt
static int16_t      curr_A = 0;                 // Battery A current
static int8_t       battery_remaining_A = 0;    // 0 to 100 <=> 0 to 1000

static float        apm_lat = 0;                    // latidude
static float        apm_lon = 0;                    // longitude
static uint8_t      apm_satellites_visible = 0;     // number of satelites
static uint8_t      apm_fix_type = 0;               // GPS lock 0-1=no fix, 2=2D, 3=3D

static float        apm_alt = 0;                    // altitude
static float        apm_airspeed = -1;              // airspeed
static float        apm_groundspeed = 0;            // ground speed
static uint16_t     apm_throttle = 0;               // throtle

static int8_t       apm_pitch = 0;                  // pitch from DCM
static int8_t       apm_roll = 0;                   // roll from DCM
static int8_t       apm_yaw = 0;                    // relative heading form DCM
static float        apm_heading = 0;                // ground course heading from GPS
static float        apm_climb = 0;

static float	    nav_roll = 0; // Current desired roll in degrees
static float        nav_pitch = 0; // Current desired pitch in degrees
static int16_t	    nav_bearing = 0; // Current desired heading in degrees
static int16_t	    wp_target_bearing = 0; // Bearing to current MISSION/target in degrees
static int8_t       wp_target_bearing_rotate_int = 0;
static uint16_t     wp_dist = 0; // Distance to active MISSION in meters
static uint8_t      wp_number = 0; // Current waypoint number
static float	    alt_error = 0; // Current altitude error in meters
static float        aspd_error = 0; // Current airspeed error in meters/second
static float	    xtrack_error = 0; // Current crosstrack error on x-y plane in meters

static int16_t      chan1_raw = 0;
static int16_t      chan2_raw = 0;
static uint16_t     apm_chan5_raw = 1000;
static uint16_t     apm_chan6_raw = 1000;
static uint16_t     apm_chan7_raw = 1000;
static uint16_t     apm_chan8_raw = 1000;

static uint8_t      apm_rssi = 0; //raw value from mavlink

static float        apm_windspeed = 0;
static float        apm_windspeedz = 0;
static float        apm_winddirection = 0;


