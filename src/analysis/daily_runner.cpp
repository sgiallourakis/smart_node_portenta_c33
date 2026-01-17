#include "daily_runner.h"
#include "storage/storage.h"
#include "battery_analysis.h"

void run_daily_analysis() {
    battery_analysis_run();
}