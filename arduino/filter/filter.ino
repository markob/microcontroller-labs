byte inDataPin = A0;

int16_t scale = 4185;

const int16_t b_1[] = { 16384, -12367,  16384 };
const int16_t b_2[] = { 16384,  -7778,  16384 };

const int16_t a_1[] = { 16384, -10565,  16063 };
const int16_t a_2[] = { 16384,  -9485,  16060 };

int16_t filter(int16_t x, const int16_t* a, const int16_t* b, int16_t* xbuf, int16_t* ybuf);

int16_t xbuf_1[3] = {0};
int16_t ybuf_1[3] = {0};

int16_t xbuf_2[3] = {0};
int16_t ybuf_2[3] = {0};

void setup(void)
{
  pinMode(inDataPin, INPUT);
  
  Serial.begin(115200);
}

void loop(void)
{
  int16_t inData, outData;
  if (inData = analogRead(inDataPin)) {
    outData = filter(inData, a_1, b_1, xbuf_1, ybuf_1);
    outData = filter(outData, a_2, b_2, xbuf_2, ybuf_2);
  } else {
    delayMicroseconds(100);
  }
}

int16_t filter(int16_t x, const int16_t* a, const int16_t* b, int16_t* xbuf, int16_t* ybuf)
{
  int16_t res;
  
  int32_t num = (x*b[0] + xbuf[0]*b[1] + xbuf[1]*b[2])/scale;
  int32_t den = (ybuf[0]*a[0] + ybuf[1]*a[1] + ybuf[2]*a[2])/scale;
  
  xbuf[2] = xbuf[1];
  xbuf[1] = xbuf[0];
  xbuf[0] = x;
  
  ybuf[2] = ybuf[1];
  ybuf[1] = ybuf[0];
  ybuf[0] = res;
  
  res = num - den;
  
  return res;
}

