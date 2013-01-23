class SevenSegmentsLED(object):

    _digits = { 0: [1, 1, 1, 1, 1, 1, 0, 0],
                1: [0, 0, 1, 1, 0, 0, 0, 0],
                2: [0, 1, 1, 0, 1, 1, 1, 0],
                3: [0, 1, 1, 1, 1, 0, 1, 0],
                4: [1, 0, 1, 1, 0, 0, 1, 0],
                5: [1, 1 ,0, 1, 1, 0, 1, 0],
                6: [1, 1, 0, 1, 1, 1, 1, 0],
                7: [0, 1, 1, 1, 0, 0, 0, 0],
                8: [1, 1 ,1, 1, 1, 1, 1, 0],
                9: [1, 1 ,1, 1, 1, 0, 1, 0] }

    _segments = { "a": 0,
                  "b": 1,
                  "c": 2,
                  "d": 3,
                  "e": 4,
                  "f": 5,
                  "g": 6,
                  "h": 7 }

    def __init__(self):
        self.digits = self._digits
        self.segments = self._segments
    
    def getNum(self, digit):
        num = 0
        for i in self.digits[digit]:
            num = (num<<1) + i 
        return num

    def printHex(self):
        for k in self.digits:
            print("%d: 0x%02x" % (k, self.getNum(k)))

    def setPlacement(self, placement, inverted=False):
        for d in range(len(self.digits)):
            for seg in placement:
                if inverted:
                    self.digits[d][placement[seg]] = abs(self._digits[d][self._segments[seg]] - 1)
                else:
                    self.digits[d][placement[seg]] = self._digits[d][self._segments[seg]]


placement = { "a": 2,
              "b": 0,
              "c": 1,
              "d": 3,
              "e": 5,
              "f": 6,
              "g": 4,
              "h": 7 }

ind = SevenSegmentsLED()

ind.setPlacement(placement, True)
ind.printHex()
