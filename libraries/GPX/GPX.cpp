/*
 * GPX Library -- GPX.cpp
 * Created by: Ryan M Sutton
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
 * DISCLAIMED. IN NO EVENT SHALL Ryan M Sutton BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <GPX.h>

GPX::GPX(){
}

//Get methods

String GPX::getOpen(){
  return String(_GPX_HEAD);
}

String GPX::getClose(){
  return String(_GPX_TAIL);
}

String GPX::getMetaData(){
  String localStr(_GPX_META_HEAD);
  if (_metaName.length() > 0){
    localStr = localStr + String(_GPX_NAME_HEAD);
    localStr = localStr + wrapCDATA(_metaName);
    localStr = localStr + String(_GPX_NAME_TAIL);
  }
  if (_metaDesc.length() > 0){
    localStr = localStr + String(_GPX_DESC_HEAD);
    localStr = localStr + wrapCDATA(_metaDesc);
    localStr = localStr + String(_GPX_DESC_TAIL);
  }
  localStr = localStr + String(_GPX_META_TAIL);
  return localStr;
}

String GPX::getTrakOpen(){
  return String(_GPX_TRAK_HEAD);
}

String GPX::getTrakClose(){
  return String(_GPX_TRAK_TAIL);
}

String GPX::getTrakSegOpen(){
  return String(_GPX_TRKSEG_HEAD);
}

String GPX::getTrakSegClose(){
  return String(_GPX_TRKSEG_TAIL);
}

String GPX::getName(){
  String localStr("");
  if (_name.length() > 0){
    localStr += _GPX_NAME_HEAD;
    localStr += wrapCDATA(_name);
    localStr += _GPX_NAME_TAIL;
  }
  return localStr;
}

String GPX::getTime(){
  String localStr("");
  if (_time.length() > 0){
    localStr += _GPX_TIME_HEAD;
    localStr += _time;
    localStr += _GPX_TIME_TAIL;
  }  
  return localStr;
}

String GPX::getSpeed(){
  String localStr("");
  if (_speed.length() > 0){
    localStr += _GPX_SPEED_HEAD;
    localStr += _speed;
    localStr += _GPX_SPEED_TAIL;
  }  
  return localStr;
}

String GPX::getSat(){
  String localStr("");
  if (_sat.length() > 0){
    localStr += _GPX_SAT_HEAD;
    localStr += _sat;
    localStr += _GPX_SAT_TAIL;
  }  
  return localStr;
}

String GPX::getFix(){
  String localStr("");
  if (_fix.length() > 0){
    localStr += _GPX_FIX_HEAD;
    localStr += _fix;
    localStr += _GPX_FIX_TAIL;
  }  
  return localStr;
}


String GPX::getInfo(){
  String localStr("");
  localStr += getName();
  localStr += getTime();
  localStr += getSpeed();
  localStr += getSat();
  localStr += getFix();

  if (_desc.length() > 0){
    localStr += _GPX_DESC_HEAD;
    localStr += wrapCDATA(_desc);
    localStr += _GPX_DESC_TAIL;
  }
  if (_ele.length() > 0){
    localStr += _GPX_ELE_HEAD;
    localStr += _ele;
    localStr += _GPX_ELE_TAIL;
  }
  if (_sym.length() > 0){
    localStr += _GPX_SYM_HEAD;
    localStr += _sym;
    localStr += _GPX_SYM_TAIL;
  }
  if (_src.length() > 0){
    localStr += _GPX_SRC_HEAD;
    localStr += wrapCDATA(_src);
    localStr += _GPX_SRC_TAIL;
  }
  if (_cmt.length() > 0){
    localStr += _GPX_CMT_HEAD;
    localStr += wrapCDATA(_cmt);
    localStr += _GPX_CMT_TAIL;
  }  
  return localStr;
}

String GPX::getPt(String typ, String lon, String lat){
  String localStr(_GPX_PT_HEAD);
  localStr.replace("TYPE",typ);
  localStr += lat + "\" lon=\""; 
  localStr += lon + "\">";
  
  localStr += getInfo();
  
  String tmpLocalTail(_GPX_PT_TAIL);
  tmpLocalTail.replace("TYPE",typ);
  localStr += tmpLocalTail;
  return localStr;
}
    
String GPX::getPt(String typ, String lon, String lat, String ele){
  setEle(ele);
  return getPt(typ, lon, lat);
}

void GPX::clear(){
  _metaName = "";
  _metaDesc = "";
  _name = "";
  _desc = "";
  _ele = "";
  _sym = "";
  _src = "";
  _time = "";
  _cmt = "";
  _speed = "";
  _sat = "";
  _fix = "";    
}

//Set Methods
void GPX::setMetaName(String name){
  _metaName = name;
}
void GPX::setMetaDesc(String desc){
  _metaDesc = desc;
}
void GPX::setName(String name){
  _name = name;
}
void GPX::setDesc(String desc){
  _desc = desc;
}
void GPX::setEle(String ele){
  _ele = ele;
}
void GPX::setSym(String sym){
  _sym = sym;
}
void GPX::setSrc(String src){
  _src = src;
}
void GPX::setTime(String time){
  _time = time;
}
void GPX::setCmt(String cmt){
  _cmt = cmt;
}
void GPX::setSpeed(String speed){
  _speed = speed;
}
void GPX::setSat(String sat){
  _sat = sat;
}
void GPX::setFix(String fix){
  _fix = fix;
}

//Private Functions
String GPX::wrapCDATA(String input){
  String localStr("<![CDATA[");
  localStr += input;
  localStr += "]]>";

  return localStr;
}
