/*
** 2010 August 28
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Code for testing all sorts of SQLite interfaces. This code
** is not included in the SQLite library. 
*/

#include <sqlite3.h>

#include "sqliteInt.h"

typedef struct Circle Circle;
struct Circle {
  struct Box {
    double xmin;
    double xmax;
    double ymin;
    double ymax;
  } aBox[2];
  double centerx;
  double centery;
  double radius;
};

static void circle_del(void *p){
  sqlite3_free(p);
}

static int circle_geom(
  sqlite3_rtree_geometry *p,
  int nCoord, 
  double *aCoord, 
  int *pRes
){
  int i;                          
  Circle *pCircle;                
  double xmin, xmax;              
  double ymin, ymax;              

  if( p->pUser==0 ){

    if( nCoord!=4 ) return SQLITE_ERROR;

    if( p->nParam!=3 || p->aParam[2]<0.0 ) return SQLITE_ERROR;

    pCircle = (Circle *)(p->pUser = sqlite3_malloc(sizeof(Circle)));
    if( !pCircle ) return SQLITE_NOMEM;
    p->xDelUser = circle_del;

    pCircle->centerx = p->aParam[0];
    pCircle->centery = p->aParam[1];
    pCircle->radius = p->aParam[2];

    pCircle->aBox[0].xmin = pCircle->centerx;
    pCircle->aBox[0].xmax = pCircle->centerx;
    pCircle->aBox[0].ymin = pCircle->centery + pCircle->radius;
    pCircle->aBox[0].ymax = pCircle->centery - pCircle->radius;
    pCircle->aBox[1].xmin = pCircle->centerx + pCircle->radius;
    pCircle->aBox[1].xmax = pCircle->centerx - pCircle->radius;
    pCircle->aBox[1].ymin = pCircle->centery;
    pCircle->aBox[1].ymax = pCircle->centery;
  }

  pCircle = (Circle *)p->pUser;
  xmin = aCoord[0];
  xmax = aCoord[1];
  ymin = aCoord[2];
  ymax = aCoord[3];

  for(i=0; i<4; i++){
    double x = (i&0x01) ? xmax : xmin;
    double y = (i&0x02) ? ymax : ymin;
    double d2;
    
    d2  = (x-pCircle->centerx)*(x-pCircle->centerx);
    d2 += (y-pCircle->centery)*(y-pCircle->centery);
    if( d2<(pCircle->radius*pCircle->radius) ){
      *pRes = 1;
      return SQLITE_OK;
    }
  }

  for(i=0; i<2; i++){
    if( xmin<=pCircle->aBox[i].xmin 
     && xmax>=pCircle->aBox[i].xmax 
     && ymin<=pCircle->aBox[i].ymin 
     && ymax>=pCircle->aBox[i].ymax 
    ){
      *pRes = 1;
      return SQLITE_OK;
    }
  }

  *pRes = 0;
  return SQLITE_OK;
}


#include <assert.h>
#include "tcl.h"

typedef struct Cube Cube;
struct Cube {
  double x;
  double y;
  double z;
  double width;
  double height;
  double depth;
};

static void cube_context_free(void *p){
  sqlite3_free(p);
}

static int gHere = 42;

static int cube_geom(
  sqlite3_rtree_geometry *p,
  int nCoord, 
  double *aCoord, 
  int *piRes
){
  Cube *pCube = (Cube *)p->pUser;

  assert( p->pContext==(void *)&gHere );

  if( pCube==0 ){
    if( p->nParam!=6 || nCoord!=6
     || p->aParam[3]<=0.0 || p->aParam[4]<=0.0 || p->aParam[5]<=0.0
    ){
      return SQLITE_ERROR;
    }
    pCube = (Cube *)sqlite3_malloc(sizeof(Cube));
    if( !pCube ){
      return SQLITE_NOMEM;
    }
    pCube->x = p->aParam[0];
    pCube->y = p->aParam[1];
    pCube->z = p->aParam[2];
    pCube->width = p->aParam[3];
    pCube->height = p->aParam[4];
    pCube->depth = p->aParam[5];

    p->pUser = (void *)pCube;
    p->xDelUser = cube_context_free;
  }

  assert( nCoord==6 );
  *piRes = 0;
  if( aCoord[0]<=(pCube->x+pCube->width)
   && aCoord[1]>=pCube->x
   && aCoord[2]<=(pCube->y+pCube->height)
   && aCoord[3]>=pCube->y
   && aCoord[4]<=(pCube->z+pCube->depth)
   && aCoord[5]>=pCube->z
  ){
    *piRes = 1;
  }

  return SQLITE_OK;
}

static int register_cube_geom(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifndef SQLITE_ENABLE_RTREE
  UNUSED_PARAMETER(clientData);
  UNUSED_PARAMETER(interp);
  UNUSED_PARAMETER(objc);
  UNUSED_PARAMETER(objv);
#else
  extern int getDbPointer(Tcl_Interp*, const char*, sqlite3**);
  extern const char *sqlite3TestErrorName(int);
  sqlite3 *db;
  int rc;

  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB");
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  rc = sqlite3_rtree_geometry_callback(db, "cube", cube_geom, (void *)&gHere);
  Tcl_SetResult(interp, (char *)sqlite3TestErrorName(rc), TCL_STATIC);
#endif
  return TCL_OK;
}

static int register_circle_geom(
  void * clientData,
  Tcl_Interp *interp,
  int objc,
  Tcl_Obj *CONST objv[]
){
#ifndef SQLITE_ENABLE_RTREE
  UNUSED_PARAMETER(clientData);
  UNUSED_PARAMETER(interp);
  UNUSED_PARAMETER(objc);
  UNUSED_PARAMETER(objv);
#else
  extern int getDbPointer(Tcl_Interp*, const char*, sqlite3**);
  extern const char *sqlite3TestErrorName(int);
  sqlite3 *db;
  int rc;

  if( objc!=2 ){
    Tcl_WrongNumArgs(interp, 1, objv, "DB");
    return TCL_ERROR;
  }
  if( getDbPointer(interp, Tcl_GetString(objv[1]), &db) ) return TCL_ERROR;
  rc = sqlite3_rtree_geometry_callback(db, "circle", circle_geom, 0);
  Tcl_SetResult(interp, (char *)sqlite3TestErrorName(rc), TCL_STATIC);
#endif
  return TCL_OK;
}

int Sqlitetestrtree_Init(Tcl_Interp *interp){
  Tcl_CreateObjCommand(interp, "register_cube_geom", register_cube_geom, 0, 0);
  Tcl_CreateObjCommand(interp, "register_circle_geom",register_circle_geom,0,0);
  return TCL_OK;
}
