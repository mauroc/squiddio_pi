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


void squiddio_pi::DownloadSatImages(wxString url_path) {

    wxLogMessage(_T("squiddio_pi: download sat. image: ") + url_path );

    ::wxDisplaySize(&m_display_width, &m_display_height);
    
    wxString res = wxEmptyString;
    
    wxString versionMajor = wxString::Format(wxT("%i"),PLUGIN_VERSION_MAJOR);
    wxString versionMinor = wxString::Format(wxT("%i"),PLUGIN_VERSION_MINOR);

//     wxString fn = wxFileName::CreateTempFileName( _T("squiddio_pi") );
    g_BaseChartDir = _T("/home/mauro/opencpn_data/charts/gmaps");
//     appendOSDirSlash(&g_BaseChartDir);
//     wxString fn = g_BaseChartDir.Append(_T("test_file.zip"));
    wxString fn = g_BaseChartDir + wxFileName::GetPathSeparator() + _T("test_file.zip");
    wxString zoom_levels = "15_17";
    wxString compl_url_path =  _T("http://localhost:3000") +
        url_path + _T("&source=ocpn_plugin&version=")  + versionMajor + "." + versionMinor +
        _T("&zooms=") + zoom_levels;

    OCPN_DLStatus result = OCPN_downloadFile(compl_url_path , fn, _("Downloading"), _("Downloading: "), wxNullBitmap, m_parent_window, OCPN_DLDS_ELAPSED_TIME|OCPN_DLDS_AUTO_CLOSE|OCPN_DLDS_SIZE|OCPN_DLDS_SPEED|OCPN_DLDS_REMAINING_TIME, 10
    );


    if( result == OCPN_DL_NO_ERROR )
    {
//         wxFile f( fn );
//         f.ReadAll( &res );
//         f.Close();
//         wxRemoveFile( fn );
        // for whatever reason, this doesn't seem to update the chart in the chart group - both with force at true and false. A bug in the OCPN implementation of the function?
        wxLogMessage(_("Squiddio_pi: downloaded ZIP file:") + fn);        
        ProcessZipFile(fn);

        bool updated = UpdateChartDBInplace(GetChartDBDirArrayString(), false, true);
        if (!updated) wxMessageBox(_("Unable to update the chart database"));
        wxLogMessage(_("Squiddio_pi: downloaded KAP file:") + fn);        
    }
    else
    {
        wxLogMessage(_("Squiddio_pi: unable to connect to host"));
    }
    
    
//     return res;
    
}

