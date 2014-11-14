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

#include "logs.h"

BEGIN_EVENT_TABLE(logsWindow, wxWindow)
    EVT_TIMER(TIMER_ID, logsWindow::OnTimerTimeout)
    EVT_PAINT ( logsWindow::OnPaint )
    //EVT_SIZE(logsWindow::OnSize)
END_EVENT_TABLE();

logsWindow::logsWindow(squiddio_pi * plugin, wxWindow *pparent, wxWindowID id) :
        wxWindow(pparent, id, wxPoint(10, 10), wxSize(300, 50), wxSIMPLE_BORDER, _T("OpenCPN PlugIn")) {
    p_plugin = plugin;
    m_parent_window = pparent;
    m_pTimer = new wxTimer(this, TIMER_ID);
    m_LogsLayer = NULL;
    g_RetrieveSecs = period_secs(p_plugin->g_RetrievePeriod);
    if (g_RetrieveSecs > 0)
       ResetTimer(g_RetrieveSecs);
}
void logsWindow::ResetTimer(int retrieveSecs){
    m_pTimer->Stop();
    if (retrieveSecs>0)
    {
        m_pTimer->Start(retrieveSecs*1000);
        Refresh(false);
    }
    g_RetrieveSecs = retrieveSecs;
}
void logsWindow::OnTimerTimeout(wxTimerEvent& event) {
    RequestRefresh(m_parent_window);
    ShowFriendsLogs();
    Refresh(false);
}

void logsWindow::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    wxString lastRcvd, lastSent;

    if (m_LastLogSent.IsValid())
        lastSent = m_LastLogSent.Format(_T(" %a-%d-%b-%Y %H:%M:%S  "),
                wxDateTime::Local);
    if (m_LastLogsRcvd.IsValid())
        lastRcvd = m_LastLogsRcvd.Format(_T(" %a-%d-%b-%Y %H:%M:%S  "),
                wxDateTime::Local);
    {
        dc.Clear();
        wxString data;
        if (p_plugin->g_PostPeriod > 0){
            data.Printf(_T("Log sent:  %s "), lastSent.c_str());
            dc.DrawText(data, 5, 5);
        }
        if (g_RetrieveSecs > 0){
            data.Printf(_T("Logs rcvd: %s "), lastRcvd.c_str());
            dc.DrawText(data, 5, 25);
        }
    }
}

void logsWindow::SetSentence(wxString &sentence) {
    wxString PostResponse;
    bool bGoodData = false;

    m_NMEA0183 << sentence;

    if (m_NMEA0183.PreParse()) {
        if (m_NMEA0183.LastSentenceIDReceived == _T("RMC")) {
            if (m_NMEA0183.Parse()) {
                if (m_NMEA0183.Rmc.IsDataValid == NTrue) {
                    float llt = m_NMEA0183.Rmc.Position.Latitude.Latitude;
                    int lat_deg_int = (int) (llt / 100);
                    float lat_deg = lat_deg_int;
                    float lat_min = llt - (lat_deg * 100);
                    mLat = lat_deg + (lat_min / 60.);
                    if (m_NMEA0183.Rmc.Position.Latitude.Northing == South)
                        mLat = -mLat;

                    float lln = m_NMEA0183.Rmc.Position.Longitude.Longitude;
                    int lon_deg_int = (int) (lln / 100);
                    float lon_deg = lon_deg_int;
                    float lon_min = lln - (lon_deg * 100);
                    mLon = lon_deg + (lon_min / 60.);
                    if (m_NMEA0183.Rmc.Position.Longitude.Easting == West)
                        mLon = -mLon;

                    mSog = m_NMEA0183.Rmc.SpeedOverGroundKnots;
                    mCog = m_NMEA0183.Rmc.TrackMadeGoodDegreesTrue;

                    if (m_NMEA0183.Rmc.MagneticVariationDirection == East)
                        mVar = m_NMEA0183.Rmc.MagneticVariation;
                    else if (m_NMEA0183.Rmc.MagneticVariationDirection
                            == West)
                        mVar = -m_NMEA0183.Rmc.MagneticVariation;
                    bGoodData = true;
                }
            }
        }
    }

    //if (false) {
    if (bGoodData) {
        wxLogMessage(_T("Latitude: %f ,  Longitude: %f "), mLat, mLon);
        PostResponse = PostPosition(mLat, mLon, mSog, mCog);
        if (PostResponse.Find(_T("error")) != wxNOT_FOUND)
            wxLogMessage(PostResponse);
        p_plugin->g_LastUpdate = wxDateTime::GetTimeNow();
        m_LastLogSent = wxDateTime::Now();
        Refresh(false);
    }

}

wxString logsWindow::PostPosition(double lat, double lon, double sog,
        double cog) {
    wxString reply = wxEmptyString;
    wxString parameters;

    parameters.Printf(_T("api_key=%s&email=%s&lat=%f&lon=%f&sog=%f&cog=%f"),
            p_plugin->g_ApiKey.c_str(), p_plugin->g_Email.c_str(), lat, lon, sog, cog);

    wxHTTP post;
    post.SetHeader(_T("Content-type"), _T("text/html; charset=utf-8"));
    post.SetPostBuffer(_T("text/html; charset=utf-8")); //this seems to be the only way to set the http method to POST. Other wxHTTP methods (e.g. SetMethod) are not supported in v 2.8
    post.SetTimeout(3);

    post.Connect(_T("squidd.io"));
    wxApp::IsMainLoopRunning();

    wxInputStream *http_stream = post.GetInputStream(
            _T("/positions?") + parameters); // not the most elegant way to set POST parameters, but SetPostText is not supported in wxWidgets 2.8?

    if (post.GetError() == wxPROTO_NOERR) {
        wxStringOutputStream out_stream(&reply);
        http_stream->Read(out_stream);
    } else
        reply = wxEmptyString;

    wxDELETE(http_stream);
    post.Close();

    return reply;
}

void logsWindow::ShowFriendsLogs() {
    wxString layerContents;
    wxString request_url;
    bool isLayerUpdate;

    wxBell();

    request_url.Printf(_T("http://squidd.io/connections.xml?api_key=%s&email=%s"), p_plugin->g_ApiKey.c_str(), p_plugin->g_Email.c_str());
    wxString gpxFilePath = p_plugin->layerdir;
    p_plugin->appendOSDirSlash(&gpxFilePath);
    gpxFilePath.Append(_T("logs.gpx"));
    wxString null_region;

    layerContents = p_plugin->DownloadLayer(request_url);

    if (layerContents.length() > 200) {
        isLayerUpdate = p_plugin->SaveLayer(layerContents, gpxFilePath);
        if (isLayerUpdate) {
            if (m_LogsLayer) {
                // hide and delete the current logs layer
                m_LogsLayer->SetVisibleOnChart(false);
                p_plugin->RenderLayerContentsOnChart(m_LogsLayer);
                p_plugin->pLayerList->DeleteObject(m_LogsLayer);
            }
            m_LogsLayer = p_plugin->LoadLayer(gpxFilePath, null_region);
            m_LogsLayer->SetVisibleNames(false);
            p_plugin->RenderLayerContentsOnChart(m_LogsLayer);
            m_LastLogsRcvd = wxDateTime::Now();
        }
    }
}


