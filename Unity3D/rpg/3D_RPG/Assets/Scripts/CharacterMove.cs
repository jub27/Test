using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharacterMove : MonoBehaviour
{
    Animator playerAnimator;
    CharacterController characterController;
    private float movingSpeed = 4.5f;
    private float runningSpeed = 2.5f;
    private float gravity = -20.0f; //가속도
    private float yVelocity = -1; // 속도
    private bool isJumping = false;
    public bool isRunning = false;
    public bool isBlocking = false;

    // Start is called before the first frame update
    void Start()
    {
        characterController = GetComponent<CharacterController>();
        playerAnimator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
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
        yVelocity = 10;
        playerAnimator.SetBool("Jump", true);
    }

    public void Block()
    {
        isBlocking = true;
        playerAnimator.SetBool("Block", isBlocking);
    }

    public void UnBlock()
    {
        isBlocking = false;
        playerAnimator.SetBool("Block", isBlocking);
    }

    public void SetPlayerDirection(Vector3 playerDir)
    {
        transform.forward = playerDir;
    }
    
    public void OnDamage()
    {
        playerAnimator.SetTrigger("Damaged");
    }
}
