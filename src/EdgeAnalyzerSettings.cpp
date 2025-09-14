#include "EdgeAnalyzerSettings.h"
#include <AnalyzerHelpers.h>

EdgeAnalyzerSettings::EdgeAnalyzerSettings()
    : mChannel( UNDEFINED_CHANNEL ),
      mMode( EdgeMode::Rising ),
      mMinPulseNs( 0 )
{
    mChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
    mChannelInterface->SetTitleAndTooltip( "Channel", "Digital channel to monitor." );
    mChannelInterface->SetChannel( mChannel );

    mModeInterface.reset( new AnalyzerSettingInterfaceNumberList() );
    mModeInterface->SetTitleAndTooltip( "Mode", "Which edges to report." );
    mModeInterface->AddNumber( (U64)EdgeMode::Rising,  "RISING",  "Report rising edges only." );
    mModeInterface->AddNumber( (U64)EdgeMode::Falling, "FALLING", "Report falling edges only." );
    mModeInterface->AddNumber( (U64)EdgeMode::Any,     "ANY",     "Report both rising and falling." );
    mModeInterface->SetNumber( (U64)mMode );

    mMinPulseNsInterface.reset( new AnalyzerSettingInterfaceInteger() );
    mMinPulseNsInterface->SetTitleAndTooltip( "Min Pulse (ns)", "Ignore pulses shorter than this (0 = disabled)." );
    mMinPulseNsInterface->SetMin( 0 );
    mMinPulseNsInterface->SetMax( 10000000000ULL );
    mMinPulseNsInterface->SetInteger( (S64)mMinPulseNs );

    AddInterface( mChannelInterface.get() );
    AddInterface( mModeInterface.get() );
    AddInterface( mMinPulseNsInterface.get() );

    ClearChannels();
    AddChannel( mChannel, "Edge", false );
}
EdgeAnalyzerSettings::~EdgeAnalyzerSettings() {}

bool EdgeAnalyzerSettings::SetSettingsFromInterfaces()
{
    mChannel    = mChannelInterface->GetChannel();
    mMode       = (EdgeMode)(U32)mModeInterface->GetNumber();
    mMinPulseNs = (U64)mMinPulseNsInterface->GetInteger();

    ClearChannels();
    AddChannel( mChannel, "Edge", true );
    return true;
}

void EdgeAnalyzerSettings::UpdateInterfacesFromSettings()
{
    mChannelInterface->SetChannel( mChannel );
    mModeInterface->SetNumber( (U64)mMode );
    mMinPulseNsInterface->SetInteger( (S64)mMinPulseNs );
}

void EdgeAnalyzerSettings::LoadSettings( const char* settings )
{
    SimpleArchive ar;
    ar.SetString( settings );

    // Channel has streaming operators; don’t cast to U32.
    ar >> mChannel;
    U32 mode = 0;
    ar >> mode;
    ar >> mMinPulseNs;

    mMode = (EdgeMode)mode;

    ClearChannels();
    AddChannel( mChannel, "Edge", true );
    UpdateInterfacesFromSettings();
}

const char* EdgeAnalyzerSettings::SaveSettings()
{
    SimpleArchive ar;
    ar << mChannel;
    ar << (U32)mMode;
    ar << (U64)mMinPulseNs;
    return SetReturnString( ar.GetString() );
}
