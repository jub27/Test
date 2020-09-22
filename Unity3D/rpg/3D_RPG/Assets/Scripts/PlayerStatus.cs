using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class PlayerStatus : MonoBehaviour
{
    Animator animator;
    public int level;
    public float maxHp = 100;
    public float curHp;
    public float maxMp = 100;
    public float curMp;
    public float maxExp = 100;
    public float curExp;
    public bool dead = false;
    public Slider expUI;
    public Image hpUI;
    public Image mpUI;
    public float attack;
    public float defense;
    public Text attack_stat;
    public Text defense_stat;
    public Text level_stat;

    // Start is called before the first frame update
    void Start()
    {
        if (GameManager.instance.is_loaded)
        {
            level = GameManager.instance.load_data.level;
            attack = GameManager.instance.load_data.attack;
            defense = GameManager.instance.load_data.defense;
            maxHp = GameManager.instance.load_data.maxHp;
            curHp = GameManager.instance.load_data.curHp;
            maxExp = GameManager.instance.load_data.maxExp;
            curExp = GameManager.instance.load_data.curExp;
            maxMp = GameManager.instance.load_data.maxMp;
            curMp = GameManager.instance.load_data.curMp;
            attack_stat.text = "공격력 : " + attack.ToString();
            defense_stat.text = "방어력 : " + defense.ToString();
            level_stat.text = "레벨 : " + level.ToString();
        }
        else
        {
            level = 1;
            animator = GetComponent<Animator>();
            curHp = maxHp;
            curMp = maxMp;
            curExp = 0;
            attack = 10;
            attack_stat.text = "공격력 : " + attack.ToString();
            defense = 0;
            defense_stat.text = "방어력 : " + defense.ToString();
            level_stat.text = "레벨 : " + level.ToString();
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
            dead = true;
            animator.SetTrigger("Dead");
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
        if(curHp >= maxHp)
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
}
