/************************************************************/
/*    NAME: Arjun Gupta                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "GenPath.h"
#include "XYSegList.h"
#include <vector>

using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
  m_end=false;
  m_points_processed=0;
  m_start_point=0;
  m_done=false;
  //m_fine_comb =true;
}

//---------------------------------------------------------
// Destructor

GenPath::~GenPath()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    bool is_string = msg.IsString();
    if(!is_string){
      Notify("PATH_STAT", "not a string");
    }
    if(key == "VISIT_POINT"){
      if(sval == "lastpoint"){
	Notify("PATH_STAT", "processing point: " + sval);
	m_end=true;
      }
      else{
	Notify("PATH_STAT", "processing point: " + sval);
	Point new_point(sval);
	m_point_list.push_back(new_point);

      }
   }
  }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GenPath::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool GenPath::Iterate()
{
  Notify("PATH_STAT", "iterating");
  if((m_point_list.size() <= m_points_processed)){
    string update = vertex_list.get_spec();
    biteStringX(update, '{');
    string to_return = biteStringX(update, '}');
    Notify("UPDATE_PATH", "points = "+ to_return);
    Notify("PATH_STAT", "points= "+ to_return);
    m_done=true;
  }
  //add points to vertex list in order to minimize total distance
  else if(m_point_list.size() != 0 && m_point_list.size() != m_used.size()){
    Notify("PATH_STAT", "about to start iteration with start : " + to_string(m_start_point));
    Point current_point= m_point_list[0];
    Point min_point=m_point_list[1];
    m_points_processed+=1;
    //go through every vertex in the list specified by "current_point"
    int iterate_length= m_point_list.size();
    for(int i=0;i<iterate_length;i++ ) {
      m_used.push_back(current_point);
      int g=0;
      while(m_point_list[g].in(m_used)){
	g++;
      }
      min_point=m_point_list[g];
      //check against everyother vertex to find closest one
      for(int j=0;j<iterate_length;j++) {
	Point new_point= m_point_list[j];
	//see if the distance to this point is less than the current minimum distance and this point is not already used
	if(current_point.distance_to(new_point)< current_point.distance_to(min_point) && !new_point.in(m_used)){
	  min_point=new_point;
	}
      }
      Notify("PATH_STAT", "adding point: "+ current_point.point_string+" total processed: "+ to_string(m_points_processed)+"/"+to_string(m_point_list.size()));
      vertex_list.add_vertex(current_point.x_val, current_point.y_val);
      current_point=min_point;
      m_points_processed+=1;
    }
  }
  
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "FOO") {
        //handled
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void GenPath::RegisterVariables()
{
  Register("VISIT_POINT", 0);
}

