#pragma once

#include <Analyzer.h>
#include "EdgeAnalyzerSettings.h"

class EdgeAnalyzerResults;

class EdgeAnalyzer : public Analyzer2
{
public:
    EdgeAnalyzer();
    virtual ~EdgeAnalyzer();

    virtual void SetupResults() override;
    virtual void WorkerThread() override;

protected:
    std::auto_ptr<EdgeAnalyzerSettings> mSettings;
    std::auto_ptr<EdgeAnalyzerResults>  mResults;

    std::auto_ptr<AnalyzerChannelData>  mData;
};
