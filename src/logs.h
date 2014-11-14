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

#ifndef LOGS_H_
#define LOGS_H_

#include "squiddio_pi.h"

class squiddio_pi;

class logsWindow : public wxWindow
{
public:
      logsWindow(squiddio_pi * plugin, wxWindow *pparent, wxWindowID id);
      ~logsWindow(){}
      void OnTimerTimeout(wxTimerEvent& event);
      void ResetTimer(int seconds);
      void OnPaint(wxPaintEvent& event);
      void SetSentence(wxString &sentence);
      wxString PostPosition(double lat, double lon, double sog, double cog);
      void ShowFriendsLogs();

      NMEA0183     m_NMEA0183;
      wxString     m_NMEASentence;
      double      mLat, mLon, mSog, mCog, mVar;

      Layer       *m_LogsLayer;

      wxTimer      * m_pTimer;
      wxStaticText * m_pStaticText;
      wxDateTime   m_LastLogsRcvd;
      wxDateTime   m_LastLogSent;

private:
      int          g_RetrieveSecs;
      wxWindow     *m_parent_window;
      squiddio_pi  *p_plugin;

DECLARE_EVENT_TABLE()
};


#endif /* LOGS_H_ */
