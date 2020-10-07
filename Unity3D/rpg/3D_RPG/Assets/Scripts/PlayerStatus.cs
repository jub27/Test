using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class PlayerStatus : MonoBehaviour
{
    public int level;
    public float maxHp = 100;
    public float curHp;
    public float maxMp = 100;
    public float curMp;
    public float maxExp = 100;
    public float curExp;
    public bool dead = false;
    public int gold;
    public float attack;
    public float defense;

    public Slider expUI;
    public Image hpUI;
    public Image mpUI;
    public Text attack_stat;
    public Text defense_stat;
    public Text Hp_stat;
    public Text Mp_stat;
    public Text level_stat;
    public Text gold_text;

    public static PlayerStatus instance = null;

    // Start is called before the first frame update
    void Start()
    {
        if (instance == null)
        {
            instance = this;
            level = GameManager.instance.cur_user_data.level;
            attack = GameManager.instance.cur_user_data.attack;
            defense = GameManager.instance.cur_user_data.defense;
            maxHp = GameManager.instance.cur_user_data.maxHp;
            curHp = GameManager.instance.cur_user_data.curHp;
            maxExp = GameManager.instance.cur_user_data.maxExp;
            curExp = GameManager.instance.cur_user_data.curExp;
            maxMp = GameManager.instance.cur_user_data.maxMp;
            curMp = GameManager.instance.cur_user_data.curMp;
            level_stat.text = "레벨 : " + level.ToString();
            gold = GameManager.instance.cur_user_data.gold;
            gold_text.text = gold.ToString();
            if (GameManager.instance.cur_user_data.weapon != 0)
            {
                CharacterInfoSystem.instance.weaponSlot.SetItem(GameManager.instance.cur_user_data.weapon);
                CharacterInfoSystem.instance.weaponSlot.equiped = true;
            }
            if (GameManager.instance.cur_user_data.armor != 0)
            {
                CharacterInfoSystem.instance.armorSlot.SetItem(GameManager.instance.cur_user_data.armor);
                CharacterInfoSystem.instance.armorSlot.equiped = true;
            }
            UpdateAttack();
            UpdateDefense();

        }
        else
        {
            Destroy(gameObject);
        }
    }

    // Update is called once per frame
    void Update()
    {
        UpdateUI();
        UpdateStat();
    }

    public void OnDamage(float damage)
    {
        if (dead)
            return;
        curHp -= damage;
        if (curHp <= 0)
        {
            PlayerControl.instance.StartCoroutine("Die");
        }
    }
    public void GetExp(float exp)
    {
        curExp += exp;

        if (curExp >= maxExp)
        {
            level++;
            curExp -= maxExp;
            maxExp = maxExp * 1.2f;
            maxHp = maxHp * 1.2f;
            curHp = maxHp;
            maxMp = maxMp * 1.2f;
            curMp = maxMp;
            level_stat.text = "레벨 : " + level.ToString();
        }
        expUI.value = curExp / maxExp;
    }

    public void UpdateUI()
    {
        hpUI.fillAmount = curHp / maxHp;
        mpUI.fillAmount = curMp / maxMp;
        expUI.value = curExp / maxExp;
    }

    public void DrinkHpPotion(int hp)
    {
        curHp += hp;
        if (curHp >= maxHp)
        {
            curHp = maxHp;
        }
    }
    public void DrinkMpPotion(int mp)
    {
        curMp += mp;
        if (curMp >= maxMp)
        {
            curMp = maxMp;
        }
    }

    public void UpdateAttack()
    {
        if (CharacterInfoSystem.instance.weaponSlot.item_id == 0)
            attack_stat.text = "공격력 : " + attack;
        else
            attack_stat.text = "공격력 : " + (attack + ItemSystem.instance.weapon_dict[CharacterInfoSystem.instance.weaponSlot.item_id].attack).ToString();
    }

    public void UpdateDefense()
    {
        if (CharacterInfoSystem.instance.armorSlot.item_id == 0)
            defense_stat.text = "방어력 : " + defense;
        else
            defense_stat.text = "방어력 : " + (defense + ItemSystem.instance.armor_dict[CharacterInfoSystem.instance.armorSlot.item_id].defense).ToString();
    }

    public void UpdateGold(int gold)
    {
        this.gold += gold;
        gold_text.text = this.gold.ToString();
    }

    void UpdateStat()
    {
        Hp_stat.text = "HP : " + ((int)curHp).ToString() + " / " + ((int)maxHp).ToString();
        Mp_stat.text = "MP : " + ((int)curMp).ToString() + " / " + ((int)maxMp).ToString();
    }
}
