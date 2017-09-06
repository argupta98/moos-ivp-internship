/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: HazardMgr.cpp                                        */
/*    DATE: Oct 26th 2012                                        */
/*                                                               */
/* This file is part of MOOS-IvP                                 */
/*                                                               */
/* MOOS-IvP is free software: you can redistribute it and/or     */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation, either version  */
/* 3 of the License, or (at your option) any later version.      */
/*                                                               */
/* MOOS-IvP is distributed in the hope that it will be useful,   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty   */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See  */
/* the GNU General Public License for more details.              */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with MOOS-IvP.  If not, see                     */
/* <http://www.gnu.org/licenses/>.                               */
/*****************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "HazardMgr.h"
#include "XYFormatUtilsHazard.h"
#include "XYFormatUtilsPoly.h"
#include "ACTable.h"
#include "NodeMessage.h"

using namespace std;

//---------------------------------------------------------
// Constructor

HazardMgr::HazardMgr()
{
  // Config variables
  m_swath_width_desired = 25;
  m_pd_desired          = 0.9;

  // State Variables 
  m_sensor_config_requested = false;
  m_sensor_config_set   = false;
  m_swath_width_granted = 0;
  m_pd_granted          = 0;

  m_sensor_config_reqs = 0;
  m_sensor_config_acks = 0;
  m_sensor_report_reqs = 0;
  m_detection_reports  = 0;

  m_summary_reports = 0;
  m_intervehicle_reports = 0;
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool HazardMgr::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();
    string sval  = msg.GetString(); 

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
    
    if(key == "UHZ_CONFIG_ACK") 
      handleMailSensorConfigAck(sval); //set sensor values

    else if(key == "UHZ_OPTIONS_SUMMARY") 
      handleMailSensorOptionsSummary(sval);

    else if(key == "UHZ_DETECTION_REPORT") 
      handleMailDetectionReport(sval); //when we get sensor detection

    else if(key == "HAZARDSET_REQUEST") 
      handleMailReportRequest(); //when the program asks for the final report

    else if(key == "UHZ_MISSION_PARAMS") 
      handleMailMissionParams(sval); //TODO4 reading in the mission parameters, publish important info to path planner
    else if(key == "NODE_MESSAGE"){
      vector<string> message_contents = parseStringQ(sval, ',');
      string subkey, subval;
      for(int i = 0; i<message_contents.size();i++){
	string tempval = message_contents[i];
	string tempkey = biteStringX(tempval, '=');
	if(tempkey == "var_name")
	  subkey = tempval;
	if(tempkey == "string_val")
	  subval = tempval;
      }
      if(subkey == "PARTNER_REQUEST")
	handleMailMissionRequest(); // send over packets

      else if(subkey == "PARTNER_HAZARDS")
	handleMailPartnerHazards(subval); //TODO3 process hazards recieved from the other vehicle

      if(subkey == "PARTNER_CONFIRM")
	handlePartnerConfirm();  // TODO4  verify that partner has recieved sent packets and delete them from m_haz_to_send
    }
    else if(key == "UHZ_HAZARD_REPORT")
      handleClassifyRequest(sval); // process the report and add the classified hazard into m_hazard_list
    
    else 
      reportRunWarning("Unhandled Mail: " + key);
  }
	
   return(true);
}

void HazardMgr::handlePartnerConfirm()
{
  int i=0;
  while(m_haz_to_send.size()>0 && i<5){
    m_haz_to_send.pop_back();
  }
}
//---------------------------------------------------------
// Procedure: OnConnectToServer

bool HazardMgr::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool HazardMgr::Iterate()
{
  AppCastingMOOSApp::Iterate();
  //if we have not yet setup our sensor, set it up
  if(!m_sensor_config_requested) 
    postSensorConfigRequest();
  //once we have our sensor configured, keep checking for sensor data. 
  if(m_sensor_config_set)
    postSensorInfoRequest();
  //if we have classified more than 5 points keep sending them until they get recieved. NAIVE, MIGHT CHANGE TO GO EVERY 60 SECONDS
  if(m_haz_to_send.size()>=5)
    handleMailMissionRequest();
  
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool HazardMgr::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(true);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if((param == "swath_width") && isNumber(value)) {
      m_swath_width_desired = atof(value.c_str());
      handled = true;
    }
    else if(((param == "sensor_pd") || (param == "pd")) && isNumber(value)) {
      m_pd_desired = atof(value.c_str());
      handled = true;
    }
    else if(param == "report_name") {
      value = stripQuotes(value);
      m_report_name = value;
      handled = true;
    }
    else if(param == "region") {
      XYPolygon poly = string2Poly(value);
      if(poly.is_convex())
	m_search_region = poly;
      handled = true;
    }
    else if(param == "partner"){
      m_dest_name = value;
      handled = true;
    }
    if(!handled)
      reportUnhandledConfigWarning(orig);
  }
  
  m_hazard_set.setSource(m_host_community);
  m_hazard_set.setName(m_report_name);
  m_hazard_set.setRegion(m_search_region);
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void HazardMgr::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("UHZ_DETECTION_REPORT", 0);
  Register("UHZ_CONFIG_ACK", 0);
  Register("UHZ_OPTIONS_SUMMARY", 0);
  Register("UHZ_MISSION_PARAMS", 0);
  Register("HAZARDSET_REQUEST", 0);
  Register("NODE_MESSAGE", 0);
  Register("UHZ_HAZARD_REPORT", 0);
  //Register for some update variable from the other robot
}

//---------------------------------------------------------
// Procedure: postSensorConfigRequest

void HazardMgr::postSensorConfigRequest()
{
  string request = "vname=" + m_host_community;
  
  request += ",width=" + doubleToStringX(m_swath_width_desired,2);
  request += ",pd="    + doubleToStringX(m_pd_desired,2);

  m_sensor_config_requested = true;
  m_sensor_config_reqs++;
  Notify("UHZ_CONFIG_REQUEST", request);
}

//---------------------------------------------------------
// Procedure: postSensorInfoRequest

void HazardMgr::postSensorInfoRequest()
{
  string request = "vname=" + m_host_community;

  m_sensor_report_reqs++;
  Notify("UHZ_SENSOR_REQUEST", request);
}

//---------------------------------------------------------
// Procedure: handleMailSensorConfigAck
//COnfigures sensor parameters once we have gotten back the request from uFldHazardSensor
bool HazardMgr::handleMailSensorConfigAck(string str)
{
  // Expected ack parameters:
  string vname, width, pd, pfa, pclass;
  
  // Parse and handle ack message components
  bool   valid_msg = true;
  string original_msg = str;

  vector<string> svector = parseString(str, ',');
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string param = biteStringX(svector[i], '=');
    string value = svector[i];

    if(param == "vname")
      vname = value;
    else if(param == "pd")
      pd = value;
    else if(param == "width")
      width = value;
    else if(param == "pfa")
      pfa = value;
    else if(param == "pclass")
      pclass = value;
    else
      valid_msg = false;       

  }


  if((vname=="")||(width=="")||(pd=="")||(pfa=="")||(pclass==""))
    valid_msg = false;
  
  if(!valid_msg)
    reportRunWarning("Unhandled Sensor Config Ack:" + original_msg);

  
  if(valid_msg) {
    m_sensor_config_set = true;
    m_sensor_config_acks++;
    m_swath_width_granted = atof(width.c_str());
    m_pd_granted = atof(pd.c_str());
  }

  return(valid_msg);
}

//---------------------------------------------------------
// Procedure: handleMailDetectionReport
//      Note: The detection report should look something like:
//            UHZ_DETECTION_REPORT = vname=betty,x=51,y=11.3,label=12 
//Handles new sensor data
bool HazardMgr::handleMailDetectionReport(string str)
{
  m_detection_reports++;
  //turns the string we have into a hazard
  XYHazard new_hazard = string2Hazard(str);
  //new_hazard.setType("hazard");
  string hazlabel = new_hazard.getLabel();
  if(hazlabel == "") {
    reportRunWarning("Detection report received for hazard w/out label");
    return(false);
  }
  m_haz_to_process.push_back(new_hazard);
  string event = "New Detection, label=" + new_hazard.getLabel();
  event += ", x=" + doubleToString(new_hazard.getX(),1);
  event += ", y=" + doubleToString(new_hazard.getY(),1);

  reportEvent(event);

  string req = "vname=" + m_host_community + ",label=" + hazlabel;
  //put new_hazard in a to_process list to get the classification
  Notify("UHZ_CLASSIFY_REQUEST", req);
  return(true);
}

//--------------------------------------------------------
//Procedure: handleClassifyRequest
//TODO: add probability of classification data being correct
//TODO: maintain a running list of hazards that need additional classification

void HazardMgr::handleClassifyRequest(string response)
{
  vector<string> parsed_response= parseStringQ(response, ',');
  string label = biteStringX(parsed_response[0], '=');
  string type = biteStringX(parsed_response[1], '=');
  
  //look through list of detected hazards
  list<XYHazard>::iterator p;
  XYHazard new_hazard = *m_haz_to_process.begin();
  for(p=m_haz_to_process.begin(); p!=m_haz_to_process.end();){
    new_hazard = *p;
    //find hazard with the classified id
    if(new_hazard.getLabel()==label){
      new_hazard.setType(type);
      m_haz_to_process.erase(p);
      break;
    }
  }
  
  //place it into classified XY_HAZARD_SET
  int ix = m_hazard_set.findHazard(label);
  if(ix == -1)
    m_hazard_set.addHazard(new_hazard);//in the case that we have not already seen it
  else
    m_hazard_set.setHazard(ix, new_hazard);// in the case that we have already seen it
  m_haz_to_send.push_back(new_hazard.getSpec(""));
}

void HazardMgr::handleMailPartnerHazards(string hazards)
{
  vector<string> hazard_list= parseStringQ(hazards, '#');
  for(int j=0;j<hazard_list.size();j++){
    XYHazard new_hazard= string2Hazard(hazard_list[j]);
    string label= new_hazard.getLabel();
    int ix = m_hazard_set.findHazard(label);
    if(ix == -1)
      m_hazard_set.addHazard(new_hazard);//in the case that we have not already seen it
    else
      m_hazard_set.setHazard(ix, new_hazard);// in the case that we have already seen it
  }
  NodeMessage node_message;

  node_message.setSourceNode(m_host_community);
  node_message.setDestNode(m_dest_name);
  node_message.setVarName("PARTNER_CONFIRM");
  node_message.setStringVal("Recieved String");

  string msg = node_message.getSpec();

  Notify("NODE_MESSAGE_LOCAL", msg);
}
//---------------------------------------------------------
// Procedure: handleMailReportRequest

void HazardMgr::handleMailReportRequest()
{
  m_summary_reports++;

  m_hazard_set.findMinXPath(20);
  //unsigned int count    = m_hazard_set.findMinXPath(20);
  string summary_report = m_hazard_set.getSpec("final_report");
  
  Notify("HAZARDSET_REPORT", summary_report);
}

//--------------------------------------------------------
//Procedure: handleMailMissionRequest
//same as above but notifies PARTNER_HAZARDS to share data with other vehicle
//the report must be as concise as possible since there is the 100 character limit.
//can send at most 4 hazards at a time 
void HazardMgr::handleMailMissionRequest()
{
  m_intervehicle_reports++;
  string parsed_report;
  for(int i=0; i<min((int)m_haz_to_send.size(), 5);i++){
    string hazard= m_haz_to_send[m_haz_to_send.size()-i];
    vector<string> broken_string = parseString(hazard, ',');
    if(broken_string.size() > 1){
      parsed_report+="#";
      for(int j=0;i<broken_string.size();j++){
	string value= broken_string[j];
	string key = biteStringX(value,'=');
	if(key=="x"){
	  parsed_report+="x="+ value+",";
	}
	else if (key == "y"){
	  parsed_report+="y="+value+",";
	}
	else if (key=="label"){
	  parsed_report+="l="+value+",";
	}
	else if (key == "type"){
	  parsed_report+="t="+value;
	}
      }
    }
  }
  NodeMessage node_message;

  node_message.setSourceNode(m_host_community);
  node_message.setDestNode(m_dest_name);
  node_message.setVarName("PARTNER_HAZARDS");
  node_message.setStringVal(parsed_report);

  string msg = node_message.getSpec();

  Notify("NODE_MESSAGE_LOCAL", msg);
}
//---------------------------------------------------------
// Procedure: handleMailMissionParams
//   Example: UHZ_MISSION_PARAMS = penalty_missed_hazard=100,               
//                       penalty_nonopt_hazard=55,                
//                       penalty_false_alarm=35,                  
//                       penalty_max_time_over=200,               
//                       penalty_max_time_rate=0.45,              
//                       transit_path_width=25,                           
//                       search_region = pts={-150,-75:-150,-50:40,-50:40,-75}


void HazardMgr::handleMailMissionParams(string str)
{
  vector<string> svector = parseStringZ(str, ',', "{");
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string param = biteStringX(svector[i], '=');
    string value = svector[i];
    // This needs to be handled by the developer. Just a placeholder.
  }
}


//------------------------------------------------------------
// Procedure: buildReport()

bool HazardMgr::buildReport() 
{
  m_msgs << "Config Requested:"                                  << endl;
  m_msgs << "    swath_width_desired: " << m_swath_width_desired << endl;
  m_msgs << "             pd_desired: " << m_pd_desired          << endl;
  m_msgs << "   config requests sent: " << m_sensor_config_reqs  << endl;
  m_msgs << "                  acked: " << m_sensor_config_acks  << endl;
  m_msgs << "------------------------ "                          << endl;
  m_msgs << "Config Result:"                                     << endl;
  m_msgs << "       config confirmed: " << boolToString(m_sensor_config_set) << endl;
  m_msgs << "    swath_width_granted: " << m_swath_width_granted << endl;
  m_msgs << "             pd_granted: " << m_pd_granted          << endl << endl;
  m_msgs << "--------------------------------------------" << endl << endl;

  m_msgs << "               sensor requests: " << m_sensor_report_reqs << endl;
  m_msgs << "             detection reports: " << m_detection_reports  << endl << endl; 

  m_msgs << "   Hazardset Reports Requested: " << m_summary_reports << endl;
  m_msgs << "      Hazardset Reports Posted: " << m_summary_reports << endl;
  m_msgs << "                   Report Name: " << m_report_name << endl;

  return(true);
}








