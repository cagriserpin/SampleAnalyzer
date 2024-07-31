#ifndef SIMPLESERIAL_ANALYZER_SETTINGS
#define SIMPLESERIAL_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class ANALYZER_EXPORT SimpleSerialAnalyzerSettings : public AnalyzerSettings
{
public:
	SimpleSerialAnalyzerSettings();
	virtual ~SimpleSerialAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mInputChannel;
	U32 mBitRate;

protected:
	AnalyzerSettingInterfaceChannel	mInputChannelInterface;
	AnalyzerSettingInterfaceInteger	mBitRateInterface;
};

#endif //SIMPLESERIAL_ANALYZER_SETTINGS
