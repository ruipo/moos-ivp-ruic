/************************************************************/
/*    NAME: Rui Chen                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_CZigZag.h                                      */
/*    DATE:                                                 */
/************************************************************/

#ifndef CZigZag_HEADER
#define CZigZag_HEADER

#include <string>
#include <vector>
#include <iostream>
#include "IvPBehavior.h"
#include "ZAIC_PEAK.h"
#include "OF_Coupler.h"
#include "XYPoint.h"

class BHV_CZigZag : public IvPBehavior {
public:
  BHV_CZigZag(IvPDomain);
  ~BHV_CZigZag() {};
  
  bool         setParam(std::string, std::string);
  void         onSetParamComplete();
  void         onCompleteState();
  void         onIdleState();
  void         onHelmStart();
  void         postConfigStatus();
  void         onRunToIdleState();
  void         onIdleToRunState();
  IvPFunction* onRunState();

protected: // Local Utility functions
  bool handleSensingReport(const std::string&);
  IvPFunction* buildFunctionWithZAIC();

protected: // Configuration parameters

//double m_stagger_width;
double m_heading;
double m_zlength;
double m_decay_factor;
double m_desired_speed;
double m_num_cycles;
double thres;
double m_zig_heading;
double m_zag_heading;

protected: // State variables

vector <double> xgradlist;
vector <double> ygradlist;
double prev_x;
double prev_y;
double current_x;
double current_y;
double prev_temp;
double current_temp;
std::string estimate_report;
double fronty;
bool crossed_front;
double prev_tgrad;
double tgrad;

};

#define IVP_EXPORT_FUNCTION

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_CZigZag(domain);}
}
#endif
