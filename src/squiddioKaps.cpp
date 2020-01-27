/******************************************************************************
 * Project:  OpenCPN
 * Purpose:  Squiddio plugin
 *
 ***************************************************************************
 *   Copyright (C) 2019 by Mauro Calvi                                     *
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
#include <wx/fileconf.h>
#include <wx/fontdlg.h>
#include "sJSON.h"
#include "version.h"

#include "wxJSON/jsonwriter.h"
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include <wx/filesys.h>

extern squiddio_pi        *g_squiddio_pi;


bool UnzipFile(const wxString& aZipFile, const wxString& aTargetDir) {
    
//     https://wiki.wxwidgets.org/WxZipInputStream
    bool ret = true;

    std::unique_ptr<wxZipEntry> entry(new wxZipEntry());

    do {

        wxFileInputStream in(aZipFile);

        if (!in) {
            wxLogMessage(_("Cannot open file '") + aZipFile + _T("'."));
            ret = false;
            break;
        }

        wxZipInputStream zip(in);

        while (entry.reset(zip.GetNextEntry()), entry.get() != NULL) {
            // access meta-data
            wxString name = entry->GetName();
            name = aTargetDir + wxFileName::GetPathSeparator() + name;
            
            // read 'zip' to access the entry's data
            if (entry->IsDir()) {
                int perm = entry->GetMode();
                wxFileName::Mkdir(name, perm, wxPATH_MKDIR_FULL);
            }
            else {
                zip.OpenEntry(*entry.get());
                if (!zip.CanRead()) {
                    wxLogError(_("Cannot read zip entry '") + entry->GetName() + _T("'."));
                    ret = false;
                    break;
                }

                wxFileOutputStream file(name);

                if (!file) {
                    wxLogError(_("Cannot create file '") + name + _T("'."));
                    ret = false;
                    break;
                }

                zip.Read(file);

            }
        }

    } while (false);

    return ret;
}


bool squiddio_pi::ProcessZipFile(wxString chart_dir, wxString tmp_file)
{
//     wxString zpath = g_BaseChartDir;

    bool ret;
    
    wxFile f( tmp_file );
    wxFileOffset zipped_file_length = f.Length();
    if (zipped_file_length < 1000 ) {
        // not a valid zipped file, i.e. squiddio returned error condition 
        wxString res = wxEmptyString;
        f.ReadAll( &res );
        wxLogMessage(_("Squiddio_pi: error in response file: ") + res) ;  
        wxMessageBox(res);
        ret = false;
    } else {
        bool unzip;
        unzip = UnzipFile(tmp_file, chart_dir);

        if (unzip == true) {
            wxLogMessage(wxString::Format(_("Squiddio_pi: extracted charts to: %s"), chart_dir));
            wxRemoveFile(tmp_file); //remove the zip file
            ret = true;
        }
        else {
            wxLogMessage(wxString::Format(_("Squiddio_pi: unable to extract charts to: %s"), chart_dir));
            ret = false;
        }
    }
    return ret;
}

bool squiddio_pi::IsPOIinLayer(int layer_id) 
{
    // POI belongs to visible layer?
    Layer * l;
    LayerList::iterator it;
    int index = 0;
    bool visible_layer = false;
    for (it = (*pLayerList).begin(); it != (*pLayerList).end();
            ++it, ++index) {
        l = (Layer *) (*it);
        if (l->m_LayerID == layer_id) {
            visible_layer = true;
            break;
        }
    }
    return visible_layer;
}


void squiddio_pi::DownloadSatImages() {

    wxLogMessage(_T("squiddio_pi: download sat. image: "));

//     ::wxDisplaySize(&m_display_width, &m_display_height);
//     double chartscale = m_vp->view_scale_ppm;

    double center_lat = m_vp->clat;
    double center_lon = m_vp->clon;
    double max_lat    = m_vp->lat_max;
    double max_lon    = m_vp->lon_max;


    wxPoiListNode *node = pPoiMan->GetWaypointList()->GetFirst();
    PoiList temp_list;
    wxString id_str;

    double poi_lat, poi_lon;
    wxString m_GUID;
    wxArrayString guid_array;
    const wxChar * sep_dash = _T("-"), * sep_undersc = _T("_"), * sep_comma = _T(",");
    int poi_count = 0;
    

    while (node) {
        // is POI candidate for download?
        Poi *rp = node->GetData();
        poi_lat = rp->GetLatitude();
        poi_lon = rp->GetLongitude();

        if ( poi_lat > m_vp->lat_min && poi_lat < m_vp->lat_max && poi_lon > m_vp->lon_min && poi_lon < m_vp->lon_max 
            && ShowType(rp) && IsPOIinLayer(rp->m_LayerID) ) {
          temp_list.Append( rp );
          m_GUID = rp->m_GUID;
          guid_array = wxSplit(m_GUID, * sep_dash);
          id_str += ((poi_count >0 ) ? sep_undersc : wxEmptyString) + guid_array[3] ; 
          poi_count += 1;
        }
        node = node->GetNext();
    }

    if (poi_count <= 0) 
        wxMessageBox(_("No sQuiddio POIs in viewport. Download local POIs, zoom out or pan to a different area."));
    else if (poi_count > 20)
        wxMessageBox(_("Too many sQuiddio POIs in viewport. Zoom in to reduce # of POIs to less than 20"));
    else {
//         wxString res = wxEmptyString;
        
        wxString versionMajor = wxString::Format(wxT("%i"),PLUGIN_VERSION_MAJOR);
        wxString versionMinor = wxString::Format(wxT("%i"),PLUGIN_VERSION_MINOR);

        wxString tmp_file = wxFileName::CreateTempFileName(_T("squiddio_pi"));
        wxString chart_dir = g_BaseChartDir + wxFileName::GetPathSeparator();
        
        wxString zoom_param = g_ZoomLevels;
        zoom_param.Replace(sep_comma, sep_undersc);
        
        wxString url_path = g_DomainName + _T("/places/") + id_str + _T("/download_kap_files");
        url_path.Append(_T("?zooms=") + zoom_param );  // gmaps zoom levels 
        if (g_DownloadVPMap) {
            url_path.Append(_T("&lat=") + wxString::Format(wxT("%f"), center_lat) );  // center latitude
            url_path.Append(_T("&lon=") + wxString::Format(wxT("%f"), center_lon) );  // center longitude
            url_path.Append(_T("&m_lat=") + wxString::Format(wxT("%f"), max_lat));    // max latitude of viewport
            url_path.Append(_T("&m_lon=") + wxString::Format(wxT("%f"), max_lon));    // max longitude of viewport
        }    
        url_path.Append(_T("&compressed=true")); 
        url_path.Append(_T("&squiddio_id=")+g_Email+_T("&api_key="+g_ApiKey)); 
        url_path.Append(_T("&source=ocpn_plugin&version=") + versionMajor + _T(".") + versionMinor); // plugin identifiers
        // L"http://localhost:3000/places/20105_22913_22916/download_kap_files?lat=9.600149&lon=-79.571587&m_lat=9.710066&m_lon=-79.360565&zooms=15_17&compressed=true&source=ocpn_plugin&version=1.5"

        int num_zooms = wxSplit(g_ZoomLevels, * sep_comma).GetCount();
        wxString download_message = wxString::Format(wxT("Downloading %i images... "), poi_count * num_zooms);
        OCPN_DLStatus result = OCPN_downloadFile(url_path , tmp_file, _("Downloading"), download_message, wxNullBitmap, m_parent_window, OCPN_DLDS_ELAPSED_TIME|OCPN_DLDS_AUTO_CLOSE|OCPN_DLDS_SIZE|OCPN_DLDS_SPEED|OCPN_DLDS_REMAINING_TIME, 10
        );

        if( result == OCPN_DL_NO_ERROR )
        {
//          wxLogMessage(_("Squiddio_pi: downloaded ZIP file:") + tmp_file);        
            if (ProcessZipFile(chart_dir, tmp_file)) {

                // UpdateChartDBInplace doesn't seem to update the chart group - both with force at true or false. A bug in the OCPN implementation of the function?
                bool updated = UpdateChartDBInplace(GetChartDBDirArrayString(), false, true);
                if (!updated) 
                    wxMessageBox(_("Unable to update the chart database"));
                else 
                    wxLogMessage(_("Squiddio_pi: downloaded KAP file:") + tmp_file);    
            }    
        }
        else
        {
            wxLogMessage(_("Squiddio_pi: unable to connect to host"));
        }
    }
//     return res;
}

