using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerInput : MonoBehaviour
{
    CharacterController characterController;
    PlayerControl cm;
    Animator playerAnimator;
    PlayerAttack pa;
    // Start is called before the first frame update
    void Start()
    {
        characterController = GetComponent<CharacterController>();
        cm = GetComponent<PlayerControl>();
        playerAnimator = GetComponent<Animator>();
        pa = GetComponent<PlayerAttack>();
    }

    // Update is called once per frame
    void Update()
    {
        float v = Input.GetAxis("Vertical");
        float h = Input.GetAxis("Horizontal");

        if (Input.GetKeyDown(KeyCode.LeftShift) || Input.GetKey(KeyCode.LeftShift))
        {
            playerAnimator.SetBool("Run", true);
        }
        if (Input.GetKeyUp(KeyCode.LeftShift))
        {
            playerAnimator.SetBool("Run", false);
        }

        if( (h != 0 || v != 0) && !playerAnimator.GetBool("Attack"))
        {
            Vector3 vVec = Camera.main.transform.forward;
            vVec.y = 0;
            vVec.Normalize();
            vVec *= v;
            Vector3 hVec = Camera.main.transform.right;
            hVec.y = 0;
            hVec.Normalize();
            hVec *= h;
            cm.Move(vVec + hVec);
        }
        else
        {
            playerAnimator.SetBool("Run", false);
            playerAnimator.SetBool("Walk", false);
        }
        if(characterController.isGrounded && Input.GetKeyDown(KeyCode.Space))
        {
            if (!playerAnimator.GetBool("Damaged") && !playerAnimator.GetBool("Jump") && !playerAnimator.GetBool("Attack"))
                cm.Jump();
        }

        if (Input.GetMouseButtonDown(0))
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

        if (Input.GetMouseButtonDown(1))
        {
            cm.Skill();
        }
        if (Input.GetMouseButtonUp(1))
        {
            cm.UnBlock();
        }
        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            cm.curSkillNum = 0;
        }
        if (Input.GetKeyDown(KeyCode.Alpha2))
        {
            cm.curSkillNum = 1;
        }
        if (Input.GetKeyDown(KeyCode.Alpha3))
        {
            cm.curSkillNum = 2;
        }
        if (Input.GetKeyDown(KeyCode.Alpha4))
        {
            cm.curSkillNum = 3;
        }
        if (Input.GetKeyDown(KeyCode.Alpha5))
        {
            cm.curSkillNum = 4;
        }
        if (Input.GetKeyDown(KeyCode.I)){
            InventorySystem.instance.InventoryOpenClose();
        }
    }
}
