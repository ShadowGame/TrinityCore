#include "ScriptPCH.h"
#include "bastion_of_twilight.h"

enum Events
{
	EVENT_Beserker                             		= 1,//alle
	EVENT_Blendende_Zerstoerung    	   			    = 2,//alle
	EVENT_Einhuellende_Magie_10		                = 3,//10
	EVENT_Einhuellende_Magie_25        	            = 4,//25
	EVENT_Einhuellende_Magie_10hc                   = 5,//hc_10 
	EVENT_Einhuellende_Magie_25hc		        	= 6,//hc_25
	EVENT_Wunderbare_Flammen_25	   					= 7,//25
	EVENT_Wunderbare_Flammen_25hc  					= 8,//hc_25
	EVENT_Wunderbare_Flammen					    = 9,//10
	EVENT_Wunderbare_Flammen_10hc				    = 10,//hc_10
	EVENT_Zwielichtdruckwelle_25				    = 11,//25
	EVENT_Zwielichtdruckwelle_10				    = 12,//10
	EVENT_Zwielichtdruckwelle_25hc   		        = 13,//hc_25
	EVENT_Zwielichtdruckwelle_10hc                  = 14,//hc_10
	ECENT_Zwielichtverschiebung_hc					= 15,//hc
	EVENT_Zwielichtverschiebung						= 16,//alle
	
};
	
	

class boss_theralion : public CreatureScript
{
public:
    boss_theralion() : CreatureScript("boss_theralion") { }

    struct boss_theralionAI : public BossAI
    {
        boss_theralionAI(Creature *c) : BossAI(c,DATA_THERALION)
        {
            pInstance = c->GetInstanceScript();
			baseSpeed = c->GetSpeedRate(MOVE_RUN);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_DISPEL, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_FEAR, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_PERIODIC_HEAL, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_STUN, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_ROOT, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_DECREASE_SPEED, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SHIELD, true);
            me->ApplySpellImmune(0, IMMUNITY_ID, 13810, true); // Frost Trap
            me->ApplySpellImmune(0, IMMUNITY_ID, 55741, true); // Desecration Rank 1
            me->ApplySpellImmune(0, IMMUNITY_ID, 68766, true); // Desecration Rank 2

			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

			pos = me->GetHomePosition();
        }

        InstanceScript* pInstance;

        void InitializeAI()
        {
			if (pInstance)
                pInstance->SetData(DATA_THERALION, NOT_STARTED);
        }

        void Reset()
        {
			_Reset();
			me->SetSpeed(MOVE_RUN, baseSpeed, true);
			me->SetVisible(true);
			me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			me->DeleteThreatList();
			me->ClearInCombat();


            if (pInstance)
                pInstance->SetData(DATA_THERALION, NOT_STARTED);
        }

		void JustReachedHome()
        {
			_JustReachedHome();
        }

        void EnterCombat(Unit* /*who*/)
        {
			me->setActive(true);
			fire=0;
			me->SetSpeed(MOVE_RUN, baseSpeed, true);
			//if (me->GetMap()->IsHeroic()) // NHC Has no Enrage
			events.ScheduleEvent(EVENT_Beserker, 420000);

			events.ScheduleEvent(EVENT_Blendende_Zerstoerung, urand(10000, 15000));
			events.ScheduleEvent(EVENT_Einhuellende_Magie_10, urand(15000, 20000));		
			events.ScheduleEvent(EVENT_Wunderbare_Flammen, urand(35000, 20000));
			events.ScheduleEvent(EVENT_Zwielichtdruckwelle_10, urand(25000, 30000));
			events.ScheduleEvent(EVENT_Zwielichtverschiebung, urand(10000, 15000));
            if (pInstance)
                pInstance->SetData(DATA_THERALION, IN_PROGRESS);
        }
        void MoveInLineOfSight(Unit* victim) {}     
		
        void AttackStart(Unit* victim)
        {
			if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                return;

            if (victim && me->Attack(victim, true))
			{
				me->GetMotionMaster()->MoveChase(victim);
				me->AddThreat(victim, 0.0f);
				me->SetInCombatWith(victim);
			}
        }


        void JustDied(Unit* killer)
        {
			/*
			Map::PlayerList const &PlList = me->GetMap()->GetPlayers();

				if (PlList.isEmpty())
					return;

				for (Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
					if (Player* player = i->getSource())
						player->ModifyCurrency(396, RAID_MODE(35,35,70,70));
*/
            if (pInstance)
                pInstance->SetData(DATA_THERALION, DONE);

			_JustDied();
        }

        void UpdateAI(const uint32 diff)
        {
           events.Update(diff);
			
			if (!UpdateVictim())
                return;

			if (me->GetDistance(pos) > 100.0f)
			{
				me->SetFullHealth();
				EnterEvadeMode();
			}
			
			if (fire){
			    
				fire = 0;
				
				if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
								DoCast(pTarget, SPELL_Blendende_Zerstoerung);
		        }
               
			while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
						
					case EVENT_Beserker:
							DoCast(me, SPELL_Beserker, true);
                            break;
							/*
						case EVENT_Blendende_Zerstoerung:
						if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
								DoCast(pTarget, SPELL_Blendende_Zerstoerung, true);
							events.ScheduleEvent(EVENT_Blendende_Zerstoerung, urand(20000, 23000));
                            break;
							*/
						case EVENT_Zwielichtdruckwelle_10:
							fire= 1;
							if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
								DoCast(pTarget, SPELL_Zwielichtdruckwelle_10, true);
							events.ScheduleEvent(EVENT_Zwielichtdruckwelle_10, urand(10000, 14000));
                            break;
						case EVENT_Zwielichtverschiebung:
							DoCast(me, SPELL_Zwielichtverschiebung);
							events.ScheduleEvent(EVENT_Zwielichtverschiebung, urand(10000, 15000));		
                            break;
						case EVENT_Einhuellende_Magie_10:
							DoCast(me, SPELL_Einhuellende_Magie_10);
							events.ScheduleEvent(EVENT_Einhuellende_Magie_10, urand(21000, 23000));
							break;
                 
			
                    }
                }
	
		   DoMeleeAttackIfReady();
        }
        private:
            float baseSpeed;
			int fire;
			Position pos;
    };


	CreatureAI* GetAI(Creature* creature) const
    {
      return new boss_theralionAI(creature);
    }
};

void AddSC_boss_theralion()
{
    new boss_theralion();
}
