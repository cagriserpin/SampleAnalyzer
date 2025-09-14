#include "EdgeAnalyzerResults.h"
#include "EdgeAnalyzer.h"
#include "EdgeAnalyzerSettings.h"
#include <AnalyzerHelpers.h>
#include <fstream>

EdgeAnalyzerResults::EdgeAnalyzerResults( EdgeAnalyzer* /*analyzer*/, EdgeAnalyzerSettings* settings )
: AnalyzerResults(),
  mSettings( settings )
{
}
EdgeAnalyzerResults::~EdgeAnalyzerResults() {}

void EdgeAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel /*channel*/, DisplayBase /*display_base*/ )
{
    ClearResultStrings();

    Frame f = GetFrame( frame_index );           // legacy Frame (V1)
    const char* edge_str = (f.mData1 ? "RISING" : "FALLING");
    AddResultString( edge_str );
}

void EdgeAnalyzerResults::GenerateExportFile( const char* file, DisplayBase /*display_base*/, U32 /*export_type_user_id*/ )
{
    std::ofstream s( file, std::ios::out );
    if( !s.is_open() ) return;

    s << "time_s,edge,width_samples\n";
    const U64 n = GetNumFrames();
    for( U64 i = 0; i < n; ++i )
    {
        Frame f = GetFrame( i );
        const double t_s = f.mStartingSampleInclusive / (double)GetSampleRate();
        char tbuf[64] = {};
        AnalyzerHelpers::DoubleToString( t_s, 10, tbuf, sizeof(tbuf) );
        s << tbuf << "," << (f.mData1 ? "RISING" : "FALLING") << "," << (U64)f.mData2 << "\n";
    }
    s.close();
}

void EdgeAnalyzerResults::GeneratePacketTabularText( U64 /*packet_id*/, DisplayBase /*display_base*/ )
{
    ClearResultStrings();
}
void EdgeAnalyzerResults::GenerateTransactionTabularText( U64 /*transaction_id*/, DisplayBase /*display_base*/ )
{
    ClearResultStrings();
}
void EdgeAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase /*display_base*/ )
{
    ClearTabularText();
    Frame f = GetFrame( frame_index );
    AddTabularText( f.mData1 ? "RISING" : "FALLING" );
}
