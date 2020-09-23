using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerControl : MonoBehaviour
{
    Animator playerAnimator;
    CharacterController characterController;
    private PlayerStatus cs;
    private float movingSpeed = 4.5f;
    private float runningSpeed = 2.5f;
    private float gravity = -20.0f; //가속도
    private float yVelocity = -1; // 속도
    public bool isBlocking = false;
    private Vector3 destination;
    private EnemyStatus target;
    public Transform skillFirePosition;
    public int curSkillNum = 0;
    public GameObject[] skillList;
    static public PlayerControl instance = null;
    PlayerAttack pa;

    private void Awake()
    {
        if(instance == null)
        {
            instance = this;
            DontDestroyOnLoad(gameObject);
        }
        else
        {
            Destroy(gameObject);
        }
    }

    // Start is called before the first frame update
    void Start()
    {
        characterController = GetComponent<CharacterController>();
        playerAnimator = GetComponent<Animator>();
        cs = GetComponent<PlayerStatus>();
        pa = GetComponent<PlayerAttack>();
        target = null;
    }

    // Update is called once per frame
    void Update()
    {
        if (cs.dead)
        {
            return;
        }
        Move();
        yVelocity += gravity * Time.deltaTime;
        playerAnimator.SetFloat("Y_Speed", yVelocity);
        characterController.Move(Vector3.up * yVelocity * Time.deltaTime);
        if (characterController.isGrounded)
        {
            yVelocity = 0;
            playerAnimator.SetBool("Jump", false);
        }
        else
        {
            //playerAnimator.SetBool("Jump", true);
        }
    }

    public void SetDestination(Vector3 destination)
    {
        this.destination = destination;
        target = null;
    }

    public void SetTarget(GameObject enemy)
    {
        target = enemy.GetComponent<EnemyStatus>();
        destination = target.transform.position;
    }

    public void Move()
    {
        if (cs.dead)
        {
            return;
        }
        if (isBlocking)
            return;
        destination.y = transform.position.y;
        if (target != null && !target.dead)
        {
            if ((transform.position - destination).magnitude < 1.5f)
            {
                destination = transform.position;
                transform.forward = (target.transform.position - transform.position).normalized;
                playerAnimator.SetBool("Walk", false);
                Attack();
                return;
            }
        }
        else
        {
            if ((transform.position - destination).magnitude < 0.2f)
            {
                transform.position = destination;
                playerAnimator.SetBool("Walk", false);
                return;
            }
        }
        transform.forward = (destination - transform.position).normalized;
        playerAnimator.SetBool("Walk", true);
        Vector3 snapGround = Vector3.zero;
        if (characterController.isGrounded)
            snapGround = Vector3.down;
        Vector3 moveVector = (destination - transform.position).normalized * movingSpeed * Time.deltaTime;
        if (playerAnimator.GetBool("Run"))
        {
            moveVector *= runningSpeed;
        }
        characterController.Move(moveVector + snapGround);
    }

    public void Attack()
    {
        if (!playerAnimator.GetBool("Run") && !playerAnimator.GetBool("Damaged") && !playerAnimator.GetBool("Jump"))
        {
            playerAnimator.SetBool("Walk", false);
            if (pa.attakEnd)
            {
                playerAnimator.SetBool("Attack", true);
                pa.attakEnd = false;
            }
            if (pa.combo)
            {
                playerAnimator.SetBool("Combo", true);
            }
        }
    }

    public void Jump()
    {
        if (cs.dead)
        {
            return;
        }
        yVelocity = 10;
        playerAnimator.SetBool("Jump", true);
    }

    public void Skill()
    {
        if (cs.dead)
        {
            return;
        }
        isBlocking = true;
        /*
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        Vector3 mousePoint = new Vector3();
        RaycastHit hit;
        if (Physics.Raycast(ray, out hit, 10000f))
        {
            mousePoint = hit.point;
        }
        mousePoint.y = skillFirePosition.position.y;
        skillFirePosition.forward = (mousePoint - skillFirePosition.position).normalized;
        */
        skillList[curSkillNum].SetActive(true);
        playerAnimator.SetTrigger("Skill");
    }

    public void UnBlock()
    {
        if (cs.dead)
        {
            return;
        }
        isBlocking = false;
    }

    public void SetPlayerDirection(Vector3 playerDir)
    {
        transform.forward = playerDir;
    }
    
    public void OnDamage()
    {
        if (cs.dead)
        {
            return;
        }
        playerAnimator.SetTrigger("Damaged");
    }

    public void Die()
    {
        Destroy(this.gameObject);
    }
}
