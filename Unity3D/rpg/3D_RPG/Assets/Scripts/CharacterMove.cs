using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharacterMove : MonoBehaviour
{
    Animator playerAnimator;
    CharacterController characterController;
    private CharacterStatus cs;
    private float movingSpeed = 4.5f;
    private float runningSpeed = 2.5f;
    private float gravity = -20.0f; //가속도
    private float yVelocity = -1; // 속도
    private bool isJumping = false;
    public bool isRunning = false;
    public bool isBlocking = false;
    public Transform skillFirePosition;
    public int curSkillNum = 0;
    public GameObject[] skillList;

    // Start is called before the first frame update
    void Start()
    {
        characterController = GetComponent<CharacterController>();
        playerAnimator = GetComponent<Animator>();
        cs = GetComponent<CharacterStatus>();
    }

    // Update is called once per frame
    void Update()
    {
        if (cs.dead)
        {
            return;
        }
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
            playerAnimator.SetBool("Jump", true);
        }
    }

    public void Move(Vector3 dir)
    {
        if (cs.dead)
        {
            return;
        }
        if (isBlocking)
            return;
        Vector3 snapGround = Vector3.zero;
        if (characterController.isGrounded)
            snapGround = Vector3.down;
        Vector3 moveVector = dir * movingSpeed * Time.deltaTime;
        if (isRunning)
            moveVector *= runningSpeed;
        characterController.Move(moveVector + snapGround);
        playerAnimator.SetFloat("MoveSpeed", moveVector.magnitude);
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
        Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
        Vector3 mousePoint = new Vector3();
        RaycastHit hit;
        if (Physics.Raycast(ray, out hit, 10000f))
        {
            mousePoint = hit.point;
        }
        mousePoint.y = skillFirePosition.position.y;
        skillFirePosition.forward = (mousePoint - skillFirePosition.position).normalized;
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
