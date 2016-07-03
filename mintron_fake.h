/*
   INDI driver to manage Mintron Cameras
   Copyright (C) 2016 Riccardo Stagni (unriccio@email.it)
 */

#ifndef MINTRONFAKE_H
#define MINTRONFAKE_H

#include <defaultdevice.h>

class MintronFake : public INDI::DefaultDevice {
	public:
		MintronFake();
		virtual ~MintronFake();

		void ISGetProperties(const char *dev);


	protected:
		const char *getDefaultName();

		bool Connect();
		bool Disconnect();

		bool initProperties();
		bool updateProperties();

		virtual bool saveConfigItems(FILE *fp);

	private:
		// Device physical port
		ITextVectorProperty PortTP;
		IText PortT[1];

		// Baud Rate
		ISwitch BaudRateS[6];
		ISwitchVectorProperty BaudRateSP;

		int PortFD;
};

#endif // MINTRONFAKE_H
