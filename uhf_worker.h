#pragma once

#include <furi.h>
#include <furi_hal.h>
#include "uhf_data.h"

typedef enum {
    // Init states
    UHFWorkerStateNone,
    UHFWorkerStateBroken,
    UHFWorkerStateReady,
    UHFWorkerStateVerify,
    // Main worker states
    UHFWorkerStateDetectSingle,
    UHFWorkerStateWriteSingle,
    UHFWorkerStateWriteKey,
    // Transition
    UHFWorkerStateStop,
} UHFWorkerState;

typedef enum {
    UHFWorkerEventSuccess,
    UHFWorkerEventFail,
    UHFWorkerEventNoTagDetected,
    UHFWorkerEventAborted,
    UHFWorkerEventCardDetected,
} UHFWorkerEvent;

typedef void (*UHFWorkerCallback)(UHFWorkerEvent event, void* ctx);

typedef struct UHFWorker {
    FuriThread* thread;
    UHFResponseData* response_data;
    UHFWorkerCallback callback;
    UHFWorkerState state;
    void* ctx;
} UHFWorker;

int32_t uhf_worker_task(void* ctx);
UHFWorker* uhf_worker_alloc();
void uhf_worker_change_state(UHFWorker* worker, UHFWorkerState state);
void uhf_worker_start(
    UHFWorker* uhf_worker,
    UHFWorkerState state,
    UHFWorkerCallback callback,
    void* ctx);
void uhf_worker_stop(UHFWorker* uhf_worker);
void uhf_worker_free(UHFWorker* uhf_worker);
uint8_t calculate_checksum(UHFData* uhf_data);
bool validate_checksum(UHFData* uhf_data);