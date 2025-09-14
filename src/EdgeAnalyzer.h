#pragma once
#include <Analyzer.h>
#include "EdgeAnalyzerSettings.h"
#include "EdgeSimulationDataGenerator.h"

class EdgeAnalyzerResults;

class EdgeAnalyzer : public Analyzer2
{
public:
    EdgeAnalyzer();
    virtual ~EdgeAnalyzer();

    // Analyzer2
    virtual void SetupResults() override;
    virtual void WorkerThread() override;

    // Analyzer (pure virtuals we must satisfy)
    virtual U32 GetMinimumSampleRateHz() override;
    virtual const char* GetAnalyzerName() const override;
    virtual bool NeedsRerun() override;
    virtual U32 GenerateSimulationData( U64 largest_sample_requested,
                                        U32 sample_rate,
                                        SimulationChannelDescriptor** simulation_channels ) override;

protected:
    std::auto_ptr<EdgeAnalyzerSettings>   mSettings;
    std::auto_ptr<EdgeAnalyzerResults>    mResults;
    std::auto_ptr<AnalyzerChannelData>    mData;

    EdgeSimulationDataGenerator           mSim;
};
