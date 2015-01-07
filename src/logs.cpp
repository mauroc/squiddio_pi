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

const int SECOND = 1;
const int MINUTE = 60 * SECOND;
const int HOUR = 60 * MINUTE;
const int DAY = 24 * HOUR;
const int MONTH = 30 * DAY;


BEGIN_EVENT_TABLE(logsWindow, wxWindow)
EVT_TIMER(TIMER_ID, logsWindow::OnTimerTimeout)
EVT_PAINT ( logsWindow::OnPaint )
//EVT_SIZE(logsWindow::OnSize)
END_EVENT_TABLE();

logsWindow::logsWindow(squiddio_pi * plugin, wxWindow *pparent, wxWindowID id) :
        wxWindow(pparent, id, wxPoint(10, 200), wxSize(560, 25),
                wxSIMPLE_BORDER, _T("OpenCPN PlugIn")) {

    this->SetTransparent(30);
    p_plugin = plugin;
    m_parent_window = pparent;
    m_pTimer = new wxTimer(this, TIMER_ID);

    m_LogsLayer = NULL;
    g_RetrieveSecs = period_secs(p_plugin->g_RetrievePeriod);
    m_LastLogSent = p_plugin->g_LastLogSent;
    m_LastLogsRcvd = p_plugin->g_LastLogsRcvd;

    m_LogsFilePath = p_plugin->layerdir;
    p_plugin->appendOSDirSlash(&m_LogsFilePath);
    m_LogsFilePath.Append(_T("logs.gpx"));

    DisplayLogsLayer();

    if (g_RetrieveSecs > 0)  // display friends' logs
            {
        if (wxDateTime::GetTimeNow()
                > m_LastLogsRcvd.GetSecond() + g_RetrieveSecs) // overdue request at startup?
                        {
            RequestRefresh(m_parent_window);
            if (p_plugin->IsOnline())
                ShowFriendsLogs();
        }
        int nextEvent = g_RetrieveSecs
                - (wxDateTime::GetTimeNow() - m_LastLogsRcvd.GetSecond());
        // if update is overdue, delay by a few seconds to prevent get request from interfering with opencpn launch, else schedule it for when it's due
        SetTimer(wxMax(wxMin(nextEvent, g_RetrieveSecs), 7));
    }
}
void logsWindow::SetTimer(int RetrieveSecs) {
    m_pTimer->Stop();
    if (RetrieveSecs > 0)
        m_pTimer->Start(RetrieveSecs * 1000);
    g_RetrieveSecs = RetrieveSecs;
    Refresh(false);
}

void logsWindow::OnTimerTimeout(wxTimerEvent& event) {
    if (p_plugin->IsOnline()) {
        RequestRefresh(m_parent_window);
        ShowFriendsLogs();
        if (m_pTimer->GetInterval() / 1000 < g_RetrieveSecs) {
            // after initial friends update, reset the timer to the required interval
            SetTimer(0);
            SetTimer(g_RetrieveSecs * 1000);
        }
    }
    Refresh(false);
}

void logsWindow::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    wxColour cs;
    GetGlobalColor(_T("GREEN2"), &cs);
    wxColour cr;
    GetGlobalColor(_T("BLUE2"), &cr);
    wxColour ci;
    GetGlobalColor(_T("DASHL"), &ci);
    wxColour cb;
    GetGlobalColor(_T("DASHB"), &cb);
    dc.SetBackground(cb);
    dc.SetTextBackground(cb);
    wxString lastRcvd, lastSent;

    wxFont *g_pFontSmall;
    g_pFontSmall = new wxFont(8, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
            wxFONTWEIGHT_NORMAL);
    dc.SetFont(*g_pFontSmall);

    if (m_LastLogSent.IsValid() && m_LastLogSent.GetYear() > 1969) // can't figure out how to assess if it is NULL (i.e. 31/12/1969)
        lastSent = m_LastLogSent.Format(_T(" %a-%d-%b-%Y %H:%M:%S  "),
                wxDateTime::Local);

    if (m_LastLogsRcvd.IsValid() && m_LastLogsRcvd.GetYear() > 1969)
        lastRcvd = m_LastLogsRcvd.Format(_T(" %a-%d-%b-%Y %H:%M:%S  "),
                wxDateTime::Local);
    {
        dc.Clear();
        wxString data;
        wxString lastSentAv = (
                lastSent.Length() > 0 ? lastSent : _T("(awaiting NMEA events)"));
        if (p_plugin->g_PostPeriod > 0 && p_plugin->last_online) {
            dc.SetTextForeground(cs);
        } else {
            dc.SetTextForeground(ci);
        }
        //data.Printf(_T("Log sent:  %s "), lastSent.c_str());
        data.Printf(_T("Log sent:  %s "), lastSentAv.c_str());
        dc.DrawText(data, 5, 5);

        if (g_RetrieveSecs > 0 && p_plugin->last_online) {
            dc.SetTextForeground(cr);
        } else {
            dc.SetTextForeground(ci);
        }

        wxString demo_msg = _T("");
        if (p_plugin->g_ApiKey==_T("squiddio_demo_api"))
            demo_msg = _T("(Demo)");

        //data.Printf(_T("Logs received: %s %s %s"), lastRcvd.c_str(), timeAgo(m_LastLogsRcvd).c_str(), demo_msg.c_str() );
        data.Printf(_T("Logs received: %s %s"), lastRcvd.c_str(), demo_msg.c_str() );

        dc.DrawText(data, 230, 5);
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
                    else if (m_NMEA0183.Rmc.MagneticVariationDirection == West)
                        mVar = -m_NMEA0183.Rmc.MagneticVariation;
                    bGoodData = true;
                }
            }
        }
    }

    //if (false) {
    if (bGoodData) {
        //wxLogMessage(_T("Latitude: %f ,  Longitude: %f "), mLat, mLon);
        PostResponse = PostPosition(mLat, mLon, mSog, mCog);
        if (PostResponse.Find(_T("error")) != wxNOT_FOUND)
            wxLogMessage(PostResponse);
        m_LastLogSent = wxDateTime::Now();
        p_plugin->g_LastLogSent = wxDateTime::GetTimeNow(); //to be saved in config file
        Refresh(false);
    }

}

wxString logsWindow::PostPosition(double lat, double lon, double sog,
        double cog) {
    wxString reply = wxEmptyString;
    wxString parameters;
    myCurlHTTP post;
    post.SetOpt(CURLOPT_TIMEOUT, 5);

    parameters.Printf(
            _T("api_key=%s&email=%s&lat=%f&lon=%f&sog=%f&cog=%f&source=ocpn"),
            p_plugin->g_ApiKey.c_str(), p_plugin->g_Email.c_str(), lat, lon,
            sog, cog);

    wxApp::IsMainLoopRunning();

    size_t result = post.Post(parameters.ToAscii(), parameters.Len(),
            _T("https://squidd.io/positions.json"));

    if (result) {
        reply = post.GetResponseBody();
        wxLogMessage(_T("Created sQuiddio log update:") + reply);
    } else
        reply = wxEmptyString;

    return reply;
}

void logsWindow::ShowFriendsLogs() {
    wxString layerContents;
    wxString request_url;
    bool isLayerUpdate;

    request_url.Printf(_T("/connections.xml?api_key=%s&email=%s"),
            p_plugin->g_ApiKey.c_str(), p_plugin->g_Email.c_str());

    layerContents = p_plugin->DownloadLayer(request_url);

    if (layerContents.length() > 200) {
        isLayerUpdate = p_plugin->SaveLayer(layerContents, m_LogsFilePath);
        if (isLayerUpdate) {
            if (m_LogsLayer) {
                // hide and delete the current logs layer
                m_LogsLayer->SetVisibleOnChart(false);
                p_plugin->RenderLayerContentsOnChart(m_LogsLayer);
                RequestRefresh(m_parent_window);
                p_plugin->pLayerList->DeleteObject(m_LogsLayer);
            }

            DisplayLogsLayer();

            m_LastLogsRcvd = wxDateTime::Now();
            p_plugin->g_LastLogsRcvd = wxDateTime::GetTimeNow(); //to be saved in config file
            wxBell();
        }
    } else {
        wxLogMessage(
                _T(
                        "sQuiddio: friends logs unavailable: check email/apikey in settings and/or your Follow List at http://squidd.io"));
    }
}

void logsWindow::DisplayLogsLayer() {
    if (::wxFileExists(m_LogsFilePath) && g_RetrieveSecs > 0) {
        wxString null_region;
        m_LogsLayer = p_plugin->LoadLayer(m_LogsFilePath, null_region);
        m_LogsLayer->SetVisibleNames(false);
        p_plugin->RenderLayerContentsOnChart(m_LogsLayer);
    }
}

