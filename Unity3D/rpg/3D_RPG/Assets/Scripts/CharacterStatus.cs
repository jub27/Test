using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class CharacterStatus : MonoBehaviour
{
    Animator animator;
    public float maxHp = 100;
    private float curHp;
    public float exp = 10;
    public bool dead = false;
    public Slider hpSlider;
    // Start is called before the first frame update
    void Start()
    {
        animator = GetComponent<Animator>();
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
            animator.SetTrigger("Dead");
        }
    }
}
