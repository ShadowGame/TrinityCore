/*
* Copyright (C) 2005 - 2012 MaNGOS <http://www.getmangos.org/>
*
* Copyright (C) 2008 - 2012 TrinityCore <http://www.trinitycore.org/>
*
* Copyright (C) 2011 - 2012 Naios <https://github.com/Naios>
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

#include "ScriptPCH.h"
#include "blackrock_caverns.h"

enum Spells
{
    SPELL_CALL_FOR_HELP             = 82137,
    SPELL_QUAKE                     = 75272,
    SPELL_CHAINS_OF_WOE             = 75539,
    SPELL_CHAINS_OF_WOE_VISUAL      = 75441,
    SPELL_CHAINS_OF_WOE_EFFECT      = 82192,
    SPELL_THE_SKULLCRACKER          = 75543,
    SPELL_WOUNDING_STRIKE           = 69651,
    SPELL_HIDE_MODEL                = 70414,
};

enum Events
{
    EVENT_QUAKE = 1,
    EVENT_CHAINS_OF_WOE,
    EVENT_THE_SCULLCRACKER,
    EVENT_WOUNDING_STRIKE,
};

enum Actions
{
    ACTION_RAZ_START_EVENT,
};

enum Yells
{

};

class boss_romogg_bonecrusher : public CreatureScript
{
public:
    boss_romogg_bonecrusher() : CreatureScript("boss_romogg_bonecrusher") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_romogg_bonecrusherAI (creature);
    }

    struct boss_romogg_bonecrusherAI : public BossAI
    {
        boss_romogg_bonecrusherAI(Creature* creature) : BossAI(creature, DATA_ROMOGG_BONECRUSHER)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        void Reset()
        {   
            _Reset();
            /* Diese Funktionen werden nicht unterstützt. TODO 
            me->DespawnCreaturesInArea(NPC_CHAINS_OF_WOE);
            me->DespawnCreaturesInArea(NPC_ANGERED_EARTH);
			*/
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();

            me->MonsterYell("Dat's what you get! Noting!", LANG_UNIVERSAL, NULL);

            events.ScheduleEvent(EVENT_QUAKE, 13000);
            events.ScheduleEvent(EVENT_CHAINS_OF_WOE, 17000);
            events.ScheduleEvent(EVENT_WOUNDING_STRIKE, 7000);

            DoCastAOE(SPELL_CALL_FOR_HELP);
        }

        void UpdateAI(const uint32 diff)
        {			
            if (!UpdateVictim())
                return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case EVENT_QUAKE:
                        DoCastAOE(SPELL_QUAKE);

                        // Summon Angered Earth
                        if(me->GetMap()->IsHeroic())
                            for(uint8 i=1; i<=5; i++)
								me->SummonCreature(NPC_ANGERED_EARTH,me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),TEMPSUMMON_CORPSE_DESPAWN);

                        events.ScheduleEvent(EVENT_QUAKE, 25000);
                        break;
                    case EVENT_CHAINS_OF_WOE:

                        DoCastAOE(SPELL_CHAINS_OF_WOE);

                        events.ScheduleEvent(EVENT_THE_SCULLCRACKER, 7000);
                        events.ScheduleEvent(EVENT_CHAINS_OF_WOE, 20000);
                        break;
                    case EVENT_THE_SCULLCRACKER:
                        if(me->getVictim())
                            DoCastVictim(SPELL_THE_SKULLCRACKER);

                        break;
                    case EVENT_WOUNDING_STRIKE:
                        DoCastVictim(SPELL_WOUNDING_STRIKE);
                        events.ScheduleEvent(EVENT_WOUNDING_STRIKE, 15000);
                        break;
                    }

                }

                DoMeleeAttackIfReady();
        }

        void JustSummoned(Creature* summon)
        {

            // Wenn Chains of Woe dann Model verstecken.
            if (summon->GetEntry()==NPC_CHAINS_OF_WOE)
            {
                summon->CastSpell(summon, SPELL_HIDE_MODEL, true);
                summon->CastSpell(summon, SPELL_CHAINS_OF_WOE_VISUAL, true);
            }

            summon->AI()->SetMinionInCombat();
        }

        void JustDied(Unit * /*victim*/)
        {
            if(Creature* razTheCrazed = me->FindNearestCreature(NPC_RAZ_THE_CRAZED, 180.f, true))
                razTheCrazed->AI()->DoAction(ACTION_RAZ_START_EVENT);
			/* Diese Funktionen werden nicht unterstützt. TODO
            me->DespawnCreaturesInArea(NPC_CHAINS_OF_WOE);
            me->DespawnCreaturesInArea(NPC_ANGERED_EARTH);
			*/

            me->MonsterYell("Boss Cho'gall not gonna be happy 'bout dis!", LANG_UNIVERSAL, NULL);

            _JustDied();
        }
   };
};

class npc_chains_of_woe : public CreatureScript
{
public:
    npc_chains_of_woe() : CreatureScript("npc_chains_of_woe") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_chains_of_woeAI (creature);
    }

    struct npc_chains_of_woeAI : public ScriptedAI
    {
        npc_chains_of_woeAI(Creature* creature) : ScriptedAI(creature), timerChecktarget(200), CastChainsofWoeVisual(19000), stopEffect(false)
        {
        }
		
        uint32 timerChecktarget;
        uint32 CastChainsofWoeVisual;
        bool stopEffect;

        void UpdateAI(const uint32 diff)
        {
            if(!stopEffect)                     
                HandleEffet(true);

            if (CastChainsofWoeVisual<=diff)
                me->CastSpell(me, SPELL_CHAINS_OF_WOE_VISUAL, true); // Zaubere Chains of Woe Visual nach 19 Sekunden
            else CastChainsofWoeVisual-=diff;
        }

        void JustDied(Unit* killer)
        {
            HandleEffet(false);
        }

        void HandleEffet(bool apply)
        {
            Map::PlayerList const &PlayerList = me->GetMap()->GetPlayers();
            if (!PlayerList.isEmpty())
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    if(apply)
                    {
                        // Zaubere Chains of Woe nur auf NICHT Gamemaster.
                        if(!i->getSource()->HasAura(SPELL_CHAINS_OF_WOE_EFFECT) && !i->getSource()->isGameMaster())
                        {
                            me->AddAura(SPELL_CHAINS_OF_WOE_EFFECT,i->getSource());
                            i->getSource()->SetTarget(me->GetGUID());
                        }
                    }
                    else
                        i->getSource()->RemoveAura(SPELL_CHAINS_OF_WOE_EFFECT);
        }
    };
};

void AddSC_boss_romogg_bonecrusher()
{
    new boss_romogg_bonecrusher();
    new npc_chains_of_woe();
}