/*
 * GPX Library -- GPX.h
 * Created by: Ryan Sutton
 *
 * Copyright (c) 2010, Ryan M Sutton
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Ryan M Sutton nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

 // tag ref: http://www.topografix.com/gpx_manual.asp
 
#ifndef GPX_h
#define GPX_h

#include <WProgram.h>
#include <WString.h>

#define _GPX_HEAD "<gpx version=\"1.1\" creator=\"Arduino GPX Lib\"\n xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n xmlns=\"http://www.topografix.com/GPX/1/1\"\n xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\"\n>\n"
#define _GPX_TAIL               "</gpx>\n"
#define _GPX_META_HEAD          "<metadata>"
#define _GPX_META_TAIL          "</metadata>\n"
#define _GPX_TRAK_HEAD          "<trk>"
#define _GPX_TRAK_TAIL          "</trk>\n"
#define _GPX_TRKSEG_HEAD        "<trkseg>"
#define _GPX_TRKSEG_TAIL        "</trkseg>\n"
#define _GPX_PT_HEAD            "<TYPE lat=\""
#define _GPX_PT_TAIL            "</TYPE>\n"

// Property Tags
#define _GPX_NAME_HEAD          "<name>"
#define _GPX_NAME_TAIL          "</name>\n"
#define _GPX_CMT_HEAD           "<cmt>"
#define _GPX_CMT_TAIL           "</cmt>\n"
#define _GPX_DESC_HEAD          "<desc>"
#define _GPX_DESC_TAIL          "</desc>\n"
#define _GPX_SYM_HEAD           "<sym>"
#define _GPX_SYM_TAIL           "</sym>\n"
#define _GPX_ELE_HEAD           "<ele>"
#define _GPX_ELE_TAIL           "</ele>\n"
#define _GPX_SRC_HEAD           "<src>"
#define _GPX_SRC_TAIL           "</src>\n"
#define _GPX_TIME_HEAD          "<time>"
#define _GPX_TIME_TAIL          "</time>\n"
#define _GPX_SPEED_HEAD         "<speed>"
#define _GPX_SPEED_TAIL         "</speed>\n"
#define _GPX_SAT_HEAD           "<sat>"
#define _GPX_SAT_TAIL           "</sat>\n"
#define _GPX_FIX_HEAD           "<fix>"
#define _GPX_FIX_TAIL           "</fix>\n"

// 'Public' Tags
#define GPX_TRKPT               "trkpt"
#define GPX_WPT                 "wpt"
#define GPX_RTEPT               "rtept"

class GPX{
  public:
    GPX();
    String getOpen();
    String getClose();
    String getMetaData();
    String getTrakOpen();
    String getTrakClose();
    String getTrakSegOpen();
    String getTrakSegClose();
    String getInfo();
    String getName();
    String getTime();
    String getSpeed();
    String getSat();
    String getFix();
    String getPt(String typ, String lon, String lat);
    String getPt(String typ, String lon, String lat, String ele);
    void setMetaName(String name);
    void setMetaDesc(String desc);
    void setName(String name);
    void setDesc(String desc);
    void setEle(String ele);
    void setSym(String sym);
    void setSrc(String src);
    void setTime(String time);
    void setCmt(String cmt);
    void setSpeed(String speed);
    void setSat(String sat);
    void setFix(String fix);
    void clear();
  private:
    //Variables
    String _metaName;
    String _metaDesc;
    String _name;
    String _desc;
    String _ele;
    String _sym;  // routepoint symbol
    String _src;
    String _time;
    String _cmt; // GPS comment of the trackpoint
    String _speed;  //Units: meters per second
    String _sat; //Number of satellites used to calculate the GPS fix. (not number of satellites in view).

//  Fix must be one of:
//    none No fix
//    2d position only
//    3d position and elevation
//    dgps DGPS
//    pps Military signal used    
    String _fix;

    //Functions
    String wrapCDATA(String input);
};

#endif

