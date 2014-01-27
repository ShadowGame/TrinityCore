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

#include"ScriptPCH.h"
#include"bastion_of_twilight.h"

/*Before activating this script, disable SmartAI!!!*/

/*

enum ChogallSpells
{
	SPELL_SHADOWS_ORDERS				= 81556,
	SPELL_CONVERSION					= 91303,
	SPELL_CONSUME_BLOOD_OF_THE_OLD_GOD	= 82630,
	SPELL_FESTER_BLOOD					= 82299,
	SPELL_UNLEASHED_SHADOWS				= 81571,
	SPELL_FLAMES_ORDERS					= 81171,
	SPELL_FLAMING_DESTRUCTION			= 81527,
	SPELL_FURY_OF_CHOGALL				= 82524,
	SPELL_SUMMON_CORRUPTING_ADHERENT	= 81628,
	SPELL_CORRUPTION_OF_THE_OLD_GOD		= 82361,
	SPELL_DARKENDED_CREATIONS			= 82414,

	//Corrupting Adherent
	SPELL_FESTERING_BLOOD				= 82914,
	SPELL_CORRUPTING_CRASH				= 81689,
	SPELL_DEPRAVITY						= 81713,
	SPELL_SPILLED_BLOOD_OF_THE_OLD_GOD	= 81757,
	SPELL_SPRAYED_CORRUPTION			= 82919,

	//DARKENED CREATION

	SPELL_DEBILLITATING_BEAM			= 82411,

};

enum ChogallEvents
{
	EVENT_SHADOWS_ORDERS				= 81556,
	EVENT_CONVERSION					= 91303,
	EVENT_CONSUME_BLOOD_OF_THE_OLD_GOD	= 82630,
	EVENT_FESTER_BLOOD					= 82299,
	EVENT_UNLEASHED_SHADOWS				= 81571,
	EVENT_FLAMES_ORDERS					= 81171,
	EVENT_FLAMING_DESTRUCTION			= 81527,
	EVENT_FURY_OF_CHOGALL				= 82524,
	EVENT_SUMMON_CORRUPTING_ADHERENT	= 81628,
	EVENT_CORRUPTION_OF_THE_OLD_GOD		= 82361,
	EVENT_DARKENDED_CREATIONS			= 82414,

	//Corrupting Adherent
	EVENT_FESTERING_BLOOD				= 82914,
	EVENT_CORRUPTING_CRASH				= 81689,
	EVENT_DEPRAVITY						= 81713,
	EVENT_SPILLED_BLOOD_OF_THE_OLD_GOD	= 81757,
	EVENT_SPRAYED_CORRUPTION			= 82919,

	//DARKENED CREATION

	EVENT_DEBILLITATING_BEAM			= 82411,
};
*/

class boss_chogall: public CreatureScript
{
public:
    boss_chogall () :
            CreatureScript("boss_chogall")
    {
    }

    CreatureAI* GetAI (Creature* creature) const
    {
        return new boss_chogallAI(creature);
    }

    struct boss_chogallAI: public ScriptedAI
    {
        boss_chogallAI (Creature* creature) :
                ScriptedAI(creature)
        {
            pInstance = creature->GetInstanceScript();
        }

        InstanceScript* pInstance;

        void Reset ()
        {
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

void AddSC_boss_chogall ()
{
    new boss_chogall();
}
