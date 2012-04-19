#include <SoftwareSerial.h>

typedef enum {
  MSG_TYPE_CMDSET,
  MSG_TYPE_CMDGET,
  MSG_TYPE_EVENT,
  MSG_TYPE_UNKNOWN
} MessageType_t;


typedef unsigned char UINT8;
typedef signed char INT8;
typedef unsigned short UINT16;
typedef signed short INT16;
typedef unsigned long UINT32;
typedef signed long INT32;

typedef enum {
  ESUCCEED,
  ENOMEM,
  ENOTEXISTS,
  EUNKNOWN
} Error_t;


// board pins setup
byte pinSensor = 2;
byte pinSwitch = 3;
byte pinLed    = 13;

// flag that indicated motion detector state
boolean isMotionDetected = false;


// device startup initialization stuff
void setup() {
  // setup UART (only speed)
  Serial.begin(115200);
}


// main loop where all events are processed
void loop() {
  // process events from virtual devices if available
  while (eventQueue.size()) {
    proc_event(eventsQueue.pop());
  }
}


UINT8 proc_event(Event_t evt)
{
  // propagate event to appropriate device
  if (evt.getAddress() < DEVICES_COUNT) {
    Device_t& dev = devices[ent.getAddress()];
    dev.procEvent(msg);
  } else {
    return ENOTEXISTS;
  }
  
  // just add appropriate header and propagate event to the control point
  Serial.write(msg.data());
  
  return ESUCCEED;
}

typedef enum {
  SET,
  GET
} Command_t;

typedef struct {
  UINT8 name;
  UINT8 value;
} Variable_t;

class DeviceProfile
{
  public:
    DeviceProfile()
  private:
};

class VirtualDevice {
  public:
    VirtualDevice();
    ~VirtualDevice();
    
    UINT8 ProcCommand(Command_t command, Variable_t& variable);
    UINT8 FireEvent(Variable_t variable);
    
  private:
    UINT8 Init(UINT8 varsCount);
    
    UINT8 varsCount_;
    Variable_t* vars_;
};

VirtualDevice::VirtualDevice():
  varsCount_(0),
  vars_(NULL)
{
}

VirtualDevice::~VirtualDevice()
{
  if (vars_ != NULL) {
    varsCount_ = 0;
    free(vars_);
  }
}

UINT8 VirtualDevice::Init(UINT8 varsCount)
{
  if (varsCount_ == 0) {
    vars_ = static_cast<Variable_t*>(malloc(varsCount*sizeof(Variable_t)));
    if (vars_ == NULL) {
      return ENOMEM;
    }
    
    varsCount_ = varsCount;
  } else {
    return EEXISTS;
  }
  
  return ESUCCEED;
}

