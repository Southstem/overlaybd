#pragma once

#include <sys/uio.h>
#include <string>

#include "../../../../photon/thread.h"
#include "../../../../fs/filesystem.h"
#include "../../../../fs/cache/ocf_cache/ocf_namespace.h"

extern "C"
{
#include <ocf/ocf.h>
}

#define ROUND_UP(N, S) ((((N) + (S) - 1) / (S)) * (S))
#define ROUND_DOWN(N, S) ((N) & ~((S) - 1))

class ease_ocf_provider;

/* Src file context */
struct OcfSrcFileCtx {
    OcfSrcFileCtx(FileSystem::IFile* src_file_, const OcfNamespace::NsInfo& ns_info_, ease_ocf_provider* provider_) :
            src_file(src_file_), ns_info(ns_info_), provider(provider_) {}

    ~OcfSrcFileCtx() {
        delete src_file;
    }

    FileSystem::IFile* src_file;
    OcfNamespace::NsInfo ns_info;
    ease_ocf_provider* provider;
};

/* IO data */
struct ease_ocf_io_data {
    ease_ocf_io_data() = default;

    ease_ocf_io_data(iovec* iovs_, int iovcnt_, uint32_t size_, off_t blk_addr_, OcfSrcFileCtx* ctx_, bool prefetch_) :
            iovs(iovs_), iovcnt(iovcnt_), size(size_), blk_addr(blk_addr_), ctx(ctx_), prefetch(prefetch_) {}

    /* basic IO related */
    iovec *iovs = nullptr;
    int iovcnt = 0;
    uint32_t size = 0;
    uint32_t seek = 0;

    /* ease related */
    off_t blk_addr = -1;
    OcfSrcFileCtx* ctx = nullptr;
    int err_no = 0;
    photon::semaphore sem;
    bool prefetch = false;
};

struct ease_ocf_config {
    /* Initial cache configuration  */
    ocf_mngt_cache_config cache;

    /* Cache device config */
    ocf_mngt_cache_device_config device;

    /* Core initial config */
    ocf_mngt_core_config core;
};

struct ease_ocf_queue {
    ocf_queue_t mngt_queue;
    ocf_queue_t io_queue;
};

/* Context config */
const ocf_ctx_config* get_context_config();
