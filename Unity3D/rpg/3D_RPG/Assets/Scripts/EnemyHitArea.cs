﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyHitArea : MonoBehaviour
{
    EnemyControl ec;
    EnemyStatus cs;
    public DamageText damageText;
    public Transform damagePrintPos;
    private void Awake()
    {
        ec = transform.root.gameObject.GetComponent<EnemyControl>();
        cs = transform.root.gameObject.GetComponent<EnemyStatus>();
    }

    public void OnEnable()
    {
        GetComponent<Collider>().enabled = true;
    }

    private void OnTriggerEnter(Collider other)
    {
        ec.SetChasing(PlayerStatus.instance.transform);
        if(transform.root.gameObject.GetComponent<EnemyAttack>() != null)
            transform.root.gameObject.GetComponent<EnemyAttack>().AttackDisable();

        float damage = 0;
        if(other.transform.root.GetComponent<SkillAttack>() != null)
        {
            damage = other.transform.root.GetComponent<SkillAttack>().power;
        }
        else
        {
            damage = other.transform.root.GetComponent<PlayerStatus>().attack;
            if (CharacterInfoSystem.instance.weaponSlot.item_id != 0)
                damage += ItemSystem.instance.weapon_dict[CharacterInfoSystem.instance.weaponSlot.item_id].attack;
        }

        cs.OnDamage(damage);
        DamageText dt = Instantiate(damageText, damagePrintPos.position, Camera.main.transform.rotation);
        dt.damage = damage;
        if (!cs.dead)
        {
            ec.OnDamage();
        }
        else
        {
            PlayerControl.instance.GetComponent<PlayerStatus>().GetExp(cs.exp);
            GetComponent<Collider>().enabled = false;
        }
    }
}
