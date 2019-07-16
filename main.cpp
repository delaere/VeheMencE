#include "include/VmeUsbBridge.h"
#include "include/TTCvi.h"
#include "include/Discri.h"
#include "include/Scaler.h"

#include "include/logger.h"
#include <iostream>
#include <exception>

int main(int argc, char* argv[]){

  if (argc > 1) {
	  Logger::initFromConfig(argv[1]);
	  Logger::addDataFileLog("test.log");
  } else {
	   Logger::init();
  }

  try {
    LOG_DEBUG("Start of configuratio");
    LOG_TRACE("Instantiating the controller");
    VmeUsbBridge myCont(DEBUG);
    LOG_TRACE("Instantiating the discri");
    Discri myDiscri(&myCont);
    LOG_TRACE("Instantiating the TTCvi");
    TtcVi myTtc(&myCont);
    
    LOG_DEBUG("Setting TTC");
    myTtc.changeChannel(1);
      
    LOG_DEBUG("Setting Discri");
    myDiscri.setMultiChannel(0x000F);
    myDiscri.setMajority(4);
    myDiscri.setTh(100);

    LOG_DEBUG("DONE");

  } catch (const CAENVMEexception& e) {
	std::cerr << e.what() << '\n';
	const boost::stacktrace::stacktrace* st = boost::get_error_info<traced>(e);
	if (st) {
          std::cerr << *st << '\n'; /*<-*/ return 0; /*->*/
        } /*<-*/ return 3; /*->*/
  }
return 5;
  
  
}
