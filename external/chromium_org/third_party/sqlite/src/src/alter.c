/*
** 2005 February 15
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains C code routines that used to generate VDBE code
** that implements the ALTER TABLE command.
*/
#include "sqliteInt.h"

#ifndef SQLITE_OMIT_ALTERTABLE


static void renameTableFunc(
  sqlite3_context *context,
  int NotUsed,
  sqlite3_value **argv
){
  unsigned char const *zSql = sqlite3_value_text(argv[0]);
  unsigned char const *zTableName = sqlite3_value_text(argv[1]);

  int token;
  Token tname;
  unsigned char const *zCsr = zSql;
  int len = 0;
  char *zRet;

  sqlite3 *db = sqlite3_context_db_handle(context);

  UNUSED_PARAMETER(NotUsed);

  if( zSql ){
    do {
      if( !*zCsr ){
        
        return;
      }

      
      tname.z = (char*)zCsr;
      tname.n = len;

      do {
        zCsr += len;
        len = sqlite3GetToken(zCsr, &token);
      } while( token==TK_SPACE );
      assert( len>0 );
    } while( token!=TK_LP && token!=TK_USING );

    zRet = sqlite3MPrintf(db, "%.*s\"%w\"%s", ((u8*)tname.z) - zSql, zSql, 
       zTableName, tname.z+tname.n);
    sqlite3_result_text(context, zRet, -1, SQLITE_DYNAMIC);
  }
}

#ifndef SQLITE_OMIT_FOREIGN_KEY
static void renameParentFunc(
  sqlite3_context *context,
  int NotUsed,
  sqlite3_value **argv
){
  sqlite3 *db = sqlite3_context_db_handle(context);
  char *zOutput = 0;
  char *zResult;
  unsigned char const *zInput = sqlite3_value_text(argv[0]);
  unsigned char const *zOld = sqlite3_value_text(argv[1]);
  unsigned char const *zNew = sqlite3_value_text(argv[2]);

  unsigned const char *z;         
  int n;                          
  int token;                      

  UNUSED_PARAMETER(NotUsed);
  for(z=zInput; *z; z=z+n){
    n = sqlite3GetToken(z, &token);
    if( token==TK_REFERENCES ){
      char *zParent;
      do {
        z += n;
        n = sqlite3GetToken(z, &token);
      }while( token==TK_SPACE );

      zParent = sqlite3DbStrNDup(db, (const char *)z, n);
      if( zParent==0 ) break;
      sqlite3Dequote(zParent);
      if( 0==sqlite3StrICmp((const char *)zOld, zParent) ){
        char *zOut = sqlite3MPrintf(db, "%s%.*s\"%w\"", 
            (zOutput?zOutput:""), z-zInput, zInput, (const char *)zNew
        );
        sqlite3DbFree(db, zOutput);
        zOutput = zOut;
        zInput = &z[n];
      }
      sqlite3DbFree(db, zParent);
    }
  }

  zResult = sqlite3MPrintf(db, "%s%s", (zOutput?zOutput:""), zInput), 
  sqlite3_result_text(context, zResult, -1, SQLITE_DYNAMIC);
  sqlite3DbFree(db, zOutput);
}
#endif

#ifndef SQLITE_OMIT_TRIGGER
static void renameTriggerFunc(
  sqlite3_context *context,
  int NotUsed,
  sqlite3_value **argv
){
  unsigned char const *zSql = sqlite3_value_text(argv[0]);
  unsigned char const *zTableName = sqlite3_value_text(argv[1]);

  int token;
  Token tname;
  int dist = 3;
  unsigned char const *zCsr = zSql;
  int len = 0;
  char *zRet;
  sqlite3 *db = sqlite3_context_db_handle(context);

  UNUSED_PARAMETER(NotUsed);

  if( zSql ){
    do {

      if( !*zCsr ){
        
        return;
      }

      
      tname.z = (char*)zCsr;
      tname.n = len;

      do {
        zCsr += len;
        len = sqlite3GetToken(zCsr, &token);
      }while( token==TK_SPACE );
      assert( len>0 );

      dist++;
      if( token==TK_DOT || token==TK_ON ){
        dist = 0;
      }
    } while( dist!=2 || (token!=TK_WHEN && token!=TK_FOR && token!=TK_BEGIN) );

    zRet = sqlite3MPrintf(db, "%.*s\"%w\"%s", ((u8*)tname.z) - zSql, zSql, 
       zTableName, tname.z+tname.n);
    sqlite3_result_text(context, zRet, -1, SQLITE_DYNAMIC);
  }
}
#endif   

void sqlite3AlterFunctions(void){
  static SQLITE_WSD FuncDef aAlterTableFuncs[] = {
    FUNCTION(sqlite_rename_table,   2, 0, 0, renameTableFunc),
#ifndef SQLITE_OMIT_TRIGGER
    FUNCTION(sqlite_rename_trigger, 2, 0, 0, renameTriggerFunc),
#endif
#ifndef SQLITE_OMIT_FOREIGN_KEY
    FUNCTION(sqlite_rename_parent,  3, 0, 0, renameParentFunc),
#endif
  };
  int i;
  FuncDefHash *pHash = &GLOBAL(FuncDefHash, sqlite3GlobalFunctions);
  FuncDef *aFunc = (FuncDef*)&GLOBAL(FuncDef, aAlterTableFuncs);

  for(i=0; i<ArraySize(aAlterTableFuncs); i++){
    sqlite3FuncDefInsert(pHash, &aFunc[i]);
  }
}

static char *whereOrName(sqlite3 *db, char *zWhere, char *zConstant){
  char *zNew;
  if( !zWhere ){
    zNew = sqlite3MPrintf(db, "name=%Q", zConstant);
  }else{
    zNew = sqlite3MPrintf(db, "%s OR name=%Q", zWhere, zConstant);
    sqlite3DbFree(db, zWhere);
  }
  return zNew;
}

#if !defined(SQLITE_OMIT_FOREIGN_KEY) && !defined(SQLITE_OMIT_TRIGGER)
static char *whereForeignKeys(Parse *pParse, Table *pTab){
  FKey *p;
  char *zWhere = 0;
  for(p=sqlite3FkReferences(pTab); p; p=p->pNextTo){
    zWhere = whereOrName(pParse->db, zWhere, p->pFrom->zName);
  }
  return zWhere;
}
#endif

static char *whereTempTriggers(Parse *pParse, Table *pTab){
  Trigger *pTrig;
  char *zWhere = 0;
  const Schema *pTempSchema = pParse->db->aDb[1].pSchema; 

  if( pTab->pSchema!=pTempSchema ){
    sqlite3 *db = pParse->db;
    for(pTrig=sqlite3TriggerList(pParse, pTab); pTrig; pTrig=pTrig->pNext){
      if( pTrig->pSchema==pTempSchema ){
        zWhere = whereOrName(db, zWhere, pTrig->zName);
      }
    }
  }
  if( zWhere ){
    char *zNew = sqlite3MPrintf(pParse->db, "type='trigger' AND (%s)", zWhere);
    sqlite3DbFree(pParse->db, zWhere);
    zWhere = zNew;
  }
  return zWhere;
}

static void reloadTableSchema(Parse *pParse, Table *pTab, const char *zName){
  Vdbe *v;
  char *zWhere;
  int iDb;                   
#ifndef SQLITE_OMIT_TRIGGER
  Trigger *pTrig;
#endif

  v = sqlite3GetVdbe(pParse);
  if( NEVER(v==0) ) return;
  assert( sqlite3BtreeHoldsAllMutexes(pParse->db) );
  iDb = sqlite3SchemaToIndex(pParse->db, pTab->pSchema);
  assert( iDb>=0 );

#ifndef SQLITE_OMIT_TRIGGER
  
  for(pTrig=sqlite3TriggerList(pParse, pTab); pTrig; pTrig=pTrig->pNext){
    int iTrigDb = sqlite3SchemaToIndex(pParse->db, pTrig->pSchema);
    assert( iTrigDb==iDb || iTrigDb==1 );
    sqlite3VdbeAddOp4(v, OP_DropTrigger, iTrigDb, 0, 0, pTrig->zName, 0);
  }
#endif

  
  sqlite3VdbeAddOp4(v, OP_DropTable, iDb, 0, 0, pTab->zName, 0);

  
  zWhere = sqlite3MPrintf(pParse->db, "tbl_name=%Q", zName);
  if( !zWhere ) return;
  sqlite3VdbeAddOp4(v, OP_ParseSchema, iDb, 0, 0, zWhere, P4_DYNAMIC);

#ifndef SQLITE_OMIT_TRIGGER
  if( (zWhere=whereTempTriggers(pParse, pTab))!=0 ){
    sqlite3VdbeAddOp4(v, OP_ParseSchema, 1, 0, 0, zWhere, P4_DYNAMIC);
  }
#endif
}

static int isSystemTable(Parse *pParse, const char *zName){
  if( sqlite3Strlen30(zName)>6 && 0==sqlite3StrNICmp(zName, "sqlite_", 7) ){
    sqlite3ErrorMsg(pParse, "table %s may not be altered", zName);
    return 1;
  }
  return 0;
}

void sqlite3AlterRenameTable(
  Parse *pParse,            
  SrcList *pSrc,            
  Token *pName              
){
  int iDb;                  
  char *zDb;                
  Table *pTab;              
  char *zName = 0;           
  sqlite3 *db = pParse->db; 
  int nTabName;             
  const char *zTabName;     
  Vdbe *v;
#ifndef SQLITE_OMIT_TRIGGER
  char *zWhere = 0;         
#endif
  VTable *pVTab = 0;        
  int savedDbFlags;         

  savedDbFlags = db->flags;  
  if( NEVER(db->mallocFailed) ) goto exit_rename_table;
  assert( pSrc->nSrc==1 );
  assert( sqlite3BtreeHoldsAllMutexes(pParse->db) );

  pTab = sqlite3LocateTable(pParse, 0, pSrc->a[0].zName, pSrc->a[0].zDatabase);
  if( !pTab ) goto exit_rename_table;
  iDb = sqlite3SchemaToIndex(pParse->db, pTab->pSchema);
  zDb = db->aDb[iDb].zName;
  db->flags |= SQLITE_PreferBuiltin;

  
  zName = sqlite3NameFromToken(db, pName);
  if( !zName ) goto exit_rename_table;

  if( sqlite3FindTable(db, zName, zDb) || sqlite3FindIndex(db, zName, zDb) ){
    sqlite3ErrorMsg(pParse, 
        "there is already another table or index with this name: %s", zName);
    goto exit_rename_table;
  }

  if( SQLITE_OK!=isSystemTable(pParse, pTab->zName) ){
    goto exit_rename_table;
  }
  if( SQLITE_OK!=sqlite3CheckObjectName(pParse, zName) ){ goto
    exit_rename_table;
  }

#ifndef SQLITE_OMIT_VIEW
  if( pTab->pSelect ){
    sqlite3ErrorMsg(pParse, "view %s may not be altered", pTab->zName);
    goto exit_rename_table;
  }
#endif

#ifndef SQLITE_OMIT_AUTHORIZATION
  
  if( sqlite3AuthCheck(pParse, SQLITE_ALTER_TABLE, zDb, pTab->zName, 0) ){
    goto exit_rename_table;
  }
#endif

#ifndef SQLITE_OMIT_VIRTUALTABLE
  if( sqlite3ViewGetColumnNames(pParse, pTab) ){
    goto exit_rename_table;
  }
  if( IsVirtual(pTab) ){
    pVTab = sqlite3GetVTable(db, pTab);
    if( pVTab->pVtab->pModule->xRename==0 ){
      pVTab = 0;
    }
  }
#endif

  v = sqlite3GetVdbe(pParse);
  if( v==0 ){
    goto exit_rename_table;
  }
  sqlite3BeginWriteOperation(pParse, pVTab!=0, iDb);
  sqlite3ChangeCookie(pParse, iDb);

#ifndef SQLITE_OMIT_VIRTUALTABLE
  if( pVTab ){
    int i = ++pParse->nMem;
    sqlite3VdbeAddOp4(v, OP_String8, 0, i, 0, zName, 0);
    sqlite3VdbeAddOp4(v, OP_VRename, i, 0, 0,(const char*)pVTab, P4_VTAB);
    sqlite3MayAbort(pParse);
  }
#endif

  
  zTabName = pTab->zName;
  nTabName = sqlite3Utf8CharLen(zTabName, -1);

#if !defined(SQLITE_OMIT_FOREIGN_KEY) && !defined(SQLITE_OMIT_TRIGGER)
  if( db->flags&SQLITE_ForeignKeys ){
    if( (zWhere=whereForeignKeys(pParse, pTab))!=0 ){
      sqlite3NestedParse(pParse, 
          "UPDATE \"%w\".%s SET "
              "sql = sqlite_rename_parent(sql, %Q, %Q) "
              "WHERE %s;", zDb, SCHEMA_TABLE(iDb), zTabName, zName, zWhere);
      sqlite3DbFree(db, zWhere);
    }
  }
#endif

  
  sqlite3NestedParse(pParse,
      "UPDATE %Q.%s SET "
#ifdef SQLITE_OMIT_TRIGGER
          "sql = sqlite_rename_table(sql, %Q), "
#else
          "sql = CASE "
            "WHEN type = 'trigger' THEN sqlite_rename_trigger(sql, %Q)"
            "ELSE sqlite_rename_table(sql, %Q) END, "
#endif
          "tbl_name = %Q, "
          "name = CASE "
            "WHEN type='table' THEN %Q "
            "WHEN name LIKE 'sqlite_autoindex%%' AND type='index' THEN "
             "'sqlite_autoindex_' || %Q || substr(name,%d+18) "
            "ELSE name END "
      "WHERE tbl_name=%Q AND "
          "(type='table' OR type='index' OR type='trigger');", 
      zDb, SCHEMA_TABLE(iDb), zName, zName, zName, 
#ifndef SQLITE_OMIT_TRIGGER
      zName,
#endif
      zName, nTabName, zTabName
  );

#ifndef SQLITE_OMIT_AUTOINCREMENT
  if( sqlite3FindTable(db, "sqlite_sequence", zDb) ){
    sqlite3NestedParse(pParse,
        "UPDATE \"%w\".sqlite_sequence set name = %Q WHERE name = %Q",
        zDb, zName, pTab->zName);
  }
#endif

#ifndef SQLITE_OMIT_TRIGGER
  if( (zWhere=whereTempTriggers(pParse, pTab))!=0 ){
    sqlite3NestedParse(pParse, 
        "UPDATE sqlite_temp_master SET "
            "sql = sqlite_rename_trigger(sql, %Q), "
            "tbl_name = %Q "
            "WHERE %s;", zName, zName, zWhere);
    sqlite3DbFree(db, zWhere);
  }
#endif

#if !defined(SQLITE_OMIT_FOREIGN_KEY) && !defined(SQLITE_OMIT_TRIGGER)
  if( db->flags&SQLITE_ForeignKeys ){
    FKey *p;
    for(p=sqlite3FkReferences(pTab); p; p=p->pNextTo){
      Table *pFrom = p->pFrom;
      if( pFrom!=pTab ){
        reloadTableSchema(pParse, p->pFrom, pFrom->zName);
      }
    }
  }
#endif

  
  reloadTableSchema(pParse, pTab, zName);

exit_rename_table:
  sqlite3SrcListDelete(db, pSrc);
  sqlite3DbFree(db, zName);
  db->flags = savedDbFlags;
}


void sqlite3MinimumFileFormat(Parse *pParse, int iDb, int minFormat){
  Vdbe *v;
  v = sqlite3GetVdbe(pParse);
  if( ALWAYS(v) ){
    int r1 = sqlite3GetTempReg(pParse);
    int r2 = sqlite3GetTempReg(pParse);
    int j1;
    sqlite3VdbeAddOp3(v, OP_ReadCookie, iDb, r1, BTREE_FILE_FORMAT);
    sqlite3VdbeUsesBtree(v, iDb);
    sqlite3VdbeAddOp2(v, OP_Integer, minFormat, r2);
    j1 = sqlite3VdbeAddOp3(v, OP_Ge, r2, 0, r1);
    sqlite3VdbeAddOp3(v, OP_SetCookie, iDb, BTREE_FILE_FORMAT, r2);
    sqlite3VdbeJumpHere(v, j1);
    sqlite3ReleaseTempReg(pParse, r1);
    sqlite3ReleaseTempReg(pParse, r2);
  }
}

void sqlite3AlterFinishAddColumn(Parse *pParse, Token *pColDef){
  Table *pNew;              
  Table *pTab;              
  int iDb;                  
  const char *zDb;          
  const char *zTab;         
  char *zCol;               
  Column *pCol;             
  Expr *pDflt;              
  sqlite3 *db;              

  db = pParse->db;
  if( pParse->nErr || db->mallocFailed ) return;
  pNew = pParse->pNewTable;
  assert( pNew );

  assert( sqlite3BtreeHoldsAllMutexes(db) );
  iDb = sqlite3SchemaToIndex(db, pNew->pSchema);
  zDb = db->aDb[iDb].zName;
  zTab = &pNew->zName[16];  
  pCol = &pNew->aCol[pNew->nCol-1];
  pDflt = pCol->pDflt;
  pTab = sqlite3FindTable(db, zTab, zDb);
  assert( pTab );

#ifndef SQLITE_OMIT_AUTHORIZATION
  
  if( sqlite3AuthCheck(pParse, SQLITE_ALTER_TABLE, zDb, pTab->zName, 0) ){
    return;
  }
#endif

  if( pDflt && pDflt->op==TK_NULL ){
    pDflt = 0;
  }

  if( pCol->isPrimKey ){
    sqlite3ErrorMsg(pParse, "Cannot add a PRIMARY KEY column");
    return;
  }
  if( pNew->pIndex ){
    sqlite3ErrorMsg(pParse, "Cannot add a UNIQUE column");
    return;
  }
  if( (db->flags&SQLITE_ForeignKeys) && pNew->pFKey && pDflt ){
    sqlite3ErrorMsg(pParse, 
        "Cannot add a REFERENCES column with non-NULL default value");
    return;
  }
  if( pCol->notNull && !pDflt ){
    sqlite3ErrorMsg(pParse, 
        "Cannot add a NOT NULL column with default value NULL");
    return;
  }

  if( pDflt ){
    sqlite3_value *pVal;
    if( sqlite3ValueFromExpr(db, pDflt, SQLITE_UTF8, SQLITE_AFF_NONE, &pVal) ){
      db->mallocFailed = 1;
      return;
    }
    if( !pVal ){
      sqlite3ErrorMsg(pParse, "Cannot add a column with non-constant default");
      return;
    }
    sqlite3ValueFree(pVal);
  }

  
  zCol = sqlite3DbStrNDup(db, (char*)pColDef->z, pColDef->n);
  if( zCol ){
    char *zEnd = &zCol[pColDef->n-1];
    int savedDbFlags = db->flags;
    while( zEnd>zCol && (*zEnd==';' || sqlite3Isspace(*zEnd)) ){
      *zEnd-- = '\0';
    }
    db->flags |= SQLITE_PreferBuiltin;
    sqlite3NestedParse(pParse, 
        "UPDATE \"%w\".%s SET "
          "sql = substr(sql,1,%d) || ', ' || %Q || substr(sql,%d) "
        "WHERE type = 'table' AND name = %Q", 
      zDb, SCHEMA_TABLE(iDb), pNew->addColOffset, zCol, pNew->addColOffset+1,
      zTab
    );
    sqlite3DbFree(db, zCol);
    db->flags = savedDbFlags;
  }

  sqlite3MinimumFileFormat(pParse, iDb, pDflt ? 3 : 2);

  
  reloadTableSchema(pParse, pTab, pTab->zName);
}

void sqlite3AlterBeginAddColumn(Parse *pParse, SrcList *pSrc){
  Table *pNew;
  Table *pTab;
  Vdbe *v;
  int iDb;
  int i;
  int nAlloc;
  sqlite3 *db = pParse->db;

  
  assert( pParse->pNewTable==0 );
  assert( sqlite3BtreeHoldsAllMutexes(db) );
  if( db->mallocFailed ) goto exit_begin_add_column;
  pTab = sqlite3LocateTable(pParse, 0, pSrc->a[0].zName, pSrc->a[0].zDatabase);
  if( !pTab ) goto exit_begin_add_column;

#ifndef SQLITE_OMIT_VIRTUALTABLE
  if( IsVirtual(pTab) ){
    sqlite3ErrorMsg(pParse, "virtual tables may not be altered");
    goto exit_begin_add_column;
  }
#endif

  
  if( pTab->pSelect ){
    sqlite3ErrorMsg(pParse, "Cannot add a column to a view");
    goto exit_begin_add_column;
  }
  if( SQLITE_OK!=isSystemTable(pParse, pTab->zName) ){
    goto exit_begin_add_column;
  }

  assert( pTab->addColOffset>0 );
  iDb = sqlite3SchemaToIndex(db, pTab->pSchema);

  pNew = (Table*)sqlite3DbMallocZero(db, sizeof(Table));
  if( !pNew ) goto exit_begin_add_column;
  pParse->pNewTable = pNew;
  pNew->nRef = 1;
  pNew->nCol = pTab->nCol;
  assert( pNew->nCol>0 );
  nAlloc = (((pNew->nCol-1)/8)*8)+8;
  assert( nAlloc>=pNew->nCol && nAlloc%8==0 && nAlloc-pNew->nCol<8 );
  pNew->aCol = (Column*)sqlite3DbMallocZero(db, sizeof(Column)*nAlloc);
  pNew->zName = sqlite3MPrintf(db, "sqlite_altertab_%s", pTab->zName);
  if( !pNew->aCol || !pNew->zName ){
    db->mallocFailed = 1;
    goto exit_begin_add_column;
  }
  memcpy(pNew->aCol, pTab->aCol, sizeof(Column)*pNew->nCol);
  for(i=0; i<pNew->nCol; i++){
    Column *pCol = &pNew->aCol[i];
    pCol->zName = sqlite3DbStrDup(db, pCol->zName);
    pCol->zColl = 0;
    pCol->zType = 0;
    pCol->pDflt = 0;
    pCol->zDflt = 0;
  }
  pNew->pSchema = db->aDb[iDb].pSchema;
  pNew->addColOffset = pTab->addColOffset;
  pNew->nRef = 1;

  
  sqlite3BeginWriteOperation(pParse, 0, iDb);
  v = sqlite3GetVdbe(pParse);
  if( !v ) goto exit_begin_add_column;
  sqlite3ChangeCookie(pParse, iDb);

exit_begin_add_column:
  sqlite3SrcListDelete(db, pSrc);
  return;
}
#endif  
