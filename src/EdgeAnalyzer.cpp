#include "EdgeAnalyzerResults.h"
#include <AnalyzerChannelData.h>

extern "C" ANALYZER_EXPORT const char* GetAnalyzerName()
{
    return "Edge Analyzer";
}
extern "C" ANALYZER_EXPORT Analyzer* CreateAnalyzer()
{
    return new EdgeAnalyzer();
}
extern "C" ANALYZER_EXPORT void DestroyAnalyzer( Analyzer* analyzer )
{
    delete analyzer;
}

EdgeAnalyzer::EdgeAnalyzer()
{
    mSettings.reset( new EdgeAnalyzerSettings() );
    SetAnalyzerSettings( mSettings.get() );
}
EdgeAnalyzer::~EdgeAnalyzer() {}

void EdgeAnalyzer::SetupResults()
{
    mResults.reset( new EdgeAnalyzerResults( this, mSettings.get() ) );
    SetAnalyzerResults( mResults.get() );

    // NOTE: Using legacy Frame for now (works everywhere).
    // When you switch to FrameV2, call UseFrameV2() here.
    // UseFrameV2();

    mResults->AddChannelBubblesWillAppearOn( mSettings->mChannel );
}

void EdgeAnalyzer::WorkerThread()
{
    mData.reset(GetAnalyzerChannelData(mSettings->mChannel));

    const U32 sr = GetSampleRate();
    const double ns_per_sample = sr ? (1e9 / (double)sr) : 0.0;

    const bool report_rise = (mSettings->mMode == EdgeMode::Rising);
    const bool report_fall = (mSettings->mMode == EdgeMode::Falling);
    const bool report_any  = (mSettings->mMode == EdgeMode::Any);

    const U64 min_pulse_samples = (mSettings->mMinPulseNs == 0 || ns_per_sample == 0.0)
        ? 0 : (U64)((double)mSettings->mMinPulseNs / ns_per_sample);

    BitState prev = mData->GetBitState();
    U64 last_edge_sample = mData->GetSampleNumber();

    for (;;)
    {
        CheckIfThreadShouldExit();

        // IMPORTANT: In this SDK, AdvanceToNextEdge() returns void.
        if (mData->DoMoreTransitionsExistInCurrentData() == false)
        {
            ReportProgress(mData->GetSampleNumber());
            mData->Advance(1000);   // keep UI responsive
            continue;
        }

        mData->AdvanceToNextEdge();     // no return value
        const U64 s = mData->GetSampleNumber();
        const BitState now = mData->GetBitState();

        const bool rising  = (prev == BIT_LOW  && now == BIT_HIGH);
        const bool falling = (prev == BIT_HIGH && now == BIT_LOW);

        const U64 width_samples = s - last_edge_sample;
        if (min_pulse_samples && width_samples < min_pulse_samples)
        {
            prev = now;
            last_edge_sample = s;
            continue;
        }

        if (report_any || (report_rise && rising) || (report_fall && falling))
        {
            Frame frame;
            frame.mStartingSampleInclusive = s;
            frame.mEndingSampleInclusive   = s;
            frame.mData1 = rising ? 1 : 0;      // 1 = RISING, 0 = FALLING
            frame.mData2 = (U64)width_samples;  // duration of previous level (samples)
            mResults->AddFrame(frame);
            mResults->CommitResults();
        }

        prev = now;
        last_edge_sample = s;
        ReportProgress(s);
    }
}



U32 EdgeAnalyzer::GetMinimumSampleRateHz()
{
    // Any realistic value; Logic will warn if too low. 1 Hz is the usual sample template value.
    return 1;
}
const char* EdgeAnalyzer::GetAnalyzerName() const
{
    return ::GetAnalyzerName();
}
bool EdgeAnalyzer::NeedsRerun()
{
    return false;
}
U32 EdgeAnalyzer::GenerateSimulationData( U64 largest_sample_requested,
                                          U32 sample_rate,
                                          SimulationChannelDescriptor** simulation_channels )
{
    mSim.Initialize( sample_rate, mSettings.get() );
    return mSim.GenerateSimulationData( largest_sample_requested, sample_rate, simulation_channels );
}
