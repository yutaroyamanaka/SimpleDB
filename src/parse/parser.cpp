/* Copyright 2021 Yutaro Yamanaka */
#include "parse/parser.hpp"

namespace parse {
  Parser::Parser(const std::string& s) {
    lex_ = std::make_unique<Lexer>(s);
  }

  std::string Parser::field() const {
    return lex_->eatId();
  }

  scan::Constant Parser::constant() const {
    if (lex_->matchStringConstant()) {
      scan::Constant c(lex_->eatStringConstant());
      return c;
    } else {
      scan::Constant c(lex_->eatIntConstant());
      return c;
    }
  }

  scan::Term Parser::term() const {
    scan::Expression lhs;
    lex_->eatDelim(EQUAL_SIGN);
    scan::Expression rhs;
    scan::Term t(lhs, rhs);
    return t;
  }

  scan::Predicate Parser::predicate() const {
    scan::Predicate pred(term());
    if (lex_->matchKeyword(AMPERSAND)) {
      lex_->eatKeyword(AMPERSAND);
      pred.conjoinWith(predicate());
    }
    return pred;
  }

  QueryData Parser::query() const {
    lex_->eatKeyword(SELECT);
    std::vector<std::string> fields = selectList();
    lex_->eatKeyword(FROM);
    std::set<std::string> tables = tableList();
    scan::Predicate pred;
    if (lex_->matchKeyword(WHERE)) {
      lex_->eatKeyword(WHERE);
      pred = predicate();
    }
    QueryData qrydata(fields, tables, pred);
    return qrydata;
  }

  std::vector<std::string> Parser::selectList() const {
    std::vector<std::string> L;
    L.emplace_back(field());
    if (lex_->matchDelm(COMMA)) {
      lex_->eatDelim(COMMA);
      for (const auto& sl : selectList()) {
        L.emplace_back(sl);
      }
    }
    return L;
  }

  std::set<std::string> Parser::tableList() const {
    std::set<std::string> L;
    L.insert(lex_->eatId());
    if (lex_->matchDelm(COMMA)) {
      lex_->eatDelim(COMMA);
      for (const auto& name : tableList()) {
        L.insert(name);
      }
    }
    return L;
  }

  Object Parser::updateCmd() const {
    if (lex_->matchKeyword(INSERT)) {
      return insert();
    } else if (lex_->matchKeyword(DELETE)) {
      return remove();
    } else if (lex_->matchKeyword(UPDATE)) {
      return modify();
    } else {
      return create();
    }
  }

  Object Parser::create() const {
    lex_->eatKeyword(CREATE);
    if (lex_->matchKeyword(TABLE)) {
      return createTable();
    } else if (lex_->matchKeyword(VIEW)) {
      return createView();
    } else {
      return createIndex();
    }
  }

  DeleteData Parser::remove() const {
    lex_->eatKeyword(DELETE);
    lex_->eatKeyword(FROM);
    std::string tblname = lex_->eatId();
    scan::Predicate pred;
    if (lex_->matchKeyword(WHERE)) {
      lex_->eatKeyword(WHERE);
      pred = predicate();
    }
    DeleteData dd(tblname, pred);
    return dd;
  }

  InsertData Parser::insert() const {
    lex_->eatKeyword(INSERT);
    lex_->eatKeyword(INTO);
    std::string tblname = lex_->eatId();
    lex_->eatDelim(LEFT_PARENTHESIS);
    std::vector<std::string> flds = fieldList();
    lex_->eatDelim(RIGHT_PARENTHESIS);
    lex_->eatKeyword(VALUES);
    lex_->eatDelim(LEFT_PARENTHESIS);
    std::vector<scan::Constant> vals = constList();
    lex_->eatDelim(RIGHT_PARENTHESIS);
    InsertData id(tblname, flds, vals);
    return id;
  }

  std::vector<std::string> Parser::fieldList() const {
    std::vector<std::string> L;
    L.emplace_back(field());
    if (lex_->matchDelm(COMMA)) {
      lex_->eatDelim(COMMA);
      for (const auto& fld : fieldList()) {
        L.emplace_back(fld);
      }
    }
    return L;
  }

  std::vector<scan::Constant> Parser::constList() const {
    std::vector<scan::Constant> L;
    L.emplace_back(constant());
    if (lex_->matchDelm(COMMA)) {
      lex_->eatDelim(COMMA);
      for (const auto& cst : constList()) {
        L.emplace_back(cst);
      }
    }
    return L;
  }

  ModifyData Parser::modify() const {
    lex_->eatKeyword(UPDATE);
    std::string tblname = lex_->eatId();
    lex_->eatKeyword(SET);
    std::string fldname = field();
    lex_->eatDelim(EQUAL_SIGN);
    scan::Expression newval;
    scan::Predicate pred;
    if (lex_->matchKeyword(WHERE)) {
      lex_->eatKeyword(WHERE);
      pred = predicate();
    }
    ModifyData md(tblname, fldname, newval, pred);
    return md;
  }

  CreateTableData Parser::createTable() const {
    lex_->eatKeyword(TABLE);
    std::string tblname = lex_->eatId();
    lex_->eatDelim(LEFT_PARENTHESIS);
    record::Schema sch = fieldDefs();
    lex_->eatDelim(RIGHT_PARENTHESIS);
    CreateTableData ctd(tblname, sch);
    return ctd;
  }

  record::Schema Parser::fieldDefs() const {
    record::Schema schema = fieldDef();
    if (lex_->matchDelm(COMMA)) {
      lex_->eatDelim(COMMA);
      record::Schema schema2 = fieldDefs();
      schema.addAll(schema2);
    }
    return schema;
  }

  record::Schema Parser::fieldDef() const {
    std::string fldname = field();
    return fieldType(fldname);
  }

  record::Schema Parser::fieldType(const std::string& fldname) const {
    record::Schema schema;
    if (lex_->matchKeyword(INT)) {
      lex_->eatKeyword(INT);
      schema.addIntField(fldname);
    } else {
      lex_->eatKeyword(VARCHAR);
      lex_->eatDelim(LEFT_PARENTHESIS);
      int strlen = lex_->eatIntConstant();
      lex_->eatDelim(RIGHT_PARENTHESIS);
      schema.addStringField(fldname, strlen);
    }
    return schema;
  }

  CreateViewData Parser::createView() const {
    lex_->eatKeyword(VIEW);
    std::string viewname = lex_->eatId();
    lex_->eatKeyword(AS);
    QueryData qd = query();
    CreateViewData cvd(viewname, qd);
    return cvd;
  }

  CreateIndexData Parser::createIndex() const {
    lex_->eatKeyword(INDEX);
    std::string idxname = lex_->eatId();
    lex_->eatKeyword(ON);
    std::string tblname = lex_->eatId();
    lex_->eatDelim(LEFT_PARENTHESIS);
    std::string fldname = field();
    lex_->eatDelim(RIGHT_PARENTHESIS);
    CreateIndexData cid(idxname, tblname, fldname);
    return cid;
  }
}  // namespace parse
