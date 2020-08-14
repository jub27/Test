using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyControl : MonoBehaviour
{
    enum State
    {
        Idle, Walking, Chasing, Return, Attacking, Blocking
    }

    private State state;
    int stateNum;
    private float delayTime;
    Animator animator;

    // Start is called before the first frame update
    void Start()
    {
        delayTime = 0;
        stateNum = 0;
        animator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        delayTime -= Time.deltaTime;
        if(delayTime <= 0)
        {
            delayTime = Random.Range(2.0f, 4.0f);
            stateNum = Random.Range(0, 5);
        }
        switch (stateNum)
        {
            case 0:
                state = State.Idle;
                break;
            case 1:
                state = State.Walking;
                animator.SetFloat("MoveSpeed", 0.05f);
                break;
            case 2:
                state = State.Chasing;
                animator.SetFloat("MoveSpeed", 0.2f);
                break;
            case 3:
                state = State.Return;
                animator.SetFloat("MoveSpeed", 0.05f);
                break;
            case 4:
                state = State.Attacking;
                animator.SetTrigger("Attack");
                break;
            case 5:
                state = State.Blocking;
                animator.SetBool("Block", true);
                break;
        }
    }
}
