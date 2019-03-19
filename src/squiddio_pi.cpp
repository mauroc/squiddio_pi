/******************************************************************************
 * Project:  OpenCPN
 * Purpose:  Squiddio plugin
 *
 ***************************************************************************
 *   Copyright (C) 2014 by Mauro Calvi                                     *
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

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include "squiddio_pi.h"
#include "logs.h"
#include <wx/fileconf.h>
#include <wx/fontdlg.h>
#include "sJSON.h"
#include "version.h"

#include "wxJSON/jsonwriter.h"

WX_DEFINE_LIST (LayerList);
WX_DEFINE_LIST (HyperlinkList);
WX_DEFINE_LIST (Plugin_HyperlinkList);

// the class factories, used to create and destroy instances of the PlugIn
//

// these variables are shared with NavObjectCollection
PoiMan *pPoiMan;
class logsWindow;

sJSON   *g_psJSON;
wxString                g_ReceivedODAPIMessage;
wxJSONValue             g_ReceivedODAPIJSONMsg;
wxString                g_ReceivedJSONMessage;
wxJSONValue             g_ReceivedJSONJSONMsg;
double                  g_dVar;
bool                    g_bODAvailable;
squiddio_pi             *g_squiddio_pi;



extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr) {
    return new squiddio_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p) {
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

int period_secs(int period) {
    switch (period) {
    case 0: return 0;
    case 1: return 85400;
    case 2: return 43200;
    case 3: return 3600;
    case 4: return 1800;
    case 5: return 60;
    case 6: return 30;
    case 7: return 10;
    default: break;
    }
    return 0;
}

BEGIN_EVENT_TABLE( squiddio_pi, wxEvtHandler )
    EVT_MYEVENT( squiddio_pi::OnThreadActionFinished )
END_EVENT_TABLE()

squiddio_pi::squiddio_pi(void *ppimgr) :
        opencpn_plugin_116(ppimgr) // constructor initialization
{
    // Create the PlugIn icons
    initialize_images();
    SetThreadRunning(false);
    g_squiddio_pi = this;
    g_bODAvailable = false;
}

squiddio_pi::~squiddio_pi(void) {
    delete _img_marina_grn;
    delete _img_anchor_blu;
    delete _img_aton_gry;
    delete _img_club_pur;
    delete _img_fuelpump_red;
    delete _img_pier_yel;
    delete _img_ramp_azu;
    delete _img_logimg_N;
    delete _img_logimg_NE;
    delete _img_logimg_E;
    delete _img_logimg_SE;
    delete _img_logimg_S;
    delete _img_logimg_SW;
    delete _img_logimg_W;
    delete _img_logimg_NW;
    delete _img_logimg_C;
    delete _img_logimg_U;

}

int squiddio_pi::Init(void) {

    wxLogMessage(_T("squiddio_pi: Init()"));

    m_plogs_window = NULL;
    g_PostPeriod = 0;
    g_RetrievePeriod = 0;
    g_LayerIdx = 0;
    g_OCPN = true;
    m_bDoneODVersionCall = false;
    m_bDoneODAPIVersionCall = false;
    m_bODAPIOK = false;
    m_iODAPIVersionMajor = 0;
    m_iODVersionMinor = 0;
    m_iODVersionPatch = 0;
    m_bODAPIMessageShown = false;
    m_bODFindPointInAnyBoundary = false;
    m_bODFindClosestBoundaryLineCrossing = false;
    m_bODFindFirstBoundaryLineCrossing = false;
    m_bODCreateBoundary = false;
    m_bODCreateBoundaryPoint = false;
    m_bODCreateTextPoint = false;
    m_bODDeleteTextPoint = false;
    m_bODAddPointIcon = false;
    m_bODDeletePointIcon = false;
    m_pOD_FindPointInAnyBoundary = NULL;
    m_pODFindClosestBoundaryLineCrossing = NULL;
    m_pODFindFirstBoundaryLineCrossing = NULL;
    m_pODFindFirstBoundaryLineCrossing = NULL;
    m_pODCreateBoundary = NULL;
    m_pODCreateBoundaryPoint = NULL;
    m_pODCreateTextPoint = NULL;
    m_pODDeleteTextPoint = NULL;
    m_pODAddPointIcon = NULL;
    m_pODDeletePointIcon = NULL;
    
    
    
    // Get a pointer to the opencpn display canvas, to use as a parent for windows created
    m_parent_window = GetOCPNCanvasWindow();

    last_online_chk = 0;
    last_online =false;

    wxMenu dummy_menu;

    wxMenuItem *pmi = new wxMenuItem(&dummy_menu, -1,
            _("Show local sQuiddio destinations"));
    m_show_id = AddCanvasContextMenuItem(pmi, this);
    SetCanvasContextMenuItemViz(m_show_id, false);

    wxMenuItem *pmih = new wxMenuItem(&dummy_menu, -1,
            _("Hide local sQuiddio destinations"));
    m_hide_id = AddCanvasContextMenuItem(pmih, this);
    SetCanvasContextMenuItemViz(m_hide_id, false);

    wxMenuItem *updi = new wxMenuItem(&dummy_menu, -1,
            _("Download local sQuiddio destinations"));
    m_update_id = AddCanvasContextMenuItem(updi, this);
    SetCanvasContextMenuItemViz(m_update_id, true);

    wxMenuItem *repi = new wxMenuItem(&dummy_menu, -1,
            _("Report a Destination at this location"));
    m_report_id = AddCanvasContextMenuItem(repi, this);
    SetCanvasContextMenuItemViz(m_report_id, true);

    AddCustomWaypointIcon(_img_marina_grn, _T("marina_grn"), _T("Marina"));
    AddCustomWaypointIcon(_img_anchor_blu, _T("anchor_blu"),
            _T("Anchorage/Buoys"));
    AddCustomWaypointIcon(_img_aton_gry, _T("aton_gry"),
            _T("AIS ATON Marker"));
    AddCustomWaypointIcon(_img_club_pur, _T("club_pur"), _T("Yacht Club"));
    AddCustomWaypointIcon(_img_fuelpump_red, _T("fuelpump_red"),
            _T("Fuel Station"));
    AddCustomWaypointIcon(_img_pier_yel, _T("pier_yel"), _T("Dock/Pier"));
    AddCustomWaypointIcon(_img_ramp_azu, _T("ramp_azu"), _T("Boat Ramp"));

    AddCustomWaypointIcon(_img_logimg_N, _T("logimg_N"), _T("North"));
    AddCustomWaypointIcon(_img_logimg_NE, _T("logimg_NE"), _T("North East"));
    AddCustomWaypointIcon(_img_logimg_E, _T("logimg_E"), _T("East"));
    AddCustomWaypointIcon(_img_logimg_SE, _T("logimg_SE"), _T("South East"));
    AddCustomWaypointIcon(_img_logimg_S, _T("logimg_S"), _T("South"));
    AddCustomWaypointIcon(_img_logimg_SW, _T("logimg_SW"), _T("South West"));
    AddCustomWaypointIcon(_img_logimg_W, _T("logimg_W"), _T("West"));
    AddCustomWaypointIcon(_img_logimg_NW, _T("logimg_NW"), _T("North West"));
    AddCustomWaypointIcon(_img_logimg_C, _T("logimg_C"), _T("Checked in"));
    AddCustomWaypointIcon(_img_logimg_U, _T("logimg_U"), _T("Unknown heading"));

    pLayerList = new LayerList;
    pPoiMan = new PoiMan;
    link = new Plugin_Hyperlink;

    m_pconfig = GetOCPNConfigObject();
    LoadConfig();

    if(!m_bDoneODAPIVersionCall) GetODAPI();
    
    layerdir = *GetpPrivateApplicationDataLocation();
    layerdir += wxFileName::GetPathSeparator();
    layerdir += _T("squiddio");

    if (!wxDir::Exists(layerdir))
        wxFileName::Mkdir(layerdir);

    if (wxDir::Exists(layerdir)) {
        wxString laymsg;
        laymsg.Printf(wxT("squiddio_pi: getting .gpx layer files from: %s"),
                layerdir.c_str());
        wxLogMessage(laymsg);

        LoadLayers(layerdir);

        Layer * l;
        LayerList::iterator it;
        int index = 0;
        for (it = (*pLayerList).begin(); it != (*pLayerList).end();
                ++it, ++index) {
            l = (Layer *) (*it);
            l->SetVisibleNames(false);
            RenderLayerContentsOnChart(l);
        }
    }

    //    This PlugIn needs a toolbar icon, so request its insertion
    m_leftclick_tool_id = InsertPlugInTool(_T(""), _img_plugin_logo,
            _img_plugin_logo, wxITEM_NORMAL, _("sQuiddio"), _T(""), NULL,
            SQUIDDIO_TOOL_POSITION, 0, this);
            
    m_pThread = new SquiddioThread(this);
    wxThreadError err = m_pThread->Run();

    if ( err != wxTHREAD_NO_ERROR )
    {
        delete m_pThread;
        m_pThread = NULL;
    }

    return (
    INSTALLS_CONTEXTMENU_ITEMS |
    WANTS_CURSOR_LATLON |
    WANTS_NMEA_SENTENCES |
    WANTS_PREFERENCES |
    USES_AUI_MANAGER |
    WANTS_CONFIG |
    WANTS_TOOLBAR_CALLBACK |
    INSTALLS_TOOLBAR_TOOL |
    WANTS_PLUGIN_MESSAGING    |
    WANTS_LATE_INIT
    );
}

bool squiddio_pi::DeInit(void) {
    RenderLayers(true);
    
    RemovePlugInTool(m_leftclick_tool_id);

    if (m_plogs_window) {
        m_AUImgr->DetachPane(m_plogs_window);
        m_plogs_window->Close();
    }

    RemoveCanvasContextMenuItem(m_show_id);
    RemoveCanvasContextMenuItem(m_hide_id);
    RemoveCanvasContextMenuItem(m_update_id);
    RemoveCanvasContextMenuItem(m_report_id);

    wxPoiListNode *node = pPoiMan->GetWaypointList()->GetFirst();
    
    while (node) {
        Poi *rp = node->GetData();
        HidePOI(rp);
        node = node->GetNext();
    }
    
    LayerList::iterator it;
    int index = 0;
    for (it = (*pLayerList).begin(); it != (*pLayerList).end(); ++index) {
        Layer * l = (Layer *) (*it);
        ++it;
        pLayerList->DeleteObject(l);
    }
    SaveConfig();
    RequestRefresh(m_parent_window);
    
    {
        wxCriticalSectionLocker enter(m_pThreadCS);
        if (m_pThread) // does the thread still exist?
        {
            while (m_pThread->IsWorking())
            {
                wxMilliSleep(10);
            }
            if (m_pThread->Delete() != wxTHREAD_NO_ERROR )
                wxLogError(_T("Can't delete the thread!"));
        }
    } // exit from the critical section to give the thread
        // the possibility to enter its destructor
        // (which is guarded with m_pThreadCS critical section!)
    while (1)
    {
        { // was the ~MyThread() function executed?
            wxCriticalSectionLocker enter(m_pThreadCS);
            if (!m_pThread)
                break;
        }
        // wait for thread completion
    }
    
    //Last resort check for thread completion, wait if it looks bad
    #define THREAD_WAIT_SECONDS  5
    //  Try to wait a bit to see if all compression threads exit nicely
    wxDateTime now = wxDateTime::Now();
    time_t stall = now.GetTicks();
    time_t end = stall + THREAD_WAIT_SECONDS;
    
    while(IsThreadRunning() && stall < end ){
        wxDateTime later = wxDateTime::Now();
        stall = later.GetTicks();
        
        wxYield();
        wxSleep(1);
        if( !IsThreadRunning() )
            break;
    }

    delete pLayerList;
    delete pPoiMan;
    delete link;
    delete m_plogs_window;
    return true;
}
bool squiddio_pi::LoadConfig(void) {
    wxFileConfig *pConf = (wxFileConfig *) m_pconfig;

    if (!pConf)
        return false;

    pConf->SetPath(_T("/PlugIns/libsquiddio_pi.so"));
    pConf->Read(_T("VisibleSqLayers"), &g_VisibleLayers);
    pConf->Read(_T("InvisibleSqLayers"), &g_InvisibleLayers);
    pConf->Read(_T("PostPeriod"), &g_PostPeriod);
    pConf->Read(_T("RetrievePeriod"), &g_RetrievePeriod);
    pConf->Read(_T("LastLogSent"), &g_LastLogSent);
    pConf->Read(_T("LastLogsRcvd"), &g_LastLogsRcvd);
    pConf->Read(_T("Email"), &g_Email);
    pConf->Read(_T("ApiKey"), &g_ApiKey);
    pConf->Read(_T("OCPN"), &g_OCPN, true);
    pConf->Read(_T("ViewMarinas"), &g_ViewMarinas, true);
    pConf->Read(_T("ViewAnchorages"), &g_ViewAnchorages, true);
    pConf->Read(_T("ViewDocks"), &g_ViewDocks, true);
    pConf->Read(_T("ViewYachtClubs"), &g_ViewYachtClubs, true);
    pConf->Read(_T("ViewFuelStations"), &g_ViewFuelStations, true);
    pConf->Read(_T("ViewRamps"), &g_ViewRamps, true);
    pConf->Read(_T("ViewOthers"), &g_ViewOthers, true);
    pConf->Read(_T("TextPointShowName"), &g_bODTextPointShowName, true);
    pConf->Read(_T("TextPosition"), &g_iODTextPointTextPosition, TEXT_BOTTOM);
    wxString  l_wxsDefautlTextColour;
    pConf->Read( wxS( "TextDefaultColour" ), &l_wxsDefautlTextColour, wxS( "BLACK" ) );
    g_colourODDefaultTextColour.Set( l_wxsDefautlTextColour );
    wxString  l_wxsDefautlTextBackgroundColour;
    pConf->Read( wxS( "TextDefaultBackgroundColour" ), &l_wxsDefautlTextBackgroundColour, wxS( "YELLOW" ) );
    g_colourODDefaultTextBackgroundColour.Set( l_wxsDefautlTextBackgroundColour );
    pConf->Read(_T("TextBackgroundTransparency"), &g_iODTextBackgroundTransparency, 100);
    int l_fontInfo;
    bool l_bfontInfo;
    wxFont *l_pDisplayTextFont = GetOCPNScaledFont_PlugIn( wxS("Marks"), 0 );
    pConf->Read( wxS( "DefaultTextPointPointSize" ), &l_fontInfo, (int)l_pDisplayTextFont->GetPointSize() );
    g_fontODDisplayTextFont.SetPointSize( l_fontInfo );
    pConf->Read( wxS( "DefaultTextPointFontFamily" ), &l_fontInfo, (int)l_pDisplayTextFont->GetFamily() );
    g_fontODDisplayTextFont.SetFamily( (wxFontFamily)l_fontInfo );
    pConf->Read( wxS( "DefaultTextPointFontStyle" ), &l_fontInfo, (int)l_pDisplayTextFont->GetStyle() );
    g_fontODDisplayTextFont.SetStyle( (wxFontStyle)l_fontInfo );
    pConf->Read( wxS( "DefaultTextPointFontWeight" ), &l_fontInfo, (int)l_pDisplayTextFont->GetWeight() );
    g_fontODDisplayTextFont.SetWeight( (wxFontWeight)l_fontInfo );
    pConf->Read( wxS( "DefaultTextPointFontUnderline" ), &l_bfontInfo, false );
    g_fontODDisplayTextFont.SetUnderlined( l_bfontInfo );
#if wxCHECK_VERSION(3,0,0)   
    pConf->Read( wxS( "DefaultTextPointFontStrikethrough" ), &l_bfontInfo, false );
    g_fontODDisplayTextFont.SetStrikethrough( l_bfontInfo );
#endif        
    wxString l_wxsFaceName;
    pConf->Read( wxS( "DefaultTextPointFaceName" ), &l_wxsFaceName, l_pDisplayTextFont->GetFaceName() );
    g_fontODDisplayTextFont.SetFaceName( l_wxsFaceName );
    pConf->Read( wxS( "DefaultTextPointFontEncoding" ), &l_fontInfo, (int)l_pDisplayTextFont->GetEncoding() );
    g_fontODDisplayTextFont.SetEncoding( (wxFontEncoding)l_fontInfo );
    pConf->Read( wxS( "DefaultTextPointDisplayTextWhen" ), &g_iTextPointDisplayTextWhen, TEXTPOINT_DISPLAY_TEXT_SHOW_ON_ROLLOVER );
    
    return true;
}

bool squiddio_pi::SaveConfig(void) {
    wxFileConfig *pConf = (wxFileConfig *) m_pconfig;

    if (!pConf)
        return false;

    pConf->SetPath(_T("/PlugIns/libsquiddio_pi.so"));
    pConf->Write(_T("VisibleSqLayers"), g_VisibleLayers);
    pConf->Write(_T("InvisibleSqLayers"), g_InvisibleLayers);
    pConf->Write(_T("PostPeriod"), g_PostPeriod);
    pConf->Write(_T("RetrievePeriod"), g_RetrievePeriod);
    pConf->Write(_T("LastLogSent"), g_LastLogSent);
    pConf->Write(_T("LastLogsRcvd"), g_LastLogsRcvd);
    pConf->Write(_T("Email"), g_Email);
    pConf->Write(_T("ApiKey"), g_ApiKey);
    pConf->Write(_T("OCPN"), g_OCPN);
    pConf->Write(_T("ViewMarinas"), g_ViewMarinas);
    pConf->Write(_T("ViewAnchorages"), g_ViewAnchorages);
    pConf->Write(_T("ViewDocks"), g_ViewDocks);
    pConf->Write(_T("ViewYachtClubs"), g_ViewYachtClubs);
    pConf->Write(_T("ViewRamps"), g_ViewRamps);
    pConf->Write(_T("ViewFuelStations"), g_ViewFuelStations);
    pConf->Write(_T("ViewOthers"), g_ViewOthers);
    pConf->Write(_T("TextPointShowName"), g_bODTextPointShowName);
    pConf->Write(_T("TextPosition"), g_iODTextPointTextPosition);
    pConf->Write(_T("TextDefaultColour"), g_colourODDefaultTextColour.GetAsString( wxC2S_CSS_SYNTAX ));
    pConf->Write(_T("TextDefaultBackgroundColour"), g_colourODDefaultTextBackgroundColour.GetAsString( wxC2S_CSS_SYNTAX ));
    pConf->Write(_T("TextBackgroundTransparency"), g_iODTextBackgroundTransparency);
    pConf->Write(_T("DefaultTextPointPointSize"), g_fontODDisplayTextFont.GetPointSize() );
    pConf->Write(_T("DefaultTextPointFontFamily"), (int)g_fontODDisplayTextFont.GetFamily() );
    pConf->Write(_T("DefaultTextPointFontStyle"), (int)g_fontODDisplayTextFont.GetStyle() );
    pConf->Write(_T("DefaultTextPointFontWeight"), (int)g_fontODDisplayTextFont.GetWeight() );
    pConf->Write(_T("DefaultTextPointFontUnderline"), g_fontODDisplayTextFont.GetUnderlined() );
    pConf->Write(_T("DefaultTextPointFontUnderline"), g_fontODDisplayTextFont.GetUnderlined() );
#if wxCHECK_VERSION(3,0,0)        
    pConf->Write(_T("DefaultTextPointFontStrikethrough"), g_fontODDisplayTextFont.GetStrikethrough() );
#endif
    pConf->Write(_T("DefaultTextPointFaceName"), g_fontODDisplayTextFont.GetFaceName() );
    pConf->Write(_T("DefaultTextPointFontEncoding"), (int)g_fontODDisplayTextFont.GetEncoding() );
    pConf->Write(_T("DefaultTextPointDisplayTextWhen"), g_iTextPointDisplayTextWhen );
    
    
    return true;
}

bool squiddio_pi::LoadLayers(wxString &path) {
    wxArrayString file_array;
    wxDir dir;
    Layer *l;
    dir.Open(path);
    if (dir.IsOpened()) {
        wxString filename;
        bool cont = dir.GetFirst(&filename);
        while (cont) {
            file_array.Clear();
            filename.Prepend(wxFileName::GetPathSeparator());
            filename.Prepend(path);
            wxFileName f(filename);
            size_t nfiles = 0;
            if (f.GetExt().IsSameAs(wxT("gpx")))
                file_array.Add(filename); // single-gpx-file layer
            else {
                wxDir dir(filename);
                if (dir.IsOpened()) {
                    nfiles = dir.GetAllFiles(filename, &file_array,
                            wxT("*.gpx"));      // layers subdirectory set
                }
            }

            if (file_array.GetCount()) {
                l = new Layer();
                l->m_LayerID = ++g_LayerIdx;
                l->m_LayerFileName = file_array[0];
                if (file_array.GetCount() <= 1)
                    wxFileName::SplitPath(file_array[0], NULL, NULL,
                            &(l->m_LayerName), NULL, NULL);
                else
                    wxFileName::SplitPath(filename, NULL, NULL,
                            &(l->m_LayerName), NULL, NULL);

                bool bLayerViz = false;

                if ((g_VisibleLayers.Contains(l->m_LayerName)) || (l->m_LayerName.Contains(_T("logs")) && g_RetrievePeriod > 0))
                    bLayerViz = true;

                l->m_bIsVisibleOnChart = bLayerViz;

                wxString laymsg;
                laymsg.Printf(wxT("squiddio_pi: new layer %d: %s"),
                        l->m_LayerID, l->m_LayerName.c_str());
                wxLogMessage(laymsg);

                pLayerList->Insert(l);

                //  Load the entire file array as a single layer
                for (unsigned int i = 0; i < file_array.GetCount(); i++) {
                    wxString file_path = file_array[i];
                    if (::wxFileExists(file_path)) {
                        LoadLayerItems(file_path, l, bLayerViz);
                    }
                //delete l;
                }
            }
            cont = dir.GetNext(&filename);
        }
    }
    return true;

}

bool squiddio_pi::LoadLayerItems(wxString &file_path, Layer *l, bool show) {
    NavObjectCollection1 *pSet = new NavObjectCollection1;
    pSet->load_file(file_path.fn_str());
    long nItems = pSet->LoadAllGPXObjectsAsLayer(l->m_LayerID, show);

    l->m_NoOfItems += nItems;

    wxString objmsg;
    objmsg.Printf(wxT("squiddio_pi: loaded GPX file %s with %ld items."),
            file_path.c_str(), nItems);
    wxLogMessage(objmsg);
    delete pSet;
    return nItems > 0;
}

Layer * squiddio_pi::LoadLayer(wxString file_path, wxString region) {
    Layer * l = new Layer();
    if (::wxFileExists(file_path)) {

        l->m_LayerID = ++g_LayerIdx;
        if (file_path.Contains(_T("logs.gpx"))) {
            l->m_LayerName = _T("logs");
        } else {
            l->m_LayerName = _T("SQ_") + region;
        }
        l->m_LayerFileName = file_path;
        l->m_bIsVisibleOnChart = true;
        pLayerList->Insert(l);

        LoadLayerItems(file_path, l, true);
    }
    return l;
}

bool squiddio_pi::ShowType(Poi * wp) {
    if (wp->m_IconName == _T("marina_grn"))
        return g_ViewMarinas;
    else if (wp->m_IconName == _T("anchor_blu"))
        return g_ViewAnchorages;
    else if (wp->m_IconName == _T("club_pur"))
        return g_ViewYachtClubs;
    else if (wp->m_IconName == _T("fuelpump_red"))
        return g_ViewFuelStations;
    else if (wp->m_IconName == _T("pier_yel"))
        return g_ViewDocks;
    else if (wp->m_IconName == _T("ramp_azu"))
        return g_ViewRamps;
    else if (wp->m_IconName == _T("others"))
        return g_ViewOthers;
    else if (wp->m_IconName == _T("aton_gry"))
        return g_ViewAIS;
    else
        return true;
}

void squiddio_pi::RenderLayers(bool hidePOI) {
    Layer * l;
    LayerList::iterator it;
    int index = 0;
    for (it = (*pLayerList).begin(); it != (*pLayerList).end(); ++it, ++index) {
        l = (Layer *) (*it);
        l->SetVisibleNames(false);
        RenderLayerContentsOnChart(l, false, hidePOI);
    }
}

void squiddio_pi::RenderLayerContentsOnChart(Layer *layer, bool save_config, bool hidePOI) {

    // Process POIs in this layer
    //if (layer->m_LayerName.Contains(_T("logs")) && g_RetrievePeriod ==0 )
    //    return;

    wxPoiListNode *node = pPoiMan->GetWaypointList()->GetFirst();

    while (node) {
        Poi *rp = node->GetData();
        if (rp && (rp->m_LayerID == layer->m_LayerID)) {
            rp->SetVisible(layer->IsVisibleOnChart());
            rp->SetNameShown(false);
            if (layer->IsVisibleOnChart() && ShowType(rp) && !hidePOI )
                ShowPOI(rp);
            else
                HidePOI(rp);
        }
        node = node->GetNext();
    }

    if (layer->IsVisibleOnChart()) {
        if (!g_VisibleLayers.Contains(layer->m_LayerName))
            g_VisibleLayers.Append(layer->m_LayerName + _T(";"));
        g_InvisibleLayers.Replace(layer->m_LayerName + _T(";"), wxEmptyString);
    } else {
        if (!g_InvisibleLayers.Contains(layer->m_LayerName))
            g_InvisibleLayers.Append(layer->m_LayerName + _T(";"));
        g_VisibleLayers.Replace(layer->m_LayerName + _T(";"), wxEmptyString);
    }
    RequestRefresh(m_parent_window);
    if (save_config)
        SaveConfig();
}
bool squiddio_pi::ShowPOI(Poi * wp) {
    double lat = wp->m_lat;
    double lon = wp->m_lon;
    wxString name = wp->GetName();
    wxString m_GUID = wp->m_GUID;
    wxString m_iconname = wp->m_IconName;
    
    if(!m_bDoneODAPIVersionCall) {
        GetODAPI();
        AddODIcons();
    }

    if(g_OCPN) {
        PlugIn_Waypoint * pPoint = new PlugIn_Waypoint(lat, lon, m_iconname, name,
                m_GUID);
        pPoint->m_MarkDescription = wp->m_MarkDescription;

        wxHyperlinkListNode *linknode = wp->m_HyperlinkList->GetFirst();
        wp_link = linknode->GetData();
        link->Link = wp_link->Link;
        link->DescrText = wp_link->DescrText;
        link->Type = wxEmptyString;

        pPoint->m_HyperlinkList = new Plugin_HyperlinkList;
        pPoint->m_HyperlinkList->Insert(link);

        bool added = AddSingleWaypoint(pPoint, false);
        return added;
    } else {
        CreateTextPoint_t *pCTP = new CreateTextPoint_t;
        pCTP->name = wp->GetName();
        pCTP->iconname = wp->m_IconName;
        pCTP->ShowName = g_bODTextPointShowName;
        pCTP->lat = wp->m_lat;
        pCTP->lon = wp->m_lon;
        pCTP->GUID = wp->m_GUID;
        pCTP->description = wp->m_MarkDescription;
        pCTP->TextToDisplay = wp->m_MarkDescription;
        pCTP->TextFont = g_fontODDisplayTextFont;
        pCTP->TextColour = g_colourODDefaultTextColour.GetAsString();
        pCTP->TextPointDisplayTextWhen = g_iTextPointDisplayTextWhen;
        pCTP->TextPosition = g_iODTextPointTextPosition;
        pCTP->BackgroundColour = g_colourODDefaultTextBackgroundColour.GetAsString();
        pCTP->BackgroundTransparancy = g_iODTextBackgroundTransparency;
        pCTP->ringscolour = _T("DEFAULT");
        pCTP->ringsvisible = false;
        pCTP->ringsnumber = 0;
        pCTP->defaultFont = false;
        pCTP->Visible = true;
        pCTP->temporary = true;
        
        wxHyperlinkListNode *linknode = wp->m_HyperlinkList->GetFirst();
        wp_link = linknode->GetData();
        pCTP->TextPointHyperLinkList.clear();
        HyperLinkList_t *l_list = new HyperLinkList_t;
        l_list->sLink = wp_link->Link;
        l_list->sDescription = wp_link->DescrText;
        pCTP->TextPointHyperLinkList.insert(pCTP->TextPointHyperLinkList.end(), l_list);
        
        bool added = false;
        
        if(m_bODCreateTextPoint) added = (*m_pODCreateTextPoint)(pCTP);
        
//        pPoint->m_HyperlinkList = new Plugin_HyperlinkList;
//        pPoint->m_HyperlinkList->Insert(link);
        
        return added;
    }
}

bool squiddio_pi::HidePOI(Poi * wp) {
    if(g_OCPN)
        return DeleteSingleWaypoint(wp->m_GUID);
    else {
        DeleteTextPoint_t * pDTP = new DeleteTextPoint_t;
        pDTP->GUID = wp->m_GUID;
        bool deleted = false;
        if(m_pODDeleteTextPoint && g_bODAvailable)
            deleted = (*m_pODDeleteTextPoint)(pDTP);
        return deleted;
    }
        
}

void squiddio_pi::UpdateAuiStatus(void) {
    //    This method is called after the PlugIn is initialized
    //    and the frame has done its initial layout, possibly from a saved wxAuiManager "Perspective"
    //    It is a chance for the PlugIn to syncronize itself internally with the state of any Panes that
    //    were added to the frame in the PlugIn ctor.

    //    We use this callback here to keep the context menu selection in sync with the window state
    //SetCanvasContextMenuItemViz(m_hide_id, false);
    //SetCanvasContextMenuItemViz(m_show_id, false);

    //IsOnline(); //sets last_online boolean, resets last_online_chk time and sQuiddio options in contextual menu
    //SetLogsWindow();

}
void squiddio_pi::LateInit(void){
    SetCanvasContextMenuItemViz(m_hide_id, false);
    SetCanvasContextMenuItemViz(m_show_id, false);

//    CheckIsOnline(); //sets last_online boolean, resets last_online_chk time and sQuiddio options in contextual menu
    SetLogsWindow();
    
}
void squiddio_pi::SetCursorLatLon(double lat, double lon) {
    m_cursor_lon = lon;
    m_cursor_lat = lat;

    int lat_sector = abs((int)m_cursor_lat / 5);
    int lon_sector = abs((int)m_cursor_lon / 5);
    wxString lat_quadrant = (m_cursor_lat > 0) ? (_T("N")) : (_T("S"));
    wxString lon_quadrant = (m_cursor_lon > 0) ? (_T("E")) : (_T("W"));
    local_region = lat_quadrant << wxString::Format(wxT("%02i"), lat_sector)
            << lon_quadrant << wxString::Format(wxT("%02i"), lon_sector);

    local_sq_layer = GetLocalLayer();
    if (local_sq_layer != NULL) {
        SetCanvasContextMenuItemViz(m_hide_id,
                local_sq_layer->IsVisibleOnChart());
        SetCanvasContextMenuItemViz(m_show_id,
                !local_sq_layer->IsVisibleOnChart());
    } else {
        SetCanvasContextMenuItemViz(m_hide_id, false);
        SetCanvasContextMenuItemViz(m_show_id, false);
    }
}

void squiddio_pi::RefreshLayer()
{
    wxString layerContents;
    Layer * new_layer = NULL;
    wxString versionMajor = wxString::Format(wxT("%i"),PLUGIN_VERSION_MAJOR);
    wxString versionMinor = wxString::Format(wxT("%i"),PLUGIN_VERSION_MINOR);

    //version << wxString::Format(wxT("%i"),PLUGIN_VERSION_MINOR);

    if (CheckIsOnline())
        layerContents = DownloadLayer(
        _T("/places/download_xml_layers.xml?src=ocpn_plugin&version=")+versionMajor+versionMinor+_T("&region=")
        + m_rgn_to_dld);

    wxString gpxFilePath = layerdir;
    appendOSDirSlash(&gpxFilePath);
    gpxFilePath.Append(_T("SQ_") + m_rgn_to_dld + _T(".gpx"));

    if (layerContents.length() != 0) {
        if (layerContents.length() > 400) {
            isLayerUpdate = SaveLayer(layerContents, gpxFilePath);
            new_layer = LoadLayer(gpxFilePath, m_rgn_to_dld);
            new_layer->SetVisibleNames(false);
            RenderLayerContentsOnChart(new_layer, true);

            if (isLayerUpdate) {
                wxLogMessage( _("Local destinations have been updated") );
            }
        } else {
            wxLogMessage( _("No destinations available for the region") );
        }
    } else {
        wxLogMessage( _("Server not responding. Check your Internet connection") );
    }
}

void squiddio_pi::SwitchPointType(bool bPointType) {
    if(local_sq_layer && local_sq_layer->IsVisibleOnChart()) {
        if(g_OCPN != bPointType) {
            if(bPointType == OCPN_WAYPOINTS)
                wxLogMessage(_T("squiddio_pi: Switch from OCPN Waypoints to ODText Points"));
            else
                wxLogMessage(_T("squiddio_pi: Switch from ODText Points to OCPN Waypoints"));
            RenderLayers(true);
            g_OCPN = bPointType;
            RenderLayers();
        }
    } else
        g_OCPN = bPointType;
}

void squiddio_pi::OnContextMenuItemCallback(int id) {
    //wxLogMessage(_T("squiddio_pi: OnContextMenuCallBack()"));

    if (id == m_show_id || id == m_hide_id) {
        local_sq_layer->SetVisibleOnChart(!local_sq_layer->IsVisibleOnChart());
        RenderLayerContentsOnChart(local_sq_layer, true);
        wxLogMessage(
                _T("squiddio_pi: toggled layer: ")
                        + local_sq_layer->m_LayerName);
    } else if (id == m_update_id) {
        if (local_sq_layer != NULL) {
            // hide and delete the current layer
            local_sq_layer->SetVisibleOnChart(false);
            RenderLayerContentsOnChart(local_sq_layer, true);
            pLayerList->DeleteObject(local_sq_layer);
        }
        m_rgn_to_dld = local_region;
        RefreshLayer();
    } else if (id == m_report_id) {
        wxString url_path = _T("http://squidd.io/locations/new?lat=");
        url_path.Append(
                wxString::Format(wxT("%f"), m_cursor_lat) << _T("&lon=")
                        << wxString::Format(wxT("%f"), m_cursor_lon));
        if (!CheckIsOnline() || !wxLaunchDefaultBrowser(url_path))
            wxMessageBox( _("Could not launch default browser. Check your Internet connection") );
    }
}

wxString squiddio_pi::DownloadLayer(wxString url_path) {
    wxString res = wxEmptyString;
    //size_t result = http.Get( response, _T("https://squidd.io") + url_path );
    //size_t result = http.Get( response, _T("http://squidd.io") + url_path );

    wxString fn = wxFileName::CreateTempFileName( _T("squiddio_pi") );
    _OCPN_DLStatus result = OCPN_downloadFile( _T("http://squidd.io") + url_path, fn, _("Downloading"), _("Downloading: "), wxNullBitmap, m_parent_window, OCPN_DLDS_ELAPSED_TIME|OCPN_DLDS_AUTO_CLOSE|OCPN_DLDS_SIZE|OCPN_DLDS_SPEED|OCPN_DLDS_REMAINING_TIME, 10 );


    if( result == OCPN_DL_NO_ERROR )
    {
        wxFile f( fn );
        f.ReadAll( &res );
        f.Close();
        wxRemoveFile( fn );
    }
    else
    {
        wxLogMessage(_("Squiddio_pi: unable to connect to host"));
    }
    
    return res;
}

bool squiddio_pi::SaveLayer(wxString layerStr, wxString file_path) {
    // write file to the squiddio directory
    bool isUpdate = wxFile::Exists(file_path);
    wxFile gpxFile;

    if (isUpdate)
        wxLogMessage(_T("squiddio_pi: updating existing layer file"));
    else
        wxLogMessage(_T("squiddio_pi: creating new layer file"));

    if (gpxFile.Create(file_path, true)) {
        gpxFile.Write(layerStr);
        gpxFile.Close();
    } else
        wxLogMessage(_T("squiddio_pi: unable to create layer file"));
    return isUpdate;
}

bool squiddio_pi::CheckIsOnline()
{
    return OCPN_isOnline();
}

Layer * squiddio_pi::GetLocalLayer() {
    LayerList::iterator it;
    int index = 0;
    wxString layer_name = _T("SQ_") + local_region;
    Layer *lay = NULL;

    for (it = (*pLayerList).begin(); it != (*pLayerList).end(); ++it, ++index) {
        Layer * l = (Layer *) (*it);
        if (l->m_LayerName == layer_name) {
            lay = l;
            break;
        }
    }
    return lay;
}

void squiddio_pi::appendOSDirSlash(wxString* pString) {
    wxChar sep = wxFileName::GetPathSeparator();
    if (pString->Last() != sep)
        pString->Append(sep);
}

int squiddio_pi::GetAPIVersionMajor() {
    return MY_API_VERSION_MAJOR;
}

int squiddio_pi::GetAPIVersionMinor() {
    return MY_API_VERSION_MINOR;
}

int squiddio_pi::GetPlugInVersionMajor() {
    return PLUGIN_VERSION_MAJOR;
}

int squiddio_pi::GetPlugInVersionMinor() {
    return PLUGIN_VERSION_MINOR;
}
wxBitmap *squiddio_pi::GetPlugInBitmap() {
    return _img_plugin_logo;
}

wxString squiddio_pi::GetCommonName() {
    return _("Squiddio");
}

wxString squiddio_pi::GetShortDescription() {
    return _("Squiddio for OpenCPN");
}

wxString squiddio_pi::GetLongDescription() {
    return _(
"== User-sourced database of sailing destinations ==\n\
To download destinations for a desired region (requires Internet connection):\n\
* Position cursor on area where you want to view destinations and right click mouse\n\
* Select 'Download local sQuiddio destinations' from context-sensitive menu.\n\n\
Destinations appear as OpenCPN waypoints: \n\
* Right-click on waypoint for link to sQuiddio's destination page. \n\
* Follow link to rate destination and add comments online.\n\n\
Other menu options: \n\
* Toggle visibility for local destinations on/off \n\
* Submit a new destination (requires Internet connection and free user account)\n\
\n== In-chart log-sharing for cruisers ==\n\
* Share your GPS coordinates with your cruising friends and visualize their position\n\
on your OpenCPN charts (requires a free sQuiddio account)\n\n\
IMPORTANT: By using this plugin you are agreeing to the sQuidd.io Terms \n\
and Conditions, available at http://squidd.io/enduser_agreement");
}

bool squiddio_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp) {
    return false;
}

bool squiddio_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp) {
    return false;

}
int squiddio_pi::GetToolbarToolCount(void) {
    return 1;
}

void squiddio_pi::PreferencesDialog(wxWindow* parent) {
    {
        SquiddioPrefsDialog * dialog = new SquiddioPrefsDialog(*this,
                m_parent_window);
        dialog->m_pfdDialog = NULL;
        if (g_ViewMarinas && g_ViewAnchorages == true
                && g_ViewYachtClubs == true && g_ViewDocks == true
                && g_ViewRamps == true && g_ViewFuelStations == true
                && g_ViewOthers == true) {
            dialog->m_checkBoxAll->SetValue(true);
            dialog->m_checkBoxMarinas->Enable(false);
            dialog->m_checkBoxAnchorages->Enable(false);
            dialog->m_checkBoxYachtClubs->Enable(false);
            dialog->m_checkBoxDocks->Enable(false);
            dialog->m_checkBoxRamps->Enable(false);
            dialog->m_checkBoxFuelStations->Enable(false);
            dialog->m_checkBoxOthers->Enable(false);

        } else {
            dialog->m_checkBoxAll->SetValue(false);
        }

        dialog->m_choiceHowOften->SetSelection(g_PostPeriod);
        dialog->m_choiceReceive->SetSelection(g_RetrievePeriod);
        dialog->m_textSquiddioID->SetValue(g_Email);
        dialog->m_textApiKey->SetValue(g_ApiKey);
        if(g_OCPN) dialog->m_radioBoxOCPNorOD->SetSelection(0);
        else dialog->m_radioBoxOCPNorOD->SetSelection(1);
        dialog->m_checkBoxMarinas->SetValue(g_ViewMarinas);
        dialog->m_checkBoxAnchorages->SetValue(g_ViewAnchorages);
        dialog->m_checkBoxYachtClubs->SetValue(g_ViewYachtClubs);
        dialog->m_checkBoxDocks->SetValue(g_ViewDocks);
        dialog->m_checkBoxRamps->SetValue(g_ViewRamps);
        dialog->m_checkBoxFuelStations->SetValue(g_ViewFuelStations);
        dialog->m_checkBoxOthers->SetValue(g_ViewOthers);
        dialog->m_checkBoxAIS->SetValue(g_ViewAIS);
        dialog->m_checkBoxShowName->SetValue(g_bODTextPointShowName);
        dialog->m_choicePosition->SetSelection(g_iODTextPointTextPosition);
        dialog->m_colourPickerText->SetColour(g_colourODDefaultTextColour);
        dialog->m_colourPickerBackgroundColour->SetColour(g_colourODDefaultTextBackgroundColour);
        dialog->m_sliderBackgroundTransparency->SetValue(g_iODTextBackgroundTransparency);
        dialog->m_staticTextFontFaceExample->SetFont(g_fontODDisplayTextFont);
        dialog->m_radioBoxShowDisplayText->SetSelection(g_iTextPointDisplayTextWhen);
        
        
        if (g_PostPeriod > 0 || g_RetrievePeriod > 0) {
            dialog->m_textSquiddioID->Enable(true);
            dialog->m_textApiKey->Enable(true);
        }

        int curr_retrieve_period = g_RetrievePeriod;
        
        if(!m_bDoneODAPIVersionCall) {
            GetODAPI();
            AddODIcons();
        }
        
        if(m_bDoneODAPIVersionCall) {
            if(!m_bODAPIOK) {
                wxString sMsg;
                if(!m_bODAPIMessageShown) {
                    m_bODAPIMessageShown = true;
                    sMsg.Printf(_("OD Text Points not available, wrong version of API\nSquiddio API Major: %i, Minor %i, OD API Major: %i, Minor %i"), ODAPI_VERSION_MAJOR, ODAPI_VERSION_MINOR, m_iODAPIVersionMajor, m_iODAPIVersionMinor);
                    wxMessageBox(sMsg, wxMessageBoxCaptionStr, wxOK | wxCENTRE | wxSTAY_ON_TOP);
                }
                sMsg.Printf(_T("squiddio_pi: OD Text Points cannot be used, wrong version of API. Squiddio API Major: %i, Minor %i, OD API Major: %i, Minor %i"), ODAPI_VERSION_MAJOR, ODAPI_VERSION_MINOR, m_iODAPIVersionMajor, m_iODAPIVersionMinor);
                wxLogMessage(sMsg);
                dialog->m_radioBoxOCPNorOD->SetSelection(0);
                dialog->m_radioBoxOCPNorOD->Disable();
            } else
                dialog->m_radioBoxOCPNorOD->Enable();
        } else{
            dialog->m_radioBoxOCPNorOD->SetSelection(0);
            dialog->m_radioBoxOCPNorOD->Disable();
        }
        
        dialog->m_fgMainSizer->SetMinSize(dialog->m_fgSubSizer->ComputeFittingClientSize(dialog));
        dialog->SetMinClientSize(dialog->m_fgMainSizer->ComputeFittingClientSize(dialog));
        dialog->GetSizer()->Fit(dialog);
        dialog->Layout();

        wxColour cl;
        GetGlobalColor(_T("DILG1"), &cl);
        dialog->SetBackgroundColour(cl);
        
        if (dialog->ShowModal() == wxOK) {
            g_PostPeriod = dialog->m_choiceHowOften->GetSelection();
            g_RetrievePeriod = dialog->m_choiceReceive->GetSelection();
            g_Email = dialog->m_textSquiddioID->GetValue().Trim();
            g_ApiKey = dialog->m_textApiKey->GetValue().Trim();
            g_ViewMarinas = dialog->m_checkBoxMarinas->GetValue();
            g_ViewAnchorages = dialog->m_checkBoxAnchorages->GetValue();
            g_ViewYachtClubs = dialog->m_checkBoxYachtClubs->GetValue();
            g_ViewDocks = dialog->m_checkBoxDocks->GetValue();
            g_ViewRamps = dialog->m_checkBoxRamps->GetValue();
            g_ViewFuelStations = dialog->m_checkBoxFuelStations->GetValue();
            g_ViewOthers = dialog->m_checkBoxOthers->GetValue();
            g_ViewAIS = dialog->m_checkBoxAIS->GetValue();
            g_bODTextPointShowName = dialog->m_checkBoxShowName->GetValue();
            g_iODTextPointTextPosition = dialog->m_choicePosition->GetSelection();
            g_colourODDefaultTextColour = dialog->m_colourPickerText->GetColour();
            g_colourODDefaultTextBackgroundColour = dialog->m_colourPickerBackgroundColour->GetColour();
            g_iODTextBackgroundTransparency = dialog->m_sliderBackgroundTransparency->GetValue();
            g_fontODDisplayTextFont = dialog->m_staticTextFontFaceExample->GetFont();
            g_iTextPointDisplayTextWhen = dialog->m_radioBoxShowDisplayText->GetSelection();
            
            if ((g_RetrievePeriod > 0 || g_PostPeriod > 0) && (g_Email.Length() == 0 || g_ApiKey.Length() == 0))
            {
                wxMessageBox(_("Log sharing was not activated. Please enter your sQuiddio user ID and API Key. \n\nTo obtain your API Key, sign up for sQuiddio (http://squidd.io/signup) and visit your online profile page (see Edit Profile link in the Dashboard), 'Numbers & Keys' tab."));
                g_RetrievePeriod=0;
                g_PostPeriod    =0;
            }

            SetLogsWindow();

            if (m_plogs_window) {
                if (g_RetrievePeriod != curr_retrieve_period){
                    if (g_RetrievePeriod > 0){
                        m_plogs_window->SetTimer(period_secs(g_RetrievePeriod));
                    }else{
                        m_plogs_window->SetTimer(0);
                    }
                }
                m_plogs_window->m_ErrorCondition = wxEmptyString;
            }

            Layer * l;
            LayerList::iterator it;
            for (it = (*pLayerList).begin(); it != (*pLayerList).end(); ++it ) {
                l = (Layer *) (*it);
                if (l->m_LayerName.Contains(_T("logs")))
                    l->m_bIsVisibleOnChart = g_RetrievePeriod > 0;
            }

            SaveConfig();

            if(dialog->m_radioBoxOCPNorOD->GetSelection() == 0) SwitchPointType(OCPN_WAYPOINTS);
            else SwitchPointType(OD_TEXTPOINTS);
        }
        dialog->Destroy();
        delete dialog;
    }
}
void squiddio_pi::ShowPreferencesDialog(wxWindow* parent) {
    PreferencesDialog(parent);
}

void squiddio_pi::SetLogsWindow() {
    if (g_Email.Length() > 0 && g_ApiKey.Length() > 0
            && (g_PostPeriod > 0 || g_RetrievePeriod > 0)) {
        // auth info available and either log type requested: open status window
        if (!m_plogs_window) {
            // open window if not yet open

            m_plogs_window = new logsWindow(this, m_parent_window, wxID_ANY);

            m_AUImgr = GetFrameAuiManager();
            m_AUImgr->AddPane(m_plogs_window);
            m_AUImgr->GetPane(m_plogs_window).Name(_T("Demo Window Name"));
            m_AUImgr->GetPane(m_plogs_window).Float();
            m_AUImgr->GetPane(m_plogs_window).FloatingPosition(300, 600);
            m_AUImgr->GetPane(m_plogs_window).Caption(_T("sQuiddio log updates"));
            m_AUImgr->GetPane(m_plogs_window).CaptionVisible(false);
            m_AUImgr->GetPane(m_plogs_window).GripperTop(false);
            m_AUImgr->GetPane(m_plogs_window).CloseButton(false);
            m_AUImgr->GetPane(m_plogs_window).MinimizeButton(true); //doesn't seem to work https://www.kirix.com/forums/viewtopic.php?f=15&t=658
        }
        // now make it visible
        m_AUImgr->GetPane(m_plogs_window).Show(true);
        m_AUImgr->Update();

    } else if (m_plogs_window) {
        // log updates no longer requested: hide status window and stop timer

        m_AUImgr->GetPane(m_plogs_window).Show(false);
        m_AUImgr->Update();
    }
}

void squiddio_pi::OnToolbarToolCallback(int id) {
    PreferencesDialog(m_parent_window);
}
void squiddio_pi::SetPluginMessage(wxString &message_id, wxString &message_body) {
    g_psJSON->ProcessMessage(message_id, message_body);
    return;
}
void squiddio_pi::SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix) {

}
void squiddio_pi::SetNMEASentence(wxString &sentence) {
    if (m_plogs_window && g_PostPeriod > 0 && wxDateTime::GetTimeNow() > g_LastLogSent + period_secs(g_PostPeriod))
        if (CheckIsOnline())
            m_plogs_window->SetSentence(sentence);
}

void squiddio_pi::OnThreadActionFinished(SquiddioEvent& event)
{
    //Whatever is needed after an action was performed in the background
}

void squiddio_pi::GetODAPI()
{
    wxJSONValue jMsg;
    wxJSONWriter writer;
    wxString    MsgString;
    
    jMsg[wxT("Source")] = wxT("SQUIDDIO_PI");
    jMsg[wxT("Type")] = wxT("Request");
    jMsg[wxT("Msg")] = wxT("Version");
    jMsg[wxT("MsgId")] = wxT("Version");
    writer.Write( jMsg, MsgString );
    SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
    if(g_ReceivedODAPIMessage == wxEmptyString) return;
    
    if(g_ReceivedODAPIMessage != wxEmptyString &&  g_ReceivedODAPIJSONMsg[wxT("MsgId")].AsString() == wxS("Version")) {
        m_iODVersionMajor = g_ReceivedODAPIJSONMsg[wxS("Major")].AsInt();
        m_iODVersionMinor = g_ReceivedODAPIJSONMsg[wxS("Minor")].AsInt();
        m_iODVersionPatch = g_ReceivedODAPIJSONMsg[wxS("Patch")].AsInt();
        m_bDoneODVersionCall = true;
    } else {
        m_iODVersionMajor = -1;
        m_iODVersionMinor = -1;
        m_iODVersionPatch = -1;
    }
    
    if(m_bDoneODVersionCall) {
        wxJSONValue jMsg1;
        jMsg1[wxT("Source")] = wxT("SQUIDDIO_PI");
        jMsg1[wxT("Type")] = wxT("Request");
        jMsg1[wxT("Msg")] = wxS("GetAPIAddresses");
        jMsg1[wxT("MsgId")] = wxS("GetAPIAddresses");
        writer.Write( jMsg1, MsgString );
        SendPluginMessage( wxS("OCPN_DRAW_PI"), MsgString );
        if(g_ReceivedODAPIMessage != wxEmptyString &&  g_ReceivedODAPIJSONMsg[wxT("MsgId")].AsString() == wxS("GetAPIAddresses")) {
            m_bDoneODAPIVersionCall = true;
            
            m_iODAPIVersionMajor = g_ReceivedODAPIJSONMsg[_T("ODAPIVersionMajor")].AsInt();
            m_iODAPIVersionMinor = g_ReceivedODAPIJSONMsg[_T("ODAPIVersionMinor")].AsInt();
            if(m_iODAPIVersionMajor == ODAPI_VERSION_MAJOR && m_iODAPIVersionMinor == ODAPI_VERSION_MINOR ) m_bODAPIOK = true;
            else g_OCPN = OCPN_WAYPOINTS;
            
            wxString sptr = g_ReceivedODAPIJSONMsg[_T("OD_FindPointInAnyBoundary")].AsString();
            if(sptr != _T("null")) {
                sscanf(sptr.To8BitData().data(), "%p", &m_pOD_FindPointInAnyBoundary);
                m_bODFindPointInAnyBoundary = true;
            }
            sptr = g_ReceivedODAPIJSONMsg[_T("OD_FindClosestBoundaryLineCrossing")].AsString();
            if(sptr != _T("null")) {
                sscanf(sptr.To8BitData().data(), "%p", &m_pODFindClosestBoundaryLineCrossing);
                m_bODFindClosestBoundaryLineCrossing = true;
            }
            sptr = g_ReceivedODAPIJSONMsg[_T("OD_FindFirstBoundaryLineCrossing")].AsString();
            if(sptr != _T("null")) {
                sscanf(sptr.To8BitData().data(), "%p", &m_pODFindFirstBoundaryLineCrossing);
                m_bODFindFirstBoundaryLineCrossing = true;
            }
            sptr = g_ReceivedODAPIJSONMsg[_T("OD_CreateBoundary")].AsString();
            if(sptr != _T("null")) {
                sscanf(sptr.To8BitData().data(), "%p", &m_pODCreateBoundary);
                m_bODCreateBoundary = true;
            }
            sptr = g_ReceivedODAPIJSONMsg[_T("OD_CreateBoundaryPoint")].AsString();
            if(sptr != _T("null")) {
                sscanf(sptr.To8BitData().data(), "%p", &m_pODCreateBoundaryPoint);
                m_bODCreateBoundaryPoint = true;
            }
            sptr = g_ReceivedODAPIJSONMsg[_T("OD_CreateTextPoint")].AsString();
            if(sptr != _T("null")) {
                sscanf(sptr.To8BitData().data(), "%p", &m_pODCreateTextPoint);
                m_bODCreateTextPoint = true;
            }
            sptr = g_ReceivedODAPIJSONMsg[_T("OD_DeleteTextPoint")].AsString();
            if(sptr != _T("null")) {
                sscanf(sptr.To8BitData().data(), "%p", &m_pODDeleteTextPoint);
                m_bODDeleteTextPoint = true;
            }
            sptr = g_ReceivedODAPIJSONMsg[_T("OD_AddPointIcon")].AsString();
            if(sptr != _T("null")) {
                sscanf(sptr.To8BitData().data(), "%p", &m_pODAddPointIcon);
                m_bODAddPointIcon = true;
            }
            sptr = g_ReceivedODAPIJSONMsg[_T("OD_DeletePointIcon")].AsString();
            if(sptr != _T("null")) {
                sscanf(sptr.To8BitData().data(), "%p", &m_pODDeletePointIcon);
                m_bODDeletePointIcon = true;
            }
        }
    }
    
#ifdef _DEBUG
    DEBUGST("OD Version: Major: ");
    DEBUGCONT(m_iODVersionMajor);
    DEBUGCONT(", Minor: ");
    DEBUGCONT(m_iODVersionMinor);
    DEBUGCONT(", Patch: ");
    DEBUGEND(m_iODVersionPatch);
    DEBUGST("ODAPI Version: Major: ");
    DEBUGCONT(m_iODAPIVersionMajor);
    DEBUGCONT(", Minor: ");
    DEBUGEND(m_iODAPIVersionMinor);
    
    wxString l_avail;
    wxString l_notavail;
    if(m_bODFindPointInAnyBoundary) l_avail.Append(_("OD_FindPointInAnyBoundary\n"));
    if(m_bODFindClosestBoundaryLineCrossing) l_avail.Append(_("OD_FindClosestBoundaryLineCrossing\n"));
    if(m_bODFindFirstBoundaryLineCrossing) l_avail.Append(_("OD_FindFirstBoundaryLineCrossing\n"));
    if(m_bODCreateBoundary) l_avail.Append(_("OD_CreateBoundary\n"));
    if(m_bODCreateBoundaryPoint) l_avail.Append(_("OD_CreateBoundaryPoint\n"));
    if(m_bODCreateTextPoint) l_avail.Append(_("OD_CreateTextPoint\n"));
    if(m_bODDeleteTextPoint) l_avail.Append(_("OD_DeleteTextPoint\n"));
    if(m_bODAddPointIcon) l_avail.Append(_("OD_AddPointIcon\n"));
    if(m_bODDeletePointIcon) l_avail.Append(_("OD_DeletePointIcon\n"));
    DEBUGST("The following ODAPI's are available:\n");
    DEBUGEND(l_avail);
    
    if(!m_bODFindPointInAnyBoundary) l_notavail.Append(_("OD_FindPointInAnyBoundary\n"));
    if(!m_bODFindClosestBoundaryLineCrossing) l_notavail.Append(_("OD_FindClosestBoundaryLineCrossing\n"));
    if(!m_bODFindFirstBoundaryLineCrossing) l_notavail.Append(_("OD_FindFirstBoundaryLineCrossing\n"));
    if(!m_bODCreateBoundary) l_notavail.Append(_("OD_CreateBoundary\n"));
    if(!m_bODCreateBoundaryPoint) l_notavail.Append(_("OD_CreateBoundaryPoint\n"));
    if(!m_bODCreateTextPoint) l_notavail.Append(_("OD_CreateTextPoint\n"));
    if(!m_bODDeleteTextPoint) l_notavail.Append(_("OD_DeleteTextPoint\n"));
    if(m_bODAddPointIcon) l_notavail.Append(_("OD_AddPointIcon\n"));
    if(m_bODDeletePointIcon) l_notavail.Append(_("OD_DeletePointIcon\n"));
    DEBUGST("The following ODAPI's are not available:\n");
    DEBUGEND(l_notavail);
#endif    
}

void squiddio_pi::ResetODAPI()
{
    m_bDoneODVersionCall = false;
    m_bDoneODAPIVersionCall = false;
    m_bODAPIOK = false;
    m_iODAPIVersionMajor = 0;
    m_iODAPIVersionMinor = 0;
    m_iODVersionMinor = 0;
    m_iODVersionPatch = 0;
    g_squiddio_pi->m_bODFindPointInAnyBoundary = false;
    g_squiddio_pi->m_bODFindClosestBoundaryLineCrossing = false;
    g_squiddio_pi->m_bODFindFirstBoundaryLineCrossing = false;
    g_squiddio_pi->m_bODCreateBoundary = false;
    g_squiddio_pi->m_bODCreateBoundaryPoint = false;
    g_squiddio_pi->m_bODCreateTextPoint = false;
    g_squiddio_pi->m_bODDeleteTextPoint = false;
    g_squiddio_pi->m_bODAddPointIcon = false;
    g_squiddio_pi->m_bODDeletePointIcon = false;
}

void squiddio_pi::AddODIcons()
{
    if(!m_bODAddPointIcon || !m_bODAPIOK) return;
    
    AddPointIcon_t *pAPI = new AddPointIcon_t;
    pAPI->PointIcon = *_img_marina_grn;
    pAPI->PointIconName = _T("marina_grn");
    pAPI->PointIconDescription = _("Marina");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_anchor_blu;
    pAPI->PointIconName = _T("aton_gry");
    pAPI->PointIconDescription = _("Anchorage/Buoys");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_aton_gry;
    pAPI->PointIconName = _T("aton_gry"); 
    pAPI->PointIconDescription = _("AIS ATON Marker");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_club_pur;
    pAPI->PointIconName = _T("club_pur"); 
    pAPI->PointIconDescription = _("Yacht Club");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_fuelpump_red;
    pAPI->PointIconName = _T("fuelpump_red"); 
    pAPI->PointIconDescription = _("Fuel Station");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_pier_yel;
    pAPI->PointIconName = _T("pier_yel"); 
    pAPI->PointIconDescription = _("Dock/Pier");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_ramp_azu;
    pAPI->PointIconName = _T("ramp_azu"); 
    pAPI->PointIconDescription = _("Boat Ramp");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_logimg_N;
    pAPI->PointIconName = _T("logimg_N"); 
    pAPI->PointIconDescription = _("North");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_logimg_NE;
    pAPI->PointIconName = _T("logimg_NE"); 
    pAPI->PointIconDescription = _("North East");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_logimg_E;
    pAPI->PointIconName = _T("logimg_E"); 
    pAPI->PointIconDescription = _("East");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_logimg_SE;
    pAPI->PointIconName = _T("logimg_SE"); 
    pAPI->PointIconDescription = _("South East");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_logimg_S;
    pAPI->PointIconName = _T("logimg_S"); 
    pAPI->PointIconDescription = _("Sout");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_logimg_SW;
    pAPI->PointIconName = _T("logimg_SW"); 
    pAPI->PointIconDescription = _("South West");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_logimg_W;
    pAPI->PointIconName = _T("logimg_W"); 
    pAPI->PointIconDescription = _("West");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_logimg_NW;
    pAPI->PointIconName = _T("logimg_NW"); 
    pAPI->PointIconDescription = _("North West");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_logimg_C;
    pAPI->PointIconName = _T("logimg_C"); 
    pAPI->PointIconDescription = _("Checked in");
    m_pODAddPointIcon(pAPI);
    pAPI->PointIcon = *_img_logimg_U;
    pAPI->PointIconName = _T("logimg_U"); 
    pAPI->PointIconDescription = _("Unknown heading");
    m_pODAddPointIcon(pAPI);
}


//---------------------------------------------- preferences dialog event handlers
void SquiddioPrefsDialog::OnCheckBoxAll(wxCommandEvent& event) {
    wxCheckBox *checkbox = (wxCheckBox*) event.GetEventObject();
    if (checkbox->IsChecked()) {
        m_checkBoxMarinas->SetValue(true);
        m_checkBoxAnchorages->SetValue(true);
        m_checkBoxYachtClubs->SetValue(true);
        m_checkBoxDocks->SetValue(true);
        m_checkBoxRamps->SetValue(true);
        m_checkBoxFuelStations->SetValue(true);
        m_checkBoxOthers->SetValue(true);

        m_checkBoxMarinas->Enable(false);
        m_checkBoxAnchorages->Enable(false);
        m_checkBoxYachtClubs->Enable(false);
        m_checkBoxDocks->Enable(false);
        m_checkBoxRamps->Enable(false);
        m_checkBoxFuelStations->Enable(false);
        m_checkBoxOthers->Enable(false);
    } else {
        m_checkBoxMarinas->Enable(true);
        m_checkBoxAnchorages->Enable(true);
        m_checkBoxYachtClubs->Enable(true);
        m_checkBoxDocks->Enable(true);
        m_checkBoxRamps->Enable(true);
        m_checkBoxFuelStations->Enable(true);
        m_checkBoxOthers->Enable(true);
    }
}

void SquiddioPrefsDialog::LaunchHelpPage(wxCommandEvent& event) {
    if (!wxLaunchDefaultBrowser(_T("http://squidd.io/faq#opencpn_setup")))
        wxMessageBox(
                _("Could not launch default browser. Check your Internet connection"));
    event.Skip();
}
void SquiddioPrefsDialog::OnShareChoice(wxCommandEvent& event) {
    if (m_choiceHowOften->GetSelection() == 0
            && m_choiceReceive->GetSelection() == 0) {
        m_textSquiddioID->Enable(false);
        m_textApiKey->Enable(false);
    } else {
        m_textSquiddioID->Enable(true);
        m_textApiKey->Enable(true);
    }
    Refresh(false);
}

void SquiddioPrefsDialog::OnButtonClickFonts( wxCommandEvent& event )
{
    if(m_pfdDialog) delete m_pfdDialog;
    
    wxFontData l_FontData;
    l_FontData.SetInitialFont( m_sq_pi.g_fontODDisplayTextFont );
    m_pfdDialog = new wxFontDialog( this, l_FontData );
    m_pfdDialog->Centre( wxBOTH );
    
    DimeWindow(m_pfdDialog);
    int iRet = m_pfdDialog->ShowModal();
    if(iRet == wxID_OK) {
        m_staticTextFontFaceExample->SetFont(m_pfdDialog->GetFontData().GetChosenFont());
        m_fgSizerODSettings->RecalcSizes();
        SendSizeEvent();
    }
}


