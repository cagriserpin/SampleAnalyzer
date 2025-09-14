#pragma once
#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

enum class EdgeMode : U32 { Rising = 0, Falling = 1, Any = 2 };

class EdgeAnalyzerSettings : public AnalyzerSettings
{
public:
    EdgeAnalyzerSettings();
    virtual ~EdgeAnalyzerSettings();

    Channel  mChannel;
    EdgeMode mMode;
    U64      mMinPulseNs; // 0 = disabled

    virtual bool SetSettingsFromInterfaces() override;
    void         UpdateInterfacesFromSettings();
    virtual void LoadSettings( const char* settings ) override;
    virtual const char* SaveSettings() override;

protected:
    std::auto_ptr<AnalyzerSettingInterfaceChannel>     mChannelInterface;
    std::auto_ptr<AnalyzerSettingInterfaceNumberList>  mModeInterface;
    std::auto_ptr<AnalyzerSettingInterfaceInteger>     mMinPulseNsInterface;
};
