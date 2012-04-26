#include <assert.h>

//------------------------------------------------------------------------------
// simple types declaration section

enum Error_t {
  ESUCCEED,
  EINVARG,
  ENOTSUPPORTED,
  ENOMEMORY,
  EUNKNOWN
};

enum State_t {
  SHDRSTART,
  SHDREND,
  SDATSTART,
  SDATBODY,
  SREADY,
  SERROR
};

enum Command_t {
  CMDINVAL,
  CMDGET,
  CMDSET,
  CMDEVT
};

enum EventType_t {
  UNDEFINED,
  COMMAND,
  RESPONSE,
  NOTIFICATION
};

struct Data_t {
  uint8_t size_;
  void*   data_;
};

struct Event_t {
  EventType_t type_;
  Data_t data_;
};

template <class T, uint8_t _maxSize_>
class Queue {
  public:
    Queue(void):size_(0),start_(0),end_(0), isLocked_(false) {}
    ~Queue(void) {}
    
    boolean add(const T& item)
    {
      if (size_ >= _maxSize_) {
        return false;
      }
      
      memcpy(&queue_[end_++], &item, sizeof(T));
      if (end_ == _maxSize_) {
        end_ = 0;
      }
      
      size_++;
      
      assert(end_ > start_ ? end_ - start_ == size_: _maxSize_ + start_ - end_ == size_);
      
      return true;
    }
    
    boolean get(T& item)
    {
      if (size_ == 0) {
        return false;
      }
      
      memcpy(&item, &queue_[start_++], sizeof(T));
      if (start_ == _maxSize_) {
        start_ = 0;
      }
      
      size_--;
      
      assert(end_ > start_ ? end_ - start_ == size_: _maxSize_ + start_ - end_ == size_);
      
      return true;
    }
    
    T* alloc(void)
    {
      if (size_ >= _maxSize_) {
        return NULL;
      }
      
      isLocked_ = true;
      
      return &queue_[end_];
    }
    
    boolean commit(void)
    {
      if (isLocked_) {
        if (++end_ == _maxSize_) {
          end_ = 0;
        }
        
        size_++;
        isLocked_ = false;
        
        assert(end_ > start_ ? end_ - start_ == size_: _maxSize_ + start_ - end_ == size_);

        return true;
      }

      return false;
    }
    
  private:
    Queue(const Queue&);
    Queue& operator=(const Queue&);
    
    T queue_[_maxSize_];
    uint8_t size_;
    uint8_t start_;
    uint8_t end_;
    boolean isLocked_;
};

typedef Queue<Event_t, 20> EventQueue_t;
  
//------------------------------------------------------------------------------
// classes used in the project

class EventManager
{
  public:
    EventManager();
    ~EventManager();
    
    Error_t fireEvent(EventType_t& type, Data_t& data);
    Error_t procEvent(Event_t& evt);
    
  private:
    EventQueue_t eventQueue_;
};

Error_t EventManager::fireEvent(EventType_t& type, Data_t& data)
{
  Event_t* evt = eventQueue_.alloc();
  if (evt != NULL) {
    evt->type_ = type;
    evt->data_ = data;
    
    eventQueue_.commit();
    
    return ESUCCEED;
  }
  
  return EINVARG;
}

Error_t EventManager::procEvent(Event_t& evt)
{
  return ESUCCEED;
}


class MessageProcessor
{
  public:
    MessageProcessor();
    ~MessageProcessor();
    
    boolean dataAvailable(void) { return state_ > SDATSTART ? varLen_: false; }
    byte getDataSize(void) { return state_ > SDATSTART ? varLen_: 0; }
    Command_t getCommand(void);
    byte getData(byte* data, byte maxSize);
    
  private:
    State_t state_;
    byte address_;
    Command_t command_;
    byte dataLeft_;
    byte varId_;
    byte varLen_;
    byte* dataBuf_;
    
    State_t push(byte byteIn);
};

byte dataBuf[255];

boolean isOn = false;
byte switchPin = 2;

MessageProcessor messageProcessor;

MessageProcessor::MessageProcessor():
  state_(SHDRSTART),
  address_(0),
  command_(CMDINVAL),
  dataLeft_(0),
  varId_(0),
  varLen_(0),
  dataBuf_(NULL)
{
}

MessageProcessor::~MessageProcessor()
{
}

State_t MessageProcessor::push(byte byteIn)
{
  switch (state_) {
  case SHDRSTART:
    // just at header start, so we've got a first byte of the message header
    address_ = byteIn>>1;
    command_ = static_cast<Command_t>((byteIn&0x01)<<1 + 1);
    
    // update the parser state
    state_ = SHDREND;
    break;
  case SHDREND:
    // second (last) byte of the message header
    command_ = static_cast<Command_t>(command_ | byteIn>>7);
    dataLeft_ = byteIn&0x7F;
    
    // analyze paylod size
    if (dataLeft_ > 0) {
      varLen_ = dataLeft_ - 1;
      state_ = SDATSTART;
    } else {
      varLen_ = 0;
      state_ = SREADY;
    }
    break;
  case SDATSTART:
    varId_ = byteIn;
    dataLeft_--;
    state_ = SDATBODY;
    break;
  case SDATBODY:
    *dataBuf_++ = byteIn;
    if (--dataLeft_ == 0) {
      state_ = SHDRSTART;
    }
    break;
  case SREADY:
    address_ = 0;
    command_ = CMDINVAL;
    varId_ = 0;
    varLen_ = 0;
    state_ = SHDRSTART;
    break;
  default:
    state_ = SERROR;
    break;
  }
}

Command_t MessageProcessor::getCommand(void)
{
  while (state_ < SDATSTART) {
    while (!Serial.available());
    push(Serial.read());
  }
  
  return command_;
}


byte MessageProcessor::getData(byte* data, byte maxSize)
{
  if (state_ > SHDREND && state_ < SREADY) {
    dataBuf_ = data;
  }
  
  while ((state_ > SHDREND && state_ < SREADY) && maxSize) {
    while (!Serial.available());
    push(Serial.read());
    maxSize--;
  }
  
  dataBuf_ = NULL;
  
  return varLen_ - dataLeft_;
}

//------------------------------------------------------------------------------
// virtual device base class definition

class VirtualDevice
{
  public:
    VirtualDevice(void);
    ~VirtualDevice(void);
    
    inline boolean isValidId(uint8_t id) { if (id < maxId_) return true; else return false; }
    
    virtual Error_t subscribeToEvent(EventType_t type) = 0;
    virtual Error_t doCommand(Command_t cmd) = 0;
    
  private:
    VirtualDevice(const VirtualDevice&);
    VirtualDevice& operator=(const VirtualDevice&);
    
    uint8_t maxId_; // stands for device params count
};

#define CHECK_DEVARG_ID(id) { if (!isValidId(id)) return EINVARG; }

//------------------------------------------------------------------------------
// virtual devices manager

template <uint8_t _maxCount_>
class DeviceManager
{
  public:
    DeviceManager(void);
    ~DeviceManager(void);
    
    Error_t addDev(uint8_t devId);
    Error_t delDev(uint8_t devId);
    
    Error_t doCommand(uint8_t devId, Command_t cmd, uint8_t paramId, Data_t& data);
        
  private:
    DeviceManager(const DeviceManager&);
    DeviceManager& operator=(const DeviceManager&);

    VirtualDevice* devices_[_maxCount_ + 1]; // first device is always a device manager
};


template<uint8_t _maxCount_>
Error_t DeviceManager<_maxCount_>::doCommand(uint8_t devId, Command_t cmd, uint8_t paramId, Data_t& data)
{
  if (devId > _maxCount_) return EINVARG;
  
  return devices_[devId]->doCommand(cmd, paramId, data);
}


template<uint8_t _maxSize_>
Error_t DeviceManager<_maxSize_>::addDev(uint8_t devId)
{
  return ESUCCEED;
}


template<uint8_t _maxCount_>
Error_t DeviceManager<_maxCount_>::delDev(uint8_t devId)
{
  return ESUCCEED;
}


//------------------------------------------------------------------------------
// default arduino stuff

void setup(void)
{
  pinMode(switchPin, OUTPUT);
  Serial.begin(115200);
}


void loop(void)
{
  // wait for some message
  while (!messageProcessor.dataAvailable());
  
  // process a message
  Command_t cmd = messageProcessor.getCommand();
  
  if (messageProcessor.dataAvailable()) {
    byte dataSize = messageProcessor.getDataSize();
    messageProcessor.getData(dataBuf, dataSize);
  }
}


//------------------------------------------------------------------------------
// implementation of simple switch device

class BinarySwitch: public VirtualDevice
{
  public:
    virtual Error_t subscribeToEvent(EventType_t type);
    virtual Error_t doCommand(Command_t cmd, uint8_t paramId, Data_t& data);
    
  private:
    BinarySwitch(const BinarySwitch&);
    BinarySwitch& operator=(const BinarySwitch&);
    
    uint8_t output_;
    uint8_t state_;
};


Error_t BinarySwitch::subscribeToEvent(EventType_t type)
{
  return ENOTSUPPORTED;
}


Error_t BinarySwitch::doCommand(Command_t cmd, uint8_t paramId, Data_t& data)
{
  Error_t rv = ESUCCEED;
  
  switch (cmd) {
  case CMDGET:
    CHECK_DEVARG_ID(paramId);
    data.size_ = 1;
    static_cast<uint8_t*>(data.data_)[0] = state_;
    break;  
  case CMDSET:
    CHECK_DEVARG_ID(paramId);
    digitalWrite(output_, static_cast<uint8_t*>(data.data_)[0]);
    state_ = static_cast<uint8_t*>(data.data_)[0];
    break;
  default:
    rv = EINVARG;
    break;
  }
  
  return rv;
}

