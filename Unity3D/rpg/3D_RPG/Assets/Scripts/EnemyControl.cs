﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
public class EnemyControl : MonoBehaviour
{
    public enum State
    {
        Idle, Walking, Chasing, Attacking, Return, Dead
    }

    public State state;
    private float delayTime;
    private float walkRange = 10.0f;
    private Animator animator;
    private EnemyStatus cs;
    private Vector3 basePosition;
    private Vector3 destination;
    private Vector3 curDir;
    public Transform target = null;

    public int attackAnimationCount = 1;
    public int[] attackPower;
    public int curPower = 0;
    private NavMeshAgent navMeshAgent;

    private bool attackEnd = true;
    public float attackStopDistance = 1.5f;
    public float retunStartDistance = 20.0f;
    private bool hitEnd = true;
    public bool preAttack = false; // 선공 후공
    public Item dropItem;
    public Gold dropGold;

    private void Awake()
    {
        state = State.Idle;
        destination = transform.position;
        basePosition = transform.position;
        delayTime = 0;
        animator = GetComponent<Animator>();
        cs = GetComponent<EnemyStatus>();
        navMeshAgent = GetComponent<NavMeshAgent>();
    }
    private void OnEnable()
    {
        transform.position = basePosition;
    }

    // Update is called once per frame
    void Update()
    {
        switch (state)
        {
            case State.Idle:
                Idle();
                break;
            case State.Walking:
                Walking();
                break;
            case State.Chasing:
                Chasing();
                break;
            case State.Attacking:
                Attacking();
                break;
            case State.Return:
                Return();
                break;
            case State.Dead:
                break;
        }
    }

    void Idle()
    {
        delayTime -= Time.deltaTime;
        if (state == State.Idle && delayTime <= 0)
        {
            state = State.Walking;
            animator.SetBool("Walk", true);
            delayTime = Random.Range(0.0f, 4.0f);
            Vector2 randomValue = Random.insideUnitCircle * walkRange;
            destination = basePosition + new Vector3(randomValue.x, 0.0f, randomValue.y);
            destination.y = transform.position.y + 100;
            Vector3 dir = Vector3.zero;
            Ray ray = new Ray(destination, new Vector3(0, -1, 0));
            RaycastHit hitInfo;
            if (Physics.Raycast(ray, out hitInfo, 1000.0f, 1 << LayerMask.NameToLayer("Ground")))
            {
                destination.y = hitInfo.point.y;
            }
        }
    }

    void Walking()
    {
        if (navMeshAgent.stoppingDistance >= Vector3.Distance(destination, transform.position))
        {
            state = State.Idle;
            navMeshAgent.isStopped = true;
            navMeshAgent.ResetPath();
            InitParameter();
            delayTime = Random.Range(0.0f, 4.0f);
        }
        else
        {
            navMeshAgent.SetDestination(destination);
        }
    }

    void Return()
    {
        if (navMeshAgent.stoppingDistance >= Vector3.Distance(destination, transform.position))
        {
            state = State.Idle;
            navMeshAgent.isStopped = true;
            navMeshAgent.ResetPath();
            InitParameter();
            delayTime = Random.Range(0.0f, 4.0f);
        }
        else
        {
            navMeshAgent.SetDestination(destination);
        }
    }

    public void SetChasing(Transform target)
    {
        if (!hitEnd)
            return;
        state = State.Chasing;
        InitParameter();
        animator.SetBool("Chase", true);
        this.target = target;
    }

    void Chasing()
    {
        if (target == null)
        {
            state = State.Idle;
            navMeshAgent.isStopped = true;
            navMeshAgent.ResetPath();
            InitParameter();
            return;
        }
        if (!hitEnd)
            return;
        if ((target.position - transform.position).magnitude >= retunStartDistance)
        {
            state = State.Return;
            navMeshAgent.isStopped = true;
            navMeshAgent.ResetPath();
            InitParameter();
            animator.SetBool("Walk", true);
            Vector2 randomValue = Random.insideUnitCircle * walkRange;
            destination = basePosition + new Vector3(randomValue.x, 0.0f, randomValue.y);
            destination.y = transform.position.y + 100;
            Ray ray = new Ray(destination, new Vector3(0, -1, 0));
            RaycastHit hitInfo;
            if (Physics.Raycast(ray, out hitInfo, 1000.0f, 1 << LayerMask.NameToLayer("Ground")))
            {
                destination.y = hitInfo.point.y;
            }
            return;
        }
        // 플레이어와의 거리가 공격거리 이내 일 때 state 를 공격으로 변경
        if (Vector3.Distance(transform.position, target.position) <= attackStopDistance)
        {
            state = State.Attacking;
            navMeshAgent.isStopped = true;
            navMeshAgent.ResetPath();
            InitParameter();
        }
        else// 플레이어와의 거리가 공격거리 보다 크다면 플레이어를 추적
        {
            navMeshAgent.SetDestination(target.transform.position);
        }
    }

    void Attacking()
    {
        if (target == null)
        {
            state = State.Idle;
            InitParameter();
            return;
        }
        Vector3 dir = target.position - transform.position;
        dir.y = 0;
        transform.forward = dir;

        // 플레이어와의 거리가 공격거리 보다 크다면 state를 추적으로 변경
        if (Vector3.Distance(transform.position, target.position) > attackStopDistance && attackEnd)
        {
            state = State.Chasing;
            InitParameter();
            animator.SetBool("Chase", true);
        }
        else // 플레이어를 향해서 무작위 공격 애니메이션으로 공격
        {
            int attackAnim = Random.Range(0, attackAnimationCount) + 1;
            string param = "Attack" + attackAnim.ToString();
            curPower = attackPower[attackAnim - 1];
            animator.SetBool(param, true);
            attackEnd = false;
        }
    }

    public void InitParameter()
    {
        animator.SetBool("Attack1", false);
        animator.SetBool("Attack2", false);
        animator.SetBool("Chase", false);
        animator.SetBool("Walk", false);
        attackEnd = true;
        hitEnd = true;
    }

    public void AttackEnd()
    {
        state = State.Chasing;
        InitParameter();
        animator.SetBool("Chase", true);
    }

    public void HitEnd()
    {
        state = State.Chasing;
        InitParameter();
        animator.SetBool("Chase", true);
    }

    public void OnDamage()
    {
        InitParameter();
        hitEnd = false;
        animator.SetTrigger("Hit");
        navMeshAgent.isStopped = true;
        navMeshAgent.ResetPath();
    }

    public void DropItem()
    {
        float random = Random.Range(0, 100);
        if (random < 50)
        {
            Instantiate(dropItem, transform.position, transform.rotation);
        }
    }

    public void DropGold()
    {
        float random = Random.Range(0, 100);
        if (random < 50)
        {
            Instantiate(dropGold, transform.position, transform.rotation);
        }
    }

    public IEnumerator Die()
    {
        animator.SetTrigger("Dead");
        yield return new WaitForSeconds(1.0f);
        gameObject.SetActive(false);
        yield break;
    }
}