//<File !Start!>
// FILE: [HXduino_GSLC.h]
// Created by GUIslice Builder version: [0.16.b011]
//
// GUIslice Builder Generated GUI Framework File
//
// For the latest guides, updates and support view:
// https://github.com/ImpulseAdventure/GUIslice
//
//<File !End!>

#ifndef _GUISLICE_GEN_H
#define _GUISLICE_GEN_H

// ------------------------------------------------
// Headers to include
// ------------------------------------------------
#include "GUIslice.h"
#include "GUIslice_drv.h"

// Include any extended elements
//<Includes !Start!>
// Include extended elements
#include "elem/XGraph.h"
//<Includes !End!>

// ------------------------------------------------
// Headers and Defines for fonts
// Note that font files are located within the Adafruit-GFX library folder:
// ------------------------------------------------
//<Fonts !Start!>
#if defined(DRV_DISP_TFT_ESPI)
  #error Project tab->Target Platform should be tft_espi
#endif
#include <Adafruit_GFX.h>
#include "Fonts/NotoSans12pt7b.h"
#include "Fonts/NotoSans22pt7b.h"
//<Fonts !End!>

// ------------------------------------------------
// Defines for resources
// ------------------------------------------------
//<Resources !Start!>
//<Resources !End!>

// ------------------------------------------------
// Enumerations for pages, elements, fonts, images
// ------------------------------------------------
//<Enum !Start!>
enum {E_PG_MAIN,E_PG2};
enum {E_DRAW_LINE1,E_DRAW_LINE2,E_DRAW_LINE3,E_DRAW_LINE4,E_ELEM_GRAPH3
      ,E_ELEM_TEXT1,E_ELEM_TEXT2,E_ELEM_TEXT4,E_ELEM_TEXT6};
// Must use separate enum for fonts with MAX_FONT at end to use gslc_FontSet.
enum {E_BUILTIN5X8,E_NOTOSANS12,E_NOTOSANS22,MAX_FONT};
//<Enum !End!>

// ------------------------------------------------
// Instantiate the GUI
// ------------------------------------------------

// ------------------------------------------------
// Define the maximum number of elements and pages
// ------------------------------------------------
//<ElementDefines !Start!>
#define MAX_PAGE                2

#define MAX_ELEM_PG_MAIN 1 // # Elems total on page
#define MAX_ELEM_PG_MAIN_RAM MAX_ELEM_PG_MAIN // # Elems in RAM

#define MAX_ELEM_PG2 8 // # Elems total on page
#define MAX_ELEM_PG2_RAM MAX_ELEM_PG2 // # Elems in RAM
//<ElementDefines !End!>

// ------------------------------------------------
// Create element storage
// ------------------------------------------------
gslc_tsGui                      m_gui;
gslc_tsDriver                   m_drv;
gslc_tsFont                     m_asFont[MAX_FONT];
gslc_tsPage                     m_asPage[MAX_PAGE];

//<GUI_Extra_Elements !Start!>
gslc_tsElem                     m_asPage1Elem[MAX_ELEM_PG_MAIN_RAM];
gslc_tsElemRef                  m_asPage1ElemRef[MAX_ELEM_PG_MAIN];
gslc_tsElem                     m_asPage2Elem[MAX_ELEM_PG2_RAM];
gslc_tsElemRef                  m_asPage2ElemRef[MAX_ELEM_PG2];
gslc_tsXGraph                   m_sGraph3;
int16_t                         m_anGraphBuf3[30]; // NRows=30

#define MAX_STR                 100

//<GUI_Extra_Elements !End!>

// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Element References for direct access
//<Extern_References !Start!>
extern gslc_tsElemRef* m_pElemGraph3;
//<Extern_References !End!>

// Define debug message function
static int16_t DebugOut(char ch);

// ------------------------------------------------
// Callback Methods
// ------------------------------------------------
bool CbBtnCommon(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY);
bool CbCheckbox(void* pvGui, void* pvElemRef, int16_t nSelId, bool bState);
bool CbDrawScanner(void* pvGui,void* pvElemRef,gslc_teRedrawType eRedraw);
bool CbKeypad(void* pvGui, void *pvElemRef, int16_t nState, void* pvData);
bool CbListbox(void* pvGui, void* pvElemRef, int16_t nSelId);
bool CbSlidePos(void* pvGui,void* pvElemRef,int16_t nPos);
bool CbSpinner(void* pvGui, void *pvElemRef, int16_t nState, void* pvData);
bool CbTickScanner(void* pvGui,void* pvScope);

// ------------------------------------------------
// Create page elements
// ------------------------------------------------
void InitGUIslice_gen()
{
  gslc_tsElemRef* pElemRef = NULL;

  if (!gslc_Init(&m_gui,&m_drv,m_asPage,MAX_PAGE,m_asFont,MAX_FONT)) { return; }

  // ------------------------------------------------
  // Load Fonts
  // ------------------------------------------------
//<Load_Fonts !Start!>
    if (!gslc_FontSet(&m_gui,E_BUILTIN5X8,GSLC_FONTREF_PTR,NULL,1)) { return; }
    if (!gslc_FontSet(&m_gui,E_NOTOSANS12,GSLC_FONTREF_PTR,&NotoSans12pt7b,1)) { return; }
    if (!gslc_FontSet(&m_gui,E_NOTOSANS22,GSLC_FONTREF_PTR,&NotoSans22pt7b,1)) { return; }
//<Load_Fonts !End!>

//<InitGUI !Start!>
  gslc_PageAdd(&m_gui,E_PG_MAIN,m_asPage1Elem,MAX_ELEM_PG_MAIN_RAM,m_asPage1ElemRef,MAX_ELEM_PG_MAIN);
  gslc_PageAdd(&m_gui,E_PG2,m_asPage2Elem,MAX_ELEM_PG2_RAM,m_asPage2ElemRef,MAX_ELEM_PG2);

  // NOTE: The current page defaults to the first page added. Here we explicitly
  //       ensure that the main page is the correct page no matter the add order.
  gslc_SetPageCur(&m_gui,E_PG_MAIN);
  
  // Set Background to a flat color
  gslc_SetBkgndColor(&m_gui,GSLC_COL_BLACK);

  // -----------------------------------
  // PAGE: E_PG_MAIN
  
  
  // Create E_ELEM_TEXT1 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT1,E_PG_MAIN,(gslc_tsRect){25,20,78,34},
    (char*)"v1.0",0,E_NOTOSANS22);
  gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_MID_MID);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);

  // -----------------------------------
  // PAGE: E_PG2
  
  
  // Create E_ELEM_TEXT2 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT2,E_PG2,(gslc_tsRect){3,30,54,33},
    (char*)"1.5",0,E_NOTOSANS22);
  gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_BOT_LEFT);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
  
  // Create E_ELEM_TEXT4 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT4,E_PG2,(gslc_tsRect){60,1,67,34},
    (char*)"093",0,E_NOTOSANS22);
  gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_TOP_RIGHT);
  gslc_ElemSetFillEn(&m_gui,pElemRef,false);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_WHITE,GSLC_COL_BLACK,GSLC_COL_BLACK);

  // Create graph E_ELEM_GRAPH3
  pElemRef = gslc_ElemXGraphCreate(&m_gui,E_ELEM_GRAPH3,E_PG2,
    &m_sGraph3,(gslc_tsRect){74,37,40,25},E_BUILTIN5X8,(int16_t*)&m_anGraphBuf3,
        30,GSLC_COL_WHITE);
  gslc_ElemXGraphSetStyle(&m_gui,pElemRef, GSLCX_GRAPH_STYLE_FILL, 5);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_WHITE,GSLC_COL_BLACK,GSLC_COL_BLACK);
  m_pElemGraph3 = pElemRef;
  
  // Create E_ELEM_TEXT6 text label
  pElemRef = gslc_ElemCreateTxt(&m_gui,E_ELEM_TEXT6,E_PG2,(gslc_tsRect){4,4,44,19},
    (char*)"0:00",0,E_NOTOSANS12);
  gslc_ElemSetTxtAlign(&m_gui,pElemRef,GSLC_ALIGN_TOP_LEFT);
  gslc_ElemSetTxtCol(&m_gui,pElemRef,GSLC_COL_WHITE);

  // Create E_DRAW_LINE1 line 
  pElemRef = gslc_ElemCreateLine(&m_gui,E_DRAW_LINE1,E_PG2,0,1,127,1);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLACK,GSLC_COL_WHITE,GSLC_COL_WHITE);

  // Create E_DRAW_LINE2 line 
  pElemRef = gslc_ElemCreateLine(&m_gui,E_DRAW_LINE2,E_PG2,0,63,127,63);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLACK,GSLC_COL_WHITE,GSLC_COL_WHITE);

  // Create E_DRAW_LINE3 line 
  pElemRef = gslc_ElemCreateLine(&m_gui,E_DRAW_LINE3,E_PG2,1,0,1,63);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLACK,GSLC_COL_WHITE,GSLC_COL_WHITE);

  // Create E_DRAW_LINE4 line 
  pElemRef = gslc_ElemCreateLine(&m_gui,E_DRAW_LINE4,E_PG2,127,0,127,63);
  gslc_ElemSetCol(&m_gui,pElemRef,GSLC_COL_BLACK,GSLC_COL_WHITE,GSLC_COL_WHITE);
//<InitGUI !End!>

//<Startup !Start!>
//<Startup !End!>

}

#endif // end _GUISLICE_GEN_H
