/*****************************************************************************
 *
 * Project:  OpenCPN
 * Purpose   Squiddio plugin
 *
 ***************************************************************************
 *   Copyright (C) 2014 by Mauro Calvi   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 */

#ifndef _SQUIDDIOPI_H_
#define _SQUIDDIOPI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#define     PLUGIN_VERSION_MAJOR    0
#define     PLUGIN_VERSION_MINOR    3

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    10

#include <wx/list.h>
#include <wx/sstream.h>
#include <wx/protocol/http.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include <wx/listimpl.cpp>

#include "ocpn_plugin.h"
#include "icons.h"
#include "Layer.h"
#include "Hyperlink.h"
#include "PoiMan.h"
#include "Poi.h"
#include "NavObjectCollection.h"
#include "squiddioPrefsDialogBase.h"
#include "nmea0183/nmea0183.h"

class Layer;
class Poi;
class PoiMan;
class NavObjectCollection1;
class wxFileConfig;
//-----------------------------------new
class demoWindow;
//-----------------------------------

extern PoiMan *pPoiMan;
enum
{
    TIMER_ID= 10
};

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------


class squiddio_pi : public opencpn_plugin_110
{
public:
      squiddio_pi(void *ppimgr);
      ~squiddio_pi(void);

//    The required PlugIn Methods
      int Init(void);
      bool DeInit(void);

      int GetAPIVersionMajor();
      int GetAPIVersionMinor();
      int GetPlugInVersionMajor();
      int GetPlugInVersionMinor();
      wxBitmap * GetPlugInBitmap();

      wxString GetCommonName();
      wxString GetShortDescription();
      wxString GetLongDescription();

//    The optional method overrides

      void RenderLayerContentsOnChart(Layer *layer, bool save_config = true);

      void OnContextMenuItemCallback(int id);
      void UpdateAuiStatus(void);

//    The override PlugIn Methods
      bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
      void SetCursorLatLon(double lat, double lon);
      bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
      int GetToolbarToolCount(void);
      bool LoadConfig(void);

      void ShowPreferencesDialog( wxWindow* parent );
      void OnToolbarToolCallback(int id);
      void SetPluginMessage(wxString &message_id, wxString &message_body);
      void SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix);
      void appendOSDirSlash( wxString* pString );
      bool IsOnline(void);
      wxString DownloadLayer(wxString url_path);
      bool SaveLayer(wxString,wxString);
      Layer * GetLocalLayer(void);
      Layer * LoadLayer(wxString, wxString);
      void ReportDestination(double lat, double lon);
      bool LoadLayers(wxString &path);
      bool LoadLayerItems(wxString & path, Layer *l, bool show);
      bool ShowPOI(Poi* wp);
      bool HidePOI(Poi* wp);

      void RenderLayers();

      void SetNMEASentence(wxString &sentence);
      wxString PostPosition(double lat, double lon, double sog, double cog);
      void ShowFriendsLogs();

      // todo can the follwoign be moved to private?
      double m_cursor_lat, m_cursor_lon;
      Layer *local_sq_layer;
      wxArrayInt  m_period_secs;

      NMEA0183        m_NMEA0183;                 // Used to parse NMEA Sentences
      wxString          m_NMEASentence;
      double            mLat, mLon, mSog, mCog, mVar;
      int         g_PostPeriod;
      int         g_RetrievePeriod;

private:

      bool SaveConfig(void);
      bool ShowType(Poi * wp);

      wxWindow         *m_parent_window;
      wxAuiManager     *m_AUImgr;
      int              	m_show_id;
      int              	m_hide_id;
      int 				m_update_id;
      int 				m_report_id;
      bool 				isLayerUpdate;
      wxString 			local_region;
      wxString			layerdir;
      wxFileConfig     *m_pconfig;
      LayerList 	   *pLayerList;
      wxString			g_VisibleLayers;
      wxString			g_InvisibleLayers;

      int 		 		g_LayerIdx;
      bool				g_bShowLayers;
      Plugin_Hyperlink *link;
      Hyperlink 	   *wp_link;
      
      long              last_online_chk;
      bool              last_online;
      int         g_LastUpdate;

      wxString     g_Email;
      wxString     g_ApiKey;
      bool        g_ViewMarinas;
      bool        g_ViewAnchorages;
      bool        g_ViewYachtClubs;
      bool        g_ViewDocks;
      bool        g_ViewRamps;
      bool        g_ViewFuelStations;
      bool        g_ViewOthers;

      int         m_squiddio_dialog_x, m_squiddio_dialog_y;
      Layer       *m_LogsLayer;

      // ------------------------------------ new
      demoWindow       *m_pdemo_window;
      int               m_demoshow_id;
      int               m_demohide_id;
      //-------------------------------------
};


//------------------------------------------ new

class demoWindow : public wxWindow
{
public:
      demoWindow(squiddio_pi * plugin, wxWindow *pparent, wxWindowID id);
      ~demoWindow(){}

      void OnPaint(wxPaintEvent& event);
      void OnSize(wxSizeEvent& event){}
      void OnTimerTimeout(wxTimerEvent& event);

      wxTimer       * m_pTimer;
      wxStaticText  * m_pStaticText;
      wxDateTime      m_LastLogsRcvd; //this and the following should be type int but can't figure out how to convert that string
      wxDateTime      m_LastLogSent;


private:
      int g_RetrieveSecs;
      wxWindow      * m_parent_window;
      squiddio_pi   * p_plugin;


DECLARE_EVENT_TABLE()
};

//-------------------------------------------

#endif






