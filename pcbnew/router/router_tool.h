/*
 * KiRouter - a push-and-(sometimes-)shove PCB router
 *
 * Copyright (C) 2013  CERN
 * Author: Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.or/licenses/>.
 */

#ifndef __ROUTER_TOOL_H
#define __ROUTER_TOOL_H

#include <set>
#include <boost/shared_ptr.hpp>

#include <math/vector2d.h>
#include <tool/tool_interactive.h>

#include <wxstruct.h>
#include <msgpanel.h>

#include "pns_layerset.h"

class PNS_ROUTER;
class PNS_ITEM;

class ROUTER_TOOL : public TOOL_INTERACTIVE
{
public:
    ROUTER_TOOL();
    ~ROUTER_TOOL();

    void Reset();
    int Main( TOOL_EVENT& aEvent );

private:

    PNS_ITEM* pickSingleItem( const VECTOR2I& aWhere, int aNet = -1, int aLayer = -1 );

    void setMsgPanel( bool enabled, int entry, const wxString& aUpperMessage = wxT(""),
            const wxString& aLowerMessage = wxT("") );
    void clearMsgPanel();

    int getDefaultWidth( int aNetCode );
    void startRouting();
    void highlightNet( bool enabled, int netcode = -1 );

    void updateStartItem( TOOL_EVENT& aEvent );
    void updateEndItem( TOOL_EVENT& aEvent );

    void getNetclassDimensions( int aNetCode, int& aWidth, int& aViaDiameter, int& aViaDrill );

    MSG_PANEL_ITEMS m_panelItems;

    PNS_ROUTER* m_router;

    PNS_ITEM* m_startItem;
    int m_startLayer;
    VECTOR2I m_startSnapPoint;

    PNS_ITEM* m_endItem;
    VECTOR2I m_endSnapPoint;

    /*boost::shared_ptr<CONTEXT_MENU> m_menu;*/
    CONTEXT_MENU* m_menu;
};

#endif
