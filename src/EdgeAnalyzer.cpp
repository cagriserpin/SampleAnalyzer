#include "EdgeAnalyzer.h"
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

    // Critical for Trigger View: emit FrameV2, not legacy Frame.
    UseFrameV2();

    mResults->AddChannelBubblesWillAppearOn( mSettings->mChannel );
}

void EdgeAnalyzer::WorkerThread()
{
    mData.reset( GetAnalyzerChannelData( mSettings->mChannel ) );

    const U32 sample_rate_hz = GetSampleRate();
    const double ns_per_sample = sample_rate_hz ? (1e9 / (double)sample_rate_hz) : 0.0;

    const bool report_rise = ( mSettings->mMode == EdgeMode::Rising );
    const bool report_fall = ( mSettings->mMode == EdgeMode::Falling );
    const bool report_any  = ( mSettings->mMode == EdgeMode::Any );

    const U64 min_pulse_samples = ( mSettings->mMinPulseNs == 0 || ns_per_sample == 0.0 )
        ? 0
        : (U64)((double)mSettings->mMinPulseNs / ns_per_sample);

    // Prime
    BitState prev = mData->GetBitState();
    U64 last_edge_sample = mData->GetSampleNumber();

    for (;;)
    {
        if (CheckIfThreadShouldExit())
            break;

        if (!mData->AdvanceToNextEdge())
            break;

        const U64 s = mData->GetSampleNumber();
        const BitState now = mData->GetBitState();

        const bool rising  = (prev == BIT_LOW  && now == BIT_HIGH);
        const bool falling = (prev == BIT_HIGH && now == BIT_LOW);

        // Deglitch: require previous level duration
        const U64 width_samples = s - last_edge_sample;
        if (min_pulse_samples && width_samples < min_pulse_samples)
        {
            prev = now;
            last_edge_sample = s;
            continue;
        }

        if (report_any || (report_rise && rising) || (report_fall && falling))
        {
            FrameV2 f("edge", s, s);          // instantaneous at transition
            f.AddString("edge", rising ? "RISING" : "FALLING");
            f.AddString("channel", ChannelShortDisplayName(mSettings->mChannel));
            f.AddNumber("width_samples", (double)width_samples);  // duration of previous level
            mResults->AddFrameV2(f);
            mResults->CommitResults();
        }

        prev = now;
        last_edge_sample = s;
        ReportProgress(s);
    }
}
