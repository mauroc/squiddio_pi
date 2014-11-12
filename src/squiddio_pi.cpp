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

#include <wx/aui/aui.h>
#include "squiddio_pi.h"

WX_DEFINE_LIST(LayerList);
WX_DEFINE_LIST(HyperlinkList );
WX_DEFINE_LIST(Plugin_HyperlinkList);

#define ONLINE_CHECK_RETRY 10

// the class factories, used to create and destroy instances of the PlugIn
//

// these variables are shared with NavObjectCollection
PoiMan *pPoiMan;
//squiddio_pi * p_sqpi;

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    //squiddio_pi * p_sqpi = new squiddio_pi(ppimgr);
    return new squiddio_pi(ppimgr);
    //return p_sqpi;
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

squiddio_pi::squiddio_pi( void *ppimgr )
    :opencpn_plugin_110( ppimgr ) // constructor initialization
{
    // Create the PlugIn icons
    initialize_images();
}

squiddio_pi::~squiddio_pi( void )
{
    delete _img_marina_grn;
    delete _img_anchor_blu;
    delete _img_club_pur ;
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
    //      printf("squiddio_pi Init()\n");
    wxLogMessage( _T("squiddio_pi: Init()") );

    m_pdemo_window = NULL;

    // Get a pointer to the opencpn display canvas, to use as a parent for windows created
    m_parent_window = GetOCPNCanvasWindow();
    
    last_online_chk = 0;

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


    m_pdemo_window = new demoWindow(m_parent_window, wxID_ANY);

    m_AUImgr = GetFrameAuiManager();
    m_AUImgr->AddPane(m_pdemo_window);
    m_AUImgr->GetPane(m_pdemo_window).Name(_T("Demo Window Name"));

    m_AUImgr->GetPane(m_pdemo_window).Float();
    m_AUImgr->GetPane(m_pdemo_window).FloatingPosition(300,30);

    m_AUImgr->GetPane(m_pdemo_window).Caption(_T("sQuiddio latest updates"));
    m_AUImgr->GetPane(m_pdemo_window).CaptionVisible(false);
    m_AUImgr->GetPane(m_pdemo_window).GripperTop(false);
    m_AUImgr->GetPane(m_pdemo_window).CloseButton(false);
    m_AUImgr->GetPane(m_pdemo_window).Show(false);
    m_AUImgr->Update();


    AddCustomWaypointIcon(_img_marina_grn,  _T("marina_grn"),   _T("Marina"));
    AddCustomWaypointIcon(_img_anchor_blu,  _T("anchor_blu"),   _T("Anchorage/Buoys"));
    AddCustomWaypointIcon(_img_club_pur,    _T("club_pur"),     _T("Yacht Club"));
    AddCustomWaypointIcon(_img_fuelpump_red,_T("fuelpump_red"), _T("Fuel Station"));
    AddCustomWaypointIcon(_img_pier_yel,    _T("pier_yel"),     _T("Dock/Pier"));
    AddCustomWaypointIcon(_img_ramp_azu,    _T("ramp_azu"),     _T("Boat Ramp"));

    pLayerList  = new LayerList;
    pPoiMan     = new PoiMan;
    link        = new Plugin_Hyperlink;

    m_pconfig = GetOCPNConfigObject();
    LoadConfig();

    layerdir = *GetpPrivateApplicationDataLocation();
    layerdir += wxFileName::GetPathSeparator();
    layerdir += _T("squiddio");

    if( !wxDir::Exists( layerdir ) )
        wxFileName::Mkdir(layerdir);

    if( wxDir::Exists( layerdir ) ) {
        wxString laymsg;
        laymsg.Printf( wxT("squiddio_pi: getting .gpx layer files from: %s"), layerdir.c_str() );
        wxLogMessage( laymsg );

        LoadLayers(layerdir);

        Layer * l;
        LayerList::iterator it;
        int index = 0;
        for (it = (*pLayerList).begin(); it != (*pLayerList).end(); ++it, ++index) {
            l = (Layer *) (*it);
            l->SetVisibleNames( false );
            RenderLayerContentsOnChart(l);
        }
    }

    m_period_secs.Add(0);
    m_period_secs.Add(85400);
    m_period_secs.Add(43200);
    m_period_secs.Add(3600);
    m_period_secs.Add(1800);
    m_period_secs.Add(60);
    m_period_secs.Add(30);
    m_period_secs.Add(10);

    return (
    INSTALLS_CONTEXTMENU_ITEMS |
    WANTS_CURSOR_LATLON |
    WANTS_NMEA_SENTENCES |
    WANTS_PREFERENCES    |
    USES_AUI_MANAGER |
    WANTS_LATE_INIT
    );
}


bool squiddio_pi::DeInit(void) {
    m_AUImgr->DetachPane(m_pdemo_window);

    if (m_pdemo_window) {
        m_pdemo_window->Close();
//          m_pdemo_window->Destroy(); //Gives a Segmentation fault
    }

    RemoveCanvasContextMenuItem(m_show_id);
    RemoveCanvasContextMenuItem(m_hide_id);
    RemoveCanvasContextMenuItem(m_update_id);
    RemoveCanvasContextMenuItem(m_report_id);
    
    LayerList::iterator it;
    int index = 0;
    for (it = (*pLayerList).begin(); it != (*pLayerList).end(); ++it, ++index) {
        Layer * l = (Layer *) (*it);
        l->SetVisibleOnChart( false );
        RenderLayerContentsOnChart( l, false );
        pLayerList->DeleteObject( l );
    }
    SaveConfig();
    RequestRefresh(m_parent_window);
    
    delete pLayerList;
    delete pPoiMan;
    delete link;
    delete m_pdemo_window;
    return true;
}
bool squiddio_pi::LoadConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(!pConf)
        return false;

    pConf->SetPath ( _T( "/PlugIns/libsquiddio_pi.so" ) );
    pConf->Read( _T ( "VisibleLayers" ), &g_VisibleLayers );
    pConf->Read( _T ( "InvisibleLayers" ), &g_InvisibleLayers );
    pConf->Read( _T ( "PostPeriod" ), &g_PostPeriod );
    pConf->Read( _T ( "LastUpdate" ), &g_LastUpdate);
    pConf->Read( _T ( "Email" ), &g_Email);
    pConf->Read( _T ( "ApiKey" ), &g_ApiKey);
    pConf->Read( _T ( "ViewMarinas" ), &g_ViewMarinas, true);
    pConf->Read( _T ( "ViewAnchorages" ), &g_ViewAnchorages,true);
    pConf->Read( _T ( "ViewDocks" ), &g_ViewDocks, true);
    pConf->Read( _T ( "ViewYachtClubs" ), &g_ViewYachtClubs,true);
    pConf->Read( _T ( "ViewFuelStations" ), &g_ViewFuelStations,true);
    pConf->Read( _T ( "ViewRamps" ), &g_ViewRamps,true);
    pConf->Read( _T ( "ViewOthers" ), &g_ViewOthers,true);
    return true;
}

bool squiddio_pi::SaveConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(!pConf)
        return false;

    pConf->SetPath ( _T( "/PlugIns/libsquiddio_pi.so" ) );
    pConf->Write ( _T ( "VisibleLayers" ), g_VisibleLayers );
    pConf->Write ( _T ( "InvisibleLayers" ), g_InvisibleLayers );
    pConf->Write ( _T ( "PostPeriod" ), g_PostPeriod );
    pConf->Write ( _T ( "LastUpdate" ), g_LastUpdate );
    pConf->Write( _T ( "Email" ), g_Email);
    pConf->Write( _T ( "ApiKey" ),g_ApiKey);
    pConf->Write( _T ( "ViewMarinas" ),     g_ViewMarinas);
    pConf->Write( _T ( "ViewAnchorages" ),  g_ViewAnchorages);
    pConf->Write( _T ( "ViewDocks" ),       g_ViewDocks);
    pConf->Write( _T ( "ViewYachtClubs" ),  g_ViewYachtClubs);
    pConf->Write( _T ( "ViewRamps" ),       g_ViewRamps);
    pConf->Write( _T ( "ViewFuelStations" ),   g_ViewFuelStations);
    pConf->Write( _T ( "ViewOthers" ),      g_ViewOthers);
    return true;
}

bool squiddio_pi::LoadLayers(wxString &path)
{
    wxArrayString file_array;
    wxDir dir;
    Layer *l;
    dir.Open( path );
    if( dir.IsOpened() ) {
        wxString filename;
        bool cont = dir.GetFirst( &filename );
        while( cont ) {
            file_array.Clear();
            filename.Prepend( wxFileName::GetPathSeparator() );
            filename.Prepend( path );
            wxFileName f( filename );
            size_t nfiles = 0;
            if( f.GetExt().IsSameAs( wxT("gpx") ) )
                file_array.Add( filename); // single-gpx-file layer
            else{
                wxDir dir( filename );
                if( dir.IsOpened() ){
                    nfiles = dir.GetAllFiles( filename, &file_array, wxT("*.gpx") );      // layers subdirectory set
                }
            }

            if( file_array.GetCount() ){
                l = new Layer();
                l->m_LayerID = ++g_LayerIdx;
                l->m_LayerFileName = file_array[0];
                if( file_array.GetCount() <= 1 )
                    wxFileName::SplitPath( file_array[0], NULL, NULL, &( l->m_LayerName ), NULL, NULL );
                else
                    wxFileName::SplitPath( filename, NULL, NULL, &( l->m_LayerName ), NULL, NULL );

                bool bLayerViz = false;

                if( g_VisibleLayers.Contains( l->m_LayerName ) )
                    bLayerViz = true;

                l->m_bIsVisibleOnChart = bLayerViz;

                wxString laymsg;
                laymsg.Printf( wxT("squiddio_pi: new layer %d: %s"), l->m_LayerID, l->m_LayerName.c_str() );
                wxLogMessage( laymsg );

                pLayerList->Insert( l );

                //  Load the entire file array as a single layer
                for( unsigned int i = 0; i < file_array.GetCount(); i++ ) {
                    wxString file_path = file_array[i];
                    if( ::wxFileExists( file_path ) ) {
                        LoadLayerItems( file_path, l ,bLayerViz);
                    }
                }
            }
            cont = dir.GetNext( &filename );
        }
    }
    return true;

}

bool squiddio_pi::LoadLayerItems(wxString &file_path, Layer *l, bool show){
    NavObjectCollection1 *pSet = new NavObjectCollection1;
    pSet->load_file(file_path.fn_str());
    long nItems = pSet->LoadAllGPXObjectsAsLayer(l->m_LayerID, show);

    l->m_NoOfItems += nItems;

    wxString objmsg;
    objmsg.Printf( wxT("squiddio_pi: loaded GPX file %s with %d items."), file_path.c_str(), nItems );
    wxLogMessage( objmsg );
    delete pSet;
    return nItems > 0;
}

Layer * squiddio_pi::LoadLayer(wxString file_path, wxString region){
    Layer * l = new Layer();
    if( ::wxFileExists( file_path ) ) {

        l->m_LayerID = ++g_LayerIdx;
        if (file_path.Contains(_T("logs.gpx"))){
            l->m_LayerName = _T("logs");
        } else {
            l->m_LayerName = _T("SQ_")+region;
        }
        l->m_LayerFileName = file_path;
        l->m_bIsVisibleOnChart = true;
        pLayerList->Insert( l );

        LoadLayerItems(file_path, l, true);
    }
    return l;
}

bool squiddio_pi::ShowType(Poi * wp){
    if      (wp->m_IconName == _T("marina_grn"))     return g_ViewMarinas;
    else if (wp->m_IconName == _T("anchor_blu"))   return g_ViewAnchorages;
    else if (wp->m_IconName == _T("club_pur"))      return g_ViewYachtClubs;
    else if (wp->m_IconName == _T("fuelpump_red"))  return g_ViewFuelStations;
    else if (wp->m_IconName == _T("pier_yel"))      return g_ViewDocks;
    else if (wp->m_IconName == _T("ramp_azu"))      return g_ViewRamps;
    else return g_ViewOthers;
}

void squiddio_pi::RenderLayers(){
    Layer * l;
    LayerList::iterator it;
    int index = 0;
    for (it = (*pLayerList).begin(); it != (*pLayerList).end(); ++it, ++index) {
        l = (Layer *) (*it);
        l->SetVisibleNames( false );
        RenderLayerContentsOnChart(l);
    }
}

void squiddio_pi::RenderLayerContentsOnChart( Layer *layer, bool save_config ){

    // Process POIs in this layer

    wxPoiListNode *node = pPoiMan->GetWaypointList()->GetFirst();

    while( node ) {
        Poi *rp = node->GetData();
        if( rp && ( rp->m_LayerID == layer->m_LayerID ) ) {
            rp->SetVisible( layer->IsVisibleOnChart() );
            rp->SetNameShown( false );
            if (layer->IsVisibleOnChart() && ShowType(rp))
                ShowPOI(rp);
            else
                HidePOI(rp);
        }
        node = node->GetNext();
    }

    if (layer->IsVisibleOnChart()){
        if (!g_VisibleLayers.Contains(layer->m_LayerName))
            g_VisibleLayers.Append(layer->m_LayerName+_T(";"));
        g_InvisibleLayers.Replace(layer->m_LayerName+_T(";"),wxEmptyString);
    } else{
        if (!g_InvisibleLayers.Contains(layer->m_LayerName))
            g_InvisibleLayers.Append(layer->m_LayerName+_T(";"));
        g_VisibleLayers.Replace(layer->m_LayerName+_T(";"),wxEmptyString);
    }
    RequestRefresh(m_parent_window);
    if (save_config)
        SaveConfig();
}
bool squiddio_pi::ShowPOI(Poi * wp){
    double lat = wp->m_lat;
    double lon = wp->m_lon;
    wxString name = wp->GetName();
    wxString m_GUID = wp->m_GUID;
    wxString m_iconname = wp->m_IconName;

    PlugIn_Waypoint * pPoint = new PlugIn_Waypoint(lat, lon, m_iconname, name, m_GUID);
    pPoint->m_MarkDescription = wp->m_MarkDescription;

    wxHyperlinkListNode *linknode = wp->m_HyperlinkList->GetFirst();
    wp_link = linknode->GetData();
    link->Link = wp_link->Link;
    link->DescrText = wp_link->DescrText;
    link->Type = wxEmptyString;

    pPoint->m_HyperlinkList = new Plugin_HyperlinkList;
    pPoint->m_HyperlinkList->Insert( link );

    bool added = AddSingleWaypoint(pPoint, false);
    return added;
}

bool squiddio_pi::HidePOI(Poi * wp){
    return DeleteSingleWaypoint( wp->m_GUID );
}

void squiddio_pi::UpdateAuiStatus(void) {
    //    This method is called after the PlugIn is initialized
    //    and the frame has done its initial layout, possibly from a saved wxAuiManager "Perspective"
    //    It is a chance for the PlugIn to syncronize itself internally with the state of any Panes that
    //    were added to the frame in the PlugIn ctor.

    //    We use this callback here to keep the context menu selection in sync with the window state

    SetCanvasContextMenuItemViz(m_hide_id, false);
    SetCanvasContextMenuItemViz(m_show_id, false);

    SetCanvasContextMenuItemViz(m_update_id, IsOnline() );
    SetCanvasContextMenuItemViz(m_report_id, IsOnline() );

    wxAuiPaneInfo &pane = m_AUImgr->GetPane(m_pdemo_window);
    pane.Show(true);
    m_AUImgr->Update();

}

void squiddio_pi::SetCursorLatLon(double lat, double lon) {
        m_cursor_lon = lon;
        m_cursor_lat = lat;

        int lat_sector = abs( m_cursor_lat/5);
        int lon_sector = abs( m_cursor_lon/5);
        wxString lat_quadrant = (m_cursor_lat > 0) ? (_T("N")) : (_T("S"));
        wxString lon_quadrant = (m_cursor_lon > 0) ? (_T("E")) : (_T("W"));
        local_region =  lat_quadrant << wxString::Format(wxT("%02i"),lat_sector)<< lon_quadrant << wxString::Format(wxT("%02i"),lon_sector);

        local_sq_layer = GetLocalLayer();
        if (local_sq_layer != NULL ){
            SetCanvasContextMenuItemViz(m_hide_id, local_sq_layer->IsVisibleOnChart() );
            SetCanvasContextMenuItemViz(m_show_id, !local_sq_layer->IsVisibleOnChart());
        }else{
            SetCanvasContextMenuItemViz(m_hide_id, false);
            SetCanvasContextMenuItemViz(m_show_id, false);
        }
}

void squiddio_pi::OnContextMenuItemCallback(int id) {
    wxLogMessage(_T("squiddio_pi: OnContextMenuCallBack()"));

    wxString request_region = local_region; // fixes the cursor's hover region at time of request so that intervening mouse movements do not alter the layer name that will be created
    Layer * request_layer = local_sq_layer; // fixes the layer at time of request so that intervening mouse movements do not alter the layer name that will be created

    if (id == m_show_id || id == m_hide_id)
    {
        request_layer->SetVisibleOnChart( !request_layer->IsVisibleOnChart() );
        RenderLayerContentsOnChart( request_layer );
        wxLogMessage(_T("squiddio_pi: toggled layer: ")+request_layer->m_LayerName);
    }
    else if (id == m_update_id )
    {
        wxString layerContents;
        Layer * new_layer = NULL;

        if (IsOnline())
            layerContents = DownloadLayer(_T("/places/download_xml_layers.xml?region=")+local_region);

        wxString gpxFilePath = layerdir;
        appendOSDirSlash( &gpxFilePath );
        gpxFilePath.Append(_T("SQ_")+request_region+_T(".gpx"));

        if (layerContents.length() != 0 ) {
            if (layerContents.length()> 400 ){
                isLayerUpdate = SaveLayer(layerContents, gpxFilePath);
                if (isLayerUpdate && request_layer != NULL ){
                    // hide and delete the current layer
                    request_layer ->SetVisibleOnChart( false );
                    RenderLayerContentsOnChart(request_layer);
                    pLayerList->DeleteObject( request_layer );
                }
                new_layer = LoadLayer(gpxFilePath, request_region);
                new_layer->SetVisibleNames( false );
                RenderLayerContentsOnChart(new_layer);

                if (isLayerUpdate){
                    wxMessageBox(_("Local destinations have been updated"));
                }
            } else {
                wxMessageBox(_("No destinations available for the region"));
            }
        } else {
            wxMessageBox(_("Server not responding. Check your Internet connection"));
        }
    } else if (id == m_report_id) {
        wxString url_path = _T("http://squidd.io/locations/new?lat=");
        url_path.Append(wxString::Format(wxT("%f"), m_cursor_lat) << _T("&lon=") << wxString::Format(wxT("%f"), m_cursor_lon));
        if (!IsOnline() || !wxLaunchDefaultBrowser(url_path))
            wxMessageBox(_("Could not launch default browser. Check your Internet connection"));
    }
}

wxString squiddio_pi::DownloadLayer(wxString url_path){
    // --------------------------------- setup http GET request
    int cnt = 0;
    wxString res;
    wxHTTP get;
    get.SetHeader(_T("Content-type"), _T("text/html; charset=utf-8"));
    get.SetTimeout(3);

    while (!get.Connect(_T("squidd.io")))
    {
        if (cnt > 10)
            break;
        wxSleep(1);
        cnt++;
    }

    wxString tmp;
    tmp.Printf( wxT("squiddio_pi: count %d."),  cnt );
    wxLogMessage(tmp);

        if (cnt <= 10)
        {
            wxApp::IsMainLoopRunning();

            //wxString url_path = _T("/places/download_xml_layers.xml?region=")+local_region;
            //wxLogMessage(url_path);

            wxInputStream *httpStream = get.GetInputStream(url_path );

    tmp.Printf( wxT("squiddio_pi: GetError %d."),  get.GetError() );
    wxLogMessage(tmp);

            if (get.GetError() == wxPROTO_NOERR)
            {
                wxStringOutputStream out_stream(&res);
                httpStream->Read(out_stream);
            }
            else
            {
                //wxMessageBox(_("Squiddio_pi: unable to connect to host"));
                wxLogMessage(_("Squiddio_pi: unable to connect to host"));
            }
            wxDELETE(httpStream);
            get.Close();

        }
        return res;
}

bool squiddio_pi::SaveLayer(wxString layerStr, wxString file_path){
    // write file to the squiddio directory
    bool isUpdate = wxFile::Exists(file_path);
    wxFile gpxFile;

    if (isUpdate)
        wxLogMessage(_T("squiddio_pi: updating existing layer file"));
    else
        wxLogMessage(_T("squiddio_pi: creating new layer file"));

    if (gpxFile.Create( file_path , true )){
           gpxFile.Write(layerStr);
           gpxFile.Close();
    }
    else
        wxLogMessage(_T("squiddio_pi: unable to create layer file"));
    return isUpdate;
}

bool squiddio_pi::IsOnline() {
    if (wxDateTime::GetTimeNow() < last_online_chk + ONLINE_CHECK_RETRY)
        return last_online;
    wxHTTP get;
    get.SetHeader(_T("Content-type"), _T("text/html; charset=utf-8"));
    get.SetTimeout(10); // 10 seconds of timeout instead of 10 minutes ...
    int cnt = 0;

    while (!get.Connect(_T("yahoo.com")))
    {
        if (cnt > 5)
            last_online = false;
            return false;
        wxSleep(1);
        cnt++;
    }
    get.Close();
    last_online = true;
    return true;
}

Layer * squiddio_pi::GetLocalLayer(){
    LayerList::iterator it;
    int index = 0;
    wxString layer_name = _T("SQ_") + local_region;
    Layer *lay = NULL;

    for (it = (*pLayerList).begin(); it != (*pLayerList).end(); ++it, ++index) {
        Layer * l = (Layer *) (*it);
        if(l->m_LayerName== layer_name){
            lay = l;
            break;
        }
    }
    return lay;
}

void squiddio_pi::appendOSDirSlash( wxString* pString ){
    wxChar sep = wxFileName::GetPathSeparator();
    if( pString->Last() != sep ) pString->Append( sep );
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
wxBitmap *squiddio_pi::GetPlugInBitmap(){
    return _img_marina_grn;
}

wxString squiddio_pi::GetCommonName() {
    return _("Squiddio");
}

wxString squiddio_pi::GetShortDescription() {
    return _("Squiddio for OpenCPN");
}

wxString squiddio_pi::GetLongDescription() {
    return _("User-sourced database of sailing destinations.\n\n\
To download destinations for a desired region (requires Internet connection):\n\
* Position cursor on area where you want to view destinations and right click mouse\n\
* Select 'Download local sQuiddio destinations' from context-sensitive menu.\n\n\
Destinations appear as OpenCPN waypoints: \n\
* Right-click on waypoint for link to sQuiddio's destination page. \n\
* Follow link to rate destination and add comments online.\n\n\
Other menu options: \n\
* Toggle visibility for local destinations on/off \n\
* Submit a new destination (requires Internet connection and free user account)");
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
void squiddio_pi::ShowPreferencesDialog(wxWindow* parent) {
    {
        SquiddioPrefsDialogBase *dialog =
            new SquiddioPrefsDialogBase( parent, wxID_ANY, _("sQuiddio Preferences"),
                                       wxPoint( m_squiddio_dialog_x, m_squiddio_dialog_y),
                                       wxDefaultSize, wxDEFAULT_DIALOG_STYLE );

        dialog->m_choiceHowOften->SetSelection(g_PostPeriod);
        dialog->m_textSquiddioID->SetValue(g_Email);
        dialog->m_textApiKey->SetValue(g_ApiKey);
        dialog->m_checkBoxMarinas->SetValue(g_ViewMarinas);
        dialog->m_checkBoxAnchorages->SetValue(g_ViewAnchorages);
        dialog->m_checkBoxYachtClubs->SetValue(g_ViewYachtClubs);
        dialog->m_checkBoxDocks->SetValue(g_ViewDocks);
        dialog->m_checkBoxRamps->SetValue(g_ViewRamps);
        dialog->m_checkBoxFuelStations->SetValue(g_ViewFuelStations);
        dialog->m_checkBoxOthers->SetValue(g_ViewOthers);

        dialog->Fit();
        wxColour cl;
        GetGlobalColor(_T("DILG1"), &cl);
        dialog->SetBackgroundColour(cl);

        if(dialog->ShowModal() == wxID_OK)
        {
            g_PostPeriod = dialog->m_choiceHowOften->GetSelection();
            g_Email = dialog->m_textSquiddioID->GetValue();
            g_ApiKey = dialog->m_textApiKey->GetValue();
            g_ViewMarinas    = dialog->m_checkBoxMarinas->GetValue();
            g_ViewAnchorages = dialog->m_checkBoxAnchorages->GetValue();
            g_ViewYachtClubs = dialog->m_checkBoxYachtClubs->GetValue();
            g_ViewDocks      = dialog->m_checkBoxDocks->GetValue();
            g_ViewRamps      = dialog->m_checkBoxRamps->GetValue();
            g_ViewFuelStations = dialog->m_checkBoxFuelStations->GetValue();
            g_ViewOthers     = dialog->m_checkBoxOthers->GetValue();

            SaveConfig();
            RenderLayers();
        }
        delete dialog;
    }
}
void squiddio_pi::OnToolbarToolCallback(int id) {

}
void squiddio_pi::SetPluginMessage(wxString &message_id,
        wxString &message_body) {

}
void squiddio_pi::SetPositionFixEx(PlugIn_Position_Fix_Ex &pfix) {

}

void squiddio_pi::SetNMEASentence(wxString &sentence) {
    wxString PostResponse;

    if (!IsOnline() || g_Email.Length() == 0 || g_ApiKey.Length() ==0 || g_PostPeriod == 0)
        return;

    if (wxDateTime::GetTimeNow() > g_LastUpdate + m_period_secs[g_PostPeriod])
    {
        m_NMEA0183 << sentence;

        bool bGoodData = false;

        if(m_NMEA0183.PreParse())
        {
              if(m_NMEA0183.LastSentenceIDReceived == _T("RMC"))
              {
                if(m_NMEA0183.Parse())
                {
                    if(m_NMEA0183.Rmc.IsDataValid == NTrue)
                    {
                      float llt = m_NMEA0183.Rmc.Position.Latitude.Latitude;
                      int lat_deg_int = (int)(llt / 100);
                      float lat_deg = lat_deg_int;
                      float lat_min = llt - (lat_deg * 100);
                      mLat = lat_deg + (lat_min/60.);
                      if(m_NMEA0183.Rmc.Position.Latitude.Northing == South)
                            mLat = -mLat;

                      float lln = m_NMEA0183.Rmc.Position.Longitude.Longitude;
                      int lon_deg_int = (int)(lln / 100);
                      float lon_deg = lon_deg_int;
                      float lon_min = lln - (lon_deg * 100);
                      mLon = lon_deg + (lon_min/60.);
                      if(m_NMEA0183.Rmc.Position.Longitude.Easting == West)
                            mLon = -mLon;

                      mSog = m_NMEA0183.Rmc.SpeedOverGroundKnots;
                      mCog = m_NMEA0183.Rmc.TrackMadeGoodDegreesTrue;

                      if(m_NMEA0183.Rmc.MagneticVariationDirection == East)
                            mVar =  m_NMEA0183.Rmc.MagneticVariation;
                      else if(m_NMEA0183.Rmc.MagneticVariationDirection == West)
                            mVar = -m_NMEA0183.Rmc.MagneticVariation;
                      bGoodData = true;
                    }
                 }
              }
        }

        if (bGoodData) {
            //ShowFriendsLogs();
            wxLogMessage(_T("Latitude: %f ,  Longitude: %f "), mLat, mLon);
            PostResponse = PostPosition(mLat, mLon, mSog, mCog);
            if (PostResponse.Find(_T("error")) != wxNOT_FOUND)
              wxLogMessage(PostResponse);
            g_LastUpdate = wxDateTime::GetTimeNow();
        }
    }
}

wxString squiddio_pi::PostPosition(double lat, double lon, double sog, double cog)
{
   wxString reply = wxEmptyString;
   wxString parameters;

   parameters.Printf(_T("api_key=%s&email=%s&lat=%f&lon=%f&sog=%f&cog=%f"), g_ApiKey.c_str(), g_Email.c_str(), lat, lon, sog,cog );

   wxHTTP post;
   post.SetHeader(_T("Content-type"), _T("text/html; charset=utf-8"));
   post.SetPostBuffer(_T("text/html; charset=utf-8")); //this seems to be the only way to set the http method to POST. Other wxHTTP methods (e.g. SetMethod) are not supported in v 2.8
   post.SetTimeout(3);

   post.Connect(_T("squidd.io"));
   wxApp::IsMainLoopRunning();

   wxInputStream *http_stream = post.GetInputStream(_T("/positions?")+parameters); // not the most elegant way to set POST parameters, but SetPostText is not supported in wxWidgets 2.8?

   if (post.GetError() == wxPROTO_NOERR)
   {
      wxStringOutputStream out_stream(&reply);
      http_stream->Read(out_stream);
   }
   else reply = wxEmptyString;

   wxDELETE(http_stream);
   post.Close();

   return reply;
}

void squiddio_pi::ShowFriendsLogs() {
    wxString layerContents;
    wxString request_url;
    //Layer * new_layer = NULL;
    wxString pp= g_ApiKey;
    wxString ee= g_Email;

    request_url.Printf(_T("http://squidd.io/connections.xml?api_key=%s&email=%s"), g_ApiKey.c_str(), g_Email.c_str() );
    wxString gpxFilePath = layerdir;
    appendOSDirSlash( &gpxFilePath );
    gpxFilePath.Append(_T("logs.gpx"));
    wxString null_region;

    layerContents = DownloadLayer(request_url);

    if (layerContents.length()> 200 ){
        isLayerUpdate = SaveLayer(layerContents, gpxFilePath);
        if (isLayerUpdate ){
             // hide and delete the current logs layer
             m_LogsLayer ->SetVisibleOnChart( false );
             RenderLayerContentsOnChart(m_LogsLayer);
             pLayerList->DeleteObject( m_LogsLayer );
         }
         m_LogsLayer = LoadLayer(gpxFilePath, null_region);
         m_LogsLayer->SetVisibleNames( false );
         RenderLayerContentsOnChart(m_LogsLayer);
        //RequestRefresh(m_parent_window);
    }
}


BEGIN_EVENT_TABLE(demoWindow, wxWindow)
  EVT_TIMER(TIMER_ID, demoWindow::OnTimerTimeout)
  EVT_PAINT ( demoWindow::OnPaint )
  EVT_SIZE(demoWindow::OnSize)
END_EVENT_TABLE();


demoWindow::demoWindow(wxWindow *pparent, wxWindowID id)
  :wxWindow(pparent, id, wxPoint(10,10), wxSize(200,200), wxSIMPLE_BORDER, _T("OpenCPN PlugIn"))
  {
    m_pTimer = new wxTimer(this,TIMER_ID);
    m_pTimer->Start(8000);
  }

void demoWindow::OnTimerTimeout(wxTimerEvent& event)
{
  //p_sqpi->ShowFriendsLogs();
  //p_sqpi->LoadConfig();
  Refresh(false);
  wxBell();
}

void demoWindow::OnPaint(wxPaintEvent& event)
{
  //wxLogMessage(_T("squidd_pi onpaint"));
  wxPaintDC dc ( this );
  {
    dc.Clear();
    wxString data;
    data.Printf(_T("Lat: %s "), wxNow().c_str() );
    dc.DrawText(data, 10, 10);
  }
}
