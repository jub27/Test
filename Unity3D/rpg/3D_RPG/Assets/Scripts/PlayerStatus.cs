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
            attack_stat.text = "공격력 : " + attack.ToString();
            defense_stat.text = "방어력 : " + defense.ToString();
            level_stat.text = "레벨 : " + level.ToString();
            gold = GameManager.instance.cur_user_data.gold;
            gold_text.text = gold.ToString();
            if (GameManager.instance.cur_user_data.weapon != 0)
            {
                CharacterInfoSystem.instance.weaponSlot.SetItem(GameManager.instance.cur_user_data.weapon);
                CharacterInfoSystem.instance.weaponSlot.equiped = true;
                UpdateAttack(ItemSystem.instance.weapon_dict[GameManager.instance.cur_user_data.weapon].attack);
            }
            if (GameManager.instance.cur_user_data.armor != 0)
            {
                CharacterInfoSystem.instance.armorSlot.SetItem(GameManager.instance.cur_user_data.armor);
                CharacterInfoSystem.instance.armorSlot.equiped = true;
                UpdateDefense(ItemSystem.instance.armor_dict[GameManager.instance.cur_user_data.armor].defense);
            }

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

    public void UpdateAttack(float attack)
    {
        this.attack += attack;
        attack_stat.text = "공격력 : " + this.attack.ToString();
    }

    public void UpdateDefense(float defense)
    {
        this.defense += defense;
        defense_stat.text = "방어력 : " + this.defense.ToString();
    }

    public void UpdateGold(int gold)
    {
        this.gold += gold;
        gold_text.text = this.gold.ToString();
    }
}
