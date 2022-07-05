#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"

uint32 levelRewardGold[8];
uint32 levelRewardItem[8];

class LevelReward : public PlayerScript
{
public:
    LevelReward() : PlayerScript("LevelReward") {}

    void OnLevelChanged(Player* player, uint8 oldlevel) override
    {
        uint32 level = 0;
        uint32 gold = 0;
        uint32 itemId = 0;

        if (oldlevel < 10 && player->getLevel() == 10)
        {
            level = 10;
            gold = levelRewardGold[0];
            itemId = levelRewardItem[0];
        }
        else if (oldlevel < 20 && player->getLevel() == 20)
        {
            level = 20;
            gold = levelRewardGold[1];
            itemId = levelRewardItem[1];
        }
        if (oldlevel < 30 && player->getLevel() == 30)
        {
            level = 30;
            gold = levelRewardGold[2];
            itemId = levelRewardItem[2];
        }
        else if (oldlevel < 40 && player->getLevel() == 40)
        {
            level = 40;
            gold = levelRewardGold[3];
            itemId = levelRewardItem[3];
        }
        else if (oldlevel < 50 && player->getLevel() == 50)
        {
            level = 50;
            gold = levelRewardGold[4];
            itemId = levelRewardItem[4];
        }
        else if (oldlevel < 60 && player->getLevel() == 60)
        {
            level = 60;
            gold = levelRewardGold[5];
            itemId = levelRewardItem[5];
        }
        else if (oldlevel < 70 && player->getLevel() == 70)
        {
            level = 70;
            gold = levelRewardGold[6];
            itemId = levelRewardItem[6];
        }
        else if (oldlevel < 80 && player->getLevel() == 80)
        {
            level = 80;
            gold = levelRewardGold[7];
            itemId = levelRewardItem[7];
        }

        if (level > 0)
        {
            if (gold > 0)
            {
                ChatHandler(player->GetSession()).PSendSysMessage("Congratulations on reaching level %i! Take this reward of %i gold, let it aid you in your travels.", level, gold);
                player->ModifyMoney(gold * GOLD);
            }

            if (itemId > 0)
            {
                if (const ItemTemplate* item = sObjectMgr->GetItemTemplate(itemId))
                {
                    std::string sub = Acore::StringFormat("Level %i", level);
                    std::string bod = Acore::StringFormat("Congratulations on reaching level %i! Take this reward, let it aid you in your travels.", level);
                    SendMailTo(player, sub, bod, itemId, 1);
                }
            }
        }
    }

private:
    void SendMailTo(Player* receiver, std::string subject, std::string body, uint32 itemId, uint32 itemCount)
    {
        uint32 guid = receiver->GetGUID().GetCounter();

        CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
        MailDraft* mail = new MailDraft(subject, body);
        ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(itemId);
        if (pProto)
        {
            Item* mailItem = Item::CreateItem(itemId, itemCount);
            if (mailItem)
            {
                mailItem->SaveToDB(trans);
                mail->AddItem(mailItem);
            }
        }

        mail->SendMailTo(trans, receiver ? receiver : MailReceiver(guid), MailSender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM), MAIL_CHECK_MASK_RETURNED);
        delete mail;
        CharacterDatabase.CommitTransaction(trans);
    }
};

class LevelRewardConfig : WorldScript
{
public:
    LevelRewardConfig() : WorldScript("LevelRewardConfig") {}

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        // Level 10
        levelRewardGold[0] = sConfigMgr->GetOption<uint32>("LevelReward.Level.10.Gold", 5);
        levelRewardItem[0] = sConfigMgr->GetOption<uint32>("LevelReward.Level.10.Item", 0);

        // Level 20
        levelRewardGold[1] = sConfigMgr->GetOption<uint32>("LevelReward.Level.20.Gold", 15);
        levelRewardItem[1] = sConfigMgr->GetOption<uint32>("LevelReward.Level.20.Item", 0);

        // Level 30
        levelRewardGold[2] = sConfigMgr->GetOption<uint32>("LevelReward.Level.30.Gold", 30);
        levelRewardItem[2] = sConfigMgr->GetOption<uint32>("LevelReward.Level.30.Item", 0);

        // Level 40
        levelRewardGold[3] = sConfigMgr->GetOption<uint32>("LevelReward.Level.40.Gold", 45);
        levelRewardItem[3] = sConfigMgr->GetOption<uint32>("LevelReward.Level.40.Item", 0);

        // Level 50
        levelRewardGold[4] = sConfigMgr->GetOption<uint32>("LevelReward.Level.50.Gold", 60);
        levelRewardItem[4] = sConfigMgr->GetOption<uint32>("LevelReward.Level.50.Item", 0);

        // Level 60
        levelRewardGold[5] = sConfigMgr->GetOption<uint32>("LevelReward.Level.60.Gold", 80);
        levelRewardItem[5] = sConfigMgr->GetOption<uint32>("LevelReward.Level.60.Item", 0);

        // Level 70
        levelRewardGold[6] = sConfigMgr->GetOption<uint32>("LevelReward.Level.70.Gold", 125);
        levelRewardItem[6] = sConfigMgr->GetOption<uint32>("LevelReward.Level.70.Item", 0);

        // Level 80
        levelRewardGold[7] = sConfigMgr->GetOption<uint32>("LevelReward.Level.80.Gold", 250);
        levelRewardItem[7] = sConfigMgr->GetOption<uint32>("LevelReward.Level.80.Item", 0);
    }
};

void AddLevelRewardScripts()
{
    new LevelReward();
    new LevelRewardConfig();
}
