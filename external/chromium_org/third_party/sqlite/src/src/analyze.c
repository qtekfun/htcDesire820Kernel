/*
** 2005 July 8
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** This file contains code associated with the ANALYZE command.
*/
#ifndef SQLITE_OMIT_ANALYZE
#include "sqliteInt.h"

static void openStatTable(
  Parse *pParse,          
  int iDb,                
  int iStatCur,           
  const char *zWhere,     
  const char *zWhereType  
){
  static const struct {
    const char *zName;
    const char *zCols;
  } aTable[] = {
    { "sqlite_stat1", "tbl,idx,stat" },
#ifdef SQLITE_ENABLE_STAT2
    { "sqlite_stat2", "tbl,idx,sampleno,sample" },
#endif
  };

  int aRoot[] = {0, 0};
  u8 aCreateTbl[] = {0, 0};

  int i;
  sqlite3 *db = pParse->db;
  Db *pDb;
  Vdbe *v = sqlite3GetVdbe(pParse);
  if( v==0 ) return;
  assert( sqlite3BtreeHoldsAllMutexes(db) );
  assert( sqlite3VdbeDb(v)==db );
  pDb = &db->aDb[iDb];

  for(i=0; i<ArraySize(aTable); i++){
    const char *zTab = aTable[i].zName;
    Table *pStat;
    if( (pStat = sqlite3FindTable(db, zTab, pDb->zName))==0 ){
      sqlite3NestedParse(pParse,
          "CREATE TABLE %Q.%s(%s)", pDb->zName, zTab, aTable[i].zCols
      );
      aRoot[i] = pParse->regRoot;
      aCreateTbl[i] = 1;
    }else{
      aRoot[i] = pStat->tnum;
      sqlite3TableLock(pParse, iDb, aRoot[i], 1, zTab);
      if( zWhere ){
        sqlite3NestedParse(pParse,
           "DELETE FROM %Q.%s WHERE %s=%Q", pDb->zName, zTab, zWhereType, zWhere
        );
      }else{
        
        sqlite3VdbeAddOp2(v, OP_Clear, aRoot[i], iDb);
      }
    }
  }

  
  for(i=0; i<ArraySize(aTable); i++){
    sqlite3VdbeAddOp3(v, OP_OpenWrite, iStatCur+i, aRoot[i], iDb);
    sqlite3VdbeChangeP4(v, -1, (char *)3, P4_INT32);
    sqlite3VdbeChangeP5(v, aCreateTbl[i]);
  }
}

static void analyzeOneTable(
  Parse *pParse,   
  Table *pTab,     
  Index *pOnlyIdx, 
  int iStatCur,    
  int iMem         
){
  sqlite3 *db = pParse->db;    
  Index *pIdx;                 
  int iIdxCur;                 
  Vdbe *v;                     
  int i;                       
  int topOfLoop;               
  int endOfLoop;               
  int jZeroRows = -1;          
  int iDb;                     
  int regTabname = iMem++;     
  int regIdxname = iMem++;     
  int regSampleno = iMem++;    
  int regCol = iMem++;         
  int regRec = iMem++;         
  int regTemp = iMem++;        
  int regRowid = iMem++;       

#ifdef SQLITE_ENABLE_STAT2
  int addr = 0;                
  int regTemp2 = iMem++;       
  int regSamplerecno = iMem++; 
  int regRecno = iMem++;       
  int regLast = iMem++;        
  int regFirst = iMem++;       
#endif

  v = sqlite3GetVdbe(pParse);
  if( v==0 || NEVER(pTab==0) ){
    return;
  }
  if( pTab->tnum==0 ){
    
    return;
  }
  if( sqlite3_strnicmp(pTab->zName, "sqlite_", 7)==0 ){
    
    return;
  }
  assert( sqlite3BtreeHoldsAllMutexes(db) );
  iDb = sqlite3SchemaToIndex(db, pTab->pSchema);
  assert( iDb>=0 );
  assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
#ifndef SQLITE_OMIT_AUTHORIZATION
  if( sqlite3AuthCheck(pParse, SQLITE_ANALYZE, pTab->zName, 0,
      db->aDb[iDb].zName ) ){
    return;
  }
#endif

  
  sqlite3TableLock(pParse, iDb, pTab->tnum, 0, pTab->zName);

  iIdxCur = pParse->nTab++;
  sqlite3VdbeAddOp4(v, OP_String8, 0, regTabname, 0, pTab->zName, 0);
  for(pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext){
    int nCol;
    KeyInfo *pKey;

    if( pOnlyIdx && pOnlyIdx!=pIdx ) continue;
    nCol = pIdx->nColumn;
    pKey = sqlite3IndexKeyinfo(pParse, pIdx);
    if( iMem+1+(nCol*2)>pParse->nMem ){
      pParse->nMem = iMem+1+(nCol*2);
    }

    
    assert( iDb==sqlite3SchemaToIndex(db, pIdx->pSchema) );
    sqlite3VdbeAddOp4(v, OP_OpenRead, iIdxCur, pIdx->tnum, iDb,
        (char *)pKey, P4_KEYINFO_HANDOFF);
    VdbeComment((v, "%s", pIdx->zName));

    
    sqlite3VdbeAddOp4(v, OP_String8, 0, regIdxname, 0, pIdx->zName, 0);

#ifdef SQLITE_ENABLE_STAT2

    if( pTab->pIndex==pIdx ){
      sqlite3VdbeAddOp2(v, OP_Integer, SQLITE_INDEX_SAMPLES, regSamplerecno);
      sqlite3VdbeAddOp2(v, OP_Integer, SQLITE_INDEX_SAMPLES*2-1, regTemp);
      sqlite3VdbeAddOp2(v, OP_Integer, SQLITE_INDEX_SAMPLES*2, regTemp2);

      sqlite3VdbeAddOp2(v, OP_Count, iIdxCur, regLast);
      sqlite3VdbeAddOp2(v, OP_Null, 0, regFirst);
      addr = sqlite3VdbeAddOp3(v, OP_Lt, regSamplerecno, 0, regLast);
      sqlite3VdbeAddOp3(v, OP_Divide, regTemp2, regLast, regFirst);
      sqlite3VdbeAddOp3(v, OP_Multiply, regLast, regTemp, regLast);
      sqlite3VdbeAddOp2(v, OP_AddImm, regLast, SQLITE_INDEX_SAMPLES*2-2);
      sqlite3VdbeAddOp3(v, OP_Divide,  regTemp2, regLast, regLast);
      sqlite3VdbeJumpHere(v, addr);
    }

    
    sqlite3VdbeAddOp2(v, OP_Integer, 0, regSampleno);
    sqlite3VdbeAddOp2(v, OP_Integer, 0, regRecno);
    sqlite3VdbeAddOp2(v, OP_Copy, regFirst, regSamplerecno);
#endif

    for(i=0; i<=nCol; i++){
      sqlite3VdbeAddOp2(v, OP_Integer, 0, iMem+i);
    }
    for(i=0; i<nCol; i++){
      sqlite3VdbeAddOp2(v, OP_Null, 0, iMem+nCol+i+1);
    }

    endOfLoop = sqlite3VdbeMakeLabel(v);
    sqlite3VdbeAddOp2(v, OP_Rewind, iIdxCur, endOfLoop);
    topOfLoop = sqlite3VdbeCurrentAddr(v);
    sqlite3VdbeAddOp2(v, OP_AddImm, iMem, 1);

    for(i=0; i<nCol; i++){
      CollSeq *pColl;
      sqlite3VdbeAddOp3(v, OP_Column, iIdxCur, i, regCol);
      if( i==0 ){
#ifdef SQLITE_ENABLE_STAT2
        int ne = sqlite3VdbeAddOp3(v, OP_Ne, regRecno, 0, regSamplerecno);
        assert( regTabname+1==regIdxname 
             && regTabname+2==regSampleno
             && regTabname+3==regCol
        );
        sqlite3VdbeChangeP5(v, SQLITE_JUMPIFNULL);
        sqlite3VdbeAddOp4(v, OP_MakeRecord, regTabname, 4, regRec, "aaab", 0);
        sqlite3VdbeAddOp2(v, OP_NewRowid, iStatCur+1, regRowid);
        sqlite3VdbeAddOp3(v, OP_Insert, iStatCur+1, regRec, regRowid);

        sqlite3VdbeAddOp2(v, OP_AddImm, regSampleno, 1);
        sqlite3VdbeAddOp3(v, OP_Subtract, regRecno, regLast, regTemp);
        sqlite3VdbeAddOp2(v, OP_AddImm, regTemp, -1);
        sqlite3VdbeAddOp2(v, OP_Integer, SQLITE_INDEX_SAMPLES, regTemp2);
        sqlite3VdbeAddOp3(v, OP_Subtract, regSampleno, regTemp2, regTemp2);
        sqlite3VdbeAddOp3(v, OP_Divide, regTemp2, regTemp, regTemp);
        sqlite3VdbeAddOp3(v, OP_Add, regSamplerecno, regTemp, regSamplerecno);

        sqlite3VdbeJumpHere(v, ne);
        sqlite3VdbeAddOp2(v, OP_AddImm, regRecno, 1);
#endif

        
        sqlite3VdbeAddOp1(v, OP_IfNot, iMem+1);
      }
      assert( pIdx->azColl!=0 );
      assert( pIdx->azColl[i]!=0 );
      pColl = sqlite3LocateCollSeq(pParse, pIdx->azColl[i]);
      sqlite3VdbeAddOp4(v, OP_Ne, regCol, 0, iMem+nCol+i+1,
                       (char*)pColl, P4_COLLSEQ);
      sqlite3VdbeChangeP5(v, SQLITE_NULLEQ);
    }
    if( db->mallocFailed ){
      return;
    }
    sqlite3VdbeAddOp2(v, OP_Goto, 0, endOfLoop);
    for(i=0; i<nCol; i++){
      int addr2 = sqlite3VdbeCurrentAddr(v) - (nCol*2);
      if( i==0 ){
        sqlite3VdbeJumpHere(v, addr2-1);  
      }
      sqlite3VdbeJumpHere(v, addr2);      
      sqlite3VdbeAddOp2(v, OP_AddImm, iMem+i+1, 1);
      sqlite3VdbeAddOp3(v, OP_Column, iIdxCur, i, iMem+nCol+i+1);
    }

    
    sqlite3VdbeResolveLabel(v, endOfLoop);
    sqlite3VdbeAddOp2(v, OP_Next, iIdxCur, topOfLoop);
    sqlite3VdbeAddOp1(v, OP_Close, iIdxCur);

    sqlite3VdbeAddOp2(v, OP_SCopy, iMem, regSampleno);
    if( jZeroRows<0 ){
      jZeroRows = sqlite3VdbeAddOp1(v, OP_IfNot, iMem);
    }
    for(i=0; i<nCol; i++){
      sqlite3VdbeAddOp4(v, OP_String8, 0, regTemp, 0, " ", 0);
      sqlite3VdbeAddOp3(v, OP_Concat, regTemp, regSampleno, regSampleno);
      sqlite3VdbeAddOp3(v, OP_Add, iMem, iMem+i+1, regTemp);
      sqlite3VdbeAddOp2(v, OP_AddImm, regTemp, -1);
      sqlite3VdbeAddOp3(v, OP_Divide, iMem+i+1, regTemp, regTemp);
      sqlite3VdbeAddOp1(v, OP_ToInt, regTemp);
      sqlite3VdbeAddOp3(v, OP_Concat, regTemp, regSampleno, regSampleno);
    }
    sqlite3VdbeAddOp4(v, OP_MakeRecord, regTabname, 3, regRec, "aaa", 0);
    sqlite3VdbeAddOp2(v, OP_NewRowid, iStatCur, regRowid);
    sqlite3VdbeAddOp3(v, OP_Insert, iStatCur, regRec, regRowid);
    sqlite3VdbeChangeP5(v, OPFLAG_APPEND);
  }

  if( pTab->pIndex==0 ){
    sqlite3VdbeAddOp3(v, OP_OpenRead, iIdxCur, pTab->tnum, iDb);
    VdbeComment((v, "%s", pTab->zName));
    sqlite3VdbeAddOp2(v, OP_Count, iIdxCur, regSampleno);
    sqlite3VdbeAddOp1(v, OP_Close, iIdxCur);
    jZeroRows = sqlite3VdbeAddOp1(v, OP_IfNot, regSampleno);
  }else{
    sqlite3VdbeJumpHere(v, jZeroRows);
    jZeroRows = sqlite3VdbeAddOp0(v, OP_Goto);
  }
  sqlite3VdbeAddOp2(v, OP_Null, 0, regIdxname);
  sqlite3VdbeAddOp4(v, OP_MakeRecord, regTabname, 3, regRec, "aaa", 0);
  sqlite3VdbeAddOp2(v, OP_NewRowid, iStatCur, regRowid);
  sqlite3VdbeAddOp3(v, OP_Insert, iStatCur, regRec, regRowid);
  sqlite3VdbeChangeP5(v, OPFLAG_APPEND);
  if( pParse->nMem<regRec ) pParse->nMem = regRec;
  sqlite3VdbeJumpHere(v, jZeroRows);
}

static void loadAnalysis(Parse *pParse, int iDb){
  Vdbe *v = sqlite3GetVdbe(pParse);
  if( v ){
    sqlite3VdbeAddOp1(v, OP_LoadAnalysis, iDb);
  }
}

static void analyzeDatabase(Parse *pParse, int iDb){
  sqlite3 *db = pParse->db;
  Schema *pSchema = db->aDb[iDb].pSchema;    
  HashElem *k;
  int iStatCur;
  int iMem;

  sqlite3BeginWriteOperation(pParse, 0, iDb);
  iStatCur = pParse->nTab;
  pParse->nTab += 2;
  openStatTable(pParse, iDb, iStatCur, 0, 0);
  iMem = pParse->nMem+1;
  assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
  for(k=sqliteHashFirst(&pSchema->tblHash); k; k=sqliteHashNext(k)){
    Table *pTab = (Table*)sqliteHashData(k);
    analyzeOneTable(pParse, pTab, 0, iStatCur, iMem);
  }
  loadAnalysis(pParse, iDb);
}

static void analyzeTable(Parse *pParse, Table *pTab, Index *pOnlyIdx){
  int iDb;
  int iStatCur;

  assert( pTab!=0 );
  assert( sqlite3BtreeHoldsAllMutexes(pParse->db) );
  iDb = sqlite3SchemaToIndex(pParse->db, pTab->pSchema);
  sqlite3BeginWriteOperation(pParse, 0, iDb);
  iStatCur = pParse->nTab;
  pParse->nTab += 2;
  if( pOnlyIdx ){
    openStatTable(pParse, iDb, iStatCur, pOnlyIdx->zName, "idx");
  }else{
    openStatTable(pParse, iDb, iStatCur, pTab->zName, "tbl");
  }
  analyzeOneTable(pParse, pTab, pOnlyIdx, iStatCur, pParse->nMem+1);
  loadAnalysis(pParse, iDb);
}

void sqlite3Analyze(Parse *pParse, Token *pName1, Token *pName2){
  sqlite3 *db = pParse->db;
  int iDb;
  int i;
  char *z, *zDb;
  Table *pTab;
  Index *pIdx;
  Token *pTableName;

  assert( sqlite3BtreeHoldsAllMutexes(pParse->db) );
  if( SQLITE_OK!=sqlite3ReadSchema(pParse) ){
    return;
  }

  assert( pName2!=0 || pName1==0 );
  if( pName1==0 ){
    
    for(i=0; i<db->nDb; i++){
      if( i==1 ) continue;  
      analyzeDatabase(pParse, i);
    }
  }else if( pName2->n==0 ){
    
    iDb = sqlite3FindDb(db, pName1);
    if( iDb>=0 ){
      analyzeDatabase(pParse, iDb);
    }else{
      z = sqlite3NameFromToken(db, pName1);
      if( z ){
        if( (pIdx = sqlite3FindIndex(db, z, 0))!=0 ){
          analyzeTable(pParse, pIdx->pTable, pIdx);
        }else if( (pTab = sqlite3LocateTable(pParse, 0, z, 0))!=0 ){
          analyzeTable(pParse, pTab, 0);
        }
        sqlite3DbFree(db, z);
      }
    }
  }else{
    
    iDb = sqlite3TwoPartName(pParse, pName1, pName2, &pTableName);
    if( iDb>=0 ){
      zDb = db->aDb[iDb].zName;
      z = sqlite3NameFromToken(db, pTableName);
      if( z ){
        if( (pIdx = sqlite3FindIndex(db, z, zDb))!=0 ){
          analyzeTable(pParse, pIdx->pTable, pIdx);
        }else if( (pTab = sqlite3LocateTable(pParse, 0, z, zDb))!=0 ){
          analyzeTable(pParse, pTab, 0);
        }
        sqlite3DbFree(db, z);
      }
    }   
  }
}

typedef struct analysisInfo analysisInfo;
struct analysisInfo {
  sqlite3 *db;
  const char *zDatabase;
};

static int analysisLoader(void *pData, int argc, char **argv, char **NotUsed){
  analysisInfo *pInfo = (analysisInfo*)pData;
  Index *pIndex;
  Table *pTable;
  int i, c, n;
  unsigned int v;
  const char *z;

  assert( argc==3 );
  UNUSED_PARAMETER2(NotUsed, argc);

  if( argv==0 || argv[0]==0 || argv[2]==0 ){
    return 0;
  }
  pTable = sqlite3FindTable(pInfo->db, argv[0], pInfo->zDatabase);
  if( pTable==0 ){
    return 0;
  }
  if( argv[1] ){
    pIndex = sqlite3FindIndex(pInfo->db, argv[1], pInfo->zDatabase);
  }else{
    pIndex = 0;
  }
  n = pIndex ? pIndex->nColumn : 0;
  z = argv[2];
  for(i=0; *z && i<=n; i++){
    v = 0;
    while( (c=z[0])>='0' && c<='9' ){
      v = v*10 + c - '0';
      z++;
    }
    if( i==0 ) pTable->nRowEst = v;
    if( pIndex==0 ) break;
    pIndex->aiRowEst[i] = v;
    if( *z==' ' ) z++;
    if( strcmp(z, "unordered")==0 ){
      pIndex->bUnordered = 1;
      break;
    }
  }
  return 0;
}

void sqlite3DeleteIndexSamples(sqlite3 *db, Index *pIdx){
#ifdef SQLITE_ENABLE_STAT2
  if( pIdx->aSample ){
    int j;
    for(j=0; j<SQLITE_INDEX_SAMPLES; j++){
      IndexSample *p = &pIdx->aSample[j];
      if( p->eType==SQLITE_TEXT || p->eType==SQLITE_BLOB ){
        sqlite3DbFree(db, p->u.z);
      }
    }
    sqlite3DbFree(db, pIdx->aSample);
  }
#else
  UNUSED_PARAMETER(db);
  UNUSED_PARAMETER(pIdx);
#endif
}

int sqlite3AnalysisLoad(sqlite3 *db, int iDb){
  analysisInfo sInfo;
  HashElem *i;
  char *zSql;
  int rc;

  assert( iDb>=0 && iDb<db->nDb );
  assert( db->aDb[iDb].pBt!=0 );

  
  assert( sqlite3SchemaMutexHeld(db, iDb, 0) );
  for(i=sqliteHashFirst(&db->aDb[iDb].pSchema->idxHash);i;i=sqliteHashNext(i)){
    Index *pIdx = sqliteHashData(i);
    sqlite3DefaultRowEst(pIdx);
    sqlite3DeleteIndexSamples(db, pIdx);
    pIdx->aSample = 0;
  }

  
  sInfo.db = db;
  sInfo.zDatabase = db->aDb[iDb].zName;
  if( sqlite3FindTable(db, "sqlite_stat1", sInfo.zDatabase)==0 ){
    return SQLITE_ERROR;
  }

  
  zSql = sqlite3MPrintf(db, 
      "SELECT tbl, idx, stat FROM %Q.sqlite_stat1", sInfo.zDatabase);
  if( zSql==0 ){
    rc = SQLITE_NOMEM;
  }else{
    rc = sqlite3_exec(db, zSql, analysisLoader, &sInfo, 0);
    sqlite3DbFree(db, zSql);
  }


  
#ifdef SQLITE_ENABLE_STAT2
  if( rc==SQLITE_OK && !sqlite3FindTable(db, "sqlite_stat2", sInfo.zDatabase) ){
    rc = SQLITE_ERROR;
  }
  if( rc==SQLITE_OK ){
    sqlite3_stmt *pStmt = 0;

    zSql = sqlite3MPrintf(db, 
        "SELECT idx,sampleno,sample FROM %Q.sqlite_stat2", sInfo.zDatabase);
    if( !zSql ){
      rc = SQLITE_NOMEM;
    }else{
      rc = sqlite3_prepare(db, zSql, -1, &pStmt, 0);
      sqlite3DbFree(db, zSql);
    }

    if( rc==SQLITE_OK ){
      while( sqlite3_step(pStmt)==SQLITE_ROW ){
        char *zIndex;   
        Index *pIdx;    

        zIndex = (char *)sqlite3_column_text(pStmt, 0);
        pIdx = zIndex ? sqlite3FindIndex(db, zIndex, sInfo.zDatabase) : 0;
        if( pIdx ){
          int iSample = sqlite3_column_int(pStmt, 1);
          if( iSample<SQLITE_INDEX_SAMPLES && iSample>=0 ){
            int eType = sqlite3_column_type(pStmt, 2);

            if( pIdx->aSample==0 ){
              static const int sz = sizeof(IndexSample)*SQLITE_INDEX_SAMPLES;
              pIdx->aSample = (IndexSample *)sqlite3DbMallocRaw(0, sz);
              if( pIdx->aSample==0 ){
                db->mallocFailed = 1;
                break;
              }
	      memset(pIdx->aSample, 0, sz);
            }

            assert( pIdx->aSample );
            {
              IndexSample *pSample = &pIdx->aSample[iSample];
              pSample->eType = (u8)eType;
              if( eType==SQLITE_INTEGER || eType==SQLITE_FLOAT ){
                pSample->u.r = sqlite3_column_double(pStmt, 2);
              }else if( eType==SQLITE_TEXT || eType==SQLITE_BLOB ){
                const char *z = (const char *)(
                    (eType==SQLITE_BLOB) ?
                    sqlite3_column_blob(pStmt, 2):
                    sqlite3_column_text(pStmt, 2)
                );
                int n = sqlite3_column_bytes(pStmt, 2);
                if( n>24 ){
                  n = 24;
                }
                pSample->nByte = (u8)n;
                if( n < 1){
                  pSample->u.z = 0;
                }else{
                  pSample->u.z = sqlite3DbStrNDup(0, z, n);
                  if( pSample->u.z==0 ){
                    db->mallocFailed = 1;
                    break;
                  }
                }
              }
            }
          }
        }
      }
      rc = sqlite3_finalize(pStmt);
    }
  }
#endif

  if( rc==SQLITE_NOMEM ){
    db->mallocFailed = 1;
  }
  return rc;
}


#endif 
