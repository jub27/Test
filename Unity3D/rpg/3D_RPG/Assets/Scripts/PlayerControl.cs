using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerControl : MonoBehaviour
{
    private Animator playerAnimator;
    private CharacterController characterController;
    private PlayerAttack pa;
    private PlayerStatus cs;
    private float movingSpeed = 4.5f;
    private float runningSpeed = 2.5f;
    private float gravity = -20.0f; //가속도
    private float yVelocity = -1; // 속도
    private Vector3 destination;
    private EnemyStatus target;
    public SkillAttack[] SkillList;
    static public PlayerControl instance = null;

    private void Awake()
    {
        if (instance == null)
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
        if (!playerAnimator.GetBool("Run") && !playerAnimator.GetBool("Damaged"))
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

    public bool Skill(int i)
    {
        if (SkillCoolTimeSystem.instance.skillCools[i].skill_cool && !playerAnimator.GetBool("Run") && !playerAnimator.GetBool("Damaged") && !playerAnimator.GetBool("Attack") && !playerAnimator.GetBool("Walk"))
        {
            Vector3 position = new Vector3();
            Quaternion rotation = new Quaternion();
            if (SkillList[i].skillType == 1)
            {
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit hit;
                if (Physics.Raycast(ray, out hit, 10000f, 1 << LayerMask.NameToLayer("Ground")))
                {
                    position = hit.point;
                }
                Vector3 dir = (position - SkillFirePosition.instance.transform.position).normalized;
                SkillFirePosition.instance.transform.forward = dir;
                transform.forward = (new Vector3(position.x, transform.position.y, position.z) - transform.position).normalized;
                position = SkillFirePosition.instance.transform.position;
                rotation = SkillFirePosition.instance.transform.rotation;
            }
            else
            {
                Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
                RaycastHit hit;
                if (Physics.Raycast(ray, out hit, 10000f, 1 << LayerMask.NameToLayer("Ground")))
                {
                    position = hit.point;
                }
            }
            playerAnimator.SetTrigger("Skill");
            Instantiate(SkillList[i], position, rotation);
            SkillCoolTimeSystem.instance.skillCools[i].CoolStart();
            return true;
        }
        return false;
    }

    public void OnDamage()
    {
        if (cs.dead)
        {
            return;
        }
        playerAnimator.SetTrigger("Damaged");
    }

    public IEnumerator Die()
    {
        playerAnimator.SetTrigger("Dead");
        cs.dead = true;
        yield return new WaitForSeconds(1.0f);
        SceneManager.LoadScene("Town");
        cs.dead = false;
        cs.curExp = 0;
        cs.curHp = cs.maxHp;
        cs.curMp = cs.maxMp;
        cs.UpdateUI();
        playerAnimator.SetTrigger("Hukkatsu");
        yield break;
    }
}
