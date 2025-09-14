#pragma once
#include <AnalyzerResults.h>

class EdgeAnalyzer;
class EdgeAnalyzerSettings;

class EdgeAnalyzerResults : public AnalyzerResults
{
public:
    EdgeAnalyzerResults( EdgeAnalyzer* analyzer, EdgeAnalyzerSettings* settings );
    virtual ~EdgeAnalyzerResults();

    // Required overrides for legacy Frame output
    virtual void GenerateBubbleText(U64 frame_index, Channel& channel, DisplayBase display_base) override;
    virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id ) override;
    virtual void GeneratePacketTabularText(U64 packet_id, DisplayBase display_base) override;
    virtual void GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base) override;
    virtual void GenerateFrameTabularText( U64 frame_index, DisplayBase display_base ) override;

protected:
    EdgeAnalyzerSettings* mSettings;
};
