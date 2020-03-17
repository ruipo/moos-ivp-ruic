/************************************************************/
/*    NAME: ruic                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <vector>

class GenPath : public CMOOSApp
{
 public:
   GenPath();
   ~GenPath();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   int MinDist(double x, double y, std::vector<double> xvec, std::vector<double> yvec);

 protected:
   void RegisterVariables();

 private: // Configuration variables
 	double startx;
 	double starty;
 	//std::string veh_name;

 private: // State variables

	 std::vector<double> v1_vpx;
	 std::vector<double> v1_vpy;
	 //std::vector<double> v2_vpx;
	 //std::vector<double> v2_vpy;

	 //std::vector<std::string> veh_names;
 	 //int num_vehs;
 	 //std::string v1out;
 	 //std::string v2out;

 	 std::string pointmsg1;
 	 //std::string pointmsg2;
 	 std::string xpointmsg;
 	 std::string xpointprefix;
 	 double xpoint;
 	 std::string ypointmsg;
 	 std::string ypointprefix;
 	 double ypoint;


};

#endif 
