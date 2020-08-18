using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharacterStatus : MonoBehaviour
{
    Animator animator;
    public float hp = 100;
    public bool dead = false;
    // Start is called before the first frame update
    void Start()
    {
        animator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {

        
    }

    public void OnDamage(float damage)
    {
        if (dead)
            return;
        hp -= damage;
        if(hp <= 0)
        {
            dead = true;
            animator.SetTrigger("Dead");
        }
    }
}
