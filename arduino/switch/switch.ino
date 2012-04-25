#include <assert.h>

//------------------------------------------------------------------------------
// simple types declaration section

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
    Queue(void):size_(0),start_(0),end_(0) {}
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
    
  private:
    Queue(const Queue&);
    Queue& operator=(const Queue&);
    
    T queue_[maxSize];
    uint8_t size_;
    uint8_t start_;
    uint8_t end_;
};

typedef Queue<Event_t, 20> EventQueue_t;
  
//------------------------------------------------------------------------------
// classes used in the project

class EventManager
{
  public:
    EventManager();
    ~EventManager();
    
    boolean fireEvent(Event_t evt);
    boolean procEvent(Event_t evt);
    
  private:
    EventQueue_t eventQueue_;
};

boolean EventManager::fireEvent(Data_t& data)
{
  ;
}

boolean EventManager::procEvent(Event_t& evt)
{
  ;
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
    command_ |= byteIn>>7;
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


byte getData(byte* data, byte maxSize)
{
  if (state_ > SHDREND && state < SREADY) {
    dataBuf_ = data;
  }
  
  while ((state > SHDREND && state_ < SDATREADY) && maxSize) {
    while (!Serial.available());
    push(Serial.read());
    maxSize--;
  }
  
  dataBuf_ = NULL;
  
  return varLen_ - dataLeft_;
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

