/*
   INDI driver to manage Mintron Cameras
   Copyright (C) 2016 Riccardo Stagni (unriccio@email.it)
 */

#include <memory>
#include <cstdlib>

#include "indicom.h"
#include "indilogger.h"

#include "mintron_fake.h"

std::auto_ptr<MintronFake> mintronfake(0);

void ISInit() {
	static int isInit=0;
	if (isInit)
		return;
	if (mintronfake.get() == 0)
	{
		isInit = 1;
		mintronfake.reset(new MintronFake());
	}
}

void ISGetProperties (const char *dev) {
	ISInit();
	mintronfake->ISGetProperties(dev);
}

void ISNewSwitch (const char *dev, const char *name, ISState *states, char *names[], int n) {
	ISInit();
	mintronfake->ISNewSwitch(dev, name, states, names, n);
}

void ISNewText (const char *dev, const char *name, char *texts[], char *names[], int n) {
	ISInit();
	mintronfake->ISNewText(dev, name, texts, names, n);
}

void ISNewNumber (const char *dev, const char *name, double values[], char *names[], int n) {
	ISInit();
	mintronfake->ISNewNumber(dev, name, values, names, n);
}

void ISNewBLOB (const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[], char *formats[], char *names[], int n) {
	INDI_UNUSED(dev);
	INDI_UNUSED(name);
	INDI_UNUSED(sizes);
	INDI_UNUSED(blobsizes);
	INDI_UNUSED(blobs);
	INDI_UNUSED(formats);
	INDI_UNUSED(names);
	INDI_UNUSED(n);
}

void ISSnoopDevice (XMLEle *root) {
	ISInit();
	mintronfake->ISSnoopDevice(root);
}


MintronFake::MintronFake() {
	PortFD=-1;
}

MintronFake::~MintronFake() {
}

const char * MintronFake::getDefaultName() {
	return "Mintron Fake";
}

bool MintronFake::Connect() {
	int connectrc = 0;
	char errorMsg[MAXRBUF];

	DEBUGF(INDI::Logger::DBG_SESSION, "Connecting to %s at %d baud", PortT[0].text, atoi(IUFindOnSwitch(&BaudRateSP)->name));

	if ((connectrc = tty_connect(PortT[0].text, atoi(IUFindOnSwitch(&BaudRateSP)->name), 8, 0, 1, &PortFD)) != TTY_OK) {
		tty_error_msg(connectrc, errorMsg, MAXRBUF);
		DEBUGF(INDI::Logger::DBG_ERROR, "Failed to connect to port %s. Error: %s", PortT[0].text, errorMsg);
		return false;
	}

	DEBUGF(INDI::Logger::DBG_DEBUG, "Port FD %d", PortFD);

	// TODO: test connection
	DEBUG(INDI::Logger::DBG_SESSION, "Device is online.");

	return true;
}

bool MintronFake::Disconnect() {
	if (PortFD > 0)
		tty_disconnect(PortFD);

	DEBUG(INDI::Logger::DBG_SESSION, "Disconnected successfully!");
	return true;
}

void MintronFake::ISGetProperties(const char *dev) {
	INDI::DefaultDevice::ISGetProperties(dev);

	defineText(&PortTP);
	defineSwitch(&BaudRateSP);

	loadConfig(true, "DEVICE_PORT");
	loadConfig(true, "DEVICE_BAUD_RATE");
}

bool MintronFake::initProperties() {
	INDI::DefaultDevice::initProperties();

	IUFillText(&PortT[0], "PORT", "Port", "/dev/ttyUSB0");
	IUFillTextVector(&PortTP, PortT, 1, getDeviceName(), "DEVICE_PORT", "Ports", OPTIONS_TAB, IP_RW, 60, IPS_IDLE);

	// Is 9600 the only viable option?
	IUFillSwitch(&BaudRateS[0], "1200", "", ISS_OFF);
	IUFillSwitch(&BaudRateS[1], "2400", "", ISS_OFF);
	IUFillSwitch(&BaudRateS[2], "4800", "", ISS_OFF);
	IUFillSwitch(&BaudRateS[3], "9600", "", ISS_ON);
	IUFillSwitch(&BaudRateS[4], "14400", "", ISS_OFF);
	IUFillSwitch(&BaudRateS[5], "19200", "", ISS_OFF);
	IUFillSwitchVector(&BaudRateSP, BaudRateS, 6, getDeviceName(),"DEVICE_BAUD_RATE", "Baud Rate", OPTIONS_TAB, IP_RW, ISR_1OFMANY, 60, IPS_IDLE);

	addDebugControl();

	return true;
}

bool MintronFake::updateProperties() {
	if (isConnected()) {
		// TODO: add define* and deleteProperty
	}

	return INDI::DefaultDevice::updateProperties();
}

bool MintronFake::saveConfigItems(FILE *fp)
{
	INDI::DefaultDevice::saveConfigItems(fp);

	IUSaveConfigText(fp, &PortTP);
	IUSaveConfigSwitch(fp, &BaudRateSP);

	return true;
}
