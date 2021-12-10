#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"

class LevelReward : public PlayerScript
{
    public:
        LevelReward() : PlayerScript("LevelReward") {}

        void OnLevelChanged(Player* player, uint8 oldlevel) override
        {
            uint32 level, reward = 0;

            if (oldlevel < 10 && player->getLevel() == 10)
            {
                level = 10;
                reward = sConfigMgr->GetOption<int32>("Reward.Gold.Level.10", 5);
            }
            else if (oldlevel < 20 && player->getLevel() == 20)
            {
                level = 20;
                reward = sConfigMgr->GetOption<int32>("Reward.Gold.Level.20", 15);
            }
            if (oldlevel < 30 && player->getLevel() == 30)
            {
                level = 30;
                reward = sConfigMgr->GetOption<int32>("Reward.Gold.Level.30", 30);
            }
            else if (oldlevel < 40 && player->getLevel() == 40)
            {
                level = 40;
                reward = sConfigMgr->GetOption<int32>("Reward.Gold.Level.40", 45);
            }
            else if (oldlevel < 50 && player->getLevel() == 50)
            {
                level = 50;
                reward = sConfigMgr->GetOption<int32>("Reward.Gold.Level.50", 60);
            }
            else if (oldlevel < 60 && player->getLevel() == 60)
            {
                level = 60;
                reward = sConfigMgr->GetOption<int32>("Reward.Gold.Level.60", 80);
            }
            else if (oldlevel < 70 && player->getLevel() == 70)
            {
                level = 70;
                reward = sConfigMgr->GetOption<int32>("Reward.Gold.Level.70", 125);
            }
            else if (oldlevel < 80 && player->getLevel() == 80)
            {
                level = 80;
                reward = sConfigMgr->GetOption<int32>("Reward.Gold.Level.80", 250);
            }

            if (level > 0 && reward > 0)
            {
                ChatHandler(player->GetSession()).PSendSysMessage("Congratulations on reaching level %i! Take this reward of %i gold, let it aid you in your travels.", level, reward);
                player->ModifyMoney(reward * GOLD);
            }
        }
};

void AddLevelRewardScripts()
{
    new LevelReward();
}
