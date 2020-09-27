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
    // Start is called before the first frame update
    void Start()
    {
        animator = GetComponent<Animator>();
        ec = GetComponent<EnemyControl>();
        curHp = maxHp;
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void OnDamage(float damage)
    {
        if (dead)
            return;
        curHp -= damage;
        if(curHp <= 0)
        {
            dead = true;
            ec.DropItem();
            ec.DropGold();
            animator.SetTrigger("Dead");
        }
    }
}
