#pragma once
#include <SimulationChannelDescriptor.h>
class EdgeAnalyzerSettings;

class EdgeSimulationDataGenerator
{
public:
    EdgeSimulationDataGenerator();
    ~EdgeSimulationDataGenerator();

    void Initialize( U32 simulation_sample_rate, EdgeAnalyzerSettings* settings );
    U32  GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
    U32 mSimSampleRateHz;
    EdgeAnalyzerSettings* mSettings;

    SimulationChannelDescriptor mChannel;
    U64 mNextTransition;
    bool mState;
};
