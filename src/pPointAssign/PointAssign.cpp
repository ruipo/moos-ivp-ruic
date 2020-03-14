/************************************************************/
/*    NAME: ruic                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "XYPoint.h"
#include "PointAssign.h"
#include <string>

using namespace std;

//---------------------------------------------------------
// Constructor

PointAssign::PointAssign()
{
  num_vehs=0;
  xpoint=0;
  ypoint=0;
  id=0;
}

//---------------------------------------------------------
// Destructor

PointAssign::~PointAssign()
{
}

//---------------------------------------------------------
// Procedure: postViewPoint

void PointAssign::postViewPoint(double x, double y, string label, string color)
{
  XYPoint point(x, y); 
  point.set_label(label); 
  point.set_color("vertex", color); 
  point.set_param("vertex_size", "2");
  string spec = point.get_spec();

  Notify("VIEW_POINT", spec); 
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PointAssign::OnNewMail(MOOSMSG_LIST &NewMail)
{
  num_vehs = veh_names.size();

  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    string key = msg.GetKey();
    
    if (key == "VISIT_POINT"){
      Notify("UTS_PAUSE","true");
      pointmsg = msg.GetString();

      if(pointmsg=="firstpoint"){
        out = pointmsg;
      }

      else if(pointmsg=="lastpoint"){
        out = pointmsg;
      }

      else{
        xpointmsg = biteStringX(pointmsg, ',');
        xpointprefix = biteStringX(xpointmsg, '=');
        xpoint = stod(xpointmsg);

        ypointmsg = biteStringX(pointmsg, ',');
        ypointprefix = biteStringX(ypointmsg, '=');
        ypoint = stod(ypointmsg);

        idmsg = biteStringX(pointmsg, ',');
        idprefix = biteStringX(idmsg, '=');
        id = stod(idmsg);

        out = "x="+to_string(xpoint)+", y="+to_string(ypoint)+", id="+to_string(id);
        //Notify("VISIT_POINT_OUT",pointmsg);
      }

    }

    


#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PointAssign::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PointAssign::Iterate()
{ 
  if(assign_by_reg){

    if((out=="firstpoint") || (out=="lastpoint")){
      v1out = "VISIT_POINT_"+veh_names[0];
      v2out = "VISIT_POINT_"+veh_names[1];
      Notify(v1out,out);
      Notify(v2out,out);
    }

    else if(xpoint>=87.5){
      v2out = "VISIT_POINT_"+veh_names[1];
      postViewPoint(xpoint, ypoint, to_string(id), colors[0]);
      Notify(v2out,out);
    }

    else{
      v1out = "VISIT_POINT_"+veh_names[0];
      postViewPoint(xpoint, ypoint, to_string(id), colors[1]);
      Notify(v1out,out);
    }

  }

  else{

    if((out=="firstpoint") || (out=="lastpoint")){
      v1out = "VISIT_POINT_"+veh_names[0];
      v2out = "VISIT_POINT_"+veh_names[1];
      Notify(v1out,out);
      Notify(v2out,out);
    }

    else if((id%2)==0){
      v1out = "VISIT_POINT_"+veh_names[0];
      postViewPoint(xpoint, ypoint, to_string(id), colors[0]);
      Notify(v1out,out);
    }

    else{
      v2out = "VISIT_POINT_"+veh_names[1];
      postViewPoint(xpoint, ypoint, to_string(id), colors[1]);
      Notify(v2out,out);
    }

  }

  Notify("UTS_PAUSE","false");
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PointAssign::OnStartUp()
{ 
  Notify("UTS_PAUSE","true");
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      if(tolower(param) == tolower("VNAME")) {
        veh_names.push_back(value);
      }

      else if(tolower(param) == tolower("ASSIGN_BY_REGION")) {

        try{
          double num = stod(value);

          if(num == 1){
            assign_by_reg = true;
          }

          else if(num == 0){
            assign_by_reg = false;
          }

          else{
            assign_by_reg = false;
            cout << "INVALIDE ASSIGN_BY_REGION INPUT; SET TO FALSE." << endl;
          }
        }

        catch(...){
          if(tolower(value) == "true"){
            assign_by_reg = true;
          }

          else if(tolower(value) == "false"){
            assign_by_reg = false;
          }

          else{
            assign_by_reg = false;
            cout << "INVALIDE ASSIGN_BY_REGION INPUT; SET TO FALSE." << endl;
          } 

        }

      }
    }
  }
  
  RegisterVariables();
  Notify("UTS_PAUSE","false");
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void PointAssign::RegisterVariables()
{
  Register("VISIT_POINT",0);
}

