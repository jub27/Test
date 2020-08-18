using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyControl : MonoBehaviour
{
    enum State
    {
        Idle, Walking, Chasing, Attacking
    }

    private State state;
    private float delayTime;
    private float walkSpeed = 3.0f;
    private float runSpeed = 4.0f;
    private float walkRange = 8.0f;
    private float yVelocity = -1;
    private float gravity = -1.0f;
    private Animator animator;
    private CharacterStatus cs;
    private Vector3 basePosition;
    private Vector3 destination;
    private Vector3 curDir;
    private Transform target = null;
    public Transform rayForward;
    private bool attackEnd = true;
    private float walkStopDistance = 1.0f;
    private float attackStopDistance = 1.8f;
    private bool hitEnd = true;
    private int randomDir;
    public bool preAttack = false;
    // Start is called before the first frame update
    void Start()
    {
        state = State.Idle;
        destination = transform.position;
        basePosition = transform.position;
        delayTime = 0;
        animator = GetComponent<Animator>();
        cs = GetComponent<CharacterStatus>();
        randomDir = Random.Range(0, 2);
    }

    // Update is called once per frame
    void Update()
    {
        if (cs.dead)
        {
            return;
        }

        if (CheckBelow() > 0.1f)
        {
            transform.position += new Vector3(0, gravity * Time.deltaTime, 0);
        }
        else if(CheckBelow() <= 0)
        {
            transform.position -= new Vector3(0, gravity * Time.deltaTime, 0);
        }

        delayTime -= Time.deltaTime;
        if(state == State.Idle && delayTime <= 0)
        {
            state = State.Walking;
            animator.SetBool("Walk", true);
            delayTime = Random.Range(0.0f, 4.0f);
            Vector2 randomValue = Random.insideUnitCircle * walkRange;
            destination = basePosition + new Vector3(randomValue.x, 0.0f, randomValue.y);
            destination.y = transform.position.y;
        }

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
        }
    }

    void Walking()
    {
        destination.y = transform.position.y;
        if ( Vector3.Distance(transform.position, destination) <= walkStopDistance)
        {
            state = State.Idle;
            InitParameter();
            delayTime = Random.Range(0.0f, 4.0f);
        }
        else
        {
            if (CollisionAvoid())
            {
                curDir = transform.right;
                curDir.y = 0;
                curDir.Normalize();
                transform.position += curDir * walkSpeed * Time.deltaTime;
            }
            else
            {
                curDir = destination - transform.position;
                curDir.y = 0;
                curDir.Normalize();
                transform.forward = curDir;
                transform.position += curDir * walkSpeed * Time.deltaTime;
            }

        }
    }
    void Idle()
    {

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
            InitParameter();
            return;
        }
        if (!hitEnd)
            return;
        if (Vector3.Distance(transform.position, target.position) <= attackStopDistance)
        {
            state = State.Attacking;
            InitParameter();
            animator.SetBool("Attack", true);
            attackEnd = false;

        }
        else
        {
            if (CollisionAvoid())
            {
                if (randomDir == 0)
                    curDir = transform.right;
                else
                    curDir = -transform.right;
                curDir.y = 0;
                curDir.Normalize();
                transform.position += curDir * runSpeed * Time.deltaTime;
            }
            else
            {
                curDir = target.position - transform.position;
                curDir.y = 0;
                curDir.Normalize();

                transform.forward = curDir;
                transform.position += curDir * runSpeed * Time.deltaTime;
            }

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
        if (Vector3.Distance(transform.position, target.position) > attackStopDistance && attackEnd)
        {
            state = State.Chasing;
            InitParameter();
            animator.SetBool("Chase", true);
        }
    }

    public void InitParameter()
    {
        animator.SetBool("Attack", false);
        animator.SetBool("Jump", false);
        animator.SetBool("Block", false);
        animator.SetBool("Chase", false);
        animator.SetBool("Walk", false);
        attackEnd = true;
        hitEnd = true;
    }

    public void AttackEnd()
    {
        print("@@@");
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
    }

    public void Die()
    {
        Destroy(this.gameObject);
    }

    float CheckBelow()
    {
        Ray ray = new Ray(transform.position, Vector3.down);
        RaycastHit hitInfo;
        if(Physics.Raycast(ray, out hitInfo, 10000.0f, 1 << LayerMask.NameToLayer("Ground")))
        {
            return hitInfo.distance;
        }
        return 0;
    }

    bool CollisionAvoid()
    {
        Vector3 dir = Vector3.zero;
        Ray ray = new Ray(rayForward.position, transform.forward);
        RaycastHit hitInfo;
        if (Physics.Raycast(ray, out hitInfo, 2.0f, 1 << LayerMask.NameToLayer("Enemy")))
        {
            return true;
        }
        return false;
    }
}