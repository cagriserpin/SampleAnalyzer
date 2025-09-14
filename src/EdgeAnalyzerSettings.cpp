#include "EdgeAnalyzerSettings.h"
#include <AnalyzerHelpers.h>
#include <sstream>

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
    SimpleArchive text_archive;
    text_archive.SetString( settings );

    U32 ch = 0, mode = 0;
    U64 minpulse = 0;

    text_archive >> ch;
    text_archive >> mode;
    text_archive >> minpulse;

    mChannel    = (Channel)ch;
    mMode       = (EdgeMode)mode;
    mMinPulseNs = minpulse;

    ClearChannels();
    AddChannel( mChannel, "Edge", true );

    UpdateInterfacesFromSettings();
}

const char* EdgeAnalyzerSettings::SaveSettings()
{
    SimpleArchive text_archive;
    text_archive << (U32)mChannel;
    text_archive << (U32)mMode;
    text_archive << (U64)mMinPulseNs;
    return SetReturnString( text_archive.GetString() );
}
