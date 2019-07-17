#include <boost/python.hpp>
#include "VmeController.h"
#include "VmeUsbBridge.h"
#include "VmeBoard.h"
#include "CaenetBridge.h"
#include "HVmodule.h"

using namespace boost::python;

struct VmeControllerWrap : VmeController, wrapper<VmeController>
{
  VmeControllerWrap():VmeController(),wrapper<VmeController>() {}
  
  void writeData(long unsigned int address,void* data) const override {
    this->get_override("writeData")(address,data);
  }
  void readData(long unsigned int address,void* data) const override {
    this->get_override("readData")(address,data);
  }
  void readWriteData(const long unsigned int address,void* data) const override {
    this->get_override("readWriteData")(address,data);
  }
  void blockReadData(const long unsigned int address,unsigned char *buffer, int size, int *count, bool multiplex=false) const override {
    this->get_override("blockReadData")(address,buffer,size,count,multiplex);
  }
  void blockWriteData(const long unsigned int address,unsigned char *buffer, int size, int *count, bool multiplex=false) const override {
    this->get_override("blockWriteData")(address,buffer,size,count,multiplex);
  }
  void ADOCycle(const long unsigned int address) const override {
    this->get_override("ADOCycle")(address);
  }
  void IRQEnable(uint32_t mask) const override {
    this->get_override("IRQEnable")(mask);
  }
  void IRQDisable(uint32_t mask) const override {
    this->get_override("IRQDisable")(mask);
  }
  void IRQWait(uint32_t mask, uint32_t timeout_ms) const override {
    this->get_override("IRQWait")(mask,timeout_ms);
  }
  unsigned char IRQCheck() const override {
    return this->get_override("IRQCheck")();
  }
  uint16_t IACK(CVIRQLevels Level) const override {
    return this->get_override("IACK")(Level);
  }
};

BOOST_PYTHON_MODULE(VMEPythonModule)
{
  //expose the enums (TODO if confirmed that we need it. We can live with integers)
  
  //expose VmeController
    class_<VmeControllerWrap, boost::noncopyable>("VmeController")
        .def("setMode", &VmeController::setMode)
        .def("mode", &VmeController::mode,return_value_policy<manage_new_object>())
        .add_property("AM",&VmeController::getAM,&VmeController::setAM)
        .add_property("DW",&VmeController::getDW,&VmeController::setDW)
        .def("writeData",pure_virtual(&VmeController::writeData)) 
        .def("readData",pure_virtual(&VmeController::readData))
        .def("readWriteData",pure_virtual(&VmeController::readWriteData))
        .def("blockReadData",pure_virtual(&VmeController::blockReadData))
        .def("blockWriteData",pure_virtual(&VmeController::blockWriteData))
        .def("ADOCycle",pure_virtual(&VmeController::ADOCycle))
        .def("IRQEnable",pure_virtual(&VmeController::IRQEnable))
        .def("IRQDisable",pure_virtual(&VmeController::IRQDisable))
        .def("IRQWait",pure_virtual(&VmeController::IRQWait))
        .def("IRQCheck",pure_virtual(&VmeController::IRQCheck))
        .def("IACK",pure_virtual(&VmeController::IACK))
    ;
  //expose VmeUsbBridge
    class_<VmeUsbBridge, bases<VmeController> >("VmeUsbBridge")
         .def("configureOutputLine",&VmeUsbBridge::configureOutputLine)
         .def("configureInputLine",&VmeUsbBridge::configureInputLine)
         .def("outputLineConfiguration",&VmeUsbBridge::outputLineConfiguration)
         .def("inputLineConfiguration",&VmeUsbBridge::inputLineConfiguration)
         .def("readRegister",&VmeUsbBridge::readRegister)
         .def("setOutputRegister",&VmeUsbBridge::setOutputRegister)
         .def("clearOutputRegister",&VmeUsbBridge::clearOutputRegister)
         .def("pulseOutputRegister",&VmeUsbBridge::pulseOutputRegister)
         .def("readDisplay",&VmeUsbBridge::readDisplay)
         .add_property("arbiterType",&VmeUsbBridge::getArbiterType,&VmeUsbBridge::setArbiterType)
         .add_property("requesterType",&VmeUsbBridge::getRequesterType,&VmeUsbBridge::setRequesterType)
         .add_property("releaseType",&VmeUsbBridge::getReleaseType,&VmeUsbBridge::setReleaseType)
         .add_property("busReqLevel",&VmeUsbBridge::getBusReqLevel,&VmeUsbBridge::setBusReqLevel)
         .add_property("timeout",&VmeUsbBridge::getTimeout,&VmeUsbBridge::setTimeout)
         .add_property("FIFOmode",&VmeUsbBridge::getFIFOMode,&VmeUsbBridge::setFIFOMode)
         .def("getPulser",&VmeUsbBridge::getPulser,return_value_policy<copy_non_const_reference>())
         .def("getScaler",&VmeUsbBridge::getScaler,return_value_policy<copy_non_const_reference>())
    ;
    class_<V1718Pulser>("V1718Pulser",init<uint32_t,CVPulserSelect>())
         .def("configure",&V1718Pulser::configure)
         .def("update",&V1718Pulser::configure)
         .def("start",&V1718Pulser::start)
         .def("stop",&V1718Pulser::stop)
         .add_property("period",&V1718Pulser::getPeriod,&V1718Pulser::setPeriod)
         .add_property("width",&V1718Pulser::getWidth,&V1718Pulser::setWidth)
         .add_property("units",&V1718Pulser::getUnits,&V1718Pulser::setUnits)
         .add_property("pulseNo",&V1718Pulser::getPulseNo,&V1718Pulser::setPulseNo)
         .add_property("startSource",&V1718Pulser::getStartSource,&V1718Pulser::setStartSource)
         .add_property("stopSource",&V1718Pulser::getStopSource,&V1718Pulser::setStopSource)
    ;
    class_<V1718Scaler>("V1718Scaler",init<uint32_t>())
         .def("configure",&V1718Scaler::configure)
         .def("update",&V1718Scaler::update)
         .def("resetCount",&V1718Scaler::resetCount)
         .def("enableGate",&V1718Scaler::enableGate)
         .def("disableGate",&V1718Scaler::disableGate)
         .def("count",&V1718Scaler::count)
         .add_property("limit",&V1718Scaler::getLimit,&V1718Scaler::setLimit)
         .add_property("autoReset",&V1718Scaler::getAutoReset,&V1718Scaler::setAutoReset)
         .add_property("hitSource",&V1718Scaler::getHitSource,&V1718Scaler::setHitSource)
         .add_property("gateSource",&V1718Scaler::getGateSource,&V1718Scaler::setGateSource)
         .add_property("resetSource",&V1718Scaler::getResetSource,&V1718Scaler::setResetSource)
    ;
    // expose VmeBoard
    class_<VmeBoard>("VmeBoard",init<VmeController*, uint32_t, CVAddressModifier, CVDataWidth, bool>())
         .add_property("enforce",&VmeBoard::isAMDWenforced,&VmeBoard::enforceAMDW)
    ;
    // expose CaenetBridge
    class_<CaenetBridge, bases<VmeBoard> >("CaenetBridge",init<VmeController*, uint32_t, uint8_t>())
         .def("reset",&CaenetBridge::reset)
         .def("validStatus",&CaenetBridge::validStatus)
         .def("write",&CaenetBridge::write)
         .def("readResponse",&CaenetBridge::readResponse)
    ;
    // expose HVmodule
    class_<N470StatusWord>("N470StatusWord",init<uint16_t>())
         .def("status",&N470StatusWord::status)
         .def("bit",&N470StatusWord::bit)
    ;
    class_<HVChannel>("HVChannel",init<uint32_t, uint32_t, CaenetBridge*>())
         .add_property("id",&HVChannel::id)
         .add_property("V0",&HVChannel::getV0,&HVChannel::setV0)
         .add_property("V1",&HVChannel::getV1,&HVChannel::setV1)
         .add_property("I0",&HVChannel::getI0,&HVChannel::setI0)
         .add_property("I1",&HVChannel::getI1,&HVChannel::setI1)
         .add_property("rampup",&HVChannel::getRampup,&HVChannel::setRampup)
         .add_property("rampdown",&HVChannel::getRampdown,&HVChannel::setRampdown)
         .add_property("trip",&HVChannel::getTrip,&HVChannel::setTrip)
         .add_property("Vmon",&HVChannel::getVmon)
         .add_property("Imon",&HVChannel::getImon)
         .add_property("maxV",&HVChannel::getmaxV)
         .add_property("status",&HVChannel::getStatus)
         .def("on",&HVChannel::on)
         .def("off",&HVChannel::off)
         .def("readOperationalParameters",&HVChannel::readOperationalParameters)
    ;
    class_<HVModule>("HVModule",init<uint32_t,CaenetBridge*>())
         .add_property("identification",&HVModule::identification)
         .add_property("status",&HVModule::getStatus)
         .def("updateStatus",&HVModule::updateStatus)
         .def("kill",&HVModule::kill)
         .def("clearAlarm",&HVModule::clearAlarm)
         .def("enableKeyboard",&HVModule::enableKeyboard)
         .def("setTTL",&HVModule::setTTL)
         .def("setNIM",&HVModule::setNIM)
         .def("channel",&HVModule::channel,return_value_policy<copy_non_const_reference>())
    ;
}