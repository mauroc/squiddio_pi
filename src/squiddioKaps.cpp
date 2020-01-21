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


void squiddio_pi::ProcessZipFile(wxString filename)
{
    wxString zpath = g_BaseChartDir;

    bool unzip;
    unzip = UnzipFile(filename, zpath);

    if (unzip == true) {
        wxLogMessage(wxString::Format(_("Squiddio_pi: extracted charts to: %s"), zpath));
        wxRemoveFile(filename); //remove the zip file

    }
    else {
        wxLogMessage(wxString::Format(_("Squiddio_pi: unable to extract charts to: %s"), zpath));
    }
}


bool squiddio_pi::UnzipFile(const wxString& aZipFile, const wxString& aTargetDir) {
    
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
    const wxChar * sep = _T("-");
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
          guid_array = wxSplit(m_GUID, * sep);
          id_str += ((poi_count >0 )?_T("_"):wxEmptyString) + guid_array[3] ; 
          poi_count += 1;
        }
        node = node->GetNext();
    }

    if (poi_count <= 0) 
        wxMessageBox(_("No sQuiddio POIs in viewport. Zoom out or pan to a different area."));
    else if (poi_count > 20)
        wxMessageBox(_("Too many sQuiddio POIs in viewport. Zoom in to reduce # of POIs to less than 20"));
    else {
        wxString res = wxEmptyString;
        
        wxString versionMajor = wxString::Format(wxT("%i"),PLUGIN_VERSION_MAJOR);
        wxString versionMinor = wxString::Format(wxT("%i"),PLUGIN_VERSION_MINOR);

        g_BaseChartDir = _T("/home/mauro/opencpn_data/charts/gmaps");
        wxString fn = g_BaseChartDir + wxFileName::GetPathSeparator() + _T("test_file.zip");
        wxString zoom_levels = "15_17";

        wxString url_path = _T("http://localhost:3000/places/") + id_str + _T("/download_kap_files");
        url_path.Append(_T("?lat=") + wxString::Format(wxT("%f"), center_lat) );
        url_path.Append(_T("&lon=") + wxString::Format(wxT("%f"), center_lon) );
        url_path.Append(_T("&m_lat=") + wxString::Format(wxT("%f"), max_lat)); 
        url_path.Append(_T("&m_lon=") + wxString::Format(wxT("%f"), max_lon)); 
        url_path.Append(_T("&zooms=") + zoom_levels );
        url_path.Append(_T("&source=ocpn_plugin&version=") + versionMajor + _T(".") + versionMinor);
        // L"http://localhost:3000/places/20105_22913_22916/download_kap_files?lat=9.600149&lon=-79.571587&m_lat=9.710066&m_lon=-79.360565&zooms=15_17&source=ocpn_plugin&version=1.0"

        wxString download_message = wxString::Format(wxT("Downloading %i images... "), poi_count * 2);
        OCPN_DLStatus result = OCPN_downloadFile(url_path , fn, _("Downloading"), download_message, wxNullBitmap, m_parent_window, OCPN_DLDS_ELAPSED_TIME|OCPN_DLDS_AUTO_CLOSE|OCPN_DLDS_SIZE|OCPN_DLDS_SPEED|OCPN_DLDS_REMAINING_TIME, 10
        );

        if( result == OCPN_DL_NO_ERROR )
        {
            wxLogMessage(_("Squiddio_pi: downloaded ZIP file:") + fn);        
            ProcessZipFile(fn);

            // for whatever reason, UpdateChartDBInplace doesn't seem to update the chart in the chart group - both with force at true and false. A bug in the OCPN implementation of the function?
            bool updated = UpdateChartDBInplace(GetChartDBDirArrayString(), false, true);
            if (!updated) wxMessageBox(_("Unable to update the chart database"));
            wxLogMessage(_("Squiddio_pi: downloaded KAP file:") + fn);        
        }
        else
        {
            wxLogMessage(_("Squiddio_pi: unable to connect to host"));
        }
    }
//     return res;

}

