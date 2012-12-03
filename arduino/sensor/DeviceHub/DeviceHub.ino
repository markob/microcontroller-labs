/*-----------------------------------------------------------------------------------------
LIBRARY HEADER
-----------------------------------------------------------------------------------------*/

#ifndef DEVICE_HUB_H_
#define DEVICE_HUB_H_

#include "Arduino.h"

#define DEVHUB_MAX_DEV_NUM 16

typedef byte Command;
typedef byte ResCode;

class Device {
  public:
    Device();
    
    static byte IdFromCommand(Command cmd);
    static byte CmdFromCommand(Command cmd);
    static byte PayloadFromCommand(Command cmd);
  private:
    byte id;
};

class DeviceHub {
  public:
    DeviceHub();
    ResCode dispatch_command(Command);
  private:
    Device devices[DEVHUB_MAX_DEV_NUM];
};

#endif

/*-----------------------------------------------------------------------------------------
LIBRARY IMPLEMENTATION
-----------------------------------------------------------------------------------------*/

Device::Device(): id(0)
{
}

byte Device::IdFromCommand(Command cmd)
{
  return (cmd&0xF0)>>8;
}

byte Device::CmdFromCommand(Command cmd)
{
  return (cmd&0x0C)>>2;
}

byte Device::PayloadFromCommand(Command cmd)
{
  return (cmd&0x03);
}

DeviceHub::DeviceHub()
{
}

ResCode DeviceHub::dispatch_command(Command cmd)
{
  ResCode rv = 0;
  byte devId   = Device::IdFromCommand(cmd);
  byte devCmd  = Device::CmdFromCommand(cmd);
  byte payload = Device::PayloadFromCommand(cmd);
  
  return payload;
}

/*-----------------------------------------------------------------------------------------
USAGE EXAMPLE
-----------------------------------------------------------------------------------------*/

DeviceHub devHub;

void setup()
{
  Serial.begin(115200);
}

void loop(void)
{
  if (Serial.available()) {
    Command cmd = Serial.read();
    
    byte payload = devHub.dispatch_command(cmd);
    for (byte i = 0; i < payload; i++) {
      byte cmdParam = Serial.read();
    }
    
    Serial.write('K');
  } else {
    delay(20);
  }
}

