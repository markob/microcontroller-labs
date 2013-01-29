oscillators = [
	1000000,
	1843200,
	2000000,
	3686400,
	4000000,
	7372800,
	8000000,
	11059200,
	12000000,
	14745600,
	16000000,
	18000000,
	18432000,
	20000000,
	20275000,
	22118000,
	24000000,
	24118400,
	24745600]

def CalculateParamsForUART(oscFreq, desSpeed, doubleSpeed=False):
	doubleSpeedCoef = 1
	if doubleSpeed:
		doubleSpeedCoef = 2
				
	regVal = 256 - oscFreq/384/desSpeed/doubleSpeedCoef
	if regVal == 256:
		print "Unable to achieve desired transfer rate %d with Fosc=%d" % (desSpeed, oscFreq)
	else:
		error = (oscFreq/384.0/(256 - regVal)/doubleSpeedCoef - desSpeed)/desSpeed*100
		print oscFreq/384.0/(256 - regVal)/doubleSpeedCoef
		print "To get transfer rate %d using Fosc=%d use reg value %d, speed error is %.2f persents" % (desSpeed, oscFreq, regVal, error)
		
def CalculateParamsForUART2(oscFreq, desSpeed):
	regVal = 65536 - oscFreq/32/desSpeed
	if regVal == 65536:
		print "Unable to achieve desired transfer rate %d with Fosc=%d" % (desSpeed, oscFreq)
	else:
		error = (oscFreq/32.0/(65536 - regVal) - desSpeed)/desSpeed*100
		print "To get transfer rate %d using Fosc=%d use reg value %d, speed error is %.2f persents" % (desSpeed, oscFreq, regVal, error)
		
def ShowOscillatorsForUART(desSpeed, doubleSpeed=False):
	for freq in oscillators:
		CalculateParamsForUART(freq, desSpeed, doubleSpeed)

ShowOscillatorsForUART(9600)