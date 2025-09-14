#include "EdgeSimulationDataGenerator.h"
#include "EdgeAnalyzerSettings.h"

EdgeSimulationDataGenerator::EdgeSimulationDataGenerator()
: mSimSampleRateHz(0),
  mSettings(nullptr),
  mNextTransition(0),
  mState(false)
{
}
EdgeSimulationDataGenerator::~EdgeSimulationDataGenerator() {}

void EdgeSimulationDataGenerator::Initialize( U32 simulation_sample_rate, EdgeAnalyzerSettings* settings )
{
    mSimSampleRateHz = simulation_sample_rate;
    mSettings = settings;

    mChannel.SetChannel( settings->mChannel );
    mChannel.SetSampleRate( simulation_sample_rate );
    mChannel.SetInitialBitState( BIT_LOW );

    mState = false;
    mNextTransition = 0;
}

U32 EdgeSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 /*sample_rate*/, SimulationChannelDescriptor** simulation_channel )
{
    const U64 period = 1000; // arbitrary slow square wave
    while( mChannel.GetCurrentSampleNumber() < largest_sample_requested )
    {
        if( mChannel.GetCurrentSampleNumber() >= mNextTransition )
        {
            mState = !mState;
            mChannel.Transition();
            mNextTransition = mChannel.GetCurrentSampleNumber() + period;
        }
        mChannel.Advance( 1 );
    }

    *simulation_channel = &mChannel;
    return 1;
}
