using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class EnemyStatus : MonoBehaviour
{
    Animator animator;
    public float maxHp = 100;
    private float curHp;
    public float exp = 10;
    public bool dead = false;
    public Slider hpSlider;
    private EnemyControl ec;

    private void Awake()
    {
        animator = GetComponent<Animator>();
        ec = GetComponent<EnemyControl>();
        curHp = maxHp;
    }

    public void OnDamage(float damage)
    {
        if (dead)
            return;
        curHp -= damage;
        if(curHp <= 0)
        {
            dead = true;
            ec.state = EnemyControl.State.Dead;
            ec.DropItem();
            ec.DropGold();
            ec.StartCoroutine("Die");
        }
    }
    public void OnEnable()
    {
        curHp = 100;
        dead = false;
        ec.state = EnemyControl.State.Idle;
    }
}
