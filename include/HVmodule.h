/*
 *  VeheMencE: a simple library for VME access
 *  Copyright (C) 2019 Universite catholique de Louvain (UCLouvain), Belgium
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __HVMODULE
#define __HVMODULE

#include "CommonDef.h"

class CaenetBridge;

class HVModule;
class HVBoard;
class ChannelProperties;
class HVChannel;

// One single HV channel.
class HVChannel
{
public:
  explicit HVChannel(uint32_t address, const HVBoard& board, uint32_t id, uint8_t type, CaenetBridge* bridge);
  virtual ~HVChannel() {}
  
  // the channel id
  uint32_t board() const; 
  uint32_t id() const { return id_; }
  
  // the board properties
  const HVBoard* getBoard() const { return board_; }
  const ChannelProperties& getProperties() const;
  
  // turn on/off
  void virtual on() = 0 ;
  void virtual off() = 0 ;
  
  // program the channel
  void virtual setV0(uint32_t v0) = 0;
  void virtual setV1(uint32_t v1) = 0;
  void virtual setI0(uint32_t i0) = 0;
  void virtual setI1(uint32_t i1) = 0;
  void virtual setRampup(uint32_t rampup) = 0;
  void virtual setRampdown(uint32_t rampdown) = 0;
  void virtual setTrip(uint32_t trip) = 0;
  void virtual setSoftMaxV(uint32_t maxv) = 0;
  
  // read all parameters from hardware
  void virtual readOperationalParameters() = 0;
  
  // get cached values (as last programmed, or read back before)
  // readOperationalParameters should be called first.
  inline float getVmon() const { return vmon_; }
  inline float getImon() const { return imon_; }
  inline uint32_t getV0() const { return v0_; }
  inline uint32_t getV1() const { return v1_; }
  inline uint32_t getI0() const { return i0_; }
  inline uint32_t getI1() const { return i1_; }
  inline uint32_t getRampup() const { return rampup_; }
  inline uint32_t getRampdown() const { return rampdown_; }
  inline uint32_t getTrip() const { return trip_; }
  inline uint32_t getmaxV() const { return maxV_; }
  inline uint32_t getStatus() const { return status_; }
  inline uint32_t getSoftMaxV() const { return softmaxV_; }
  
protected:
  
  void attach(const HVBoard &board);
  void attachWithoutReciprocating(const HVBoard &board) const;
  
  CaenetBridge* bridge_;
  mutable HVBoard const* board_;
  
  uint8_t type_;
  uint32_t address_;
  uint32_t id_;
  float vmon_;
  float imon_;
  uint32_t v0_;
  uint32_t v1_;
  uint32_t i0_;
  uint32_t i1_;
  uint32_t maxV_;
  uint32_t softmaxV_;
  uint32_t status_;
  uint32_t rampup_;
  uint32_t rampdown_;
  uint32_t trip_;
  
  friend class HVModule;
  friend class HVBoard;
};

// One board, containing several similar channels
class HVBoard{
public:
  HVBoard(uint32_t slot,
          std::string name,
          uint16_t serial_number,
          uint16_t software_version,
          uint8_t nChannels);
  HVBoard():slot_(0),name_(""),serial_number_(0),software_version_(0),nChannels_(0) {}
  virtual ~HVBoard() {};
  void add(ChannelProperties &properties) {
    channelsProps_.push_back(properties);
  }

  inline uint32_t getSlot() const { return slot_; }
  inline std::string getName() const { return name_; }
  inline uint16_t getSerialNumber() const { return serial_number_; }
  inline uint16_t getSoftwareVersion() const { return software_version_; }
  inline uint8_t getNChannels() const { return nChannels_; }
  
  inline HVChannel& getChannel(unsigned int n) { return *channels_.at(n); }
  
protected:
  void attach(HVChannel &channel) const {
    channels_.push_back(&channel);
    channel.attachWithoutReciprocating(*this);
  }

  void attachWithoutReciprocating(HVChannel &channel) const {
    channels_.push_back(&channel);
  }
  
  inline const ChannelProperties& getChannelProperties(uint8_t ch) const { return channelsProps_.at(ch); }
  
private:
  uint32_t slot_;
  std::string name_;
  uint16_t serial_number_;
  uint16_t software_version_;
  uint8_t nChannels_;
  
  mutable std::vector<HVChannel*> channels_;
  std::vector<ChannelProperties> channelsProps_;

  friend class HVModule;
  friend class HVChannel;
};

// channel properties
class ChannelProperties{
public:
  ChannelProperties(
          uint8_t current_unit,
          uint32_t vmax,
          uint16_t imax,
          uint16_t rampmin,
          uint16_t rampmax,
          uint16_t vres,
          uint16_t ires,
          uint16_t vdec,
          uint16_t idec,
          uint16_t vmdec
                   );
  ChannelProperties():current_unit_(0),vmax_(0),imax_(0),
            rampmin_(0),rampmax_(0),vres_(0),ires_(0),vdec_(0),idec_(0),vmdec_(0) { }
  virtual ~ChannelProperties() {};

  inline uint8_t getCurrentUnit() const { return current_unit_; }
  inline uint32_t getVMax() const { return vmax_; }
  inline uint16_t getIMax() const { return imax_; }
  inline uint16_t getRampMin() const { return rampmin_; }
  inline uint16_t getRampMax() const { return rampmax_; }
  inline uint16_t getVResolution() const { return vres_; }
  inline uint16_t getIResolution() const { return ires_; }
  inline uint16_t getVDecimals() const { return vdec_; }
  inline uint16_t getIDecimals() const { return idec_; }
  inline uint16_t getVMaxDecimals() const { return vmdec_; }
  
private:
  uint8_t current_unit_; // 0:A, 1:mA, 2:uA, 3:nA
  uint32_t vmax_;
  uint16_t imax_;
  uint16_t rampmin_;
  uint16_t rampmax_;
  uint16_t vres_;
  uint16_t ires_;
  uint16_t vdec_;
  uint16_t idec_;
  uint16_t vmdec_;
};

// One HV module or crate
class HVModule{
public:
  explicit HVModule(uint32_t address, CaenetBridge* bridge);
  
  virtual ~HVModule() {
    for( auto [key,channel] : channels_) {
      delete channel;
    }
  }
  
  // returns the identification string
  inline std::string identification() const { return identification_; } 

  // static method that instantiates the proper derived class, according to the identification string
  static HVModule* HVModuleFactory(uint32_t address, CaenetBridge* bridge);
  
  // returns one channel
  inline HVChannel* channel(uint32_t board, uint32_t channel) { return channels_.at(std::make_pair(board,channel)); }
  
  // returns one board
  inline const HVBoard& board(uint32_t slot) { return boards_.at(slot); }
  
  // returns all channels
  inline std::map<std::pair<uint32_t,uint32_t>, HVChannel*>& getChannels() { return channels_; }
  
  // returns all boards
  inline std::map<uint32_t, const HVBoard>& getBoards() { return boards_; }

protected:
  // method to be implemented to build the channel map. It populates the boards map
  virtual void discoverBoards() = 0;
  
  // method to be implemented to check that the returned identity is ok for this class
  virtual void assertIdentity() const = 0;
  
  CaenetBridge* bridge_;
  uint32_t address_;

  std::map<std::pair<uint32_t,uint32_t>, HVChannel*> channels_; 

  std::map<uint32_t, const HVBoard> boards_;

  std::string identification_;
};

#endif
