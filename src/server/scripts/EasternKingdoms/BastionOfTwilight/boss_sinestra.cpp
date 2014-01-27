/*
 * Copyright (C) 2005 - 2011 MaNGOS <http://www.getmangos.org/>
 *
 * Copyright (C) 2008 - 2011 TrinityCore <http://www.trinitycore.org/>
 *
 * Copyright (C) 2011 - 2013 ArkCORE <http://www.arkania.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
/*
#include"ScriptPCH.h"
#include"bastion_of_twilight.h"

/*!Only available in heroic mode!*/
/*Before activating this script, disable SmartAI!!!*/
/*
enum SinastraSpells
{
	SPELL_CALL_FLAMES			= 95855,
	SPELL_FLAME_BREATH			= 92944,
	SPELL_MANA_BARRIER			= 87299,
	SPELL_WRACK					= 92956,
	SPELL_TWILIGHT_EXTINCTION	= 86227,
	SPELL_TWILIGHT_BLAST		= 92949,
	
	//Calen
	SPELL_ESSENCE_OF_THE_RED	= 87946,
	SPELL_FLERY_BARRIER			= 87231,
	SPELL_FLERY_RESOLVE			= 87221,
	SPELL_PYHRRIC_FOCUS			= 92941,

	//Pulsing Twilight Egg

	SPELL_TWILIGHT_CARAPACE		= 87654,
	
	//Shadow Orb

	SPELL_TWILIGHT_PULSE		= 92959,
	SPELL_TWILIGHT_SCLICER		= 92954,

	//Twilight Drake

	SPELL_ABSORB_ESSENCE		= 90107,
	SPELL_TWILIGHT_BREATH		= 92942,

	//Twilight Whelp

	SPELL_TWILIGHT_SPIT			= 92953,

	//Twilight Spitecaller

	SPELL_UNLEASH_ESSENCE		= 92947,
	SPELL_IDOMITABLE			= 92946,




};

enum SinastraEvents
{
	EVENT_CALL_FLAMES,
	EVENT_FLAME_BREATH,
	EVENT_MANA_BARRIER,
	EVENT_WRACK,
	EVENT_TWILIGHT_EXTINCTION,
	EVENT_TWILIGHT_BLAST,
	
	//Calen
	EVENT_ESSENCE_OF_THE_RED,
	EVENT_FLERY_BARRIER,
	EVENT_FLERY_RESOLVE,
	EVENT_PYHRRIC_FOCUS,

	//Pulsing Twilight Egg

	EVENT_TWILIGHT_CARAPACE,
	
	//Shadow Orb

	EVENT_TWILIGHT_PULSE,
	EVENT_TWILIGHT_SCLICER,

	//Twilight Drake

	EVENT_ABSORB_ESSENCE,
	EVENT_TWILIGHT_BREATH,

	//Twilight Whelp

	EVENT_TWILIGHT_SPIT,

	//Twilight Spitecaller

	EVENT_UNLEASH_ESSENCE,
	EVENT_IDOMITABLE,
};
*/

class boss_sinestra: public CreatureScript
{
public:
    boss_sinestra () :
    CreatureScript("boss_sinestra")
    {
    }

    CreatureAI* GetAI (Creature* creature) const
    {
        return new boss_sinestraAI(creature);
    }

    struct boss_sinestraAI: public ScriptedAI
    {
        boss_sinestraAI (Creature* creature) :
        ScriptedAI(creature)
        {
            pInstance = creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
		uint32 phase;

        void Reset ()
        {
			phase = 1;
        }

        void EnterCombat (Unit* /*pWho*/)
        {
        }

        void UpdateAI (const uint32 Diff)
        {
            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_sinestra ()
{
    new boss_sinestra();
}
