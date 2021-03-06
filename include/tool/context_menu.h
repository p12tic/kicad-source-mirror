/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013 CERN
 * @author Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef __CONTEXT_MENU_H
#define __CONTEXT_MENU_H

#include <wx/menu.h>
#include <tool/tool_action.h>
#include <map>

class TOOL_INTERACTIVE;

/**
 * Class CONTEXT_MENU
 *
 * Defines the structure of a context (usually right-click) popup menu
 * for a given tool.
 */
class CONTEXT_MENU
{
public:
    ///> Default constructor
    CONTEXT_MENU();

    ///> Copy constructor
    CONTEXT_MENU( const CONTEXT_MENU& aMenu );

    /**
     * Function SetTitle()
     * Sets title for the context menu. The title is shown as a text label shown on the top of
     * the menu.
     * @param aTitle is the new title.
     */
    void SetTitle( const wxString& aTitle );

    /**
     * Function Add()
     * Adds an entry to the menu. After highlighting/selecting the entry, a TOOL_EVENT command is
     * sent that contains ID of the entry.
     * @param aLabel is the text label show in the menu.
     * @param aId is the ID that is sent in the TOOL_EVENT. It should be unique for every entry.
     */
    void Add( const wxString& aLabel, int aId );

    /**
     * Function Add()
     * Adds an entry to the menu, basing on the TOOL_ACTION object. After selecting the entry,
     * a TOOL_EVENT command containing name of the action is sent.
     * @param aAction is the action to be added to menu entry.
     */
    void Add( const TOOL_ACTION& aAction );

    /**
     * Function Clear()
     * Removes all the entries from the menu (as well as its title). It leaves the menu in the
     * initial state.
     */
    void Clear();

    /**
     * Function GetMenu()
     * Returns the instance of wxMenu object used to display the menu.
     */
    wxMenu* GetMenu() const
    {
        return const_cast<wxMenu*>( &m_menu );
    }

private:
    ///> Class CMEventHandler takes care of handling menu events. After reception of particular
    ///> events, it translates them to TOOL_EVENTs that may control tools.
    class CMEventHandler : public wxEvtHandler
    {
    public:
        ///> Default constructor
        ///> aMenu is the CONTEXT_MENU instance for which it handles events.
        CMEventHandler( CONTEXT_MENU* aMenu ) : m_menu( aMenu ) {};

        ///> Handler for menu events.
        void onEvent( wxEvent& aEvent );

    private:
        ///> CONTEXT_MENU instance for which it handles events.
        CONTEXT_MENU* m_menu;
    };

    friend class TOOL_INTERACTIVE;

    /**
     * Function setTool()
     * Sets a tool that is the creator of the menu.
     * @param aTool is the tool that created the menu.
     */
    void setTool( TOOL_INTERACTIVE* aTool )
    {
        m_tool = aTool;
    }

    /**
     * Function getHotKeyDescription()
     * Returns a hot key in the string format accepted by wxMenu.
     * @param aAction is the action with hot key to be translated..
     * @return Hot key in the string format compatible with wxMenu.
     */
    std::string getHotKeyDescription( const TOOL_ACTION& aAction ) const;

    ///> Flag indicating that the menu title was set up.
    bool m_titleSet;

    ///> Instance of wxMenu used for display of the context menu.
    wxMenu m_menu;

    ///> Instance of menu event handler.
    CMEventHandler m_handler;

    ///> Creator of the menu
    TOOL_INTERACTIVE* m_tool;

    /// Menu items with ID higher than that are considered TOOL_ACTIONs
    static const int m_actionId = 10000;

    /// Associates tool actions with menu item IDs. Non-owning.
    std::map<int, const TOOL_ACTION*> m_toolActions;
};

#endif
