/***********************************************************************/
/* Methodes de base de gestion des classes des elements de schematique */
/***********************************************************************/

#include "fctsys.h"
#include "class_drawpanel.h"

#include "common.h"
#include "program.h"
#include "general.h"

#include "class_marker_sch.h"
#include "erc.h"
#include "dialog_display_info_HTML_base.h"

/* Marker are mainly used to show an ERC error
 * but they could be used to give a specifi info
 */


const wxChar* NameMarqueurType[] =
{
    wxT( "" ),
    wxT( "ERC" ),
    wxT( "PCB" ),
    wxT( "SIMUL" ),
    wxT( "?????" )
};


/**************************/
/* class MARKER_SCH */
/**************************/

MARKER_SCH::MARKER_SCH() :
    SCH_ITEM( NULL, DRAW_MARKER_STRUCT_TYPE ),
    MARKER_BASE()
{
}


MARKER_SCH::MARKER_SCH( const wxPoint& pos, const wxString& text ) :
    SCH_ITEM( NULL, DRAW_MARKER_STRUCT_TYPE ),
    MARKER_BASE( 0, pos, text, pos )
{
}


MARKER_SCH::~MARKER_SCH()
{
}


MARKER_SCH* MARKER_SCH::GenCopy()
{
    MARKER_SCH* newitem = new MARKER_SCH( GetPos(), GetReporter().GetMainText() );

    newitem->SetMarkerType( GetMarkerType() );
    newitem->SetErrorLevel( GetErrorLevel() );

    return newitem;
}


#if defined(DEBUG)

/**
 * Function Show
 * is used to output the object tree, currently for debugging only.
 * @param nestLevel An aid to prettier tree indenting, and is the level
 *          of nesting of this object within the overall tree.
 * @param os The ostream& to output to.
 */
void MARKER_SCH::Show( int nestLevel, std::ostream& os )
{
    // for now, make it look like XML:
    NestedSpace( nestLevel, os ) << '<' << GetClass().Lower().mb_str() << GetPos()
                                 << "/>\n";
}


#endif

/**
 * Function Save
 * writes the data structures for this object out to a FILE in "*.brd" format.
 * @param aFile The FILE to write to.
 * @return bool - true if success writing else false.
 * Currently: do nothing (markers are no more saved in files)
 */
bool MARKER_SCH::Save( FILE* aFile ) const
{
    bool     success = true;
#if 0
    wxString msg     = m_drc.GetMainText();

    if( fprintf( aFile, "Kmarq %c %-4d %-4d \"%s\"",
                GetMarkerType() + 'A', GetPos().x, GetPos().y,
                CONV_TO_UTF8( msg ) ) == EOF )
        success = false;

    if ( m_drc.HasSecondItem() )
    {
            msg     = GetReporter().GetAuxiliaryText();
        if( fprintf( aFile, " \"%s\" %-4d %-4d",
                    GetMarkerType() + 'A', m_drc.GetPointB().x, m_drc.GetPointB().y,
                    CONV_TO_UTF8( msg ) ) == EOF )
            success = false;
    }
    if( fprintf( aFile, " F=%X T=%X\n",
                GetErrorLevel(), GetReporter().GetErrorCode() ) == EOF )
#endif
    return success;
}


/****************************************************************************/
void MARKER_SCH::Draw( WinEDA_DrawPanel* aPanel, wxDC* aDC,
                       const wxPoint& aOffset, int aDrawMode, int aColor )
/****************************************************************************/
{
    EDA_Colors color = (EDA_Colors) m_Color;
    EDA_Colors tmp   = color;

    if( GetMarkerType() == MARK_ERC )
    {
        color = (GetErrorLevel() == WAR ) ?
                (EDA_Colors) g_LayerDescr.LayerColor[LAYER_ERC_WARN] :
                (EDA_Colors) g_LayerDescr.LayerColor[LAYER_ERC_ERR];
    }

    if( aColor < 0 )
        m_Color = color;
    else
        m_Color = (EDA_Colors) aColor;

    DrawMarker( aPanel, aDC, aDrawMode, aOffset );
    m_Color = tmp;
}


/**
 * Function GetBoundingBox
 * returns the orthogonal, bounding box of this object for display purposes.
 * This box should be an enclosing perimeter for visible components of this
 * object, and the units should be in the pcb or schematic coordinate system.
 * It is OK to overestimate the size by a few counts.
 */
EDA_Rect MARKER_SCH::GetBoundingBox()
{
    return GetBoundingBoxMarker();
}


/** Function DisplayMarkerInfo()
 * Displays the full info of this marker, within an HTML window
 */
void MARKER_SCH::DisplayMarkerInfo( WinEDA_SchematicFrame* aFrame )
{
    wxString msg = GetReporter().ShowHtml();

    DIALOG_DISPLAY_HTML_TEXT_BASE infodisplay( aFrame, -1, wxEmptyString,
                                              wxGetMousePosition(), wxSize( 550, 170 ) );

    infodisplay.m_htmlWindow->SetPage( msg );
    infodisplay.ShowModal();
}