/************************************************************/
/*    NAME: ruic                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "PrimeFactor.h"
#include <string>

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeFactor::PrimeFactor()
{
  m_input_val = 0;
}

//---------------------------------------------------------
// Destructor

PrimeFactor::~PrimeFactor()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PrimeFactor::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    string key = msg.GetKey();

    if (key == "NUM_VALUE"){

    	m_input_list.push_front(to_string(msg.GetDouble()));

      // m_input_val = msg.GetDouble();

      // if (m_input_val % 2 == 0){
      //   m_output = to_string(m_input_val)+", even";
      // }
      // else
      //   m_output = to_string(m_input_val)+", odd";

      // Notify("NUM_RESULT",m_output);

    }

// #if 0 // Keep these around just for template
//     string key   = msg.GetKey();
//     string comm  = msg.GetCommunity();
//     double dval  = msg.GetDouble();
//     string sval  = msg.GetString(); 
//     string msrc  = msg.GetSource();
//     double mtime = msg.GetTime();
//     bool   mdbl  = msg.IsDouble();
//     bool   mstr  = msg.IsString();
// #endif
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PrimeFactor::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PrimeFactor::Iterate()
{

	list<string>::reverse_iterator p;
	for(p=m_input_list.rbegin(); p!=m_input_list.rend(); p++) {
		string& str = *p;
		m_input_val = stoi(str);

		if (m_input_val % 2 == 0){
        m_output = to_string(m_input_val)+", even";
      }
    else
    	m_output = to_string(m_input_val)+", odd";

    Notify("NUM_RESULT",m_output);

    m_input_list.remove(str);

	}
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PrimeFactor::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(param == "foo") {
        //handled
      }
      else if(param == "bar") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void PrimeFactor::RegisterVariables()
{
  Register("NUM_VALUE", 0);
}

