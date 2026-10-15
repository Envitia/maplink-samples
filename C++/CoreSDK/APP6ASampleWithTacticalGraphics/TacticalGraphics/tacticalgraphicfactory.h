/* -*-c++-*- */
/* Tactical Graphics Factory (APP6C)
*
* Copyright 2014 Envitia PLC
* http://www.envitia.com
*
* Author: Damian Dixon
*/
#ifndef _TACTICALGRAPHICFACTORY_H_
#define _TACTICALGRAPHICFACTORY_H_

#include <cassert>

namespace envitia { namespace MapLink { namespace TacticalGraphics {

  /**
   * Tactical Graphic Modifier class.
   *
   */
  class Modifiers
  {
  public:

    Modifiers(const char *fieldLetters, const char *replacementString)
    {
      assert(fieldLetters != NULL);
      assert(replacementString != NULL);
      m_fieldLetters[0] = '\0';
      m_replacementString = NULL;
      if (fieldLetters)
      {        
        assert( strlen(fieldLetters) <= 2);
        m_fieldLetters[0] = fieldLetters[0];
        m_fieldLetters[1] = fieldLetters[1];
        m_fieldLetters[2] = '\0';
      }
      if (replacementString)
      {
        m_replacementString = strdup(replacementString);
      }
    }
    ~Modifiers()
    {
      if (m_replacementString)
        free(m_replacementString);
    }

    const char *filedLetters() const
    {
      return &m_fieldLetters[0];
    }
    const char *replacementString() const
    {
      return m_replacementString;
    }

  private:
    char m_fieldLetters[3];
    char *m_replacementString;
  } ;


  /**
   * Tactical Graphic Factory for APP6(C)
   *
   */
  class APP6CTacticalGraphicsFactory
  {
  public:

    /**
     * Creates Tactical Graphic Lines.
     *
     * The following Tactical Graphic Areas are implemented:
     *
     *  - Attack By Fire Position: APP-6A(C) May 2011, Table 7-7 page 7-61
     *   - SIDC: G*C*MOOP--*****
     *  - Support By Fire Position: APP-6A(C) May 2011, Table 7-7 page 7-62
     *   - SIDC: G*C*MOOS--*****
     *  - Contain: APP-6A(C) May 2011, Table 7-A-1 page 7-A-10
     *   - SIDC: G*T*GJ----*****
     *  - Ambush: APP-6A(C) May 2011, Table 7-7 page 7-64
     *   - SIDC: G*C*MSGA--*****
     *
     * @param sidc The SIDC code for the tactical graphic. Only the graphic type is currently used.
     * @param positions The TMC positions that define the control points for the tactical graphic.
     * @param modifiers The modifiers that define the text that should be drawn with the tactical graphic. This is not currently implemented.
     * @param numberOfModifiers The number of modifiers passed in.
     * @param spline If set to true some of the tactical graphics are created using a spline algorithm (mainly the Axis arrows).
     * @param lineWidth The width of the lines in pixels. The default is 1.
     *  
     * @return A TSLEntitySet containing the geometry that is to be drawn. This has to be added to a TSLStandardDataLayer that is attached to a
     * TSLDrawingSurface for the tactical graphic to be visualized.
     * A NULL pointer may be returned if an error occurs.
     *
     */

    static TSLEntitySet *createArea(const char *sidc, const TSLCoordSet &positions, Modifiers *modifiers, size_t numberOfModifiers, int lineWidth = 1);

    /**
     * Creates Tactical Graphic Lines.
     *
     * The following Tactical Graphic Lines are implemented:
     *
     * Axis Arrows
     *
     *  - Friendly Main Attack
     *    Main Attack: APP-6A(C) May 2011, Table 7-7 page 7-47
     *   - SIDC: GFC*MOLAM-*****
     *  - Supporting Attack
     *    Supporting Attack: APP-6A(C) May 2011, Table 7-7 page 7-48
     *   - SIDC: GFC*MOLAS-*****
     *  - Friendly Planned Attack
     *    Supporting Attack Planned or On Order: APP-6A(C) May 2011, Table 7-7 page 7-49
     *    Friendly Ground Axis On Order with Date and Time (if known) Effective
     *   - SIDC: GFC*MOLAO-*****
     *  - Enemy Confirmed
     *    Enemy Confirmed: APP-6A(C) May 2011, Table 7-7 page 7-51
     *   - SIDC: GHC*MOLAE-*****
     *  - Friendly Airborne/Aviation: APP-6A(C) May 2011, Table 7-7 page 7-45
     *   - SIDC: GFC*MOLAF-*****
     *  - Friendly Feint
     *    Axis of Advance for a Feint: APP-6A(C) May 2011, Table 7-7 page 7-50
     *   - SIDC G*C*MDA---*****
     *
     * Convoys
     *  -Moving convoy
     *  - SIDC: G*C*SLCM--******
     *  - Halted Convoy
     *  - SIDC: G*C*SLCH--******

     *
     * Direction of Attack 7-54
     *  - friendly direction of main attack
     *   - SIDC GFC*MOLDM-*****
     *  - friendly direction of supporting attack
     *   - SIDC GFC*MOLDS-*****
     *  - friendly ground axis planned or On order with effective date and time (if known)
     *   - SIDC GFC*MOLDO-*****
     *  - Direction of attack for a feint
     *   - SIDC G*C*MDF---*****     
     *  - Enemy Confirmed
     *   - SIDC GHC*MOLDG-*****    
     *  - Enemy Templated or Suspected
     *   - SIDC GHC*MOLDR-*****
     *
     * Task Graphics:
     *  - Counterattack (MT/MTV): APP-6A(C) May 2011, Table 7-A-7 page 7-A-11
     *    - SIDC G*T*GK----*****
     *  - Follow and Assume, 7-A-22
     *    - SIDC G*T*GA----*****
     *  - Follow and Support 7-A-22
     *    - SIDC: G*T*GAS---*****
     *
     * Obstacle Effects:
     * - block p 7-165
     *  - SIDC: G*C*BOAEB-*****
     * - disrupt p 7-166
     *  - SIDC: G*C*BOAED-*****
     *
     * Obstacle Control Measure:
     * - Obstacle Bypass Easy p7-171
     *  - SIDC: G*C*BYOE--*****
     * - Obstacle Bypass Difficult p7-172
     *   Note the liestyle used for the bottom is not quite correct.
     *  - SIDC: G*C*BYOD--*****
     * -  Obstacle Bypass Impossible p7-173
     *  - SIDC: G*C*BYOI--*****
     *
     * @param sidc The SIDC code for the tactical graphic. Only the graphic type is currently used.
     * @param positions The TMC positions that define the control points for the tactical graphic.
     * @param modifiers The modifiers that define the text that should be drawn with the tactical graphic. This is not currently implemented.
     * @param numberOfModifiers The number of modifiers passed in.
     * @param spline If set to true some of the tactical graphics are created using a spline algorithm (mainly the Axis arrows).
     * @param lineWidth The width of the lines in pixels. The default is 1.
     *  
     * @return A TSLEntitySet containing the geometry that is to be drawn. This has to be added to a TSLStandardDataLayer that is attached to a
     * TSLDrawingSurface for the tactical graphic to be visualized.
     * A NULL pointer may be returned if an error occurs.
     *
     */
    static TSLEntitySet *createLine(const char *sidc, const TSLCoordSet &positions, Modifiers *modifiers, size_t numberOfModifiers, bool spline, int lineWidth = 1);
  };

}; }; };


#endif