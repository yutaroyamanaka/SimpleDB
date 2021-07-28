#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "transaction.hpp"
#include "schema.hpp"
#include "layout.hpp"
#include "tablescan.hpp"
#include "simpledb.hpp"
#include "metadatamanager.hpp"

TEST(MetadataManagerTest, Main) {
  std::string file_name = "metadatamgrtest";
  simpledb::SimpleDB db(file_name, 400, 8);
  auto transaction = db.getNewTx();
  meta::MetaDataManager mdm(true, transaction.get());

  record::Schema sch;
  sch.addIntField("A");
  sch.addStringField("B", 9);

  // Part 1: Table Metadata
  mdm.createTable("MyTable", sch, transaction.get());
  record::Layout layout = mdm.getLayout("MyTable", transaction.get());
  int size = layout.slotSize();
  record::Schema sch2 = layout.schema();
  std::cout << "MyTable has slot size " << size << std::endl;
  std::cout << "Its fields are:" << std::endl;

  for(auto& fldname: sch2.fields()) {
    std::string type;
    if(sch2.type(fldname) == record::Schema::INTEGER) {
      type = "int";
    } else if(sch2.type(fldname) == record::Schema::VARCHAR) {
      int strlen = sch2.length(fldname);
      type = "varchar(" + std::to_string(strlen) + ")";
    }
    std::cout << fldname << ": " << type << std::endl;
  }

  // Part 2: Statistics Metadata
  record::TableScan ts(transaction.get(), "MyTable", layout);

  for(int i = 0; i < 50; i++) {
    ts.insert();
    int n = i;
    ts.setInt("A", n);
    ts.setString("B", "rec" + std::to_string(n));
  }

  meta::StatInfo si = mdm.getStatInfo("MyTable", layout, transaction.get());
  std::cout << "B(MyTable) = " << si.blocksAccessed() << std::endl; 
  std::cout << "R(MyTable) = " << si.recordOutput() << std::endl; 
  std::cout << "V(MyTable, A) = " << si.distinctValues("A") << std::endl; 
  std::cout << "V(MyTable, B) = " << si.distinctValues("B") << std::endl; 

  // Part 3: View Metadata
  std::string viewdef = "select B from MyTable where A = 1";
  mdm.createView("viewA", viewdef, transaction.get());
  std::string v = mdm.getViewDef("viewA", transaction.get());
  std::cout << "View def = " << v << std::endl;

  // Part 4: Index Metadata
  mdm.createIndex("indexA", "MyTable", "A", transaction.get());
  mdm.createIndex("indexB", "MyTable", "B", transaction.get());
  std::map<std::string, meta::IndexInfo> idxmap = mdm.getIndexInfo("MyTable", transaction.get());

  meta::IndexInfo ii = idxmap["A"];
  std::cout << "B(indexA) = " << ii.blocksAccessed() << std::endl;
  std::cout << "R(indexA) = " << ii.recordOutput() << std::endl;
  std::cout << "V(indexA, A) = " << ii.distinctValues("A") << std::endl;
  std::cout << "V(indexA, B) = " << ii.distinctValues("B") << std::endl;

  ii = idxmap["B"];
  std::cout << "B(indexB) = " << ii.blocksAccessed() << std::endl;
  std::cout << "R(indexB) = " << ii.recordOutput() << std::endl;
  std::cout << "V(indexB, A) = " << ii.distinctValues("B") << std::endl;
  std::cout << "V(indexB, B) = " << ii.distinctValues("B") << std::endl;
  transaction->commit();
 
}
