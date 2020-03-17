/************************************************************/
/*    NAME: ruic                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "XYSegList.h"
#include "GenPath.h"
#include <algorithm>

using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
  //num_vehs=0;
  xpoint=0;
  ypoint=0;
  v1_vpx = {};
  v1_vpy = {};
  //v2_vpx = {};
  //v2_vpy = {};
}

//---------------------------------------------------------
// Destructor

GenPath::~GenPath()
{
}

//---------------------------------------------------------
// Procedure: Greedy minDistance

int GenPath::MinDist(double x, double y, vector<double> xvec, vector<double> yvec)
{
  int vecsize = xvec.size();
  vector<double> dist = {};

  for(int ii=vecsize-1; ii==0; ii--){

    dist.push_back(pow(pow(x-xvec[ii],2)+pow(y-yvec[ii],2),0.5));
  }

  vector<double>::iterator min = min_element(dist.begin(), dist.end());
  int argmin = distance(dist.begin(), min);
  //vector<double> closestpt {xvec[argmin],yvec[argmin]}

  return(argmin);

}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    string key = msg.GetKey();
    
    if (key == "VISIT_POINT"){
      pointmsg1 = msg.GetString();

      if(pointmsg1=="firstpoint" || pointmsg1=="lastpoint"){
        continue;
      }

      else{
        xpointmsg = biteStringX(pointmsg1, ',');
        xpointprefix = biteStringX(xpointmsg, '=');
        xpoint = stod(xpointmsg);
        

        ypointmsg = biteStringX(pointmsg1, ',');
        ypointprefix = biteStringX(ypointmsg, '=');
        ypoint = stod(ypointmsg);

        if(v1_vpx.empty() && v1_vpy.empty()){
          v1_vpx.push_back(xpoint);
          v1_vpy.push_back(ypoint);
        }

        else if(xpoint==v1_vpx.back() && ypoint==v1_vpy.back()){
          continue;
        }

        else{
          v1_vpx.push_back(xpoint);
          v1_vpy.push_back(ypoint);
        }
      }
    }

    // else if (key == v2out){
    //   pointmsg2 = msg.GetString();

    //   if(pointmsg2=="firstpoint" || pointmsg2=="lastpoint"){
    //     continue;
    //   }

    //   else{
    //     xpointmsg = biteStringX(pointmsg2, ',');
    //     xpointprefix = biteStringX(xpointmsg, '=');
    //     xpoint = stod(xpointmsg);
        

    //     ypointmsg = biteStringX(pointmsg2, ',');
    //     ypointprefix = biteStringX(ypointmsg, '=');
    //     ypoint = stod(ypointmsg);

    //     if(v2_vpx.empty() && v2_vpy.empty()){
    //       v2_vpx.push_back(xpoint);
    //       v2_vpy.push_back(ypoint);
    //     }

    //     else if(xpoint==v2_vpx.back() && ypoint==v2_vpy.back()){
    //       continue;
    //     }

    //     else{
    //       v2_vpx.push_back(xpoint);
    //       v2_vpy.push_back(ypoint);
    //     }
    //   }
    // }

   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GenPath::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool GenPath::Iterate()
{
  if(pointmsg1=="lastpoint" && !v1_vpx.empty()){

    double currentx = startx;
    double currenty = starty;
    XYSegList v1_seglist;
    v1_seglist.add_vertex(currentx,currenty);

    while(!v1_vpx.empty()){
      //cout << v1_vpx[0] << endl;
      int arg = MinDist(currentx,currenty,v1_vpx,v1_vpy);
      currentx = v1_vpx[arg];
      currenty = v1_vpy[arg];
      v1_seglist.add_vertex(currentx,currenty);
      v1_vpx.erase (v1_vpx.begin()+arg);
      v1_vpy.erase (v1_vpy.begin()+arg);

    }

    //cout << v1_seglist.get_spec() << endl;
    string updates_str = "points = ";
    updates_str += v1_seglist.get_spec();
    Notify("WPT_UPDATES", updates_str);

  }

  // if(pointmsg2=="lastpoint" && !v2_vpx.empty()){

  //   double currentx = 0;
  //   double currenty = 0;
  //   XYSegList v2_seglist;
  //   v2_seglist.add_vertex(currentx,currenty);

  //   //cout << v2_vpx.size() << endl;

  //   while(!v2_vpx.empty()){

  //     int arg = MinDist(currentx,currenty,v2_vpx,v2_vpy);
  //     currentx = v2_vpx[arg];
  //     currenty = v2_vpy[arg];
  //     v2_seglist.add_vertex(currentx,currenty);
  //     v2_vpx.erase (v2_vpx.begin()+arg);
  //     v2_vpy.erase (v2_vpy.begin()+arg);
  //   }
  //   //cout << v2_seglist.get_spec() << endl;
  //   string updates_str = "points = ";
  //   updates_str += v2_seglist.get_spec();
  //   Notify("UPDATES_V2", updates_str);
  // }

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
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(tolower(param) == tolower("START_X")) {
        startx = stod(line);
      }

      if(tolower(param) == tolower("START_Y")) {
        starty = stod(line);
      }
    }
  }
  
  // v1out = "VISIT_POINT_"+veh_names[0];
  // v2out = "VISIT_POINT_"+veh_names[1];
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void GenPath::RegisterVariables()
{
  Register("VISIT_POINT", 0);
  //Register(v2out, 0);

}

