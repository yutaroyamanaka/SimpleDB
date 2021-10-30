/* Copyright 2021 Yutaro Yamanaka */
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "multibuffer/bufferneeds.hpp"
#include "multibuffer/chunkscan.hpp"
#include "record/layout.hpp"
#include "record/schema.hpp"
#include "tx/transaction.hpp"
#include "scan/constant.hpp"
#include "scan/scan.hpp"
#include "scan/productscan.hpp"

namespace multibuffer {
class MultiBufferProductScan : public scan::Scan {
 public:
    MultiBufferProductScan(const std::shared_ptr<scan::Scan>& lhsscan, const std::string& filename, const record::Layout& layout, tx::Transaction* transaction);
    void beforeFirst() override;
    bool next() override;
    void close() override;
    scan::Constant getVal(const std::string& fldname) override;
    int getInt(const std::string& fldname) override;
    std::string getString(const std::string& fldname) override;
    bool hasField(const std::string& fldname) override;
 private:
    bool useNextChunk();
    tx::Transaction* transaction_;
    std::shared_ptr<scan::Scan> lhsscan_, rhsscan_, prodscan_;
    std::string filename_;
    record::Layout layout_;
    int chunksize_, nextblknum_, filesize_;
};
}  // namespace multibuffer
