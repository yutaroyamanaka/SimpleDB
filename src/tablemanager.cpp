/* Copyright 2021 Yutaro Yamanaka */
#include "tablemanager.hpp"
#include "tablescan.hpp"
#include "schema.hpp"

namespace meta {
  TableManager::TableManager(bool isNew, tx::Transaction* transaction) {
    record::Schema tcat_schema;
    tcat_schema.addStringField("tblname", MAX_NAME);
    tcat_schema.addIntField("slotsize");
    tcat_layout_ = std::make_unique<record::Layout>(tcat_schema);

    record::Schema fcat_schema;
    fcat_schema.addStringField("tblname", MAX_NAME);
    fcat_schema.addStringField("fldname", MAX_NAME);
    fcat_schema.addIntField("type");
    fcat_schema.addIntField("length");
    fcat_schema.addIntField("offset");
    fcat_layout_ = std::make_unique<record::Layout>(fcat_schema);

    if (isNew) {
      createTable("tblcat", tcat_schema, transaction);
      createTable("fldcat", fcat_schema, transaction);
    }
  }

  void TableManager::createTable(const std::string& tblname, const record::Schema& schema, tx::Transaction* transaction) {
    record::Layout layout(schema);
    // insert one record into tblcat
    record::TableScan tcat(transaction, "tblcat", *tcat_layout_);
    tcat.insert();
    tcat.setString("tblname", tblname);
    tcat.setInt("slotsize", layout.slotSize());
    tcat.close();
    // insert a record into fldcat for each field
    record::TableScan fcat(transaction, "fldcat", *fcat_layout_);
    for (const auto& fldname : schema.fields()) {
      fcat.insert();
      fcat.setString("tblname", tblname);
      fcat.setString("fldname", fldname);
      fcat.setInt("type", schema.type(fldname));
      fcat.setInt("length", schema.length(fldname));
      fcat.setInt("offset", layout.offset(fldname));
    }
    fcat.close();
  }

  record::Layout TableManager::getLayout(const std::string& tblname, tx::Transaction* transaction) {
    int size = -1;
    record::TableScan tcat(transaction, "tblcat", *tcat_layout_);
    while (tcat.next()) {
      if (tcat.getString("tblname") == tblname) {
        size = tcat.getInt("slotsize");
        break;
      }
    }
    tcat.close();
    record::Schema sch;
    std::map<std::string, int> offsets;
    record::TableScan fcat(transaction, "fldcat", *fcat_layout_);

    while (fcat.next()) {
      if (fcat.getString("tblname") == tblname) {
        std::string fldname = fcat.getString("fldname");
        int fldtype = fcat.getInt("type");
        int fldlen = fcat.getInt("length");
        int offset = fcat.getInt("offset");
        offsets[fldname] = offset;
        if (fldtype == record::Schema::INTEGER) {
          sch.addIntField(fldname);
        } else if (fldtype == record::Schema::VARCHAR) {
          sch.addStringField(fldname, fldlen);
        }
      }
    }
    fcat.close();
    record::Layout layout(sch, offsets, size);
    return layout;
  }
}  // namespace meta
